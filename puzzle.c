// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include "puzzle.h"

#define GET(v, i) (((v) >> (i)) & 1)
#define PUT(v, bit)                                                            \
	(v) <<= 1;                                                             \
	(v) |= (bit)

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

static inline unsigned char puzzle_formulae_v1(unsigned char switch_state)
{
	unsigned char result = 0;

	PUT(result, XOR(B7, C7));
	PUT(result, AND(NOT(A7), NOT(D7)));
	PUT(result, AND(B7, E7));
	PUT(result, E7);
	PUT(result, AND(NOT(C7), NOT(D7)));
	PUT(result, AND(NOT(A7), B7));

	return result;
}

static inline unsigned char puzzle_formulae_v2(unsigned char switch_state)
{
	unsigned char result = 0;

	// the following branch helps compiler to produce code of less size
	if (!A20)
		return 0;

	PUT(result, AND(OR(XOR(A20, C20), XOR(D20, E20)), A20));
	PUT(result, AND(OR(XOR(A20, D20), XOR(B20, E20)), A20));
	PUT(result, AND(OR(XOR(B20, E20), XOR(C20, D20)), A20));
	PUT(result, AND(OR(XOR(E20, A20), XOR(B20, D20)), A20));
	PUT(result, AND(XOR(D20, C20), A20));
	PUT(result, AND(XOR(A20, B20), A20));

	return result;
}

unsigned char puzzle_update(unsigned char switch_state)
{
	if (switch_state == 0)
		return 0;

	return puzzle_formulae_v2(switch_state);
}
