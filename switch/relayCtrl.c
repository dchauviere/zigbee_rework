/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "endpointCfg.h"
#include "zclApp.h"
#include "switchApp.h"
#include "relayCtrl.h"
#include "backlight.h"


#define ZCL_ONOFF_TIMER_INTERVAL		100    //the timer interval to change the offWaitTime/onTime attribute of the ONOFF cluster
static ev_timer_event_t *onWithTimedOffTimerEvt = NULL;

relayAttr_t g_relayAttr[RELAYS_NUM];


/*********************************************************************
 * @fn      initRelays
 *
 * @brief   Initialize relays
 *
 * @param   None
 *
 * @return  None
 */
void initRelays(void)
{
	for(u8 r=0;r<RELAYS_NUM;r++){
		refreshRelay(r);
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
void setHardwareRelay(u8 relay, bool state)
{
	drv_gpio_write(GET_RELAY_PIN(relay), (u8)state);
}

/*********************************************************************
 * @fn      refreshRelay
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */

 void refreshRelay(u8 relay) {
  u8 relayState = (u8)g_relayAttr[relay].onOff;
	enum backlightState state;
    
	setHardwareRelay(relay, relayState);
	switch(g_epConfigAttrs[relay].backlightMode) {
		case ZCL_EPCONFIG_BACKLIGHT_MODE_ON:
			state = BACKLIGHT_ON;
			break;
		case ZCL_EPCONFIG_BACKLIGHT_MODE_OFF:
			state = BACKLIGHT_OFF;
			break;
		case ZCL_EPCONFIG_BACKLIGHT_MODE_ONOFF:
			state = relayState ? BACKLIGHT_ON : BACKLIGHT_OFF;
			break;
		case ZCL_EPCONFIG_BACKLIGHT_MODE_OFFON:
			state = relayState ? BACKLIGHT_OFF : BACKLIGHT_ON;
			break;
	}
	setBacklight(relay, state);
}

/*********************************************************************
 * @fn      setRelay
 *
 * @brief
 *
 * @param   ZCL_CMD_ONOFF_ON / ZCL_ONOFF_STATUS_OFF / ZCL_CMD_ONOFF_TOGGLE
 *
 * @return  None
 */
void setRelay(u8 relay, u8 cmd)
{
	relayAttr_t *pOnOff = &g_relayAttr[relay];
	bool onOff = ZCL_ONOFF_STATUS_ON;

	if(cmd == ZCL_CMD_ONOFF_ON){
		onOff = ZCL_ONOFF_STATUS_ON;
	}else if(cmd == ZCL_CMD_ONOFF_OFF){
		onOff = ZCL_ONOFF_STATUS_OFF;
	}else if(cmd == ZCL_CMD_ONOFF_TOGGLE){
		onOff = (pOnOff->onOff == ZCL_ONOFF_STATUS_ON) ? ZCL_ONOFF_STATUS_OFF
													   : ZCL_ONOFF_STATUS_ON;
	}else{
		return;
	}

	//update attributes
	if(onOff == ZCL_ONOFF_STATUS_ON){
		pOnOff->globalSceneControl = TRUE;
		pOnOff->onOff = ZCL_ONOFF_STATUS_ON;
		if(pOnOff->onTime == 0){
			pOnOff->offWaitTime = 0;
		}
	}else{
		pOnOff->onOff = ZCL_ONOFF_STATUS_OFF;
		pOnOff->onTime = 0;
	}

	zcl_sceneAttr_t *pScene = &g_zcl_sceneAttrs;
	pScene->sceneValid = 0;

	refreshRelay(relay);
}

/*********************************************************************
 * @fn      setRelayOffWithEffectProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
void setRelayOffWithEffectProcess(u8 relay, zcl_onoff_offWithEffectCmd_t *cmd)
{
	g_relayAttr[relay].globalSceneControl = FALSE;

	//TODO:

	setRelay(relay, ZCL_CMD_ONOFF_OFF);
}

/*********************************************************************
 * @fn      setRelayOnWithRecallGlobalSceneProcess
 *
 * @brief
 *
 * @param   relay
 *
 * @return  None
 */
void setRelayOnWithRecallGlobalSceneProcess(u8 relay)
{
	g_relayAttr[relay].globalSceneControl = TRUE;

	//TODO:
}

/*********************************************************************
 * @fn      setRelayOnWithTimedOffTimerCb
 *
 * @brief   timer event to process the ON_WITH_TIMED_OFF command
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 setRelayOnWithTimedOffTimerCb(void *arg)
{
	u8 relay = *(u8 *)arg;
	relayAttr_t *pOnOff = &g_relayAttr[relay];

	if((pOnOff->onOff == ZCL_ONOFF_STATUS_ON) && pOnOff->onTime){
		pOnOff->onTime--;
		if(pOnOff->onTime <= 0){
			pOnOff->offWaitTime = 0;
			setRelay(relay, ZCL_CMD_ONOFF_OFF);
		}
	}

	if((pOnOff->onOff == ZCL_ONOFF_STATUS_OFF) && pOnOff->offWaitTime){
		pOnOff->offWaitTime--;
		if(pOnOff->offWaitTime <= 0){
			onWithTimedOffTimerEvt = NULL;
			return -1;
		}
	}

	if(pOnOff->onTime || pOnOff->offWaitTime){
		return 0;
	}else{
		onWithTimedOffTimerEvt = NULL;
		return -1;
	}
}

/*********************************************************************
 * @fn      setRelayOnWithTimedOffTimerStart
 *
 * @brief   start the onWithTimedOff timer
 *
 * @param
 *
 * @return
 */
static void setRelayOnWithTimedOffTimerStart(u8 relay)
{
	if(onWithTimedOffTimerEvt){
		TL_ZB_TIMER_CANCEL(&onWithTimedOffTimerEvt);
	}
	void *ptr = &relay;
	onWithTimedOffTimerEvt = TL_ZB_TIMER_SCHEDULE(setRelayOnWithTimedOffTimerCb, ptr, ZCL_ONOFF_TIMER_INTERVAL);
}

/*********************************************************************
 * @fn      setRelayOnWithTimedOffProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
void setRelayOnWithTimedOffProcess(u8 relay, zcl_onoff_onWithTimeOffCmd_t *cmd)
{
	relayAttr_t *pOnOff = &g_relayAttr[relay];

	if(cmd->onOffCtrl.bits.acceptOnlyWhenOn && (pOnOff->onOff == ZCL_ONOFF_STATUS_OFF)){
		return;
	}

	if(pOnOff->offWaitTime && (pOnOff->onOff == ZCL_ONOFF_STATUS_OFF)){
		pOnOff->offWaitTime = min2(pOnOff->offWaitTime, cmd->offWaitTime);
	}else{
		pOnOff->onTime = max2(pOnOff->onTime, cmd->onTime);
		pOnOff->offWaitTime = cmd->offWaitTime;
		setRelay(relay, ZCL_CMD_ONOFF_ON);
	}

	if((pOnOff->onTime < 0xFFFF) && (pOnOff->offWaitTime < 0xFFFF)){
		if(pOnOff->onTime || pOnOff->offWaitTime){
			setRelayOnWithTimedOffTimerStart(relay);
		}
	}
}


/*********************************************************************
 * @fn      saveRelayConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t saveRelayConfig(u8 relay)
{
	nv_sts_t st = NV_SUCC;
	bool changed = false;

	relayAttr_t l_relayAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_ON_OFF_BASE + relay, sizeof(relayAttr_t), (u8*)&l_relayAttr);

	if(st == NV_SUCC){
		if((l_relayAttr.onOff != g_relayAttr[relay].onOff) || (l_relayAttr.startUpOnOff != g_relayAttr[relay].startUpOnOff) ||
		   	(l_relayAttr.onTime != g_relayAttr[relay].onTime) || (l_relayAttr.offWaitTime != g_relayAttr[relay].offWaitTime) ||
			(l_relayAttr.globalSceneControl != g_relayAttr[relay].globalSceneControl) || (l_relayAttr.backlightMode != g_relayAttr[relay].backlightMode) ){
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_ON_OFF_BASE + relay, sizeof(relayAttr_t), (u8*)&g_relayAttr[relay]);
	}

	return st;
}

/*********************************************************************
 * @fn      restoreRelayConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t restoreRelayConfig(u8 relay)
{
	nv_sts_t st = NV_SUCC;

	relayAttr_t l_relayAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_ON_OFF_BASE + relay, sizeof(relayAttr_t), (u8*)&l_relayAttr);

	if(st == NV_SUCC){
		g_relayAttr[relay].onOff				      = l_relayAttr.onOff;
		g_relayAttr[relay].globalSceneControl	= l_relayAttr.globalSceneControl;
		g_relayAttr[relay].onTime				      = l_relayAttr.onTime;
		g_relayAttr[relay].offWaitTime			  = l_relayAttr.offWaitTime;
		g_relayAttr[relay].startUpOnOff 		  = l_relayAttr.startUpOnOff;
		g_relayAttr[relay].backlightMode 		  = l_relayAttr.backlightMode;
	}else{
		g_relayAttr[relay].onOff				      = 0x00;
		g_relayAttr[relay].globalSceneControl	= 1;
		g_relayAttr[relay].onTime				      = 0x0000;
		g_relayAttr[relay].offWaitTime			  = 0x0000;
		g_relayAttr[relay].startUpOnOff 		  = ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF;
		g_relayAttr[relay].backlightMode		  = ZCL_EPCONFIG_BACKLIGHT_MODE_ONOFF;
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_ON_OFF_BASE + relay, sizeof(relayAttr_t), (u8*)&g_relayAttr[relay]);
	}

	return st;
}
