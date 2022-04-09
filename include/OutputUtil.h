#ifndef AUTOMATIC_CAT_SCOLDER_OUTPUT_UTIL_H
#define AUTOMATIC_CAT_SCOLDER_OUTPUT_UTIL_H

#include <ArduinoLog.h>

namespace AutomaticCatScolder {
    class OutputUtil {
    public:
        /**
         * Initialize the output pins.
         *
         * @param ledPin The pin for the LED.
         * @param buzzerPowerPin The power pin for the buzzer.
         * @param buzzerSignalPin The signal pin for the buzzer.
         */
        static void initializeOutputPins(pin_size_t ledPin, pin_size_t buzzerPowerPin, pin_size_t buzzerSignalPin);

        /**
         * Play a sound using the connected buzzer.
         */
        static void playSound();

        /**
         * Turn the LED on.
         */
        static void turnLedOn();

        /**
         * Turn the LED off.
         */
        static void turnLedOff();

        /**
         * Flash the LED for the given duration.
         *
         * @param duration The duration, in milliseconds.
         */
        static void flashLed(unsigned long duration);
    private:
        static Logging logger;
        static bool outputPinsInitialized;
        static pin_size_t ledPin;
        static pin_size_t buzzerPowerPin;
        static pin_size_t buzzerSignalPin;

        /**
         * Change the given frequency by the given number of half steps.
         *
         * @param frequency The frequency, in hertz.
         * @param halfSteps The number of half steps to change the frequency by. If negative, the frequency goes down.
         * @return The new frequency, in hertz.
         */
        static float changeFrequency(float frequency, int halfSteps);

        /**
         * Convert the given frequency to half of its period.
         *
         * @param frequency The frequency, in hertz.
         * @return The period, in microseconds.
         */
        static float frequencyToHalfPeriod(float frequency);
    };
}

#endif
