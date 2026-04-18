#include <Arduino.h>

#include "hal.h"
#include "puzzle.h"

#define ARDUINO_DIGITAL_PINS_COUNT 14
#define RESERVED_PINS_COUNT 2

static const int SWITCH_PINS_OFFSET = RESERVED_PINS_COUNT;
static const int LIGHT_PINS_OFFSET = SWITCH_PINS_OFFSET + SWITCH_COUNT;

#if SWITCH_COUNT + LED_COUNT + RESERVED_PINS_COUNT > ARDUINO_DIGITAL_PINS_COUNT
#error "Won't be able to allocate required number of digital pins!"
#endif

#define LIGHT_PIN(x) ((x) + LIGHT_PINS_OFFSET)
#define SWITCH_PIN(x) ((x) + SWITCH_PINS_OFFSET)

extern "C" bool hal_read(int switch_number) {
    return digitalRead(SWITCH_PIN(switch_number)) == LOW;
}

extern "C" void hal_write(int light_number, bool value) {
    digitalWrite(LIGHT_PIN(light_number), value ? HIGH : LOW)
}

extern "C" void hal_log(const char* message) {
    Serial.print(message);
}

void start() {
    for (int i = 0; i < SWITCH_COUNT; ++i) {
        pinMode(SWITCH_PIN(i), INPUT_PULLUP);
    }
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        pinMode(LIGHT_PIN(i), OUTPUT);
    }
    Serial.begin(9600);
}

void loop() {
    main_loop();
    delay(750);
}
