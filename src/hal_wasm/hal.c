#include "hal.h"
#include "main_loop.h"
#include "puzzle.h"
#include "libc_subset.h"

extern int jsReadSwitch(int);
extern void jsSetLightState(int, int);
extern void jsAddLights(int);
extern void jsAddSwitches(int);
extern void jsLog(int, int);

tl_bool hal_read(int switch_number) {
    return jsReadSwitch(switch_number) == 1;
}

void hal_write(int light_number, tl_bool value) {
    jsSetLightState(light_number, value ? 1 : 0);
}

void hal_log(const char* message) {
    jsLog((int)message, tl_strlen(message));
}

__attribute__((export_name("setup")))
void setup(void) {
    jsAddLights(LIGHT_COUNT);
    jsAddSwitches(SWITCH_COUNT);
    main_loop();
}

__attribute__((export_name("loop")))
void loop(void) {
    main_loop();
}
