#include "hal.h"
#include "puzzle.h"

static tl_bool switch_state[SWITCH_COUNT];
static tl_bool light_state[LIGHT_COUNT];

static void input(void) {
    for (int i = 0; i < SWITCH_COUNT; ++i) {
        switch_state[i] = hal_read(i);
    }
}

static void logic(void) {
    puzzle_update(switch_state, light_state);
}

static void render(void) {
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        hal_write(i, light_state[i]);
    }
}

void main_loop(void) {
    input();
    logic();
    render();
}
