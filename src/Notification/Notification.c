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

    // IMU buffer overrun
    const uint32_t bufferOverflow = imu1.icm->bufferOverflow();
    if (bufferOverflow > 0) {
        SendError(&send1, "IMU buffer overflow. %u samples lost.", bufferOverflow);
    }
}

//------------------------------------------------------------------------------
// End of file
