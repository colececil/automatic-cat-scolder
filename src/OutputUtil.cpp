#include "OutputUtil.h"
#include "LogUtil.h"

using namespace AutomaticCatScolder;

const int SOUND_DURATION_MICROSECONDS = 2000000;
const float NOTE_CHANGE_FREQUENCY = 20.0f;
const float MIDDLE_A_FREQUENCY = 440.0f;
const int NOTE_1_OFFSET = 24;
const int NOTE_2_OFFSET = 25;

Logging OutputUtil::logger = LogUtil::getLogger();
bool OutputUtil::outputPinsInitialized = false;
pin_size_t OutputUtil::ledPin;
pin_size_t OutputUtil::buzzerPowerPin;
pin_size_t OutputUtil::buzzerSignalPin;

void OutputUtil::initializeOutputPins(pin_size_t ledPin, pin_size_t buzzerPowerPin, pin_size_t buzzerSignalPin) {
    OutputUtil::ledPin = ledPin;
    OutputUtil::buzzerPowerPin = buzzerPowerPin;
    OutputUtil::buzzerSignalPin = buzzerSignalPin;

    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPowerPin, OUTPUT);
    pinMode(buzzerSignalPin, OUTPUT);

    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPowerPin, LOW);
    digitalWrite(buzzerSignalPin, LOW);

    outputPinsInitialized = true;
}

void OutputUtil::playSound() {
    if (!outputPinsInitialized) {
        logger.error(F("msg=\"Output pins have not been initialized.\""));
        return;
    }

    logger.info(F("msg=\"Playing sound.\""));

    float note1Frequency = changeFrequency(MIDDLE_A_FREQUENCY, NOTE_1_OFFSET);
    float note2Frequency = changeFrequency(MIDDLE_A_FREQUENCY, NOTE_2_OFFSET);

    int noteChangeHalfPeriod = (int) round(frequencyToHalfPeriod(NOTE_CHANGE_FREQUENCY));

    int microsecondsSinceLastNoteChange = 0;
    float currentFrequency = note1Frequency;

    digitalWrite(buzzerPowerPin, HIGH);

    for (int i = 0; i < SOUND_DURATION_MICROSECONDS; i++) {
        if (microsecondsSinceLastNoteChange >= noteChangeHalfPeriod) {
            if (currentFrequency == note1Frequency) {
                currentFrequency = note2Frequency;
            } else {
                currentFrequency = note1Frequency;
            }
            microsecondsSinceLastNoteChange = 0;
        }

        logger.info(F("time=%l"), i);
        float frequencies[] = {currentFrequency};
        float amplitude = getAmplitude(i, frequencies);
        logger.info(F("msg=\"Amplitude is %F.\""), amplitude);
        if (amplitude < -5.0f) {
            logger.info(F("msg=\"Setting to LOW."));
            digitalWrite(buzzerSignalPin, LOW);
        } else if (amplitude > 5.0f) {
            logger.info(F("msg=\"Setting to HIGH."));
            digitalWrite(buzzerSignalPin, HIGH);
        }

        delayMicroseconds(1);
        microsecondsSinceLastNoteChange++;
    }

    digitalWrite(buzzerSignalPin, LOW);
    digitalWrite(buzzerPowerPin, LOW);
}

void OutputUtil::turnLedOn() {
    if (!outputPinsInitialized) {
        logger.error(F("msg=\"Output pins have not been initialized.\""));
        return;
    }

    logger.info(F("msg=\"Turning LED on.\""));
    digitalWrite(ledPin, HIGH);
}

void OutputUtil::turnLedOff() {
    if (!outputPinsInitialized) {
        logger.error(F("msg=\"Output pins have not been initialized.\""));
        return;
    }

    logger.info(F("msg=\"Turning LED off.\""));
    digitalWrite(ledPin, LOW);
}

void OutputUtil::flashLed(unsigned long duration) {
    if (!outputPinsInitialized) {
        logger.error(F("msg=\"Output pins have not been initialized.\""));
        return;
    }

    logger.info(F("msg=\"Flashing LED.\", duration=%d"), duration);
    digitalWrite(ledPin, HIGH);
    delay(duration);
    digitalWrite(ledPin, LOW);
}

float OutputUtil::changeFrequency(float frequency, int halfSteps) {
    float sign = halfSteps >= 0 ? 1.0f : -1.0f;
    return frequency + sign * pow(2.0f, abs(halfSteps) / 12.0f);
}

float OutputUtil::frequencyToHalfPeriod(float frequency) {
    return ((1.0f / frequency) * 1000000.0f) / 2.0f;
}

float OutputUtil::getAmplitude(unsigned long time, const float frequencies[]) {
    float sum = 0.0f;
    for (int i = 0; i < sizeof(frequencies); i++) {
        float period = ((1.0f / frequencies[i]) * 1000000.0f);
        sum += sinf((float) time) * (2.0f * (float) M_PI); // should be in radians
//        sum += sinf((period * (float) time) / (2.0f * (float) M_PI));
    }
    return sum;
}
