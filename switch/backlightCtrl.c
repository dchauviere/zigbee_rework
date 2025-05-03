#include "tl_common.h"
#include "zcl_include.h"
#include "endpoints.h"
#include "zclApp.h"
#include "switchApp.h"
#include "hardware.h"
#include "backlightCtrl.h"

void initBacklight(void) {
    for (u8 l=0;l<LEDS_NUM;l++){
        setHardwareLed(l, HW_STATE_OFF);
    }
}

void setBacklight(u8 led, enum hwState state) {
    setHardwareLed(led, state);
}

void setBacklightOn(u8 led) {
    setHardwareLed(led, HW_STATE_ON);
}

void setBacklightOff(u8 led) {
    setHardwareLed(led, HW_STATE_OFF);
}

s32 backlightBlinkTimerCb(void *arg)
{
    u8 led = *(u8 *)arg;
	u32 interval = 0;

	if(g_switchAppCtx.backlightAttrs[led].state == g_switchAppCtx.backlightAttrs[led].originalState){
		g_switchAppCtx.backlightAttrs[led].times--;
		if(g_switchAppCtx.backlightAttrs[led].times <= 0){
			g_switchAppCtx.backlightAttrs[led].timer = NULL;
			return -1;
		}
	}

	g_switchAppCtx.backlightAttrs[led].state = !g_switchAppCtx.backlightAttrs[led].state;
	if(g_switchAppCtx.backlightAttrs[led].state){
        setHardwareLed(led, HW_STATE_ON);
		interval = g_switchAppCtx.backlightAttrs[led].ledOnTime;
	}else{
        setHardwareLed(led, HW_STATE_OFF);
		interval = g_switchAppCtx.backlightAttrs[led].ledOffTime;
	}

	return interval;
}

void startBacklightBlink(u8 led, u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	g_switchAppCtx.backlightAttrs[led].times = times;

	if(!g_switchAppCtx.backlightAttrs[led].timer){
		if(g_switchAppCtx.backlightAttrs[led].originalState){
			setHardwareLed(led, HW_STATE_OFF);
			g_switchAppCtx.backlightAttrs[led].state = 0;
			interval = ledOffTime;
		}else{
			setHardwareLed(led, HW_STATE_ON);
			g_switchAppCtx.backlightAttrs[led].state = 1;
			interval = ledOnTime;
		}
		g_switchAppCtx.backlightAttrs[led].ledOnTime = ledOnTime;
		g_switchAppCtx.backlightAttrs[led].ledOffTime = ledOffTime;

		g_switchAppCtx.backlightAttrs[led].timer = TL_ZB_TIMER_SCHEDULE(backlightBlinkTimerCb, NULL, interval);
	}
}

void stopBacklightBlink(u8 led)
{
	if(g_switchAppCtx.backlightAttrs[led].timer){
		TL_ZB_TIMER_CANCEL(&g_switchAppCtx.backlightAttrs[led].timer);

		g_switchAppCtx.backlightAttrs[led].times = 0;
		if(g_switchAppCtx.backlightAttrs[led].originalState){
			setHardwareLed(led, HW_STATE_ON);
		}else{
			setHardwareLed(led, HW_STATE_OFF);
		}
	}
}
