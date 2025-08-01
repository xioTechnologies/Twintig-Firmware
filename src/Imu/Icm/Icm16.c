/**
 * @file Icm16.c
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "Fifo.h"
#include "Icm16.h"
#include "IcmConfig.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Function declarations

static uint8_t ReadRegister(const uint8_t address);
static void WriteRegister(const uint8_t address, const uint8_t value);
static void ExternalInterrupt(GPIO_PIN pin, uintptr_t context);
static void TransferComplete(void);

//------------------------------------------------------------------------------
// Variables

const Icm icm16 = {
    .initialise = Icm16Initialise,
    .deinitialise = Icm16Deinitialise,
    .getData = Icm16GetData,
    .bufferOverflow = Icm16BufferOverflow,
};
static SpiBusClient* spiBusClient;
static uint8_t deviceId;
static volatile __attribute__((coherent)) IcmSpiPacket spiPacket;
static volatile uint64_t timestamp;
static uint8_t fifoData[100 * sizeof (IcmFifoPacket)];
static Fifo fifo = {.data = fifoData, .dataSize = sizeof (fifoData)};
static volatile uint32_t bufferOverflow;

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises the module.
 * @param odr ODR.
 */
void Icm16Initialise(const IcmOdr odr) {

    // Add SPI bus client
    if (spiBusClient == NULL) {
        spiBusClient = ICM16_SPI_BUS.addClient(ICM16_CS_PIN);
    }

    // Ensure default states
    Icm16Deinitialise();

    // Read device ID
    deviceId = ReadRegister(ICM_WHO_AM_I_ADDRESS);

    // Software reset
    IcmDeviceConfigRegister deviceConfigRegister = {.value = ICM_DEVICE_CONFIG_RESET_VALUE};
    deviceConfigRegister.softResetConfig = 1;
    WriteRegister(ICM_DEVICE_CONFIG_ADDRESS, deviceConfigRegister.value);
    TimerDelayMilliseconds(1);

    // Configure endianness
    IcmIntfConfig0Register intfConfig0Register = {.value = ICM_INTF_CONFIG0_RESET_VALUE};
    intfConfig0Register.sensorDataEndian = 0; // sensor data is reported in Little Endian format
    WriteRegister(ICM_INTF_CONFIG0_ADDRESS, intfConfig0Register.value);

    // Configure interrupt pin
    IcmIntConfigRegister intConfigRegister = {.value = ICM_INT_CONFIG_RESET_VALUE};
    intConfigRegister.int1DriveCircuit = 1; // push pull
    WriteRegister(ICM_INT_CONFIG_ADDRESS, intConfigRegister.value);

    // Configure interrupt pulse
    IcmIntConfig1Register intConfig1Register = {.value = ICM_INT_CONFIG1_RESET_VALUE};
    intConfig1Register.IntTpulseDuration = 1; // interrupt pulse duration is 8 us. Required if ODR > 4kHz, optional for ODR < 4kHz.
    intConfig1Register.IntTdeassertDisable = 1; // disables de-assert duration. Required if ODR > 4kHz, optional for ODR < 4kHz
    intConfig1Register.IntAysncReset = 1; // user should change setting to 0 from default setting of 1, for proper INT1 and INT2 pin operation
    WriteRegister(ICM_INT_CONFIG1_ADDRESS, intConfig1Register.value);

    // Configure interrupt source
    IcmIntSource0Register intSource0Register = {.value = ICM_INT_SOURCE0_RESET_VALUE};
    intSource0Register.UIDrdyInt1EN = 1; // UI data ready interrupt routed to INT1
    WriteRegister(ICM_INT_SOURCE0_ADDRESS, intSource0Register.value);

    // Configure gyroscope
    IcmGyroConfig0Register gyroConfig0Register = {.value = ICM_GYRO_CONFIG0_RESET_VALUE};
    gyroConfig0Register.gyroOdr = odr;
    WriteRegister(ICM_GYRO_CONFIG0_ADDRESS, gyroConfig0Register.value);

    // Configure accelerometer
    IcmAccelConfig0Register accelConfig0Register = {.value = ICM_ACCEL_CONFIG0_RESET_VALUE};
    accelConfig0Register.accelOdr = odr;
    WriteRegister(ICM_ACCEL_CONFIG0_ADDRESS, accelConfig0Register.value);

    // Turn on gyroscope and accelerometer
    IcmPwrMgmt0Register pwrMgmt0Register = {.value = ICM_PWR_MGMT0_RESET_VALUE};
    pwrMgmt0Register.gyroMode = 0b11;
    pwrMgmt0Register.accelMode = 0b11;
    WriteRegister(ICM_PWR_MGMT0_ADDRESS, pwrMgmt0Register.value);
    TimerDelayMilliseconds(45);

    // Configure interrupt
    GPIO_PinInterruptCallbackRegister(ICM16_INT_PIN, ExternalInterrupt, 0);
    GPIO_PinIntEnable(ICM16_INT_PIN, GPIO_INTERRUPT_ON_BOTH_EDGES); // only both edges supported
}

