#ifndef AUTOMATIC_CAT_SCOLDER_BLE_UTIL_H
#define AUTOMATIC_CAT_SCOLDER_BLE_UTIL_H

#include <ArduinoBLE.h>
#include <ArduinoLog.h>

namespace AutomaticCatScolder {
    class BleUtil {
    public:
        enum class PeripheralStatus {
            NEARBY,
            NOT_NEARBY,
            UNDETERMINED
        };

        /**
         * Find a peripheral with the given name.
         *
         * @param name The name.
         * @return The peripheral.
         */
        static BLEDevice findPeripheral(String name);

        /**
         * Connect to the given peripheral and get its status.
         *
         * @param peripheral The peripheral.
         * @return The peripheral status.
         */
        static PeripheralStatus getPeripheralStatus(BLEDevice peripheral);
    private:
        static Logging logger;
        static bool bleDeviceStarted;

        /**
         * Start the BLE device.
         */
        static void startBleDevice();
    };
}

#endif
