/**
 * @file SpiBus.c
 * @author Seb Madgwick
 * @brief Manages access to the SPI bus.
 */

//------------------------------------------------------------------------------
// Includes

#include "SpiBus4.h"
#include "SpiBusConfig.h"

//------------------------------------------------------------------------------
// Function declarations

static void BeginTrasnfer(SpiBusClient * const client);
static void TransferComplete(void);

//------------------------------------------------------------------------------
// Variables

static int numberOfClients;
static SpiBusClient clients[SPI_BUS_4_MAX_NUMBER_OF_CLIENTS];
static SpiBusClient* activeClient;

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Adds a client to the SPI bus.
 * @param csPin CS pin.
 * @return Client.
 */
SpiBusClient * const SpiBus4AddClient(const GPIO_PIN csPin) {
    if (numberOfClients >= SPI_BUS_4_MAX_NUMBER_OF_CLIENTS) {
        return NULL;
    }
    SpiBusClient * const client = &clients[numberOfClients++];
    client->csPin = csPin;
    return client;
}

/**
 * @brief Transfers data. The data will be overwritten with the received data.
 * The data must be declared __attribute__((coherent)) for PIC32MZ devices.
 * This function must not be called while a transfer is in progress. The
 * callback function will be called from within an interrupt once the transfer
 * is complete.
 * @return Client.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param transferComplete Transfer complete callback function.
 */
void SpiBus4Transfer(SpiBusClient * const client, void* const data, const size_t numberOfBytes, void (*transferComplete)(void)) {
    if (client == NULL) {
        return;
    }
    client->transferComplete = transferComplete;
    client->data = data;
    client->numberOfBytes = numberOfBytes; // set this member last else interrupt may use invalid structure
    BeginTrasnfer(client);
}

/**
 * @brief Returns true if a transfer is in progress.
 * @param client Client.
 * @return True if a transfer is in progress.
 */
bool SpiBus4TransferInProgress(const SpiBusClient * const client) {
    if (client == NULL) {
        return false;
    }
    return client->numberOfBytes > 0;
}

/**
 * @brief Begins transfer.
 * @param client Client.
 */
static void BeginTrasnfer(SpiBusClient * const client) {
    static int lock;
    if (__sync_lock_test_and_set(&lock, 1) == 1) {
        return;
    }
    if (activeClient == NULL) {
        activeClient = client;
        SPI_BUS_4_TRANSFER(activeClient->csPin, activeClient->data, activeClient->numberOfBytes, TransferComplete);
    }
    __sync_lock_release(&lock);
}

/**
 * @brief This function is called from within an interrupt once the transfer is
 * complete.
 */
static void TransferComplete(void) {

    // End current transfer
    activeClient->numberOfBytes = 0;
    if (activeClient->transferComplete != NULL) {
        activeClient->transferComplete();
    }
    activeClient = NULL;

    // Begin next transfer
    for (int index = 0; index < numberOfClients; index++) {
        if (clients[index].numberOfBytes > 0) {
            BeginTrasnfer(&clients[index]);
            return;
        }
    }
}

//------------------------------------------------------------------------------
// End of file
