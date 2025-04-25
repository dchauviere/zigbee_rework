#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "endpointCfg.h"
#include "zclApp.h"
#include "switchApp.h"
#include "hardware.h"
#include "relayCtrl.h"
#include "backlightCtrl.h"

#define ZCL_ONOFF_TIMER_INTERVAL		100    //the timer interval to change the offWaitTime/onTime attribute of the ONOFF cluster
static ev_timer_event_t *onWithTimedOffTimerEvt = NULL;


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
    u8 relayState = (u8)g_switchAppCtx.relayAttrs[relay].onOff;
	bool backlightState = true;
    
	setHardwareRelay(relay, relayState);
	switch(g_switchAppCtx.relayAttrs[relay].backlightMode) {
		case ZCL_ONOFF_BACKLIGHT_MODE_ON:
			backlightState = true;
			break;
		case ZCL_ONOFF_BACKLIGHT_MODE_OFF:
			backlightState = false;
			break;
		case ZCL_ONOFF_BACKLIGHT_MODE_ONOFF:
			backlightState = relayState;
			break;
		case ZCL_ONOFF_BACKLIGHT_MODE_OFFON:
			backlightState = !relayState;
			break;
	}
	setBacklight(relay, backlightState);
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
	zcl_onOffAttr_t *pOnOff = &g_switchAppCtx.relayAttrs[relay];
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

	zcl_sceneAttr_t *pScene = zcl_sceneAttrGet();
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
	g_switchAppCtx.relayAttrs[relay].globalSceneControl = FALSE;

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
	g_switchAppCtx.relayAttrs[relay].globalSceneControl = TRUE;

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
	zcl_onOffAttr_t *pOnOff = &g_switchAppCtx.relayAttrs[relay];

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
	zcl_onOffAttr_t *pOnOff = &g_switchAppCtx.relayAttrs[relay];

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

void saveRelayAll(void) {
	for(u8 r=0;r<RELAYS_NUM;r++){
		saveRelayState(r);
		saveRelayConfig(r);
	}
}

void restoreRelayAll(void) {
	for(u8 r=0;r<RELAYS_NUM;r++){
		restoreRelayState(r);
		restoreRelayConfig(r);
	}
}

/*********************************************************************
 * @fn      saveRelayState
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t saveRelayState(u8 relay)
{
	nv_sts_t st = NV_SUCC;
	bool changed = false;

	zcl_onOffAttr_t zcl_nv_onOff;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_ON_OFF_BASE + relay, sizeof(zcl_onOffAttr_t), (u8*)&zcl_nv_onOff);

	if(st == NV_SUCC){
		if((zcl_nv_onOff.onOff != g_switchAppCtx.relayAttrs[relay].onOff) || (zcl_nv_onOff.startUpOnOff != g_switchAppCtx.relayAttrs[relay].startUpOnOff) ||
		   	(zcl_nv_onOff.onTime != g_switchAppCtx.relayAttrs[relay].onTime) || (zcl_nv_onOff.offWaitTime != g_switchAppCtx.relayAttrs[relay].offWaitTime) ||
			(zcl_nv_onOff.globalSceneControl != g_switchAppCtx.relayAttrs[relay].globalSceneControl) || (zcl_nv_onOff.onSwitchTrigger != g_switchAppCtx.relayAttrs[relay].onSwitchTrigger) ||
			(zcl_nv_onOff.backlightMode != g_switchAppCtx.relayAttrs[relay].backlightMode) ){
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_ON_OFF_BASE + relay, sizeof(zcl_onOffAttr_t), (u8*)&g_switchAppCtx.relayAttrs[relay]);
	}

	return st;
}

/*********************************************************************
 * @fn      restoreRelayState
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t restoreRelayState(u8 relay)
{
	nv_sts_t st = NV_SUCC;

	zcl_onOffAttr_t zcl_nv_onOff;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_ON_OFF_BASE + relay, sizeof(zcl_onOffAttr_t), (u8*)&zcl_nv_onOff);

	if(st == NV_SUCC){
			g_switchAppCtx.relayAttrs[relay].onOff				= zcl_nv_onOff.onOff;
			g_switchAppCtx.relayAttrs[relay].globalSceneControl	= zcl_nv_onOff.globalSceneControl;
			g_switchAppCtx.relayAttrs[relay].onTime				= zcl_nv_onOff.onTime;
			g_switchAppCtx.relayAttrs[relay].offWaitTime		= zcl_nv_onOff.offWaitTime;
			g_switchAppCtx.relayAttrs[relay].startUpOnOff 		= zcl_nv_onOff.startUpOnOff;
			g_switchAppCtx.relayAttrs[relay].onSwitchTrigger    = zcl_nv_onOff.onSwitchTrigger;
			g_switchAppCtx.relayAttrs[relay].backlightMode 		= zcl_nv_onOff.backlightMode;
	}else{
			g_switchAppCtx.relayAttrs[relay].onOff				= 0x00;
			g_switchAppCtx.relayAttrs[relay].globalSceneControl	= 1;
			g_switchAppCtx.relayAttrs[relay].onTime				= 0x0000;
			g_switchAppCtx.relayAttrs[relay].offWaitTime		= 0x0000;
			g_switchAppCtx.relayAttrs[relay].startUpOnOff 		= ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON;
			g_switchAppCtx.relayAttrs[relay].onSwitchTrigger    = ZCL_ONOFF_ON_SWITCH_TRIGGER_SINGLE;
			g_switchAppCtx.relayAttrs[relay].backlightMode      = ZCL_ONOFF_BACKLIGHT_MODE_ONOFF;
	}

	return st;
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

	zcl_onOffSwitchCfgAttr_t zcl_nv_onOffSwitchCfg;

	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_APP_ON_OFF_SWITCH_CFG_BASE + relay, sizeof(zcl_onOffSwitchCfgAttr_t), (u8*)&zcl_nv_onOffSwitchCfg);

	if(st == NV_SUCC){
		if((zcl_nv_onOffSwitchCfg.switchMode != g_switchAppCtx.relayCfgAttrs[relay].switchMode) || (zcl_nv_onOffSwitchCfg.switchAction != g_switchAppCtx.relayCfgAttrs[relay].switchAction) ){
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_ON_OFF_SWITCH_CFG_BASE + relay, sizeof(zcl_onOffSwitchCfgAttr_t), (u8*)&g_switchAppCtx.relayCfgAttrs[relay]);
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

	zcl_onOffSwitchCfgAttr_t zcl_nv_onOffSwitchCfg;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_ON_OFF_SWITCH_CFG_BASE + relay, sizeof(zcl_onOffSwitchCfgAttr_t), (u8*)&zcl_nv_onOffSwitchCfg);

	if(st == NV_SUCC){
		g_switchAppCtx.relayCfgAttrs[relay].switchMode	= zcl_nv_onOffSwitchCfg.switchMode;
		g_switchAppCtx.relayCfgAttrs[relay].switchAction	= zcl_nv_onOffSwitchCfg.switchAction;
	}else{
		g_switchAppCtx.relayCfgAttrs[relay].switchMode	= ZCL_SWITCH_TYPE_TOGGLE;
		g_switchAppCtx.relayCfgAttrs[relay].switchAction	= ZCL_SWITCH_ACTION_TOGGLE;
	}

	return st;
}

#endif
