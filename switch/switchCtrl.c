#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "endpointCfg.h"
#include "switchApp.h"
#include "switchCtrl.h"

const u16 RELAYS[BUTTON_NUM] = RELAYS_PIN;
const u16 LEDS[BUTTON_NUM] = LEDS_PIN;

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
	for(u8 l=0;l<BUTTON_NUM;l++){
		drv_gpio_write(RELAYS[l], 0);
		drv_gpio_write(LEDS[l], 0);
	}
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
	switch_onOffInit();
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
		case SWITCH_STA_ON_OFF:
			switch_onOffUpdate(btn);
			break;
		default:
			return;
			break;
	}

	g_switchAppCtx.switchAttrsChanged = TRUE;
}

#endif
