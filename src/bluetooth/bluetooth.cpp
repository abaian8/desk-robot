#include "bluetooth.h"
#include "../config/pins.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>

// ─────────────────────────────────────────────────────────────────────────────
// Nordic UART Service UUIDs (widely supported by BLE UART libraries)
// ─────────────────────────────────────────────────────────────────────────────

#define NUS_SERVICE_UUID   "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUS_RX_CHAR_UUID   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"  // app writes here
#define NUS_TX_CHAR_UUID   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"  // robot notifies here

// ─────────────────────────────────────────────────────────────────────────────
// Internal state
// ─────────────────────────────────────────────────────────────────────────────

static BLEServer*           _server    = nullptr;
static BLECharacteristic*   _txChar    = nullptr;
static bool                 _connected = false;

// Incoming command is written here by the BLE callback, consumed in update().
static volatile bool  _cmdPending = false;
static char           _cmdBuffer[64];

static Bluetooth::CommandCallback _onCommand = nullptr;

// ─────────────────────────────────────────────────────────────────────────────
// BLE callbacks (run on BLE task — keep them short)
// ─────────────────────────────────────────────────────────────────────────────

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer*)    override { _connected = true;  }
    void onDisconnect(BLEServer* s) override {
        _connected = false;
        // Restart advertising so the app can reconnect.
        BLEDevice::startAdvertising();
    }
};

class RxCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* c) override {
        std::string raw = c->getValue();
        if (raw.empty()) return;

        // Strip trailing newline / carriage-return.
        while (!raw.empty() && (raw.back() == '\n' || raw.back() == '\r'))
            raw.pop_back();

        size_t len = raw.size();
        if (len >= sizeof(_cmdBuffer)) len = sizeof(_cmdBuffer) - 1;
        memcpy(_cmdBuffer, raw.c_str(), len);
        _cmdBuffer[len] = '\0';
        _cmdPending = true;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

void Bluetooth::init(CommandCallback onCommand) {
    _onCommand = onCommand;

    BLEDevice::init(BLE_DEVICE_NAME);

    _server = BLEDevice::createServer();
    _server->setCallbacks(new ServerCallbacks());

    BLEService* service = _server->createService(NUS_SERVICE_UUID);

    // TX characteristic — robot → app (Notify)
    _txChar = service->createCharacteristic(
        NUS_TX_CHAR_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    _txChar->addDescriptor(new BLE2902());

    // RX characteristic — app → robot (Write)
    BLECharacteristic* rxChar = service->createCharacteristic(
        NUS_RX_CHAR_UUID,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
    );
    rxChar->setCallbacks(new RxCallbacks());

    service->start();

    BLEAdvertising* adv = BLEDevice::getAdvertising();
    adv->addServiceUUID(NUS_SERVICE_UUID);
    adv->setScanResponse(true);
    BLEDevice::startAdvertising();

    Serial.println("[BT] Advertising as " BLE_DEVICE_NAME);
}

void Bluetooth::update() {
    if (_cmdPending && _onCommand) {
        _cmdPending = false;
        _onCommand(_cmdBuffer);
    }
}

void Bluetooth::send(const char* message) {
    if (!_connected || !_txChar) return;
    _txChar->setValue((uint8_t*)message, strlen(message));
    _txChar->notify();
}

bool Bluetooth::isConnected() {
    return _connected;
}
