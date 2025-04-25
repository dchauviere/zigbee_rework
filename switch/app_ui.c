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

#if (__PROJECT_TL_SWITCH__)

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
#include "switchApp.h"
#include "app_ui.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */

/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */

void set_detect_voltage(s32 v){
    g_switchAppCtx.Vbat = v;
}

void app_processMomentary(u8 btn, bool released) {
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstEp = btn;
	dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif
	switch(g_switchAppCtx.relayCfgAttrs[btn].switchAction){
		case ZCL_SWITCH_ACTION_ON_OFF:
			if (released) {
				setRelay(btn, ZCL_CMD_ONOFF_OFF);
				zcl_onOff_offCmd(btn, &dstEpInfo, FALSE);
			} else {
				setRelay(btn, ZCL_CMD_ONOFF_ON);
				zcl_onOff_onCmd(btn, &dstEpInfo, FALSE);
			}
			break;
		case ZCL_SWITCH_ACTION_OFF_ON:
			if (released) {
				setRelay(btn, ZCL_CMD_ONOFF_ON);
				zcl_onOff_onCmd(btn, &dstEpInfo, FALSE);
			} else {
				setRelay(btn, ZCL_CMD_ONOFF_OFF);
				zcl_onOff_offCmd(btn, &dstEpInfo, FALSE);
			}
			break;
		case ZCL_SWITCH_ACTION_TOGGLE:
			setRelay(btn, ZCL_CMD_ONOFF_TOGGLE);
			zcl_onOff_toggleCmd(btn, &dstEpInfo, FALSE);
			break;
	}
}

