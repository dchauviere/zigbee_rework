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
#include "sampleSwitch.h"
#include "sampleSwitchCtrl.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define ZCL_ONOFF_TIMER_INTERVAL		100    //the timer interval to change the offWaitTime/onTime attribute of the ONOFF cluster

/**********************************************************************
 * LOCAL VARIABLES
 */
static ev_timer_event_t *onWithTimedOffTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      sampleSwitch_onOffInit
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void sampleSwitch_onOffInit(void)
{
	zcl_onOffAttr_t *pOnOff1 = zcl_onoffAttrGet(0);
	zcl_onOffAttr_t *pOnOff2 = zcl_onoffAttrGet(1);

	sampleSwitch_onOffUpdate(1, pOnOff1->onOff);
	sampleSwitch_onOffUpdate(2, pOnOff2->onOff);
}

/*********************************************************************
 * @fn      sampleSwitch_onOffUpdate
 *
 * @brief
 *
 * @param   ZCL_CMD_ONOFF_ON / ZCL_ONOFF_STATUS_OFF / ZCL_CMD_ONOFF_TOGGLE
 *
 * @return  None
 */
void sampleSwitch_onOffUpdate(u8 btn, u8 cmd)
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

#ifdef ZCL_SCENE
	zcl_sceneAttr_t *pScene = zcl_sceneAttrGet();
	pScene->sceneValid = 0;
#endif

	switch_refresh(btn, SWITCH_STA_ON_OFF);
}

/*********************************************************************
 * @fn      sampleSwitch_onWithTimedOffTimerCb
 *
 * @brief   timer event to process the ON_WITH_TIMED_OFF command
 *
 * @param   arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 sampleSwitch_onWithTimedOffTimerCb(void *arg)
{
	u8 btn = *(u8 *)arg;
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn-1);

	if((pOnOff->onOff == ZCL_ONOFF_STATUS_ON) && pOnOff->onTime){
		pOnOff->onTime--;
		if(pOnOff->onTime <= 0){
			pOnOff->offWaitTime = 0;
			sampleSwitch_onOffUpdate(btn, ZCL_CMD_ONOFF_OFF);
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
 * @fn      sampleSwitch_onWithTimedOffTimerStart
 *
 * @brief   start the onWithTimedOff timer
 *
 * @param
 *
 * @return
 */
static void sampleSwitch_onWithTimedOffTimerStart(u8 btn)
{
	if(onWithTimedOffTimerEvt){
		TL_ZB_TIMER_CANCEL(&onWithTimedOffTimerEvt);
	}
	void *ptr = &btn;
	onWithTimedOffTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSwitch_onWithTimedOffTimerCb, ptr, ZCL_ONOFF_TIMER_INTERVAL);
}

/*********************************************************************
 * @fn      sampleSwitch_onoff_onWithTimedOffProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleSwitch_onoff_onWithTimedOffProcess(u8 btn, zcl_onoff_onWithTimeOffCmd_t *cmd)
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
		sampleSwitch_onOffUpdate(btn, ZCL_CMD_ONOFF_ON);
	}

	if((pOnOff->onTime < 0xFFFF) && (pOnOff->offWaitTime < 0xFFFF)){
		if(pOnOff->onTime || pOnOff->offWaitTime){
			sampleSwitch_onWithTimedOffTimerStart(btn);
		}
	}
}

/*********************************************************************
 * @fn      sampleSwitch_onoff_offWithEffectProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleSwitch_onoff_offWithEffectProcess(u8 btn, zcl_onoff_offWithEffectCmd_t *cmd)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn-1);
	pOnOff->globalSceneControl = FALSE;

	//TODO:

	sampleSwitch_onOffUpdate(btn, ZCL_CMD_ONOFF_OFF);
}

/*********************************************************************
 * @fn      sampleSwitch_onoff_onWithRecallGlobalSceneProcess
 *
 * @brief
 *
 * @param   cmd
 *
 * @return  None
 */
static void sampleSwitch_onoff_onWithRecallGlobalSceneProcess(u8 btn)
{
	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn);
	pOnOff->globalSceneControl = TRUE;

	//TODO:
}

/*********************************************************************
 * @fn      sampleSwitch_onOffCb
 *
 * @brief   Handler for ZCL ONOFF command. This function will set ONOFF attribute first.
 *
 * @param	pAddrInfo
 * @param   cmdId - onoff cluster command id
 * @param	cmdPayload
 *
 * @return  status_t
 */
status_t sampleSwitch_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == SAMPLE_SWITCH_ENDPOINT || pAddrInfo->dstEp == SAMPLE_SWITCH_ENDPOINT_2){
		switch(cmdId){
			case ZCL_CMD_ONOFF_ON:
			case ZCL_CMD_ONOFF_OFF:
			case ZCL_CMD_ONOFF_TOGGLE:
				sampleSwitch_onOffUpdate(pAddrInfo->dstEp, cmdId);
				break;
			case ZCL_CMD_OFF_WITH_EFFECT:
				sampleSwitch_onoff_offWithEffectProcess(pAddrInfo->dstEp, (zcl_onoff_offWithEffectCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_ON_WITH_RECALL_GLOBAL_SCENE:
				sampleSwitch_onoff_onWithRecallGlobalSceneProcess(pAddrInfo->dstEp);
				break;
			case ZCL_CMD_ON_WITH_TIMED_OFF:
				sampleSwitch_onoff_onWithTimedOffProcess(pAddrInfo->dstEp, (zcl_onoff_onWithTimeOffCmd_t *)cmdPayload);
				break;
			default:
				break;
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif  /* __PROJECT_TL_SWITCH__ */
