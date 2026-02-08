/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
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

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "endpointCfg.h"
#include "relayCtrl.h"
#include "zclApp.h"
#include "globalConfig.h"
#include "switchApp.h"
#include "switchCtrl.h"


/**********************************************************************
 * GLOBAL VARIABLES
 */
switchAttr_t g_switchAttr[BUTTON_NUM];


/**********************************************************************
 * TYPEDEFS
 */
enum{
	APP_STATE_IDLE,
	APP_STATE_WAIT_KEY_MODE,
	APP_STATE_ACTION_CLICKS,
	APP_STATE_ACTION_HOLD,
	APP_STATE_WAIT_ACTION_END,
};

typedef struct{
	u8 level;
	bool dir;
} switchState_t;


/**********************************************************************
 * LOCAL VARIABLES
 */
switchState_t l_switchState[BUTTON_NUM];


/**********************************************************************
 * LOCAL FUNCTIONS
 */

void initSwitches(void) {
	for(u8 sw=0;sw<BUTTON_NUM;sw++) {
		l_switchState[sw].dir = true;
		l_switchState[sw].level = 1;
		restoreSwitchConfig(sw);
	}
}

void app_processMomentary(u8 btn, bool released) {
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.profileId = HA_PROFILE_ID;
	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

	switch(g_switchAttr[btn].switchAction){
		case ZCL_SWITCH_ACTION_ON_OFF:
			if (released) {
				setRelay(btn, ZCL_CMD_ONOFF_OFF);
				zcl_onOff_offCmd(getEndpointFromSwitch(btn), &dstEpInfo, FALSE);
			} else {
				setRelay(btn, ZCL_CMD_ONOFF_ON);
				zcl_onOff_onCmd(getEndpointFromSwitch(btn), &dstEpInfo, FALSE);
			}
			break;
		case ZCL_SWITCH_ACTION_OFF_ON:
			if (released) {
				setRelay(btn, ZCL_CMD_ONOFF_ON);
				zcl_onOff_onCmd(getEndpointFromSwitch(btn), &dstEpInfo, FALSE);
			} else {
				setRelay(btn, ZCL_CMD_ONOFF_OFF);
				zcl_onOff_offCmd(getEndpointFromSwitch(btn), &dstEpInfo, FALSE);
			}
			break;
		case ZCL_SWITCH_ACTION_TOGGLE:
			if (g_switchAttr[btn].relayControlMode == ZCL_RELAY_CONTROL_MODE_TOGGLE) {
				setRelay(btn, ZCL_CMD_ONOFF_TOGGLE);
			}
				zcl_onOff_toggleCmd(getEndpointFromSwitch(btn), &dstEpInfo, FALSE);
			break;
	}
}

void app_processToggle(u8 btn) {
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.profileId = HA_PROFILE_ID;
	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

	if (g_switchAttr[btn].relayControlMode == ZCL_RELAY_CONTROL_MODE_TOGGLE) {
		setRelay(btn, ZCL_CMD_ONOFF_TOGGLE);
	}
	zcl_onOff_toggleCmd(getEndpointFromSwitch(btn), &dstEpInfo, FALSE);
}

void app_processDblClick(u8 btn) {
	//zcl_onOffAttr_t *onOffAttr = zcl_onoffAttrGet(btn-1);
	//refreshSwitch(btn);
}

void app_processHold(u8 btn) {

	if(zb_isDeviceJoinedNwk()){
		epInfo_t dstEpInfo;
		TL_SETSTRUCTCONTENT(dstEpInfo, 0);

		dstEpInfo.profileId = HA_PROFILE_ID;
		dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

		moveToLvl_t move2Level;

		move2Level.optPresent = 0;
		move2Level.transitionTime = g_switchAttr[btn].transitionTime;
		move2Level.level = l_switchState[btn].level;

		zcl_level_move2levelCmd(getEndpointFromSwitch(btn), &dstEpInfo, FALSE, &move2Level);

		if(l_switchState[btn].dir){
			l_switchState[btn].level += g_globalConfig.actionHoldStep;
			if(l_switchState[btn].level >= 250){
				l_switchState[btn].dir = !l_switchState[btn].dir;
			}
		}else{
			l_switchState[btn].level -= g_globalConfig.actionHoldStep;
			if(l_switchState[btn].level <= 1){
				l_switchState[btn].dir = !l_switchState[btn].dir;
			}
		}
	} else {
		zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
	}
}

void app_processClicks(u8 btn, u8 nbClicks) {
	if(zb_isDeviceJoinedNwk()){
    	if (nbClicks == 1) {
    	    // Send Toggle
			printf("send toggle\n");
			app_processToggle(btn);
    	} else if (nbClicks == 2) {
    	  // Send double click
		  printf("send double click\n");
		  app_processDblClick(btn);
    	} else if (btn == VK_SW1 && nbClicks == 5) {
    	  	// Factory Reset
			printf("factory reset\n");
			zb_factoryReset();
    	} else {
			printf("action with %d clicks not implemented", nbClicks);
		}
	} else {
		printf("try to rejoin network");
		zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
	}
}

