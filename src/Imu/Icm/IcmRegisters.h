/**
 * @file IcmRegisters.h
 * @author Seb Madgwick
 * @brief ICM-45686 registers.
 */

#ifndef ICM_REGISTERS_H
#define ICM_REGISTERS_H

//------------------------------------------------------------------------------
// Includes

#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions - Register bank 0

#define ICM_ACCEL_DATA_X1_UI_ADDRESS            (0x00)
#define ICM_PWR_MGMT0_ADDRESS                   (0x10)
#define ICM_FIFO_COUNT_0_ADDRESS                (0x12)
#define ICM_FIFO_COUNT_1_ADDRESS                (0x13)
#define ICM_FIFO_DATA_ADDRESS                   (0x14)
#define ICM_INT1_CONFIG0_ADDRESS                (0x16)
#define ICM_INT1_CONFIG2_ADDRESS                (0x18)
#define ICM_ACCEL_CONFIG0_ADDRESS               (0x1B)
#define ICM_GYRO_CONFIG0_ADDRESS                (0x1C)
#define ICM_FIFO_CONFIG0_ADDRESS                (0x1D)
#define ICM_FIFO_CONFIG0_0_ADDRESS              (0x1E)
#define ICM_FIFO_CONFIG0_1_ADDRESS              (0x1F)
#define ICM_FIFO_CONFIG2_ADDRESS                (0x20)
#define ICM_FIFO_CONFIG3_ADDRESS                (0x21)
#define ICM_FIFO_CONFIG4_ADDRESS                (0x22)
#define ICM_RTC_CONFIG_ADDRESS                  (0x26)
#define ICM_IOC_PAD_SCENARIO_OVRD_ADDRESS       (0x31)
#define ICM_WHO_AM_I_ADDRESS                    (0x72)
#define ICM_IREG_ADDR_15_8_ADDRESS              (0x7C)
#define ICM_REG_MISC2_ADDRESS                   (0x7F)

#define ICM_PWR_MGMT0_RESET_VALUE               (0x00)
#define ICM_INT1_CONFIG0_RESET_VALUE            (0x80)
#define ICM_INT1_CONFIG2_RESET_VALUE            (0x04)
#define ICM_ACCEL_CONFIG0_RESET_VALUE           (0x06)
#define ICM_GYRO_CONFIG0_RESET_VALUE            (0x06)
#define ICM_FIFO_CONFIG0_RESET_VALUE            (0x00)
#define ICM_FIFO_CONFIG2_RESET_VALUE            (0x20)
#define ICM_FIFO_CONFIG3_RESET_VALUE            (0x00)
#define ICM_FIFO_CONFIG4_RESET_VALUE            (0x00)
#define ICM_RTC_CONFIG_RESET_VALUE              (0x03)
#define ICM_IOC_PAD_SCENARIO_OVRD_RESET_VALUE   (0x00) /* undocumented but determined by reading register immediately after soft-reset */
#define ICM_WHO_AM_I_RESET_VALUE                (0xE9)
#define ICM_REG_MISC2_RESET_VALUE               (0x01)

typedef struct {
    int16_t accelDataX;
    int16_t accelDataY;
    int16_t accelDataZ;
    int16_t gyroDataX;
    int16_t gyroDataY;
    int16_t gyroDataZ;
    int16_t tempData;
} __attribute__((__packed__)) IcmSensorRegisters;

