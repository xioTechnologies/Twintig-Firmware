/**
 * @file Notification.c
 * @author Seb Madgwick
 * @brief Notifications.
 */

//------------------------------------------------------------------------------
// Includes

#include "Imu/Imu.h"
#include "OnChange.h"
#include "Periodic.h"
#include "Send/Send.h"
#include "Usb/UsbCdc.h"

//------------------------------------------------------------------------------
// Function declarations

static inline __attribute__((always_inline)) void ImuBufferOverflow(Send * const send, Imu * const imu);
static inline __attribute__((always_inline)) void SendBufferOverflow(Send * const send);

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Module tasks. This function should be called repeatedly within the
 * main program loop.
 */
void NotificationTasks(void) {

    // USB host
    switch (ON_CHANGE_POLL_DEBOUNCE(UsbCdcHostConnected())) {
        case OnChangeEdgeLowToHigh:
            SendNotification(&sendMain, "USB host connected");
            break;
        case OnChangeEdgeHighToLow:
            SendNotification(&sendMain, "USB host disconnected");
            break;
        default:
            break;
    }

    // USB port
    switch (ON_CHANGE_POLL_DEBOUNCE(UsbCdcPortOpen())) {
        case OnChangeEdgeLowToHigh:
            SendNotification(&sendMain, "USB port opened");
            break;
        case OnChangeEdgeHighToLow:
            SendNotification(&sendMain, "USB port closed");
            break;
        default:
            break;
    }

    // Do nothing else until polling period elapsed
    if (PERIODIC_POLL(1.0f) == false) {
        return;
    }

    // IMU buffer overflow
    ImuBufferOverflow(&sendA, &imuA);
    ImuBufferOverflow(&sendB, &imuB);
    ImuBufferOverflow(&sendC, &imuC);
    ImuBufferOverflow(&sendD, &imuD);
    ImuBufferOverflow(&sendE, &imuE);
    ImuBufferOverflow(&sendF, &imuF);
    ImuBufferOverflow(&sendG, &imuG);
    ImuBufferOverflow(&sendH, &imuH);
    ImuBufferOverflow(&sendI, &imuI);
    ImuBufferOverflow(&sendJ, &imuJ);
    ImuBufferOverflow(&sendK, &imuK);
    ImuBufferOverflow(&sendL, &imuL);
    ImuBufferOverflow(&sendM, &imuM);
    ImuBufferOverflow(&sendN, &imuN);
    ImuBufferOverflow(&sendO, &imuO);
    ImuBufferOverflow(&sendP, &imuP);
    ImuBufferOverflow(&sendQ, &imuQ);
    ImuBufferOverflow(&sendR, &imuR);
    ImuBufferOverflow(&sendS, &imuS);
    ImuBufferOverflow(&sendT, &imuT);

    // Send buffer overflow
    SendBufferOverflow(&sendMain);
    SendBufferOverflow(&sendA);
    SendBufferOverflow(&sendB);
    SendBufferOverflow(&sendC);
    SendBufferOverflow(&sendD);
    SendBufferOverflow(&sendE);
    SendBufferOverflow(&sendF);
    SendBufferOverflow(&sendG);
    SendBufferOverflow(&sendH);
    SendBufferOverflow(&sendI);
    SendBufferOverflow(&sendJ);
    SendBufferOverflow(&sendK);
    SendBufferOverflow(&sendL);
    SendBufferOverflow(&sendM);
    SendBufferOverflow(&sendN);
    SendBufferOverflow(&sendO);
    SendBufferOverflow(&sendP);
    SendBufferOverflow(&sendQ);
    SendBufferOverflow(&sendR);
    SendBufferOverflow(&sendS);
    SendBufferOverflow(&sendT);
}

/**
 * @brief IMU buffer overflow.
 * @param send Send structure.
 * @param imu IMU structure.
 */
static inline __attribute__((always_inline)) void ImuBufferOverflow(Send * const send, Imu * const imu) {
    const uint32_t numberOfSamples = imu->icm->bufferOverflow();
    if (numberOfSamples > 0) {
        SendError(send, "IMU buffer overflow. %u samples lost.", numberOfSamples);
    }
}

/**
 * @brief Send buffer overflow.
 * @param send Send structure.
 */
static inline __attribute__((always_inline)) void SendBufferOverflow(Send * const send) {
    const uint32_t usbBufferOverflow = SendUsbBufferOverflow(send);
    if (usbBufferOverflow > 0) {
        SendError(send, "USB buffer overflow. %u bytes lost.", usbBufferOverflow);
    }
    const uint32_t serialBufferOverflow = SendSerialBufferOverflow(send);
    if (serialBufferOverflow > 0) {
        SendError(send, "Serial buffer overflow. %u bytes lost.", serialBufferOverflow);
    }
}

//------------------------------------------------------------------------------
// End of file
