#include <stdio.h>
#include "hal.h"
#include "puzzle.h"

static bool switch_state[SWITCH_COUNT];
static bool light_state[LIGHT_COUNT];

static void input(void) {
    for (int i = 0; i < SWITCH_COUNT; ++i) {
        switch_state[i] = hal_read(i);
    }

    const char message_template[] = "Switch state just read: %s\n";
    char read_line[SWITCH_COUNT + 1];
    read_line[SWITCH_COUNT] = '\0';
    char buffer[sizeof(read_line) + sizeof(message_template)];
    for (int i = 0; i < SWITCH_COUNT; ++i) {
        read_line[i] = switch_state[i] ? '1' : '0';
    }
    snprintf(buffer, sizeof(buffer), message_template, read_line);
    hal_log(buffer);
}

static void logic(void) {
    puzzle_update(switch_state, light_state);
}

static void render(void) {
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        hal_write(i, light_state[i]);
    }

    const char message_template[] = "Light state just rendered: %s\n";
    char rendered_line[LIGHT_COUNT + 1];
    rendered_line[LIGHT_COUNT] = '\0';
    char buffer[sizeof(rendered_line) + sizeof(message_template)];
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        rendered_line[i] = light_state[i] ? '1' : '0';
    }
    snprintf(buffer, sizeof(buffer), message_template, rendered_line);
    hal_log(buffer);
}

void main_loop(void) {
    input();
    logic();
    render();
}
