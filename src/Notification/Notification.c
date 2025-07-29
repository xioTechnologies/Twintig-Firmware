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
            SendNotification(&send0, "USB host connected");
            break;
        case OnChangeEdgeHighToLow:
            SendNotification(&send0, "USB host disconnected");
            break;
        default:
            break;
    }

    // USB port
    switch (ON_CHANGE_POLL_DEBOUNCE(UsbCdcPortOpen())) {
        case OnChangeEdgeLowToHigh:
            SendNotification(&send0, "USB port open");
            break;
        case OnChangeEdgeHighToLow:
            SendNotification(&send0, "USB port closed");
            break;
        default:
            break;
    }

    // Do nothing else until polling period elapsed
    if (PERIODIC_POLL(1.0f) == false) {
        return;
    }

    // IMU buffer overflow
    ImuBufferOverflow(&send1, &imu1);
    ImuBufferOverflow(&send2, &imu2);
    ImuBufferOverflow(&send3, &imu3);
    ImuBufferOverflow(&send4, &imu4);
    ImuBufferOverflow(&send5, &imu5);
    ImuBufferOverflow(&send6, &imu6);
    ImuBufferOverflow(&send7, &imu7);
    ImuBufferOverflow(&send8, &imu8);
    ImuBufferOverflow(&send9, &imu9);
    ImuBufferOverflow(&send10, &imu10);
    ImuBufferOverflow(&send11, &imu11);
    ImuBufferOverflow(&send12, &imu12);
    ImuBufferOverflow(&send13, &imu13);
    ImuBufferOverflow(&send14, &imu14);
    ImuBufferOverflow(&send15, &imu15);
    ImuBufferOverflow(&send16, &imu16);
    ImuBufferOverflow(&send17, &imu17);
    ImuBufferOverflow(&send18, &imu18);
    ImuBufferOverflow(&send19, &imu19);
    ImuBufferOverflow(&send20, &imu20);

    // Send buffer overflow
    SendBufferOverflow(&send0);
    SendBufferOverflow(&send1);
    SendBufferOverflow(&send2);
    SendBufferOverflow(&send3);
    SendBufferOverflow(&send4);
    SendBufferOverflow(&send5);
    SendBufferOverflow(&send6);
    SendBufferOverflow(&send7);
    SendBufferOverflow(&send8);
    SendBufferOverflow(&send9);
    SendBufferOverflow(&send10);
    SendBufferOverflow(&send11);
    SendBufferOverflow(&send12);
    SendBufferOverflow(&send13);
    SendBufferOverflow(&send14);
    SendBufferOverflow(&send15);
    SendBufferOverflow(&send16);
    SendBufferOverflow(&send17);
    SendBufferOverflow(&send18);
    SendBufferOverflow(&send19);
    SendBufferOverflow(&send20);
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
    const uint32_t serialBufferOverflow = SendSerialBufferOverflow(&send0);
    if (serialBufferOverflow > 0) {
        SendError(&send0, "Serial buffer overflow. %u bytes lost.", serialBufferOverflow);
    }
}

//------------------------------------------------------------------------------
// End of file
