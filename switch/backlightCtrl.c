#include "tl_common.h"
#include "zcl_include.h"
#include "endpointCfg.h"
#include "zclApp.h"
#include "switchApp.h"
#include "backlightCtrl.h"

const u16 LEDS_PIN[] = {
	LED1, 
#if RELAYS_NUM > 1
	LED2, 
#endif
#if RELAYS_NUM > 2
	LED3
#endif
};

app_backlight_t g_backlightAttrs[LEDS_NUM];

/*********************************************************************
 * @fn      initLeds
 *
 * @brief   Initialize leds
 *
 * @param   None
 *
 * @return  None
 */
void initLeds(void)
{
	for(u8 l=0;l<LEDS_NUM;l++){
		drv_gpio_write(LEDS_PIN[l], (u8)HW_STATE_OFF);
	}
}

/*********************************************************************
 * @fn      setHardwareLed
 *
 * @brief   Set relay state
 *
 * @param   None
 *
 * @return  None
 */
void setHardwareLed(u8 led, enum hwState state)
{
	drv_gpio_write(LEDS_PIN[led], (u8)state);
}

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

	if(g_backlightAttrs[led].state == g_backlightAttrs[led].originalState){
		g_backlightAttrs[led].times--;
		if(g_backlightAttrs[led].times <= 0){
			g_backlightAttrs[led].timer = NULL;
			return -1;
		}
	}

	g_backlightAttrs[led].state = !g_backlightAttrs[led].state;
	if(g_backlightAttrs[led].state){
        setHardwareLed(led, HW_STATE_ON);
		interval = g_backlightAttrs[led].ledOnTime;
	}else{
        setHardwareLed(led, HW_STATE_OFF);
		interval = g_backlightAttrs[led].ledOffTime;
	}

	return interval;
}

void startBacklightBlink(u8 led, u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	g_backlightAttrs[led].times = times;

	if(!g_backlightAttrs[led].timer){
		if(g_backlightAttrs[led].originalState){
			setHardwareLed(led, HW_STATE_OFF);
			g_backlightAttrs[led].state = 0;
			interval = ledOffTime;
		}else{
			setHardwareLed(led, HW_STATE_ON);
			g_backlightAttrs[led].state = 1;
			interval = ledOnTime;
		}
		g_backlightAttrs[led].ledOnTime = ledOnTime;
		g_backlightAttrs[led].ledOffTime = ledOffTime;

		g_backlightAttrs[led].timer = TL_ZB_TIMER_SCHEDULE(backlightBlinkTimerCb, NULL, interval);
	}
}

void stopBacklightBlink(u8 led)
{
	if(g_backlightAttrs[led].timer){
		TL_ZB_TIMER_CANCEL(&g_backlightAttrs[led].timer);

		g_backlightAttrs[led].times = 0;
		if(g_backlightAttrs[led].originalState){
			setHardwareLed(led, HW_STATE_ON);
		}else{
			setHardwareLed(led, HW_STATE_OFF);
		}
	}
}
