#include "version_cfg.h"

#include "hardware.h"

const u16 RELAYS_PIN[] = {
	RELAY1, 
#if RELAYS_NUM > 1
	RELAY2, 
#endif
#if RELAYS_NUM > 2
	RELAY3
#endif
};

const u16 LEDS_PIN[] = {
	LED1, 
#if RELAYS_NUM > 1
	LED2, 
#endif
#if RELAYS_NUM > 2
	LED3
#endif
};

/*********************************************************************
 * @fn      initHardware
 *
 * @brief   Initialize relays
 *
 * @param   None
 *
 * @return  None
 */
void initHardware(void)
{
	for(u8 r=0;r<RELAYS_NUM;r++){
		drv_gpio_write(RELAYS_PIN[r], (u8)HW_STATE_OFF);
	}
	for(u8 l=0;l<LEDS_NUM;l++){
		drv_gpio_write(LEDS_PIN[l], (u8)HW_STATE_OFF);
	}
}

/*********************************************************************
 * @fn      setHardwareRelay
 *
 * @brief   Set relay state
 *
 * @param   None
 *
 * @return  None
 */
void setHardwareRelay(u8 relay, enum hwState state)
{
	drv_gpio_write(RELAYS_PIN[relay], (u8)state);
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
