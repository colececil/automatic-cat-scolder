#include <climits>
#include "BleUtil.h"
#include "LogUtil.h"

using namespace AutomaticCatScolder;

Logging BleUtil::logger = LogUtil::getLogger();
bool BleUtil::bleDeviceStarted = false;

BLEDevice BleUtil::findPeripheral(String name) {
    if (!bleDeviceStarted) {
        startBleDevice();
    }

    logger.info(F("msg=\"Starting scan to find peripheral.\", targetPeripheralName=\"%s\""), name.c_str());
    BLE.scan();

    bool found = false;
    BLEDevice peripheral;
    while (!found) {
        peripheral = BLE.available();
        if (peripheral && peripheral.localName().startsWith(name)) {
            logger.info(F("msg=\"Target peripheral found.\""));
            found = true;
        }
    }

    logger.info(F("msg=\"Stopping peripheral scan.\""));
    BLE.stopScan();

    return peripheral;
}

BleUtil::PeripheralStatus BleUtil::getPeripheralStatus(BLEDevice peripheral) {
    if (!bleDeviceStarted) {
        startBleDevice();
    }

    PeripheralStatus status = PeripheralStatus::UNDETERMINED;

    logger.info(F("msg=\"Attempting to connect to target peripheral.\""));
    if (peripheral.connect()) {
        logger.info(F("msg=\"Attempting to discover attributes of target peripheral.\""));

        int rssi = INT_MIN;
        if (peripheral.discoverAttributes()) {
            rssi = peripheral.rssi();
            logger.info(F("msg=\"Target peripheral RSSI checked.\", rssi=%d"), rssi);

            if (rssi > -50) {
                logger.info(F("msg=\"Target peripheral is nearby.\""));
                status = PeripheralStatus::NEARBY;
            } else {
                logger.info(F("msg=\"Target peripheral is not nearby.\""));
                status = PeripheralStatus::NOT_NEARBY;
            }
        } else {
            logger.error(F("msg=\"Failed to discover attributes of target peripheral.\""));
        }

        if (peripheral.connected()) {
            if (peripheral.disconnect()) {
                logger.info(F("msg=\"Disconnected from target peripheral.\""));
            } else {
                logger.warning(F("msg=\"Failed to disconnect from target peripheral.\""));
            }
        }
    } else {
        logger.error(F("msg=\"Failed to connect to target peripheral.\""));
    }

    return status;
}

void BleUtil::startBleDevice() {
    logger.info(F("msg=\"Starting BLE device.\""));

    while (!BLE.begin()) {
        logger.info(F("msg=\"Failed to start BLE device.\""));
        delay(10);
    }

    logger.info(F("msg=\"BLE device started.\""));
    bleDeviceStarted = true;
}
