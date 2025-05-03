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

#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "endpoints.h"
#include "zclApp.h"
#include "backlightCtrl.h"


/**********************************************************************
 * LOCAL FUNCTIONS
 */
static void switch_zclReadRspCmd(u16 clusterId, zclReadRspCmd_t *pReadRspCmd);
static void switch_zclWriteRspCmd(u16 clusterId, zclWriteRspCmd_t *pWriteRspCmd);
static void switch_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd);
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
			switch_zclWriteReqCmd(cluster, pInHdlrMsg->attrCmd);
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
static void switch_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd)
{

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
	for (u8 i=0;i<pReportCmd->numAttr;i++){
		if (pReportCmd->attrList[i].attrID == ZCL_ATTRID_ONOFF) {
			u8 state = pReportCmd->attrList[i].attrData[0];
			if (state) {
				setBacklightOn(getRelayFromEndpoint(endpoint));
			}else{
				setBacklightOff(getRelayFromEndpoint(endpoint));
			}
		}
	}
}

#endif  /* __PROJECT_TL_SWITCH__ */
