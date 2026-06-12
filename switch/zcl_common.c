/********************************************************************************************************
 * @file    zcl_switchCb.c
 *
 * @brief   This is the source file for zcl_sampleSwitchCb
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
#include "endpointCfg.h"
#include "zclApp.h"
#include "backlight.h"


/**********************************************************************
 * LOCAL FUNCTIONS
 */
static void switch_zclReadRspCmd(u16 clusterId, zclReadRspCmd_t *pReadRspCmd);
static void switch_zclWriteRspCmd(u16 clusterId, zclWriteRspCmd_t *pWriteRspCmd);
static void switch_zclWriteReqCmd(u8 endpoint, u16 clusterId, zclWriteCmd_t *pWriteReqCmd);
static void switch_zclCfgReportCmd(u16 clusterId, zclCfgReportCmd_t *pCfgReportCmd);
static void switch_zclCfgReportRspCmd(u16 clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void switch_zclReportCmd(u8 endpoint, u16 clusterId, zclReportCmd_t *pReportCmd);
static void switch_zclDfltRspCmd(u16 clusterId, zclDefaultRspCmd_t *pDftRspCmd);


/*********************************************************************
 * @fn      switch_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void switch_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
	u16 cluster = pInHdlrMsg->msg->indInfo.cluster_id;
	u8 endpoint = pInHdlrMsg->addrInfo.dstEp;
	switch(pInHdlrMsg->hdr.cmd)
	{
		case ZCL_CMD_READ_RSP:
			switch_zclReadRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_WRITE_RSP:
			switch_zclWriteRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_WRITE:
			switch_zclWriteReqCmd(endpoint, cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_CONFIG_REPORT:
			switch_zclCfgReportCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_CONFIG_REPORT_RSP:
			switch_zclCfgReportRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_REPORT:
			switch_zclReportCmd(endpoint, cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_DEFAULT_RSP:
			switch_zclDfltRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		default:
			break;
	}
}

/*********************************************************************
 * @fn      switch_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void switch_zclReadRspCmd(u16 clusterId, zclReadRspCmd_t *pReadRspCmd)
{

}

/*********************************************************************
 * @fn      switch_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void switch_zclWriteRspCmd(u16 clusterId, zclWriteRspCmd_t *pWriteRspCmd)
{

}

/*********************************************************************
 * @fn      switch_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void switch_zclWriteReqCmd(u8 endpoint, u16 clusterId, zclWriteCmd_t *pWriteReqCmd)
{
	if (clusterId == ZCL_CLUSTER_EPCONFIG) {
		zclWriteRec_t *rec = &pWriteReqCmd->attrList[0];
		if (rec->attrID == ZCL_ATTRID_EPCONFIG_EVENT_CFG) {
			g_epConfigAttrs[endpoint].eventCfgLen = epConfigEvent_decode(endpoint, g_epConfigAttrs[endpoint].eventCfgList, EP_CONFIG_EVENT_MAX_COUNT);
			printf("Write EP Config Event, endpoint: %d, event count: %d\n", endpoint, g_epConfigAttrs[endpoint].eventCfgLen);
			printf("eventCfgRaw: ");
			for (u8 i=0; i<g_epConfigAttrs[endpoint].eventCfgLen; i++) {
				printf("  event: %02x, nbClicks: %02x, cmd: %02x, dstAddr: %04x, dstEndpoint: %02x, extra0: %02x, extra1: %02x\n", 
					g_epConfigAttrs[endpoint].eventCfgList[i].event,
					g_epConfigAttrs[endpoint].eventCfgList[i].nbClicks,
					g_epConfigAttrs[endpoint].eventCfgList[i].cmd,
					g_epConfigAttrs[endpoint].eventCfgList[i].dstAddr,
					g_epConfigAttrs[endpoint].eventCfgList[i].dstEndpoint,
					g_epConfigAttrs[endpoint].eventCfgList[i].extra[0],
					g_epConfigAttrs[endpoint].eventCfgList[i].extra[1]
				);
			}
			saveEPConfig(endpoint);
		}
	}
}


/*********************************************************************
 * @fn      switch_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void switch_zclDfltRspCmd(u16 clusterId, zclDefaultRspCmd_t *pDftRspCmd)
{

}

/*********************************************************************
 * @fn      switch_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void switch_zclCfgReportCmd(u16 clusterId, zclCfgReportCmd_t *pCfgReportCmd)
{

}

/*********************************************************************
 * @fn      switch_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void switch_zclCfgReportRspCmd(u16 clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd)
{

}

/*********************************************************************
 * @fn      switch_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void switch_zclReportCmd(u8 endpoint, u16 clusterId, zclReportCmd_t *pReportCmd)
{

}


void saveAllAttrsToNVRAM(void)
{
	saveGlobalConfig();
	for (u8 b=0;b<BUTTON_NUM;b++) {
		saveRelayConfig(b);
		saveSwitchConfig(b);	
		saveEPConfig(b);
	}
}

void restoreAllAttrsFromNVRAM(void)
{
	restoreGlobalConfig();
	for (u8 b=0;b<BUTTON_NUM;b++) {
		restoreRelayConfig(b);
		restoreSwitchConfig(b);	
		restoreEPConfig(b);
	}
}
