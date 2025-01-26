#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "sampleSwitch.h"
#include "sampleSwitchCtrl.h"

extern void sampleSwitch_onOffInit(void);

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
static void hwSwitch_onOffUpdate(bool onOff)
{
	if(onOff){
		drv_gpio_write(RELAY1, 1);
	}else{
		drv_gpio_write(RELAY1, 0);
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
static void switch_onOffUpdate(void)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();
	hwSwitch_onOffUpdate(pOnOff->onOff);
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
void switch_refresh(switchSta_e sta)
{
	switch(sta){
#if ZCL_ON_OFF_SUPPORT
		case SWITCH_STA_ON_OFF:
			switch_onOffUpdate();
			break;
#endif
		default:
			return;
			break;
	}

	g_switchAppCtx.switchAttrsChanged = TRUE;
}

#endif
