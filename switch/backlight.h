#ifndef __BACKLIGHT_CTRL_H__
#define __BACKLIGHT_CTRL_H__

const enum backlightState {
    BACKLIGHT_OFF,
    BACKLIGHT_ON,
};

typedef struct{
	ev_timer_event_t *timer;
	u8 times;
	u8 originalState;
	u8 state;
	u16 ledOnTime;
	u16 ledOffTime;
}backlightInternalAttr_t;

void setBacklight(u8 led, enum backlightState state);
void setBacklightOn(u8 led);
void setBacklightOff(u8 led);
void stopBacklightBlink(u8 led);
void startBacklightBlink(u8 led, u8 times, u16 ledOnTime, u16 ledOffTime);

#endif
