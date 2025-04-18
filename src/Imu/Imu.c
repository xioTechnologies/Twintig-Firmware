/**
 * @file Imu.c
 * @author Seb Madgwick
 * @brief ICM-42688-P IMU driver.
 */

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "Fifo.h"
#include "Imu.h"
#include "Spi/Spi4Dma.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief DEVICE_CONFIG register address.
 */
#define DEVICE_CONFIG_ADDRESS (0x11)

/**
 * @brief INT_CONFIG register address.
 */
#define INT_CONFIG_ADDRESS (0x14)

/**
 * @brief TEMP_DATA1 register address.
 */
#define TEMP_DATA1_ADDRESS (0x1D)

/**
 * @brief INTF_CONFIG0 register address.
 */
#define INTF_CONFIG0_ADDRESS (0x4C)

/**
 * @brief PWR_MGMT0 register address.
 */
#define PWR_MGMT0_ADDRESS (0x4E)

/**
 * @brief GYRO_CONFIG0 register address.
 */
#define GYRO_CONFIG0_ADDRESS (0x4F)

/**
 * @brief ACCEL_CONFIG0 register address.
 */
#define ACCEL_CONFIG0_ADDRESS (0x50)

/**
 * @brief INT_CONFIG1 register address.
 */
#define INT_CONFIG1_ADDRESS (0x64)

/**
 * @brief INT_SOURCE0 register address.
 */
#define INT_SOURCE0_ADDRESS (0x65)

/**
 * @brief WHO_AM_I register address.
 */
#define WHO_AM_I (0x75)

/**
 * @brief DEVICE_CONFIG register.
 */
typedef union {

    struct {
        unsigned softResetConfig : 1;
        unsigned : 3;
        unsigned spiMode : 1;
        unsigned : 3;
    } __attribute__((__packed__));
    uint8_t value;
} DeviceConfigRegister;

/**
 * @brief INT_CONFIG register.
 */
typedef union {

    struct {
        unsigned int1Polarity : 1;
        unsigned int1DriveCircuit : 1;
        unsigned int1Mode : 1;
        unsigned int2Polarity : 1;
        unsigned int2DriveCircuit : 1;
        unsigned int2Mode : 1;
        unsigned : 2;
    } __attribute__((__packed__));
    uint8_t value;
} IntConfigRegister;

/**
 * @brief Sensor registers.
 */
typedef struct {
    int16_t tempData;
    int16_t accelDataX;
    int16_t accelDataY;
    int16_t accelDataZ;
    int16_t gyroDataX;
    int16_t gyroDataY;
    int16_t gyroDataZ;
} __attribute__((__packed__)) SensorRegisters;

/**
 * @brief INTF_CONFIG0 register.
 */
