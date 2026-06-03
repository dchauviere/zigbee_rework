#include "tl_common.h"
#include "zcl_include.h"
#include "endpointCfg.h"
#include "zclApp.h"
#include "switchApp.h"
#include "backlight.h"


backlightInternalAttr_t g_backlightInternalAttrs[BACKLIGHTS_NUM];

/*********************************************************************
 * @fn      setBacklight
 *
 * @brief   Set backlight state
 *
 * @param   None
 *
 * @return  None
 */
void setBacklight(u8 led, enum backlightState state) {
	u32 pin = GET_BACKLIGHT_PIN(led);
	printf("set backlight %d to %d\n", pin, state);
	drv_gpio_write(GET_BACKLIGHT_PIN(led), (u8)state);
}

void setBacklightOn(u8 led) {
	u32 pin = GET_BACKLIGHT_PIN(led);
	printf("set backlight %d on\n", pin);
	drv_gpio_write(GET_BACKLIGHT_PIN(led), (u8)BACKLIGHT_ON);
}

void setBacklightOff(u8 led) {
	u32 pin = GET_BACKLIGHT_PIN(led);
	printf("set backlight %d off\n", pin);
	drv_gpio_write(GET_BACKLIGHT_PIN(led), (u8)BACKLIGHT_OFF);
}

s32 backlightBlinkTimerCb(void *arg)
{
    u8 led = *(u8 *)arg;
	u32 interval = 0;

	if(g_backlightInternalAttrs[led].state == g_backlightInternalAttrs[led].originalState){
		g_backlightInternalAttrs[led].times--;
		if(g_backlightInternalAttrs[led].times <= 0){
			g_backlightInternalAttrs[led].timer = NULL;
			return -1;
		}
	}

	g_backlightInternalAttrs[led].state = !g_backlightInternalAttrs[led].state;
	if(g_backlightInternalAttrs[led].state){
        setBacklight(led, BACKLIGHT_ON);
		interval = g_backlightInternalAttrs[led].ledOnTime;
	}else{
        setBacklight(led, BACKLIGHT_OFF);
		interval = g_backlightInternalAttrs[led].ledOffTime;
	}

	return interval;
}

void startBacklightBlink(u8 led, u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	g_backlightInternalAttrs[led].times = times;

	if(!g_backlightInternalAttrs[led].timer){
		if(g_backlightInternalAttrs[led].originalState){
			setBacklight(led, BACKLIGHT_OFF);
			g_backlightInternalAttrs[led].state = 0;
			interval = ledOffTime;
		}else{
			setBacklight(led, BACKLIGHT_ON);
			g_backlightInternalAttrs[led].state = 1;
			interval = ledOnTime;
		}
		g_backlightInternalAttrs[led].ledOnTime = ledOnTime;
		g_backlightInternalAttrs[led].ledOffTime = ledOffTime;

		g_backlightInternalAttrs[led].timer = TL_ZB_TIMER_SCHEDULE(backlightBlinkTimerCb, NULL, interval);
	}
}

void stopBacklightBlink(u8 led)
{
	if(g_backlightInternalAttrs[led].timer){
		TL_ZB_TIMER_CANCEL(&g_backlightInternalAttrs[led].timer);

		g_backlightInternalAttrs[led].times = 0;
		if(g_backlightInternalAttrs[led].originalState){
			setBacklight(led, BACKLIGHT_ON);
		}else{
			setBacklight(led, BACKLIGHT_OFF);
		}
	}
}
