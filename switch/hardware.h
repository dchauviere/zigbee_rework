#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "tl_common.h"

const enum hwState {
    HW_STATE_OFF,
    HW_STATE_ON,
};

void initHardware(void);
void setHardwareRelay(u8 relay, enum hwState state);
void setHardwareLed(u8 led, enum hwState state);

#endif
