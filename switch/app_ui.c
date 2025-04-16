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
#include "switchApp.h"
#include "switchCtrl.h"
#include "zclApp.h"
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
void relay_on(u32 pin)
{
	drv_gpio_write(pin, RELAY_ON);
}

void relay_off(u32 pin)
{
	drv_gpio_write(pin, RELAY_OFF);
}

void led_on(u32 pin)
{
	drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
	drv_gpio_write(pin, LED_OFF);
}

void light_on(void)
{
	led_on(LED_POWER);
}

void light_off(void)
{
	led_off(LED_POWER);
}

void light_init(void)
{
	led_off(LED_POWER);
}

s32 zclLightTimerCb(void *arg)
{
	u32 interval = 0;

	if(g_switchAppCtx.sta == g_switchAppCtx.oriSta){
		g_switchAppCtx.times--;
		if(g_switchAppCtx.times <= 0){
			g_switchAppCtx.timerLedEvt = NULL;
			return -1;
		}
	}

	g_switchAppCtx.sta = !g_switchAppCtx.sta;
	if(g_switchAppCtx.sta){
		light_on();
		interval = g_switchAppCtx.ledOnTime;
	}else{
		light_off();
		interval = g_switchAppCtx.ledOffTime;
	}

	return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	g_switchAppCtx.times = times;

	if(!g_switchAppCtx.timerLedEvt){
		if(g_switchAppCtx.oriSta){
			light_off();
			g_switchAppCtx.sta = 0;
			interval = ledOffTime;
		}else{
			light_on();
			g_switchAppCtx.sta = 1;
			interval = ledOnTime;
		}
		g_switchAppCtx.ledOnTime = ledOnTime;
		g_switchAppCtx.ledOffTime = ledOffTime;

		g_switchAppCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
	}
}

void light_blink_stop(void)
{
	if(g_switchAppCtx.timerLedEvt){
		TL_ZB_TIMER_CANCEL(&g_switchAppCtx.timerLedEvt);

		g_switchAppCtx.times = 0;
		if(g_switchAppCtx.oriSta){
			light_on();
		}else{
			light_off();
		}
	}
}

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
	switch(g_zcl_onOffSwitchCfgAttrs[btn].switchAction){
		case ZCL_SWITCH_ACTION_ON_OFF:
			if (released) {
				switch_onOffUpdate(btn, ZCL_CMD_ONOFF_OFF);
				zcl_onOff_offCmd(btn, &dstEpInfo, FALSE);
			} else {
				switch_onOffUpdate(btn, ZCL_CMD_ONOFF_ON);
				zcl_onOff_onCmd(btn, &dstEpInfo, FALSE);
			}
			break;
		case ZCL_SWITCH_ACTION_OFF_ON:
			if (released) {
				switch_onOffUpdate(btn, ZCL_CMD_ONOFF_ON);
				zcl_onOff_onCmd(btn, &dstEpInfo, FALSE);
			} else {
				switch_onOffUpdate(btn, ZCL_CMD_ONOFF_OFF);
				zcl_onOff_offCmd(btn, &dstEpInfo, FALSE);
			}
			break;
		case ZCL_SWITCH_ACTION_TOGGLE:
			switch_onOffUpdate(btn, ZCL_CMD_ONOFF_TOGGLE);
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
	switch_onOffUpdate(btn, ZCL_CMD_ONOFF_TOGGLE);
	zcl_onOff_toggleCmd(btn, &dstEpInfo, FALSE);
}

void app_processDblClick(u8 btn) {
	zcl_onOffAttr_t *onOffAttr = zcl_onoffAttrGet(btn-1);
	switch_refresh(btn, SWITCH_STA_ON_OFF);
}

void app_processHold(u8 btn) {
	static u8 lvl = 1;
	static bool dir = 1;

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
		move2Level.transitionTime = 0x0A;
		move2Level.level = lvl;

		zcl_level_move2levelCmd(btn, &dstEpInfo, FALSE, &move2Level);

		if(dir){
			lvl += APP_DEFAULT_ACTION_HOLD_STEP;
			if(lvl >= 250){
				dir = 0;
			}
		}else{
			lvl -= APP_DEFAULT_ACTION_HOLD_STEP;
			if(lvl <= 1){
				dir = 1;
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
			valid_keyCode = kb_event.keycode[0];
			keyPressed = 1;
			g_switchAppCtx.keyPressedTime = clock_time();
			if (g_zcl_onOffSwitchCfgAttrs[valid_keyCode].switchType == ZCL_SWITCH_TYPE_MOMENTARY) {
				app_processMomentary(valid_keyCode, false);
				g_switchAppCtx.state = APP_STATE_WAIT_ACTION_END;
			} else if (g_zcl_onOffSwitchCfgAttrs[valid_keyCode].switchType == ZCL_SWITCH_TYPE_TOGGLE) {
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

#endif  /* __PROJECT_TL_SWITCH__ */
