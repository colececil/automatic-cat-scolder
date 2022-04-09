#include "LogUtil.h"

using namespace AutomaticCatScolder;

const int MILLISECONDS_IN_SECOND = 1000;
const int MILLISECONDS_IN_MINUTE = 1000 * 60;
const int MILLISECONDS_IN_HOUR = 1000 * 60 * 60;

bool LogUtil::initialized = false;

Logging LogUtil::getLogger() {
    if (!initialized) {
        Log.begin(LOG_LEVEL_INFO, &Serial, false);
        Log.setPrefix(printPrefix);
        Log.setSuffix(printNewLine);
        initialized = true;
    }
    return Log;
}

void LogUtil::printPrefix(Print *print, int logLevel) {
    unsigned long uncountedMilliseconds = millis();

    String hours = String(uncountedMilliseconds / MILLISECONDS_IN_HOUR);
    uncountedMilliseconds %= MILLISECONDS_IN_HOUR;

    String minutes = String(uncountedMilliseconds / MILLISECONDS_IN_MINUTE);
    if (minutes.length() == 1) {
        minutes = "0" + minutes;
    }
    uncountedMilliseconds %= MILLISECONDS_IN_MINUTE;

    String seconds = String(uncountedMilliseconds / MILLISECONDS_IN_SECOND);
    if (seconds.length() == 1) {
        seconds = "0" + seconds;
    }
    uncountedMilliseconds %= MILLISECONDS_IN_SECOND;

    String milliseconds = String(uncountedMilliseconds);
    while (milliseconds.length() < 3) {
        milliseconds = "0" + milliseconds;
    }

    print->print("+" + hours + ":" + minutes + ":" + seconds + "." + milliseconds + " ");
    switch (logLevel) {
        case LOG_LEVEL_VERBOSE:
            print->print(F("VERBOSE"));
            break;
        case LOG_LEVEL_TRACE:
            print->print(F("TRACE"));
            break;
        case LOG_LEVEL_INFO:
            print->print(F("INFO"));
            break;
        case LOG_LEVEL_WARNING:
            print->print(F("WARNING"));
            break;
        case LOG_LEVEL_ERROR:
            print->print(F("ERROR"));
            break;
        case LOG_LEVEL_FATAL:
            print->print(F("FATAL"));
            break;
    }
    print->print(F(": "));
}

void LogUtil::printNewLine(Print *print, int logLevel) {
    print->println();
}
