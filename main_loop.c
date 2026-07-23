// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include "hal.h"
#include "puzzle.h"

void main_loop(void)
{
        u8 switch_state;
        u8 light_state;

        switch_state = hal_read();
        light_state = puzzle_update(switch_state);
        hal_write(light_state);
}
