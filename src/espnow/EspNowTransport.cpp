#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_idf_version.h>

#include "espnow/EspNowConfig.h"
#include "transport/Transport.h"


namespace
{
    constexpr size_t RECEIVE_BUFFER_CAPACITY = 96;

    uint8_t receiveBuffer[RECEIVE_BUFFER_CAPACITY];
    size_t receiveReadIndex = 0;
    size_t receiveWriteIndex = 0;

    portMUX_TYPE receiveMutex = portMUX_INITIALIZER_UNLOCKED;

#if defined(NEXUS_ROLE_ATLAS)
    const uint8_t* peerMac = FIREFLY_MAC;
#elif defined(NEXUS_ROLE_FIREFLY)
    const uint8_t* peerMac = ATLAS_MAC;
#else
    #error "Define NEXUS_ROLE_ATLAS or NEXUS_ROLE_FIREFLY"
#endif

    size_t bufferedByteCount()
    {
        if (receiveWriteIndex >= receiveReadIndex)
        {
            return receiveWriteIndex - receiveReadIndex;
        }

        return RECEIVE_BUFFER_CAPACITY - receiveReadIndex + receiveWriteIndex;
    }

    void queueReceivedData(const uint8_t* data, int length)
    {
        if (data == nullptr || length <= 0)
        {
            return;
        }

        portENTER_CRITICAL(&receiveMutex);

        const size_t freeBytes = (RECEIVE_BUFFER_CAPACITY - 1) - bufferedByteCount();

        if (static_cast<size_t>(length) <= freeBytes)
        {
            for (int index = 0; index < length; index++)
            {
                receiveBuffer[receiveWriteIndex] = data[index];
                receiveWriteIndex = (receiveWriteIndex + 1) % RECEIVE_BUFFER_CAPACITY;
            }
        }

        portEXIT_CRITICAL(&receiveMutex);
    }

#if ESP_IDF_VERSION_MAJOR >= 5
    void onDataReceived(const esp_now_recv_info_t* info, const uint8_t* data, int length)
    {
        queueReceivedData(data, length);
    }
#else
    void onDataReceived(const uint8_t* macAddress, const uint8_t* data, int length)
    {
        queueReceivedData(data, length);
    }
#endif
}

void transportSetup()
{
    WiFi.mode(WIFI_STA);

    esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ESP-NOW initialization failed");
        return;
    }

    esp_now_register_recv_cb(onDataReceived);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, peerMac, sizeof(ATLAS_MAC));
    peerInfo.channel = ESPNOW_CHANNEL;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("ESP-NOW peer registration failed");
    }
}

void transportSend(const uint8_t* data, size_t length)
{
    if (data == nullptr || length == 0)
    {
        return;
    }

    esp_now_send(peerMac, data, length);
}

bool transportAvailable(size_t bytes)
{
    portENTER_CRITICAL(&receiveMutex);
    const bool available = bufferedByteCount() >= bytes;
    portEXIT_CRITICAL(&receiveMutex);

    return available;
}

size_t transportReceive(uint8_t* buffer, size_t maxLength)
{
    if (buffer == nullptr || maxLength == 0)
    {
        return 0;
    }

    portENTER_CRITICAL(&receiveMutex);
    size_t receivedLength = 0;

    while (receivedLength < maxLength && receiveReadIndex != receiveWriteIndex)
    {
        buffer[receivedLength] = receiveBuffer[receiveReadIndex];
        receiveReadIndex = (receiveReadIndex + 1) % RECEIVE_BUFFER_CAPACITY;
        receivedLength++;
    }

    portEXIT_CRITICAL(&receiveMutex);

    return receivedLength;
}