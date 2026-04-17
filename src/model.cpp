#include "model.hpp"

#if SWITCH_COUNT != 5 || LED_COUNT != 6
#error "Unexpected counts of switches and led!"
#endif

int model_in[SWITCH_COUNT];
int model_out[LED_COUNT];

void model_init(void) {
    model_update();
}

#define A7 model_in[0]
#define B7 model_in[1]
#define C7 model_in[2]
#define D7 model_in[3]
#define E7 model_in[4]

#define NOT(x) ((x) == false)
#define AND(x, y) ((x) && (y))
#define OR(x, y) ((x) || (y))
#define XOR(x, y) ((x) ^ (y))

void model_update(void) {
    model_out[0] = AND(NOT(A7), B7);
    model_out[1] = AND(NOT(C7), NOT(D7));
    model_out[2] = E7;
    model_out[3] = AND(B7, E7);
    model_out[4] = AND(NOT(A7), NOT(D7));
    model_out[5] = XOR(B7, C7);
}
