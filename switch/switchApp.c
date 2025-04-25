/********************************************************************************************************
 * @file    sampleSwitch.c
 *
 * @brief   This is the source file for sampleSwitch
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
#include "ota.h"
#include "zclApp.h"
#include "switchApp.h"
#include "app_ui.h"
#include "endpointCfg.h"
#include "zb_appCb.h"
#include "relayCtrl.h"
#include "backlightCtrl.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
app_ctx_t g_switchAppCtx;


//running code firmware information
ota_preamble_t switch_otaInfo = {
	.fileVer 			= FILE_VERSION,
	.imageType 			= IMAGE_TYPE,
	.manufacturerCode 	= MANUFACTURER_CODE_TELINK,
};


//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
	bdb_zdoStartDevCnf,//start device cnf cb
	NULL,//reset cnf cb
	NULL,//device announce indication cb
	switch_leaveIndHandler,//leave ind cb
	switch_leaveCnfHandler,//leave cnf cb
	NULL,//nwk update ind cb
	NULL,//permit join ind cb
	NULL,//nlme sync cnf cb
	NULL,//tc join ind cb
	NULL,//tc detects that the frame counter is near limit
};


/**
 *  @brief Definition for BDB finding and binding cluster
 */
u16 bdb_findBindClusterList[] =
{
	ZCL_CLUSTER_GEN_ON_OFF,
};

/**
 *  @brief Definition for BDB finding and binding cluster number
 */
#define FIND_AND_BIND_CLUSTER_NUM		(sizeof(bdb_findBindClusterList)/sizeof(bdb_findBindClusterList[0]))

/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
	.linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
	.linkKey.tcLinkKey.key = (u8 *)tcLinkKeyCentralDefault,       		//can use unique link key stored in NV

	.linkKey.distributeLinkKey.keyType = MASTER_KEY,
	.linkKey.distributeLinkKey.key = (u8 *)linkKeyDistributedMaster,  	//use linkKeyDistributedCertification before testing

	.linkKey.touchLinkKey.keyType = MASTER_KEY,
	.linkKey.touchLinkKey.key = (u8 *)touchLinkKeyMaster,   			//use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
	.touchlinkEnable = 1,												/* enable touch-link */
#else
	.touchlinkEnable = 0,												/* disable touch-link */
#endif
	.touchlinkChannel = DEFAULT_CHANNEL, 								/* touch-link default operation channel for target */
	.touchlinkLqiThreshold = 0xA0,			   							/* threshold for touch-link scan req/resp command */
};

/**********************************************************************
 * LOCAL VARIABLES
 */

ev_timer_event_t *sampleSwitchAttrsStoreTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
	/* Initialize ZB stack */
	zb_init();

	/* Register stack CB */
	zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{
	af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_WHEN_STIMULATED);

	af_nodeDescManuCodeUpdate(MANUFACTURER_CODE_TELINK);

	restoreRelayAll();
	restoreButtonConfigAll();

    /* Initialize ZCL layer */
	/* Register Incoming ZCL Foundation command/response messages */
	zcl_init(switch_zclProcessIncomingMsg);

	/* register endPoint */
	registerAllEndpoints();

	zcl_reportingTabInit();

	/* Register ZCL specific cluster information */
	registerAllZCL();

    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&endpoint_simpleDesc, &switch_otaInfo, &switch_otaCb);
}

s32 sampleSwitchAttrsStoreTimerCb(void *arg)
{
	saveRelayAll();
	saveButtonConfigAll();

	sampleSwitchAttrsStoreTimerEvt = NULL;
	return -1;
}

void sampleSwitchAttrsStoreTimerStart(void)
{
	if(sampleSwitchAttrsStoreTimerEvt){
		TL_ZB_TIMER_CANCEL(&sampleSwitchAttrsStoreTimerEvt);
	}
	sampleSwitchAttrsStoreTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSwitchAttrsStoreTimerCb, NULL, 200);
}

void sampleSwitchAttrsChk(void)
{
	if(g_switchAppCtx.relayCfgAttrsChanged){
		g_switchAppCtx.relayCfgAttrsChanged = FALSE;
		if(zb_isDeviceJoinedNwk()){
			sampleSwitchAttrsStoreTimerStart();
		}
	}
}

void led_init(void)
{
	light_init();
}

void app_task(void)
{
	app_key_handler();

	if(bdb_isIdle()){
		report_handler();
	}
}

static void sampleSwitchSysException(void)
{
	saveRelayAll();
	saveButtonConfigAll();
	SYSTEM_RESET();
}

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
	/* Initialize LEDs*/
	initBacklight();

	if(!isRetention){
		/* Initialize Stack */
		stack_init();

		/* Initialize user application */
		user_app_init();

		/* Register except handler for test */
		sys_exceptHandlerRegister(sampleSwitchSysException);

		/* User's Task */
		ev_on_poll(EV_POLL_IDLE, app_task);

		/* Load the pre-install code from flash */
		if(bdb_preInstallCodeLoad(&g_switchAppCtx.tcLinkKey.keyType, g_switchAppCtx.tcLinkKey.key) == RET_OK){
			g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = g_switchAppCtx.tcLinkKey.keyType;
			g_bdbCommissionSetting.linkKey.tcLinkKey.key = g_switchAppCtx.tcLinkKey.key;
		}

		bdb_findBindMatchClusterSet(FIND_AND_BIND_CLUSTER_NUM, bdb_findBindClusterList);

	    /* Set default reporting configuration */
    	u8 reportableChange = 0x00;
		for(u8 ep=1;ep<=BUTTON_NUM;ep++){
			bdb_defaultReportingCfg(ep, HA_PROFILE_ID, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF,
    						0x0000, 0x003c, (u8 *)&reportableChange);
		}

		/* Initialize BDB */
		u8 repower = drv_pm_deepSleep_flag_get() ? 0 : 1;
		bdb_init((af_simple_descriptor_t *)&endpoint_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, repower);
	}else{
		/* Re-config phy when system recovery from deep sleep with retention */
		mac_phyReconfig();
	}
}

#endif  /* __PROJECT_TL_SWITCH__ */
