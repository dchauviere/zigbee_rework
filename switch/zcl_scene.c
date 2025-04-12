/********************************************************************************************************
 * @file    zcl_sceneCb.c
 *
 * @brief   This is the source file for zcl_sceneCb
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
#include "endpointCfg.h"
#include "zclApp.h"


/*********************************************************************
 * @fn      switch_sceneRecallReqHandler
 *
 * @brief   Handler for ZCL scene recall command. This function will recall scene.
 *
 * @param	pApsdeInd
 * @param	pScene
 *
 * @return  None
 */
static void switch_sceneRecallReqHandler(u8 btn, zclIncomingAddrInfo_t *pAddrInfo, zcl_sceneEntry_t *pScene)
{
	u8 *pData = pScene->extField;
	u16 clusterID = 0xFFFF;
	u8 extLen = 0;

	while(pData < pScene->extField + pScene->extFieldLen){
		clusterID = BUILD_U16(pData[0], pData[1]);
		pData += 2;//cluster id

		extLen = *pData++;//length

		if(clusterID == ZCL_CLUSTER_GEN_ON_OFF){
			if(extLen >= 1){
				u8 onOff = *pData++;

				switch_onOffCb(pAddrInfo, onOff, NULL);
				extLen--;
			}
		}
		pData += extLen;
	}
}

/*********************************************************************
 * @fn      switch_sceneStoreReqHandler
 *
 * @brief   Handler for ZCL scene store command. This function will set scene attribute first.
 *
 * @param   pApsdeInd
 * @param   pScene
 *
 * @return  None
 */
static void switch_sceneStoreReqHandler(u8 btn, zcl_sceneEntry_t *pScene)
{
	u8 extLen = 0;

	zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet(btn-1);

	pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_GEN_ON_OFF);
	pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_GEN_ON_OFF);
	pScene->extField[extLen++] = sizeof(u8);
	pScene->extField[extLen++] = pOnOff->onOff;

	pScene->extFieldLen = extLen;
}

/*********************************************************************
 * @fn      switch_sceneCb
 *
 * @brief   Handler for ZCL Scene command.
 *
 * @param   pApsdeInd
 *
 * @return  None
 */
status_t switch_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dstEp <= BUTTON_NUM){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_SCENE_STORE_SCENE:
					switch_sceneStoreReqHandler(pAddrInfo->dstEp, (zcl_sceneEntry_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_RECALL_SCENE:
					switch_sceneRecallReqHandler(pAddrInfo->dstEp, pAddrInfo, (zcl_sceneEntry_t *)cmdPayload);
					break;
				default:
					status = ZCL_STA_UNSUP_MANU_CLUSTER_COMMAND;
					break;
			}
		}
	}

	return status;
}

#endif  /* __PROJECT_TL_SWITCH__ */
