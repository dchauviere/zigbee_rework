/********************************************************************************************************
 * @file    zcl_onOffCb.c
 *
 * @brief   This is the source file for zcl_onOffCb
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "endpointCfg.h"
#include "zclApp.h"
#include "switchCtrl.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define ZCL_ONOFF_TIMER_INTERVAL		100    //the timer interval to change the offWaitTime/onTime attribute of the ONOFF cluster

/**********************************************************************
 * LOCAL VARIABLES
 */
static ev_timer_event_t *onWithTimedOffTimerEvt = NULL;

zcl_onOffAttr_t g_zcl_onOffAttrs[BUTTON_NUM];

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      switch_onOffInit
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void switch_onOffInit(void)
{
	zcl_onOffAttr_t *pOnOff1 = zcl_onoffAttrGet(0);
	zcl_onOffAttr_t *pOnOff2 = zcl_onoffAttrGet(1);

	switch_onOffUpdate(1, pOnOff1->onOff);
	switch_onOffUpdate(2, pOnOff2->onOff);
}

/*********************************************************************
 * @fn      switch_onOffUpdate
 *
 * @brief
 *
 * @param   ZCL_CMD_ONOFF_ON / ZCL_ONOFF_STATUS_OFF / ZCL_CMD_ONOFF_TOGGLE
 *
 * @return  None
 */
void switch_onOffUpdate(u8 btn, u8 cmd)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn-1);
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

	switch_refresh(btn, SWITCH_STA_ON_OFF);
}

/*********************************************************************
 * @fn      switch_onWithTimedOffTimerCb
 *
 * @brief   timer event to process the ON_WITH_TIMED_OFF command
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 switch_onWithTimedOffTimerCb(void *arg)
{
	u8 btn = *(u8 *)arg;
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn-1);

	if((pOnOff->onOff == ZCL_ONOFF_STATUS_ON) && pOnOff->onTime){
		pOnOff->onTime--;
		if(pOnOff->onTime <= 0){
			pOnOff->offWaitTime = 0;
			switch_onOffUpdate(btn, ZCL_CMD_ONOFF_OFF);
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
 * @fn      switch_onWithTimedOffTimerStart
 *
 * @brief   start the onWithTimedOff timer
 *
 * @param
 *
 * @return
 */
static void switch_onWithTimedOffTimerStart(u8 btn)
{
	if(onWithTimedOffTimerEvt){
		TL_ZB_TIMER_CANCEL(&onWithTimedOffTimerEvt);
	}
	void *ptr = &btn;
	onWithTimedOffTimerEvt = TL_ZB_TIMER_SCHEDULE(switch_onWithTimedOffTimerCb, ptr, ZCL_ONOFF_TIMER_INTERVAL);
}

/*********************************************************************
 * @fn      switch_onoff_onWithTimedOffProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void switch_onoff_onWithTimedOffProcess(u8 btn, zcl_onoff_onWithTimeOffCmd_t *cmd)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn-1);

	if(cmd->onOffCtrl.bits.acceptOnlyWhenOn && (pOnOff->onOff == ZCL_ONOFF_STATUS_OFF)){
		return;
	}

	if(pOnOff->offWaitTime && (pOnOff->onOff == ZCL_ONOFF_STATUS_OFF)){
		pOnOff->offWaitTime = min2(pOnOff->offWaitTime, cmd->offWaitTime);
	}else{
		pOnOff->onTime = max2(pOnOff->onTime, cmd->onTime);
		pOnOff->offWaitTime = cmd->offWaitTime;
		switch_onOffUpdate(btn, ZCL_CMD_ONOFF_ON);
	}

	if((pOnOff->onTime < 0xFFFF) && (pOnOff->offWaitTime < 0xFFFF)){
		if(pOnOff->onTime || pOnOff->offWaitTime){
			switch_onWithTimedOffTimerStart(btn);
		}
	}
}

/*********************************************************************
 * @fn      switch_onoff_offWithEffectProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void switch_onoff_offWithEffectProcess(u8 btn, zcl_onoff_offWithEffectCmd_t *cmd)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn-1);
	pOnOff->globalSceneControl = FALSE;

	//TODO:

	switch_onOffUpdate(btn, ZCL_CMD_ONOFF_OFF);
}

/*********************************************************************
 * @fn      switch_onoff_onWithRecallGlobalSceneProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void switch_onoff_onWithRecallGlobalSceneProcess(u8 btn)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn-1);
	pOnOff->globalSceneControl = TRUE;

	//TODO:
}

/*********************************************************************
 * @fn      switch_onOffCb
 *
 * @brief   Handler for ZCL ONOFF command. This function will set ONOFF attribute first.
 *
 * @param	pAddrInfo
 * @param   cmdId - onoff cluster command id
 * @param	cmdPayload
 *
 * @return  status_t
 */
