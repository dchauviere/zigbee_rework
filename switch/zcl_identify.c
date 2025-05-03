#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "endpoints.h"
#include "zclApp.h"

/**********************************************************************
 * LOCAL VARIABLES
 */
static ev_timer_event_t *identifyTimerEvt = NULL;

s32 switch_zclIdentifyTimerCb(void *arg)
{
	if(g_zcl_identifyAttrs.identifyTime <= 0){
		identifyTimerEvt = NULL;
		return -1;
	}
	g_zcl_identifyAttrs.identifyTime--;
	return 0;
}

void switch_zclIdentifyTimerStop(void)
{
	if(identifyTimerEvt){
		TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
	}
}

/*********************************************************************
 * @fn      switch_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void switch_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
	g_zcl_identifyAttrs.identifyTime = identifyTime;

	if(identifyTime == 0){
		switch_zclIdentifyTimerStop();
		stopBacklightBlink(0);
	}else{
		if(!identifyTimerEvt){
			startBacklightBlink(0, identifyTime, 500, 500);
			identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(switch_zclIdentifyTimerCb, NULL, 1000);
		}
	}
}

/*********************************************************************
 * @fn      switch_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void switch_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
	u8 effectId = pTriggerEffect->effectId;
	//u8 effectVariant = pTriggerEffect->effectVariant;

	switch(effectId){
		case IDENTIFY_EFFECT_BLINK:
			startBacklightBlink(0, 1, 500, 500);
			break;
		case IDENTIFY_EFFECT_BREATHE:
			startBacklightBlink(0, 15, 300, 700);
			break;
		case IDENTIFY_EFFECT_OKAY:
			startBacklightBlink(0, 2, 250, 250);
			break;
		case IDENTIFY_EFFECT_CHANNEL_CHANGE:
			startBacklightBlink(0, 1, 500, 7500);
			break;
		case IDENTIFY_EFFECT_FINISH_EFFECT:
			startBacklightBlink(0, 1, 300, 700);
			break;
		case IDENTIFY_EFFECT_STOP_EFFECT:
			stopBacklightBlink(0);
			break;
		default:
			break;
	}
}

/*********************************************************************
 * @fn      switch_zclIdentifyQueryRspCmdHandler
 *
 * @brief   Handler for ZCL Identify Query response command.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyRsp
 *
 * @return  None
 */
static void switch_zclIdentifyQueryRspCmdHandler(u8 endpoint, u16 srcAddr, zcl_identifyRspCmd_t *identifyRsp)
{
	if(identifyRsp->timeout){
		findBindDst_t dstInfo;
		dstInfo.addr = srcAddr;
		dstInfo.endpoint = endpoint;

		bdb_addIdentifyActiveEpForFB(dstInfo);
	}
}

/*********************************************************************
 * @fn      switch_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t switch_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
		switch(cmdId){
			case ZCL_CMD_IDENTIFY:
				switch_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
				break;
			case ZCL_CMD_TRIGGER_EFFECT:
				switch_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
				break;
			default:
				break;
		}
	}else{
		if(cmdId == ZCL_CMD_IDENTIFY_QUERY_RSP){
			switch_zclIdentifyQueryRspCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, (zcl_identifyRspCmd_t *)cmdPayload);
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif
