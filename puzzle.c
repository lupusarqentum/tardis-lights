// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include "puzzle.h"

#if SWITCH_COUNT != 5 || LIGHT_COUNT != 6
#error "Unexpected counts of switches and lights!"
#endif

#define A7 switch_state[0]
#define B7 switch_state[1]
#define C7 switch_state[2]
#define D7 switch_state[3]
#define E7 switch_state[4]

#define NOT(x) ((x) == false)
#define AND(x, y) ((x) && (y))
#define OR(x, y) ((x) || (y))
#define XOR(x, y) ((x) ^ (y))

void puzzle_update(const tl_bool switch_state[SWITCH_COUNT],
		   tl_bool light_state[LIGHT_COUNT])
{
	light_state[0] = AND(NOT(A7), B7);
	light_state[1] = AND(NOT(C7), NOT(D7));
	light_state[2] = E7;
	light_state[3] = AND(B7, E7);
	light_state[4] = AND(NOT(A7), NOT(D7));
	light_state[5] = XOR(B7, C7);
}
