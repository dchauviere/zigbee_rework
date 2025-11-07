#ifndef __BACKLIGHT_CTRL_H__
#define __BACKLIGHT_CTRL_H__

typedef struct{
	bool inverted;
	ev_timer_event_t *timer;
	u8 times;
	u8 originalState;
	u8 state;
	u16 ledOnTime;
	u16 ledOffTime;
}app_backlight_t;

extern app_backlight_t g_backlightAttrs[RELAYS_NUM];

const enum hwState {
    HW_STATE_OFF,
    HW_STATE_ON,
};

void initLeds(void);
void setHardwareLed(u8 led, enum hwState state);

void initBacklight(void);
void setBacklight(u8 led, enum hwState state);
void setBacklightOn(u8 led);
void setBacklightOff(u8 led);
void stopBacklightBlink(u8 led);
void startBacklightBlink(u8 led, u8 times, u16 ledOnTime, u16 ledOffTime);

#endif
