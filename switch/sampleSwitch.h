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

#ifndef _SAMPLE_SWITCH_H_
#define _SAMPLE_SWITCH_H_


/**********************************************************************
 * CONSTANT
 */
#define SAMPLE_SWITCH_ENDPOINT  0x01

/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	u8 keyType; /* ERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
	 	 	 	 SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
	u8 key[16];	/* the key used */
}app_linkKey_info_t;

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

	bool switchAttrsChanged;

	u32 keyPressedTime;
	u32 actionTime;

	app_linkKey_info_t tcLinkKey;
}app_ctx_t;

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct{
	u8 	zclVersion;
	u8	appVersion;
	u8	stackVersion;
	u8	hwVersion;
	u8	manuName[ZCL_BASIC_MAX_LENGTH];
	u8	modelId[ZCL_BASIC_MAX_LENGTH];
	u8	swBuildId[ZCL_BASIC_MAX_LENGTH];
	u8	powerSource;
	u8  genDevClass;                        //attr 8
	u8  genDevType;                         //attr 9
	u8	deviceEnable;
}zcl_basicAttr_t;

/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
	u16	identifyTime;
}zcl_identifyAttr_t;

/**
 *  @brief Defined for group cluster attributes
 */
typedef struct{
	u8	nameSupport;
}zcl_groupAttr_t;

/**
 *  @brief Defined for scene cluster attributes
 */
typedef struct{
	u8	 sceneCount;
	u8	 currentScene;
	u8	 nameSupport;
	bool sceneValid;
	u16	 currentGroup;
}zcl_sceneAttr_t;

/**
 *  @brief Defined for on/off cluster attributes
 */
typedef struct{
	u16	 onTime;
	u16	 offWaitTime;
	u8	 startUpOnOff;
	bool onOff;
	bool globalSceneControl;
}zcl_onOffAttr_t;

/**
 *  @brief  Defined for poll control cluster attributes
 */
typedef struct{
	u32	chkInInterval;
	u32	longPollInterval;
	u32	chkInIntervalMin;
	u32	longPollIntervalMin;
	u16	shortPollInterval;
	u16	fastPollTimeout;
	u16	fastPollTimeoutMax;
}zcl_pollCtrlAttr_t;

/**
 *  @brief Defined for saving on/off attributes
 */
typedef struct {
	u8	onOff;
	u8	startUpOnOff;
}zcl_nv_onOff_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t g_switchAppCtx;

extern bdb_appCb_t g_zbDemoBdbCb;

extern bdb_commissionSetting_t g_bdbCommissionSetting;

extern u8 SAMPLE_SWITCH_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_sampleSwitchClusterList[];
extern const af_simple_descriptor_t sampleSwitch_simpleDesc;

/* Attributes */
extern zcl_basicAttr_t g_zcl_basicAttrs;
extern zcl_identifyAttr_t g_zcl_identifyAttrs;
extern zcl_groupAttr_t g_zcl_groupAttrs;
extern zcl_sceneAttr_t g_zcl_sceneAttrs;
extern zcl_onOffAttr_t g_zcl_onOffAttrs; // for relay1
extern zcl_pollCtrlAttr_t g_zcl_pollCtrlAttrs;

#define zcl_sceneAttrGet()		&g_zcl_sceneAttrs
#define zcl_onoffAttrGet()		&g_zcl_onOffAttrs
#define zcl_pollCtrlAttrGet()	&g_zcl_pollCtrlAttrs
/**********************************************************************
 * FUNCTIONS
 */
void sampleSwitch_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t sampleSwitch_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleSwitch_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleSwitch_groupCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleSwitch_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleSwitch_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleSwitch_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

void sampleSwitch_onoff(u8 cmd);
void sampleSwitch_onOffUpdate(u8 cmd);

void sampleSwitch_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void sampleSwitch_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void sampleSwitch_otaProcessMsgHandler(u8 evt, u8 status);

void zcl_sampleSwitchAttrsInit(void);
nv_sts_t zcl_onOffAttr_save(void);

#endif /* _SAMPLE_SWITCH_H_ */
