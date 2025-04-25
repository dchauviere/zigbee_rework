/********************************************************************************************************
 * @file    sampleSwitch.h
 *
 * @brief   This is the header file for sampleSwitch
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

#ifndef _SWITCH_APP_H_
#define _SWITCH_APP_H_

/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	u8 keyType; /* ERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
	 	 	 	 SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
	u8 key[16];	/* the key used */
}app_linkKey_info_t;

typedef struct{
	u8 level;
	bool dir;
	u16 transitionTime;
}app_buttonAttr_t;

typedef struct{
	bool inverted;
	ev_timer_event_t *timer;
	u8 times;
	u8 originalState;
	u8 state;
	u16 ledOnTime;
	u16 ledOffTime;
}app_backlight_t;

typedef struct{
	ev_timer_event_t *bdbFBTimerEvt;
	ev_timer_event_t *timerLedEvt;
    s32 Vbat;		//current voltage

	u16 ledOnTime;
	u16 ledOffTime;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
	u8  state;

	bool relayCfgAttrsChanged;

	u32 keyPressedTime;
	u32 actionTime;

	app_linkKey_info_t tcLinkKey;

	zcl_onOffAttr_t relayAttrs[RELAYS_NUM];
	zcl_onOffSwitchCfgAttr_t relayCfgAttrs[BUTTON_NUM];
	app_buttonAttr_t buttonAttrs[BUTTON_NUM];
	app_backlight_t backlightAttrs[LEDS_NUM];
}app_ctx_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t g_switchAppCtx;

extern bdb_appCb_t g_zbDemoBdbCb;

extern bdb_commissionSetting_t g_bdbCommissionSetting;

#endif /* _SAMPLE_SWITCH_H_ */
