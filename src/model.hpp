#pragma once

#define SWITCH_COUNT 5
#define LED_COUNT 6

extern int model_in[SWITCH_COUNT];
extern int model_out[LED_COUNT];

void model_init(void);
void model_update(void);
