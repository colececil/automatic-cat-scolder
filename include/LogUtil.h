#ifndef AUTOMATIC_CAT_SCOLDER_LOG_UTIL_H
#define AUTOMATIC_CAT_SCOLDER_LOG_UTIL_H

#include <ArduinoLog.h>

namespace AutomaticCatScolder {
    class LogUtil {
    public:
        /**
         * Get the logger object, initializing if necessary.
         *
         * @return The logger.
         */
        static Logging getLogger();
    private:
        static bool initialized;

        /**
         * Print the time since the program started and the log level, using the given Print object.
         *
         * @param print The Print object.
         * @param logLevel the log level.
         */
        static void printPrefix(Print *print, int logLevel);

        /**
         * Print a newline using the given Print object.
         *
         * @param print The Print object.
         * @param logLevel The log level.
         */
        static void printNewLine(Print *print, int logLevel);
    };
}

#endif
