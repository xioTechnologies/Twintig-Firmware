/**
 * @file Notification.c
 * @author Seb Madgwick
 * @brief Notifications.
 */

//------------------------------------------------------------------------------
// Includes

#include "Imu/Imu.h"
#include "OnChange.h"
#include "Send/Send.h"
#include "Send/Send.h"
#include "Timer/TimerScheduler.h"
#include "Usb/UsbCdc.h"

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
            SendNotification(&send1, "USB host connected");
            break;
        case OnChangeEdgeHighToLow:
            SendNotification(&send1, "USB host disconnected");
            break;
        default:
            break;
    }

    // Do nothing else until polling period elapsed
    if (TIMER_SCHEDULER_POLL(1.0f) == false) {
        return;
    }

    // IMU buffer overflow
    const uint32_t bufferOverflow = imu1.icm->bufferOverflow();
    if (bufferOverflow > 0) {
        SendError(&send1, "IMU buffer overflow. %u samples lost.", bufferOverflow);
    }

    // Send buffer overflow
    const uint32_t usbBufferOverflow = SendUsbBufferOverflow(&send1);
    if (usbBufferOverflow > 0) {
        SendError(&send1, "USB buffer overflow. %u bytes lost.", usbBufferOverflow);
    }
    const uint32_t serialBufferOverflow = SendSerialBufferOverflow(&send1);
    if (serialBufferOverflow > 0) {
        SendError(&send1, "Serial buffer overflow. %u bytes lost.", serialBufferOverflow);
    }
}

//------------------------------------------------------------------------------
// End of file
