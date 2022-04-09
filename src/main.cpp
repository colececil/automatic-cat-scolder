#include <Arduino.h>
#include <ArduinoBLE.h>
#include <wdt_samd21.h>
#include <climits>
#include "LogUtil.h"
#include "BleUtil.h"
#include "OutputUtil.h"

using namespace AutomaticCatScolder;

const String TARGET_PERIPHERAL_NAME = "";
const pin_size_t LED_PIN = UCHAR_MAX;
const pin_size_t BUZZER_POWER_PIN = UCHAR_MAX;
const pin_size_t BUZZER_SIGNAL_PIN = UCHAR_MAX;
#if __has_include("../constants.h")
#include "../constants.h"
#endif

const unsigned long WATCHDOG_TIMEOUT = WDT_CONFIG_PER_8K; // 8192 clock cycles (8 seconds)
const unsigned long WATCHDOG_TIMEOUT_SHORT = WDT_CONFIG_PER_8; // 8 clock cycles (7.8 milliseconds)
const unsigned long WATCHDOG_TIMEOUT_LONG = WDT_CONFIG_PER_16K; // 8 clock cycles (16 seconds)
const unsigned long WAIT_TIME = 5000L;
const unsigned long LED_FLASH_DURATION = 250L;

Logging logger;
BLEDevice targetPeripheral;
int totalErrors = 0;

void setup() {
    Serial.begin(9600);
    logger = LogUtil::getLogger();

    if (String(TARGET_PERIPHERAL_NAME).length() == 0
            || LED_PIN == UCHAR_MAX
            || BUZZER_POWER_PIN == UCHAR_MAX
            || BUZZER_SIGNAL_PIN == UCHAR_MAX) {
        logger.error(F("msg=\"One or more required constants were not defined in constants.h.\""));
        exit(EXIT_FAILURE);
    }

    OutputUtil::initializeOutputPins(LED_PIN, BUZZER_POWER_PIN, BUZZER_SIGNAL_PIN);

    OutputUtil::turnLedOn();
    wdt_init(WATCHDOG_TIMEOUT_LONG);
    targetPeripheral = BleUtil::findPeripheral(TARGET_PERIPHERAL_NAME);
    wdt_reset();
    OutputUtil::turnLedOff();
}

void loop() {
    BleUtil::PeripheralStatus status = BleUtil::getPeripheralStatus(targetPeripheral);
    switch (status) {
        case BleUtil::PeripheralStatus::NEARBY: {
            totalErrors = 0;
            wdt_disable();
            OutputUtil::playSound();
            wdt_init(WATCHDOG_TIMEOUT);
            break;
        }
        case BleUtil::PeripheralStatus::NOT_NEARBY: {
            totalErrors = 0;
            wdt_disable();

            delay(WAIT_TIME - 3 * LED_FLASH_DURATION);
            OutputUtil::flashLed(LED_FLASH_DURATION);
            delay(LED_FLASH_DURATION);
            OutputUtil::flashLed(LED_FLASH_DURATION);

            wdt_init(WATCHDOG_TIMEOUT);
            break;
        }
        case BleUtil::PeripheralStatus::UNDETERMINED: {
            totalErrors++;
            logger.error(F("msg=\"Unable to get RSSI due to errors.\", totalErrors=\"%d\""), totalErrors);
            if (totalErrors >= 5) {
                logger.error(F("msg=\"Error threshold exceeded. Restarting device.\""));
                OutputUtil::turnLedOn();
                wdt_init(WATCHDOG_TIMEOUT_SHORT);
                delay(WAIT_TIME);
            }
            wdt_reset();
            break;
        }
    }
}