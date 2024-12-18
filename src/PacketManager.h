#ifndef PACKET_MANAGER_H
#define PACKET_MANAGER_H

#include <Arduino.h>
#include <functional>
#include "PacketStructs.h"
#include "PacketQueue.h"
#include "PacketProcessor.h"
#include "ErrorLogger.h"

// Configurable MTU size
#ifndef PM_MTU_SIZE
#define PM_MTU_SIZE 250  // Default MTU size
#endif

// Debug levels
#define PM_DEBUG_NONE 0
#define PM_DEBUG_ERROR 1
#define PM_DEBUG_WARNING 2
#define PM_DEBUG_INFO 3

#ifndef PM_DEBUG_LEVEL
#define PM_DEBUG_LEVEL PM_DEBUG_ERROR
#endif

// Callback types
using WriteCallback = std::function<bool(uint8_t*, size_t)>;
using DataCallback = std::function<void(uint8_t*, size_t)>;

class PacketManager {
public:
    PacketManager();
    ~PacketManager();

    // Initialize with callbacks
    bool begin(WriteCallback writeCallback, DataCallback dataCallback);
    
    // Main interface
    bool sendData(uint8_t* data, size_t len, uint8_t receiverId = 0);
    void consumeData(uint8_t* data, size_t len);
    void process();  // Call in loop()

    // Configuration
    void setEncryption(bool enable);
    void setAckRequired(bool enable);
    void setMTU(uint16_t mtu);
    void setRetransmitCount(uint8_t count);
    void setRetransmitTimeout(uint32_t timeout);

private:
    WriteCallback _writeCallback;
    DataCallback _dataCallback;
    PacketQueue* _txQueue;
    PacketQueue* _rxQueue;
    PacketProcessor* _processor;
    
    bool _encryptionEnabled;
    bool _ackRequired;
    uint16_t _mtu;
    uint8_t _retransmitCount;
    uint32_t _retransmitTimeout;
};

#endif // PACKET_MANAGER_H