status_t switch_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp <= BUTTON_NUM){
		switch(cmdId){
			case ZCL_CMD_ONOFF_ON:
			case ZCL_CMD_ONOFF_OFF:
			case ZCL_CMD_ONOFF_TOGGLE:
				switch_onOffUpdate(pAddrInfo->dstEp, cmdId);
				break;
			case ZCL_CMD_OFF_WITH_EFFECT:
				switch_onoff_offWithEffectProcess(pAddrInfo->dstEp, (zcl_onoff_offWithEffectCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_ON_WITH_RECALL_GLOBAL_SCENE:
				switch_onoff_onWithRecallGlobalSceneProcess(pAddrInfo->dstEp);
				break;
			case ZCL_CMD_ON_WITH_TIMED_OFF:
				switch_onoff_onWithTimedOffProcess(pAddrInfo->dstEp, (zcl_onoff_onWithTimeOffCmd_t *)cmdPayload);
				break;
			default:
				break;
		}
	}

	return ZCL_STA_SUCCESS;
}

/*********************************************************************
 * @fn      zcl_onOffAttr_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffAttr_save(void)
{
	nv_sts_t st = NV_SUCC;
	bool changed = false;

	zcl_onOffAttr_t zcl_nv_onOff[BUTTON_NUM];

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ON_OFF, sizeof(zcl_onOffAttr_t)*BUTTON_NUM, (u8*)&zcl_nv_onOff);

	if(st == NV_SUCC){
		for(u8 ep=0;ep<BUTTON_NUM;ep++){
			if((zcl_nv_onOff[ep].onOff != g_zcl_onOffAttrs[ep].onOff) || (zcl_nv_onOff[ep].startUpOnOff != g_zcl_onOffAttrs[ep].startUpOnOff) ||
		   		(zcl_nv_onOff[ep].onTime != g_zcl_onOffAttrs[ep].onTime) || (zcl_nv_onOff[ep].offWaitTime != g_zcl_onOffAttrs[ep].offWaitTime) ||
				(zcl_nv_onOff[ep].globalSceneControl != g_zcl_onOffAttrs[ep].globalSceneControl) ){
				changed = true;
			}
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ON_OFF, sizeof(zcl_onOffAttr_t)*BUTTON_NUM, (u8*)&g_zcl_onOffAttrs);
	}

	return st;
}

/*********************************************************************
 * @fn      zcl_onOffAttr_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffAttr_restore(void)
{
	nv_sts_t st = NV_SUCC;

	zcl_onOffAttr_t zcl_nv_onOff[BUTTON_NUM];

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ON_OFF, sizeof(zcl_onOffAttr_t)*BUTTON_NUM, (u8*)&zcl_nv_onOff);

	if(st == NV_SUCC){
		for(u8 ep=0;ep<BUTTON_NUM;ep++){
			g_zcl_onOffAttrs[ep].onOff				= zcl_nv_onOff[ep].onOff;
			g_zcl_onOffAttrs[ep].globalSceneControl	= zcl_nv_onOff[ep].globalSceneControl;
			g_zcl_onOffAttrs[ep].onTime				= zcl_nv_onOff[ep].onTime;
			g_zcl_onOffAttrs[ep].offWaitTime		= zcl_nv_onOff[ep].offWaitTime;
			g_zcl_onOffAttrs[ep].startUpOnOff 		= zcl_nv_onOff[ep].startUpOnOff;
		}
	}else{
		for(u8 ep=0;ep<BUTTON_NUM;ep++){
			g_zcl_onOffAttrs[ep].onOff				= 0x00;
			g_zcl_onOffAttrs[ep].globalSceneControl	= 1;
			g_zcl_onOffAttrs[ep].onTime				= 0x0000;
			g_zcl_onOffAttrs[ep].offWaitTime		= 0x0000;
			g_zcl_onOffAttrs[ep].startUpOnOff 		= ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON;
		}
	}

	return st;
}

#endif  /* __PROJECT_TL_SWITCH__ */
