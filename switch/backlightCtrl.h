void initBacklight(void);
void setBacklight(u8 led, enum hwState state);
void setBacklightOn(u8 led);
void setBacklightOff(u8 led);
void stopBacklightBlink(u8 led);
void startBacklightBlink(u8 led, u8 times, u16 ledOnTime, u16 ledOffTime);