void switchesHandler(void){
	static u8 valid_keyCode = 0xff;
	static u8 nbClicks = 0x00;
	static u32 resetTime = 0x00;
	static u32 keyPressedTime = 0x00;
	static u32 actionTime = 0x00;
	static u8  state = APP_STATE_IDLE;

	if (state == APP_STATE_ACTION_CLICKS && clock_time_exceed(actionTime, g_globalConfig.actionclickTransition*1000)){
		printf("Action clicks nbClicks=%d\n", nbClicks);
		app_processClicks(valid_keyCode, nbClicks);
		state = APP_STATE_IDLE;
  	nbClicks = 0;
	} else if (state == APP_STATE_WAIT_KEY_MODE && clock_time_exceed(keyPressedTime, g_globalConfig.actionHoldThreshold*1000)) {
		state = APP_STATE_ACTION_HOLD;
		app_processHold(valid_keyCode);
		keyPressedTime = clock_time();
	} else if (state == APP_STATE_ACTION_HOLD && clock_time_exceed(keyPressedTime, g_globalConfig.actionHoldTransition*1000)) {
		app_processHold(valid_keyCode);
		keyPressedTime = clock_time();
	}

	if(kb_scan_key(0, 1)){
		if(kb_event.cnt == 1){
			// Key Pressed
			printf("key pressed\n");
			resetTime = clock_time();
			valid_keyCode = kb_event.keycode[0] - 1;
			keyPressedTime = clock_time();
			if (g_switchAttr[valid_keyCode].switchMode == ZCL_SWITCH_TYPE_MOMENTARY) {
				app_processMomentary(valid_keyCode, false);
				state = APP_STATE_WAIT_ACTION_END;
			} else if (g_switchAttr[valid_keyCode].switchMode == ZCL_SWITCH_TYPE_TOGGLE) {
				state = APP_STATE_WAIT_ACTION_END;
			} else {
				state = APP_STATE_WAIT_KEY_MODE;
			}
		}else{
			// Key Released
			printf("key released\n");
			if (clock_time_exceed(resetTime, g_globalConfig.resetDuration*1000*1000)){
    	  // Factory Reset
			  printf("factory reset\n");
			  zb_factoryReset();  
			}
			
			if (state == APP_STATE_WAIT_KEY_MODE) { 
				nbClicks++;
				actionTime = clock_time();
				state = APP_STATE_ACTION_CLICKS;
			} else if (state == APP_STATE_WAIT_ACTION_END) {
				if (clock_time_exceed(actionTime, 500*1000)) {
					app_processMomentary(valid_keyCode, true);
				}
				valid_keyCode = 0xff;
				state = APP_STATE_IDLE;
			} else {
				valid_keyCode = 0xff;
				state = APP_STATE_IDLE;
			}
		}
	}
}


/*********************************************************************
 * @fn      saveSwitchConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t saveSwitchConfig(u8 sw)
{
	nv_sts_t st = NV_SUCC;
	bool changed = false;

	switchAttr_t l_switchAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_APP_SWITCH_BASE + sw, sizeof(switchAttr_t), (u8*)&l_switchAttr);

	if(st == NV_SUCC){
		if ((l_switchAttr.transitionTime != g_switchAttr[sw].transitionTime)
		|| (l_switchAttr.switchAction != g_switchAttr[sw].switchAction)
		|| (l_switchAttr.switchMode != g_switchAttr[sw].switchMode) ){
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_SWITCH_BASE + sw, sizeof(switchAttr_t), (u8*)&g_switchAttr[sw]);
	}

	return st;
}

/*********************************************************************
 * @fn      restoreSwitchConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t restoreSwitchConfig(u8 sw)
{
	nv_sts_t st = NV_SUCC;

	switchAttr_t l_switchAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_SWITCH_BASE + sw, sizeof(switchAttr_t), (u8*)&l_switchAttr);

	if(st == NV_SUCC){
		g_switchAttr[sw].transitionTime	= l_switchAttr.transitionTime;
		g_switchAttr[sw].switchAction = l_switchAttr.switchAction;
		g_switchAttr[sw].switchMode = l_switchAttr.switchMode;
	}else{
		g_switchAttr[sw].transitionTime = 0x0A;
		g_switchAttr[sw].switchAction = ZCL_SWITCH_ACTION_ON_OFF;
		g_switchAttr[sw].switchMode = ZCL_SWITCH_TYPE_TOGGLE;
	}

	return st;
}