typedef union {

    struct {
        unsigned UISifsCfg : 2;
        unsigned : 2;
        unsigned sensorDataEndian : 1;
        unsigned fifoCountEndian : 1;
        unsigned fifoCountRec : 1;
        unsigned fifoHoldLastDataEN : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IntfConfig0Register;

/**
 * @brief PWR_MGMT0 register.
 */
typedef union {

    struct {
        unsigned accelMode : 2;
        unsigned gyroMode : 2;
        unsigned idle : 1;
        unsigned tempDis : 1;
        unsigned : 2;
    } __attribute__((__packed__));
    uint8_t value;
} PwrMgmt0Register;

/**
 * @brief GYRO_CONFIG0 register.
 */
typedef union {

    struct {
        unsigned gyroOdr : 4;
        unsigned : 1;
        unsigned gyroFSSel : 3;
    } __attribute__((__packed__));
    uint8_t value;
} GyroConfig0Register;

/**
 * @brief ACCEL_CONFIG0 register.
 */
typedef union {

    struct {
        unsigned accelOdr : 4;
        unsigned : 1;
        unsigned accelFSSel : 3;
    } __attribute__((__packed__));
    uint8_t value;
} AccelConfig0Register;

/**
 * @brief INT_CONFIG1 register.
 */
typedef union {

    struct {
        unsigned : 4;
        unsigned IntAysncReset : 1;
        unsigned IntTdeassertDisable : 1;
        unsigned IntTpulseDuration : 1;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IntConfig1Register;

/**
 * @brief INT_SOURCE0 register.
 */
typedef union {

    struct {
        unsigned UIAgcRdyInt1EN : 1;
        unsigned FifoFullInt1EN : 1;
        unsigned FifoThsInt1EN : 1;
        unsigned UIDrdyInt1EN : 1;
        unsigned ResetDoneInt1EN : 1;
        unsigned PllRdyInt1EN : 1;
        unsigned UIFsyncInt1EN : 1;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IntSource0Register;

/**
 * @brief SPI packet.
 */
typedef struct {
    unsigned int address : 7;
    unsigned int rw : 1;

    union {
        uint8_t data[32];
        uint8_t value;
    };
} __attribute__((__packed__)) SpiPacket;

/**
 * @brief FIFO packet.
 */
typedef struct {
    uint64_t timestamp;
    SensorRegisters registers;
} __attribute__((__packed__)) FifoPacket;

//------------------------------------------------------------------------------
// Function declarations

static uint8_t ReadRegister(const uint8_t address);
static void WriteRegister(const uint8_t address, const uint8_t value);
static void ExternalInterrupt(GPIO_PIN pin, uintptr_t context);
static void TransferComplete(void);

//------------------------------------------------------------------------------
// Variables

static __attribute__((coherent)) SpiPacket spiPacket;
static bool initialisationFailed;
static uint64_t timestamp;
static uint8_t fifoData[1000 * sizeof (FifoPacket)];
static Fifo fifo = {.data = fifoData, .dataSize = sizeof (fifoData)};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises the module.
 * @param odr ODR.
 */
void ImuInitialise(const ImuOdr odr) {

    // Ensure default states
    ImuDeinitialise();

    // Initialise SPI
    SpiSettings settings = spiSettingsDefault;
    settings.clockFrequency = 16000000;
    Spi4DmaInitialise(&settings);

    // Verify identity of the device
    initialisationFailed = ReadRegister(WHO_AM_I) != 0x47;

    // Software reset
    DeviceConfigRegister deviceConfigRegister = {.value = ReadRegister(DEVICE_CONFIG_ADDRESS)};
    deviceConfigRegister.softResetConfig = 1;
    WriteRegister(DEVICE_CONFIG_ADDRESS, deviceConfigRegister.value);
    TimerDelayMilliseconds(1);

    // Configure endianness
    IntfConfig0Register intfConfig0Register = {.value = ReadRegister(INTF_CONFIG0_ADDRESS)};
    intfConfig0Register.sensorDataEndian = 0; // sensor data is reported in Little Endian format
    WriteRegister(INTF_CONFIG0_ADDRESS, intfConfig0Register.value);

    // Configure interrupt pin
    IntConfigRegister intConfigRegister = {.value = ReadRegister(INT_CONFIG_ADDRESS)};
    intConfigRegister.int1DriveCircuit = 1; // push pull
    WriteRegister(INT_CONFIG_ADDRESS, intConfigRegister.value);

    // Configure interrupt pulse
    IntConfig1Register intConfig1Register = {.value = ReadRegister(INT_CONFIG1_ADDRESS)};
    intConfig1Register.IntTpulseDuration = 1; // interrupt pulse duration is 8 us. Required if ODR ? 4kHz, optional for ODR < 4kHz.
    intConfig1Register.IntTdeassertDisable = 1; // disables de-assert duration. Required if ODR ? 4kHz, optional for ODR < 4kHz
    intConfig1Register.IntAysncReset = 1; // user should change setting to 0 from default setting of 1, for proper INT1 and INT2 pin operation
    WriteRegister(INT_CONFIG1_ADDRESS, intConfig1Register.value);

    // Configure interrupt source
    IntSource0Register intSource0Register = {.value = ReadRegister(INT_SOURCE0_ADDRESS)};
    intSource0Register.UIDrdyInt1EN = 1; // UI data ready interrupt routed to INT1
    WriteRegister(INT_SOURCE0_ADDRESS, intSource0Register.value);

    // Configure gyroscope
    GyroConfig0Register gyroConfig0Register = {.value = ReadRegister(GYRO_CONFIG0_ADDRESS)};
    gyroConfig0Register.gyroOdr = odr;
    WriteRegister(GYRO_CONFIG0_ADDRESS, gyroConfig0Register.value);

    // Configure accelerometer
    AccelConfig0Register accelConfig0Register = {.value = ReadRegister(ACCEL_CONFIG0_ADDRESS)};
    accelConfig0Register.accelOdr = odr;
    WriteRegister(ACCEL_CONFIG0_ADDRESS, accelConfig0Register.value);

    // Turn on gyroscope and accelerometer
    PwrMgmt0Register pwrMgmt0Register = {.value = ReadRegister(PWR_MGMT0_ADDRESS)};
    pwrMgmt0Register.gyroMode = 0b11;
    pwrMgmt0Register.accelMode = 0b11;
    WriteRegister(PWR_MGMT0_ADDRESS, pwrMgmt0Register.value);
    TimerDelayMilliseconds(45);

    // Configure interrupt
    GPIO_PinInterruptCallbackRegister(INT4_CH1_PIN, ExternalInterrupt, (uintptr_t) NULL);
    GPIO_PinIntEnable(INT4_CH1_PIN, GPIO_INTERRUPT_ON_BOTH_EDGES); // only both edges supported
}

/**
 * @brief Deinitialises the module.
 */
void ImuDeinitialise(void) {
    while (Spi4DmaTransferInProgress());
    Spi4DmaDeinitialise();
    GPIO_PinIntDisable(INT4_CH1_PIN);
    FifoClear(&fifo);
}

/**
 * @brief Reads the register value.
 * @param address Address.
 * @return Value.
 */
static uint8_t ReadRegister(const uint8_t address) {
    spiPacket = (SpiPacket){.rw = 1, .address = address};
    Spi4DmaTransfer(CS4_CH1_PIN, &spiPacket, 2, NULL);
    while (Spi4DmaTransferInProgress());
    return spiPacket.data[0];
}

/**
 * @brief Writes the register value.
 * @param address Address.
 * @param value Value.
 */
static void WriteRegister(const uint8_t address, const uint8_t value) {
    spiPacket = (SpiPacket){.rw = 0, .address = address, .value = value};
    Spi4DmaTransfer(CS4_CH1_PIN, &spiPacket, 2, NULL);
    while (Spi4DmaTransferInProgress());
}

/**
 * @brief External interrupt callback function.
 * @param pin Pin.
 * @param context COntext.
 */
static void ExternalInterrupt(GPIO_PIN pin, uintptr_t context) {
    if (GPIO_PinRead(pin) == true) { // ignore rising edges
        return;
    }
    if (Spi4DmaTransferInProgress()) {
        return;
    }
    timestamp = TimerGetTicks64();
    spiPacket.rw = 1;
    spiPacket.address = TEMP_DATA1_ADDRESS;
    Spi4DmaTransfer(CS4_CH1_PIN, (void *const) &spiPacket, sizeof (SensorRegisters) + 1, TransferComplete);
}

/**
 * @brief Transfer complete callback function.
 */
static void TransferComplete(void) {
    const FifoPacket fifoPacket = {
        .timestamp = timestamp,
        .registers = *((SensorRegisters*) spiPacket.data),
    };
    FifoWrite(&fifo, &fifoPacket, sizeof (fifoPacket));
}

/**
 * @brief Get IMU data.
 * @param data Data.
 * @return True if IMU data avaliable.
 */
bool ImuGetData(ImuData * const data) {
    FifoPacket fifoPacket;
    if (FifoRead(&fifo, &fifoPacket, sizeof (fifoPacket)) == 0) {
        return false;
    }
    data->timestamp = fifoPacket.timestamp;
    data->gyroscopeX = (float) fifoPacket.registers.gyroDataX * (1.0f / 16.4f);
    data->gyroscopeY = (float) fifoPacket.registers.gyroDataY * (1.0f / 16.4f);
    data->gyroscopeZ = (float) fifoPacket.registers.gyroDataZ * (1.0f / 16.4f);
    data->accelerometerX = (float) fifoPacket.registers.accelDataX * (1.0f / 2048.0f);
    data->accelerometerY = (float) fifoPacket.registers.accelDataY * (1.0f / 2048.0f);
    data->accelerometerZ = (float) fifoPacket.registers.accelDataZ * (1.0f / 2048.0f);
    data->temperature = (float) fifoPacket.registers.tempData * (1.0f / 132.48f) + 25.0f;
    return true;
}

//------------------------------------------------------------------------------
// End of file
