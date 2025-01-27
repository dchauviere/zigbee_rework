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
#include "sampleSwitch.h"
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

void app_sendToggle(void) {
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstEp = SAMPLE_SWITCH_ENDPOINT;
	dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif
	zcl_onOff_toggleCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE);
}

void app_sendDblClick(void) {
	u8 relayState = !drv_gpio_read(RELAY1);
	drv_gpio_write(RELAY1, relayState);
	drv_gpio_write(LED1, relayState);
	if (relayState) {
		sampleSwitch_onOffUpdate(ZCL_CMD_ONOFF_ON);
	}else{
		sampleSwitch_onOffUpdate(ZCL_CMD_ONOFF_OFF);
	}
}

void app_sendHold(void) {
	static u8 lvl = 1;
	static bool dir = 1;

	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstEp = SAMPLE_SWITCH_ENDPOINT;
	dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif

	moveToLvl_t move2Level;

	move2Level.optPresent = 0;
	move2Level.transitionTime = 0x0A;
	move2Level.level = lvl;

	zcl_level_move2levelCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &move2Level);

	if(dir){
		lvl += 50;
		if(lvl >= 250){
			dir = 0;
		}
	}else{
		lvl -= 50;
		if(lvl <= 1){
			dir = 1;
		}
	}
}

s32 app_action(void) {
	if(zb_isDeviceJoinedNwk()){
		if (g_switchAppCtx.keyCode == VK_SW1 && g_switchAppCtx.nbClicks == 5 && g_switchAppCtx.longClick == true) {
    	  	// Factory Reset
			printf("factory reset\n");
			zb_factoryReset();
    	}
    	if (g_switchAppCtx.nbClicks == 1) {
    	  	if (g_switchAppCtx.longClick == false) {
    	    	// Send Toggle
				app_sendToggle();
				printf("send click\n");
    	  	} else {
    	    	// Send Hold
			  printf("send hold\n");
				app_sendHold();
    	  	}
    	}
    	if (g_switchAppCtx.nbClicks == 2 && g_switchAppCtx.longClick == false) {
    	  // Send double click
		  printf("send double click\n");
		  app_sendDblClick();
    	}
	} else {
		zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
	}
	g_switchAppCtx.state = APP_STATE_IDLE;
  	g_switchAppCtx.longClick = false;
  	g_switchAppCtx.nbClicks = 0;
}

void app_key_handler(void){
	static u8 valid_keyCode = 0xff;
	if (g_switchAppCtx.keyPressed == 0 && g_switchAppCtx.state == APP_STATE_ACTION){
		if (clock_time_exceed(g_switchAppCtx.actionTime, 500*1000)){
			app_action();
		}
	}
	if (g_switchAppCtx.keyPressed == 1 && clock_time_exceed(g_switchAppCtx.keyPressedTime, 2*1000*1000)) {
		g_switchAppCtx.longClick = true;
	}
	if(kb_scan_key(0, 1)){
		if(kb_event.cnt){
			// Key Pressed
			printf("key pressed\n");
			g_switchAppCtx.keyPressed = 1;
			g_switchAppCtx.state = APP_STATE_WAIT_RELEASE;
			// Stop action timer
			g_switchAppCtx.nbClicks++;
			// start long press timer
			g_switchAppCtx.keyCode = kb_event.keycode[0];
			g_switchAppCtx.longClick = false;
			g_switchAppCtx.keyPressedTime = clock_time();
			if(kb_event.cnt == 1){
				valid_keyCode = kb_event.keycode[0];
			}
		}else{
			// Key Released
			printf("key released longClick=%x nbClicks=%d\n", g_switchAppCtx.longClick, g_switchAppCtx.nbClicks);
			g_switchAppCtx.keyCode = 0xff;
			// start action timer
			g_switchAppCtx.state = APP_STATE_ACTION;
			g_switchAppCtx.actionTime = clock_time();
			valid_keyCode = 0xff;
			g_switchAppCtx.keyPressed = 0;
		}
	}
}

#endif  /* __PROJECT_TL_SWITCH__ */
