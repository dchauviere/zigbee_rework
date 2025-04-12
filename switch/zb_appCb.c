/********************************************************************************************************
 * @file    zb_appCb.c
 *
 * @brief   This is the source file for zb_appCb
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
#include "switchApp.h"
#include "endpointCfg.h"
#include "zclApp.h"
#include "zb_appCb.h"
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
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork);
void zbdemo_bdbCommissioningCb(u8 status, void *arg);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zbdemo_bdbFindBindSuccessCb(findBindDst_t *pDstInfo);


/**********************************************************************
 * LOCAL VARIABLES
 */
bdb_appCb_t g_zbDemoBdbCb =
{
	zbdemo_bdbInitCb,
	zbdemo_bdbCommissioningCb,
	zbdemo_bdbIdentifyCb,
	zbdemo_bdbFindBindSuccessCb
};

ota_callBack_t switch_otaCb =
{
	switch_otaProcessMsgHandler,
};

ev_timer_event_t *steerTimerEvt = NULL;
ev_timer_event_t *switchRejoinBackoffTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */
s32 switch_bdbNetworkSteerStart(void *arg){
	bdb_networkSteerStart();

	steerTimerEvt = NULL;
	return -1;
}

s32 switch_bdbFindAndBindStart(void *arg){
	BDB_ATTR_GROUP_ID_SET(0x1234);//only for initiator
	bdb_findAndBindStart(BDB_COMMISSIONING_ROLE_INITIATOR);

	g_switchAppCtx.bdbFBTimerEvt = NULL;
	return -1;
}

s32 switch_rejoinBacckoff(void *arg){
	if(zb_isDeviceFactoryNew()){
		switchRejoinBackoffTimerEvt = NULL;
		return -1;
	}

	zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
    return 0;
}

/*********************************************************************
 * @fn      zbdemo_bdbInitCb
 *
 * @brief   application callback for bdb initiation
 *
 * @param   status - the status of bdb init BDB_INIT_STATUS_SUCCESS or BDB_INIT_STATUS_FAILURE
 *
 * @param   joinedNetwork  - 1: node is on a network, 0: node isn't on a network
 *
 * @return  None
 */
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork){
//	printf("bdbInitCb: sta = %x, joined = %x\n", status, joinedNetwork);

	if(status == BDB_INIT_STATUS_SUCCESS){
		/*
		 * for non-factory-new device:
		 * 		load zcl data from NV, start poll rate, start ota query, bdb_networkSteerStart
		 *
		 * for factory-new device:
		 * 		steer a network
		 *
		 */
		if(joinedNetwork){
			zb_setPollRate(POLL_RATE * 3);

			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);

		}else{
			u16 jitter = 0;
			do{
				jitter = zb_random() % 0x0fff;
			}while(jitter == 0);

			if(steerTimerEvt){
				TL_ZB_TIMER_CANCEL(&steerTimerEvt);
			}
			steerTimerEvt = TL_ZB_TIMER_SCHEDULE(switch_bdbNetworkSteerStart, NULL, jitter);
		}
	}else{
		if(joinedNetwork){
//			zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
			if(!switchRejoinBackoffTimerEvt){
				switchRejoinBackoffTimerEvt = TL_ZB_TIMER_SCHEDULE(switch_rejoinBacckoff, NULL, 60 * 1000);
			}
		}
	}
}

/*********************************************************************
 * @fn      zbdemo_bdbCommissioningCb
 *
 * @brief   application callback for bdb commissioning
 *
 * @param   status - the status of bdb commissioning
 *
 * @param   arg
 *
 * @return  None
 */
