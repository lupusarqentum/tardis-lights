#include <Arduino.h>
#include "model.hpp"

#if SWITCH_COUNT + LED_COUNT > 12
#error "Number of switches and lights is too big!"
#endif

static const int PINS_IN_OFFSET = 2;
static const int PINS_OUT_OFFSET = PINS_IN_OFFSET + SWITCH_COUNT;

static void hal_log(const char* msg) {
    Serial.print(msg);
}

static void hal_init(void) {
    for (int i = 0; i < SWITCH_COUNT; ++i) {
        pinMode(PINS_IN_OFFSET + i, INPUT_PULLUP);
    }
    for (int i = 0; i < LED_COUNT; ++i) {
        pinMode(PINS_OUT_OFFSET + i, OUTPUT);
    }
    Serial.begin(9600);
}

static void hal_log_read(void) {
    const char message_template[] = "Input state just read: %s\n";
    char read_line[SWITCH_COUNT + 1];
    read_line[SWITCH_COUNT] = '\0';
    char buffer[sizeof(read_line) + sizeof(message_template)];
    for (int i = 0; i < SWITCH_COUNT; ++i) {
        read_line[i] = model_in[i] ? '1' : '0';
    }
    snprintf(buffer, sizeof(buffer), message_template, read_line);
    hal_log(buffer);
}

static void hal_read(void) {
    for (int i = 0; i < SWITCH_COUNT; ++i) {
        model_in[i] = digitalRead(PINS_IN_OFFSET + i) == LOW;
    }
}

static void hal_log_render(void) {
    const char message_template[] = "Input state just read: %s\n";
    char rendered_line[LED_COUNT + 1];
    rendered_line[LED_COUNT] = '\0';
    char buffer[sizeof(rendered_line) + sizeof(message_template)];
    for (int i = 0; i < LED_COUNT; ++i) {
        rendered_line[i] = model_out[i] ? '1' : '0';
    }
    snprintf(buffer, sizeof(buffer), message_template, rendered_line);
    hal_log(buffer);
}

static void hal_render(void) {
    for (int i = 0; i < LED_COUNT; ++i) {
        digitalWrite(PINS_OUT_OFFSET + i, model_out[i] ? HIGH : LOW);
    }
}

void setup(void) {
    hal_init();
    hal_read();
    model_init();
}

void loop(void) {
    hal_read();
    model_update();
    hal_render();
    delay(750);
}