/**
 * @brief Deinitialises the module.
 */
void Icm16Deinitialise(void) {
    GPIO_PinIntDisable(ICM16_INT_PIN);
    while (ICM16_SPI_BUS.transferInProgress(spiBusClient));
    FifoClear(&fifo);
    bufferOverflow = 0;
}

/**
 * @brief Reads the register value.
 * @param address Address.
 * @return Value.
 */
static uint8_t ReadRegister(const uint8_t address) {
    spiPacket = (IcmSpiPacket){.rw = 1, .address = address};
    ICM16_SPI_BUS.transfer(spiBusClient, &spiPacket, 2, NULL);
    while (ICM16_SPI_BUS.transferInProgress(spiBusClient));
    return spiPacket.data[0];
}

/**
 * @brief Writes the register value.
 * @param address Address.
 * @param value Value.
 */
static void WriteRegister(const uint8_t address, const uint8_t value) {
    spiPacket = (IcmSpiPacket){.rw = 0, .address = address, .value = value};
    ICM16_SPI_BUS.transfer(spiBusClient, &spiPacket, 2, NULL);
    while (ICM16_SPI_BUS.transferInProgress(spiBusClient));
}

/**
 * @brief External interrupt callback.
 * @param pin Pin.
 * @param context COntext.
 */
static void ExternalInterrupt(GPIO_PIN pin, uintptr_t context) {
    if (GPIO_PinRead(pin) == true) { // ignore rising edges
        return;
    }
    if (ICM16_SPI_BUS.transferInProgress(spiBusClient)) {
        return;
    }
    timestamp = TimerGetTicks64();
    spiPacket.rw = 1;
    spiPacket.address = ICM_TEMP_DATA1_ADDRESS;
    ICM16_SPI_BUS.transfer(spiBusClient, &spiPacket, sizeof (IcmSensorRegisters) + 1, TransferComplete);
}

/**
 * @brief Transfer complete callback.
 */
static void TransferComplete(void) {
    const IcmFifoPacket fifoPacket = {
        .timestamp = timestamp,
        .registers = *((IcmSensorRegisters*) spiPacket.data),
    };
    if (FifoWrite(&fifo, &fifoPacket, sizeof (fifoPacket)) != FifoResultOk) {
        bufferOverflow++;
    }
}

/**
 * @brief Get data.
 * @param data Data.
 * @return Result.
 */
IcmResult Icm16GetData(IcmData * const data) {
    IcmFifoPacket fifoPacket;
    if (FifoRead(&fifo, &fifoPacket, sizeof (fifoPacket)) == 0) {
        return IcmResultError;
    }
    data->timestamp = fifoPacket.timestamp;
    data->gyroscopeX = (float) fifoPacket.registers.gyroDataX * (1.0f / 16.4f);
    data->gyroscopeY = (float) fifoPacket.registers.gyroDataY * (1.0f / 16.4f);
    data->gyroscopeZ = (float) fifoPacket.registers.gyroDataZ * (1.0f / 16.4f);
    data->accelerometerX = (float) fifoPacket.registers.accelDataX * (1.0f / 2048.0f);
    data->accelerometerY = (float) fifoPacket.registers.accelDataY * (1.0f / 2048.0f);
    data->accelerometerZ = (float) fifoPacket.registers.accelDataZ * (1.0f / 2048.0f);
    data->temperature = (float) fifoPacket.registers.tempData * (1.0f / 132.48f) + 25.0f;
    return IcmResultOk;
}

/**
 * @brief Returns the number of samples lost due to buffer overflow. Calling
 * this function will reset the value.
 * @return Number of samples lost due to buffer overflow.
 */
uint32_t Icm16BufferOverflow(void) {
    return __sync_lock_test_and_set(&bufferOverflow, 0);
}

/**
 * @brief Performs self-test.
 * @return Test result.
 */
IcmTestResult Icm16Test(void) {

    // Check device ID
    if (deviceId != ICM_WHO_AM_I_RESET_VALUE) {
        return IcmTestResultInvalidID;
    }

    // Check interrupt
    const uint32_t timeout = TimerGetTicks64() + (TIMER_TICKS_PER_SECOND / 10);
    while (true) {
        IcmData data;
        if (Icm16GetData(&data) == IcmResultOk) {
            break;
        }
        if (TimerGetTicks64() > timeout) {
            return IcmTestResultInterruptFailed;
        }
    }

    // Self-test passed
    return IcmTestResultPassed;
}

//------------------------------------------------------------------------------
// End of file