void zbdemo_bdbCommissioningCb(u8 status, void *arg){
//	printf("bdbCommCb: sta = %x\n", status);

	switch(status){
		case BDB_COMMISSION_STA_SUCCESS:
			light_blink_start(2, 200, 200);

			zb_setPollRate(POLL_RATE * 3);

			if(steerTimerEvt){
				TL_ZB_TIMER_CANCEL(&steerTimerEvt);
			}

			if(switchRejoinBackoffTimerEvt){
				TL_ZB_TIMER_CANCEL(&switchRejoinBackoffTimerEvt);
			}
			if(!g_zbNwkCtx.joined){
				zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
			}

			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
			//start Finding & Binding
			if(!g_switchAppCtx.bdbFBTimerEvt){
				g_switchAppCtx.bdbFBTimerEvt = TL_ZB_TIMER_SCHEDULE(switch_bdbFindAndBindStart, NULL, 50);
			}
			break;
		case BDB_COMMISSION_STA_IN_PROGRESS:
			break;
		case BDB_COMMISSION_STA_NOT_AA_CAPABLE:
			break;
		case BDB_COMMISSION_STA_NO_NETWORK:
		case BDB_COMMISSION_STA_TCLK_EX_FAILURE:
		case BDB_COMMISSION_STA_TARGET_FAILURE:
			{
				u16 jitter = 0;
				do{
					jitter = zb_random() % 0x0fff;
				}while(jitter == 0);

				if(steerTimerEvt){
					TL_ZB_TIMER_CANCEL(&steerTimerEvt);
				}
				steerTimerEvt = TL_ZB_TIMER_SCHEDULE(switch_bdbNetworkSteerStart, NULL, jitter);
			}
			break;
		case BDB_COMMISSION_STA_FORMATION_FAILURE:
			break;
		case BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE:
			break;
		case BDB_COMMISSION_STA_BINDING_TABLE_FULL:
			break;
		case BDB_COMMISSION_STA_NO_SCAN_RESPONSE:
			break;
		case BDB_COMMISSION_STA_NOT_PERMITTED:
			break;
		case BDB_COMMISSION_STA_PARENT_LOST:
			//zb_rejoinSecModeSet(REJOIN_INSECURITY);
			zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
//			zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
			break;
		case BDB_COMMISSION_STA_REJOIN_FAILURE:
			if(!switchRejoinBackoffTimerEvt){
				switchRejoinBackoffTimerEvt = TL_ZB_TIMER_SCHEDULE(switch_rejoinBacckoff, NULL, 60 * 1000);
			}
			break;
		default:
			break;
	}
}


extern void switch_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);

void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime){
	switch_zclIdentifyCmdHandler(endpoint, srcAddr, identifyTime);
}

/*********************************************************************
 * @fn      zbdemo_bdbFindBindSuccessCb
 *
 * @brief   application callback for finding & binding
 *
 * @param   pDstInfo
 *
 * @return  None
 */
void zbdemo_bdbFindBindSuccessCb(findBindDst_t *pDstInfo){
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstAddr.shortAddr = pDstInfo->addr;
	dstEpInfo.dstEp = pDstInfo->endpoint;
	dstEpInfo.profileId = HA_PROFILE_ID;

	zcl_identify_identifyCmd(ENDPOINT_1, &dstEpInfo, FALSE, 0, 0);
}



void switch_otaProcessMsgHandler(u8 evt, u8 status)
{
	//printf("sampleSwitch_otaProcessMsgHandler: status = %x\n", status);
	if(evt == OTA_EVT_START){
		if(status == ZCL_STA_SUCCESS){
			zb_setPollRate(QUEUE_POLL_RATE);
		}else{

		}
	}else if(evt == OTA_EVT_COMPLETE){
		zb_setPollRate(POLL_RATE * 3);

		if(status == ZCL_STA_SUCCESS){
			ota_mcuReboot();
		}else{
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
		}
	}else if(evt == OTA_EVT_IMAGE_DONE){
		zb_setPollRate(POLL_RATE * 3);
	}
}

/*********************************************************************
 * @fn      switch_leaveCnfHandler
 *
 * @brief   Handler for ZDO Leave Confirm message.
 *
 * @param   pRsp - parameter of leave confirm
 *
 * @return  None
 */
void switch_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf)
{
    if(pLeaveCnf->status == SUCCESS){
    	//SYSTEM_RESET();

		if(switchRejoinBackoffTimerEvt){
			TL_ZB_TIMER_CANCEL(&switchRejoinBackoffTimerEvt);
		}
    }
}

/*********************************************************************
 * @fn      switch_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pInd - parameter of leave indication
 *
 * @return  None
 */
void switch_leaveIndHandler(nlme_leave_ind_t *pLeaveInd)
{
    //printf("switch_leaveIndHandler, rejoin = %d\n", pLeaveInd->rejoin);
    //printfArray(pLeaveInd->device_address, 8);
}


#endif  /* __PROJECT_TL_SWITCH__ */