typedef union {

    struct {
        unsigned gyroOdr : 1;
        unsigned accelOdr : 1;
        unsigned fsyncTagEn : 1;
        unsigned tmstFieldEn : 1;
        unsigned hiresEn : 1;
        unsigned gyroEn : 1;
        unsigned accelEn : 1;
        unsigned extHeader : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmFifoHeader;

typedef struct {
    unsigned gyro : 2;
    unsigned accel : 2;
} __attribute__((__packed__)) IcmFifoLsb;

typedef struct {
    IcmFifoHeader header;
    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
    int8_t temp;
    int16_t timestamp;
    IcmFifoLsb lsbX;
    IcmFifoLsb lsbY;
    IcmFifoLsb lsbZ;
} __attribute__((__packed__)) IcmFifoPacket;

typedef union {

    struct {
        unsigned accelMode : 2;
        unsigned gyroMode : 2;
        unsigned : 4;
    } __attribute__((__packed__));
    uint8_t value;
} IcmPwrMgmt0Register;

typedef union {

    struct {
        unsigned int1StatusEnFifoFull : 1;
        unsigned int1StatusEnFifoThs : 1;
        unsigned int1StatusEnDrdy : 1;
        unsigned int1StatusEnAux1Drdy : 1;
        unsigned int1StatusEnApFsync : 1;
        unsigned int1StatusEnApAgcRdy : 1;
        unsigned int1StatusEnAux1AgcRdy : 1;
        unsigned int1StatusEnResetDone : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmInt1Config0Register;

typedef union {

    struct {
        unsigned int1Polarity : 1;
        unsigned int1Mode : 1;
        unsigned int1Drive : 1;
        unsigned : 5;
    } __attribute__((__packed__));
    uint8_t value;
} IcmInt1Config2Register;

typedef union {

    struct {
        unsigned accelOdr : 4;
        unsigned accelUiFsSel : 3;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmAccelConfig0Register;

typedef union {

    struct {
        unsigned gyroOdr : 4;
        unsigned gyroUiFsSel : 4;
    } __attribute__((__packed__));
    uint8_t value;
} IcmGyroConfig0Register;

typedef union {

    struct {
        unsigned fifoDepth : 6;
        unsigned fifoMode : 2;
    } __attribute__((__packed__));
    uint8_t value;
} IcmFifoConfig0Register;

typedef union {

    struct {
        unsigned : 3;
        unsigned fifoWrWmGtTh : 1;
        unsigned : 3;
        unsigned fifoFlush : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmFifoConfig2Register;

typedef union {

    struct {
        unsigned fifoIfEn : 1;
        unsigned fifoAccelEn : 1;
        unsigned fifoGyroEn : 1;
        unsigned fifoHiresEn : 1;
        unsigned fifoEs0En : 1;
        unsigned fifoEs1En : 1;
        unsigned : 2;
    } __attribute__((__packed__));
    uint8_t value;
} IcmFifoConfig3Register;

typedef union {

    struct {
        unsigned fifoEs0Es06b9b : 1;
        unsigned fifoTmstFsynEn : 1;
        unsigned fifoCompEn : 1;
        unsigned fifoCompNcFlowCfg : 3;
        unsigned : 2;
    } __attribute__((__packed__));
    uint8_t value;
} IcmFifoConfig4Register;

typedef union {

    struct {
        unsigned : 5;
        unsigned rtcMode : 1;
        unsigned rtcAlign : 1;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmRtcConfigRegister;

typedef union {

    struct {
        unsigned padsInt2CfgOvrdVal : 2;
        unsigned padsInt2CfgOvrd : 1;
        unsigned : 5;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIocPadScenarioOvrdRegister;

typedef union {

    struct {
        unsigned iregDone : 1;
        unsigned softRst : 1;
        unsigned : 6;
    } __attribute__((__packed__));
    uint8_t value;
} IcmRegMisc2Register;

//------------------------------------------------------------------------------
// Definitions - Register bank IPREG_TOP1

#define ICM_IPREG_TOP1_ADDRESS                  (0x00A2)
#define ICM_SIFS_I3C_STC_CFG_ADDRESS            (0x6800 | ICM_IPREG_TOP1_ADDRESS)
#define ICM_SMC_CONTROL_0_ADDRESS               (0x8800 | ICM_IPREG_TOP1_ADDRESS)

#define ICM_SIFS_I3C_STC_CFG_RESET_VALUE        (0x23)
#define ICM_SMC_CONTROL_0_RESET_VALUE           (0x60)

typedef union {

    struct {
        unsigned : 2;
        unsigned i3cStcMode : 1;
        unsigned : 5;
    } __attribute__((__packed__));
    uint8_t value;
} IcmSifsI3cStcCfgRegister;

typedef union {

    struct {
        unsigned tmstEn : 1;
        unsigned tmstFsyncEn : 1;
        unsigned tmstForceAuxFineEn : 1;
        unsigned tempDis : 1;
        unsigned accelLpClkSel : 1;
        unsigned : 3;
    } __attribute__((__packed__));
    uint8_t value;
} IcmSmcControl0Register;

//------------------------------------------------------------------------------
// Definitions - Register bank IPREG_SYS1

#define ICM_IPREG_SYS1_ADDRESS                  (0x00A4)
#define ICM_IPREG_SYS1_REG_166_ADDRESS          (0xA600 | ICM_IPREG_SYS1_ADDRESS)
#define ICM_IPREG_SYS1_REG_172_ADDRESS          (0xAC00 | ICM_IPREG_SYS1_ADDRESS)

#define ICM_IPREG_SYS1_REG_166_RESET_VALUE      (0x1B)
#define ICM_IPREG_SYS1_REG_172_RESET_VALUE      (0x80)

typedef union {

    struct {
        unsigned : 5;
        unsigned gyroSrcCtrl : 2;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIpregSys1Reg166Register;

typedef union {

    struct {
        unsigned gyroUiLpfbwSel : 3;
        unsigned : 4;
        unsigned gyroOisHpf1Byp : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIpregSys1Reg172Register;

//------------------------------------------------------------------------------
// Definitions - Register bank IPREG_SYS2

#define ICM_IPREG_SYS2_ADDRESS                  (0x00A5)
#define ICM_IPREG_SYS2_REG_123_ADDRESS          (0x7B00 | ICM_IPREG_SYS2_ADDRESS)
#define ICM_IPREG_SYS2_REG_131_ADDRESS          (0x8300 | ICM_IPREG_SYS2_ADDRESS)

#define ICM_IPREG_SYS2_REG_123_RESET_VALUE      (0x14)
#define ICM_IPREG_SYS2_REG_131_RESET_VALUE      (0x00)

typedef union {

    struct {
        unsigned accelSrcCtrl : 2;
        unsigned : 6;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIpregSys2Reg123Register;

typedef union {

    struct {
        unsigned accelUiLpfbwSel : 3;
        unsigned : 5;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIpregSys2Reg131Register;

#endif

//------------------------------------------------------------------------------
// End of file
