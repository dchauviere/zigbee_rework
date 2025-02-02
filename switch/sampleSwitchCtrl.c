#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "sampleSwitch.h"
#include "sampleSwitchCtrl.h"

extern void sampleSwitch_onOffInit(void);

const u16 RELAYS[2] = {RELAY1, RELAY2};
const u16 LEDS[2] = {LED1, LED2};

/*********************************************************************
 * @fn      switch_init
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void switch_init(void)
{
	drv_gpio_write(RELAY1, 0);
}

/*********************************************************************
 * @fn      switch_adjust
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void switch_adjust(void)
{
	sampleSwitch_onOffInit();
}

/*********************************************************************
 * @fn      hwSwitch_onOffUpdate
 *
 * @brief
 *
 * @param   onOff - onOff attribute value
 *
 * @return  None
 */
static void hwSwitch_onOffUpdate(u8 btn, bool onOff)
{
	if(onOff){
		drv_gpio_write(RELAYS[btn-1], 1);
		drv_gpio_write(LEDS[btn-1], 1);
	}else{
		drv_gpio_write(RELAYS[btn-1], 0);
		drv_gpio_write(LEDS[btn-1], 0);
	}
}

/*********************************************************************
 * @fn      switch_onOffUpdate
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
static void switch_onOffUpdate(u8 btn)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn-1);
	hwSwitch_onOffUpdate(btn, pOnOff->onOff);
}

/*********************************************************************
 * @fn      switch_refresh
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void switch_refresh(u8 btn, switchSta_e sta)
{
	switch(sta){
#if ZCL_ON_OFF_SUPPORT
		case SWITCH_STA_ON_OFF:
			switch_onOffUpdate(btn);
			break;
#endif
		default:
			return;
			break;
	}

	g_switchAppCtx.switchAttrsChanged = TRUE;
}

#endif