void app_processToggle(u8 btn) {
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstEp = btn;
	dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif
	setRelay(btn, ZCL_CMD_ONOFF_TOGGLE);
	zcl_onOff_toggleCmd(btn, &dstEpInfo, FALSE);
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
#if FIND_AND_BIND_SUPPORT
		dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
		dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEpInfo.dstEp = btn;
		dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif

		moveToLvl_t move2Level;

		move2Level.optPresent = 0;
		move2Level.transitionTime = g_switchAppCtx.buttonAttrs[btn].transitionTime;
		move2Level.level = g_switchAppCtx.buttonAttrs[btn].level;

		zcl_level_move2levelCmd(btn, &dstEpInfo, FALSE, &move2Level);

		if(g_switchAppCtx.buttonAttrs[btn].dir){
			g_switchAppCtx.buttonAttrs[btn].level += APP_DEFAULT_ACTION_HOLD_STEP;
			if(g_switchAppCtx.buttonAttrs[btn].level >= 250){
				g_switchAppCtx.buttonAttrs[btn].dir = !g_switchAppCtx.buttonAttrs[btn].dir;
			}
		}else{
			g_switchAppCtx.buttonAttrs[btn].level -= APP_DEFAULT_ACTION_HOLD_STEP;
			if(g_switchAppCtx.buttonAttrs[btn].level <= 1){
				g_switchAppCtx.buttonAttrs[btn].dir = !g_switchAppCtx.buttonAttrs[btn].dir;
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

void app_key_handler(void){
	static u8 valid_keyCode = 0xff;
	static u8 nbClicks = 0x00;
	static u8 keyPressed = 0x00;

	if (g_switchAppCtx.state == APP_STATE_ACTION_CLICKS && clock_time_exceed(g_switchAppCtx.actionTime, 500*1000)){
		printf("Action clicks nbClicks=%d\n", nbClicks);
		app_processClicks(valid_keyCode, nbClicks);
		g_switchAppCtx.state = APP_STATE_IDLE;
  		nbClicks = 0;
	} else if (g_switchAppCtx.state == APP_STATE_WAIT_KEY_MODE && clock_time_exceed(g_switchAppCtx.keyPressedTime, APP_DEFAULT_ACTION_HOLD_THRESHOLD)) {
		g_switchAppCtx.state = APP_STATE_ACTION_HOLD;
		app_processHold(valid_keyCode);
		g_switchAppCtx.keyPressedTime = clock_time();
	} else if (g_switchAppCtx.state == APP_STATE_ACTION_HOLD && clock_time_exceed(g_switchAppCtx.keyPressedTime, APP_DEFAULT_ACTION_HOLD_TRANSITION)) {
		app_processHold(valid_keyCode);
		g_switchAppCtx.keyPressedTime = clock_time();
	}

	if(kb_scan_key(0, 1)){
		if(kb_event.cnt == 1){
			// Key Pressed
			printf("key pressed\n");
			valid_keyCode = kb_event.keycode[0] - 1;
			keyPressed = 1;
			g_switchAppCtx.keyPressedTime = clock_time();
			if (g_switchAppCtx.relayCfgAttrs[valid_keyCode].switchMode == ZCL_SWITCH_TYPE_MOMENTARY) {
				app_processMomentary(valid_keyCode, false);
				g_switchAppCtx.state = APP_STATE_WAIT_ACTION_END;
			} else if (g_switchAppCtx.relayCfgAttrs[valid_keyCode].switchMode == ZCL_SWITCH_TYPE_TOGGLE) {
				g_switchAppCtx.state = APP_STATE_WAIT_ACTION_END;
			} else {
				g_switchAppCtx.state = APP_STATE_WAIT_KEY_MODE;
			}
		}else{
			// Key Released
			printf("key released\n");
			keyPressed = 0;
			if (g_switchAppCtx.state == APP_STATE_WAIT_KEY_MODE) { 
				nbClicks++;
				g_switchAppCtx.actionTime = clock_time();
				g_switchAppCtx.state = APP_STATE_ACTION_CLICKS;
			} else if (g_switchAppCtx.state == APP_STATE_WAIT_ACTION_END) {
				if (clock_time_exceed(g_switchAppCtx.actionTime, 500*1000)) {
					app_processMomentary(valid_keyCode, true);
				}
				valid_keyCode = 0xff;
				g_switchAppCtx.state = APP_STATE_IDLE;
			} else {
				valid_keyCode = 0xff;
				g_switchAppCtx.state = APP_STATE_IDLE;
			}
		}
	}
}

void saveButtonConfigAll(void) {
	for (u8 b=0;b<BUTTON_NUM;b++) {
		saveButtonConfig(b);
	}
}

void restoreButtonConfigAll(void) {
	for (u8 b=0;b<BUTTON_NUM;b++) {
		restoreButtonConfig(b);
	}
}

/*********************************************************************
 * @fn      saveButtonConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t saveButtonConfig(u8 button)
{
	nv_sts_t st = NV_SUCC;
	bool changed = false;

	app_buttonAttr_t app_buttonAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_APP_BUTTON_BASE + button, sizeof(app_buttonAttr_t), (u8*)&app_buttonAttr);

	if(st == NV_SUCC){
		if((app_buttonAttr.level != g_switchAppCtx.buttonAttrs[button].level) || (app_buttonAttr.dir != g_switchAppCtx.buttonAttrs[button].dir) || (app_buttonAttr.transitionTime != g_switchAppCtx.buttonAttrs[button].transitionTime) ){
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_BUTTON_BASE + button, sizeof(app_buttonAttr_t), (u8*)&g_switchAppCtx.buttonAttrs[button]);
	}

	return st;
}

/*********************************************************************
 * @fn      restoreButtonConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t restoreButtonConfig(u8 button)
{
	nv_sts_t st = NV_SUCC;

	app_buttonAttr_t app_buttonAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_ON_OFF_SWITCH_CFG_BASE + button, sizeof(app_buttonAttr_t), (u8*)&app_buttonAttr);

	if(st == NV_SUCC){
		g_switchAppCtx.buttonAttrs[button].level	= app_buttonAttr.level;
		g_switchAppCtx.buttonAttrs[button].dir	= app_buttonAttr.dir;
		g_switchAppCtx.buttonAttrs[button].transitionTime	= app_buttonAttr.transitionTime;
	}else{
		g_switchAppCtx.buttonAttrs[button].level	= 1;
		g_switchAppCtx.buttonAttrs[button].dir	= TRUE;
		g_switchAppCtx.buttonAttrs[button].transitionTime = 0x0A;
	}

	return st;
}

#endif  /* __PROJECT_TL_SWITCH__ */
