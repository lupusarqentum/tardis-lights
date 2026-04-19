#include "libc_subset.h"
#include "hal.h"
#include "puzzle.h"

static bool switch_state[SWITCH_COUNT];
static bool light_state[LIGHT_COUNT];

static void input(void) {
    for (int i = 0; i < SWITCH_COUNT; ++i) {
        switch_state[i] = hal_read(i);
    }

    const char message_start[] = "Switch state just read: ";
    char buffer[sizeof(message_start) - 1 + SWITCH_COUNT + 2];
    memcpy(buffer, message_start, sizeof(message_start) - 1);
    for (int i = 0; i < SWITCH_COUNT; ++i) {
        buffer[sizeof(message_start) - 1 + i] = switch_state[i] ? '1' : '0';
    }
    buffer[sizeof(message_start) - 1 + SWITCH_COUNT + 0] = '\n';
    buffer[sizeof(message_start) - 1 + SWITCH_COUNT + 1] = '\0';
    hal_log(buffer);
}

static void logic(void) {
    puzzle_update(switch_state, light_state);
}

static void render(void) {
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        hal_write(i, light_state[i]);
    }

    const char message_start[] = "Light state just rendered: ";
    char buffer[sizeof(message_start) - 1 + LIGHT_COUNT + 2];
    memcpy(buffer, message_start, sizeof(message_start) - 1);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        buffer[sizeof(message_start) - 1 + i] = light_state[i] ? '1' : '0';
    }
    buffer[sizeof(message_start) - 1 + LIGHT_COUNT + 0] = '\n';
    buffer[sizeof(message_start) - 1 + LIGHT_COUNT + 1] = '\0';
    hal_log(buffer);
}

void main_loop(void) {
    input();
    logic();
    render();
}
