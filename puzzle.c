// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include "puzzle.h"

#define GET(v, i) (((v) >> (i)) & 1)
#define SET(v, i, bit) ((v) |= (((bit) & 1) << (i)))

#define NOT(x) (!(x))
#define AND(x, y) ((x) && (y))
#define OR(x, y) ((x) || (y))
#define XOR(x, y) ((x) ^ (y))

#define A7 (GET(switch_state, 0))
#define B7 (GET(switch_state, 1))
#define C7 (GET(switch_state, 2))
#define D7 (GET(switch_state, 3))
#define E7 (GET(switch_state, 4))

#define A20 A7
#define B20 B7
#define C20 C7
#define D20 D7
#define E20 E7

// unsigned char puzzle_update(unsigned char switch_state)
// {
// 	unsigned char result = 0;

// 	SET(result, 0, AND(NOT(A7), B7));
// 	SET(result, 1, AND(NOT(C7), NOT(D7)));
// 	SET(result, 2, E7);
// 	SET(result, 3, AND(B7, E7));
// 	SET(result, 4, AND(NOT(A7), NOT(D7)));
// 	SET(result, 5, XOR(B7, C7));

// 	return result;
// }

unsigned char puzzle_update(unsigned char switch_state)
{
	unsigned char result = 0;

	if (!A20)
		return 0;

	SET(result, 0, AND(XOR(A20, B20), A20));
	SET(result, 1, AND(XOR(D20, C20), A20));
	SET(result, 2, AND(OR(XOR(E20, A20), XOR(B20, D20)), A20));
	SET(result, 3, AND(OR(XOR(B20, E20), XOR(C20, D20)), A20));
	SET(result, 4, AND(OR(XOR(A20, D20), XOR(B20, E20)), A20));
	SET(result, 5, AND(OR(XOR(A20, C20), XOR(D20, E20)), A20));

	return result;
}
