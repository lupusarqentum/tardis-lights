#pragma once

#include "libc_subset.h"

#define SWITCH_COUNT 5
#define LIGHT_COUNT 6

void puzzle_update(const tl_bool switch_state[SWITCH_COUNT],
		   tl_bool light_state[LIGHT_COUNT]);
