/********************************************************************************************************
 * @file    zcl_onOffCb.c
 *
 * @brief   This is the source file for zcl_onOffCb
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
 
#define NV_ITEM_ZCL_ON_OFF_SWITCH_CFG 0x40

_CODE_ZCL_ status_t zcl_onOffSwitchCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    //	return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, manuCode, attrNum, attrTbl, NULL, cb);
	u8 status = ZCL_STA_SUCCESS;
    status = zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, manuCode, attrNum, attrTbl, NULL, cb);

    if(status == ZCL_STA_SUCCESS){
    //    	zcl_level_startUpCurrentLevel(endpoint);
    }
    
    return status;
}

/*********************************************************************
 * @fn      switch_onOffSwitchCfgCb
 *
 * @brief   Handler for ZCL ONOFFSwitchConfig command.
 *
 * @param	pAddrInfo
 * @param   cmdId - onoffswitchcfg cluster command id
 * @param	cmdPayload
 *
 * @return  status_t
 */
status_t switch_onOffSwitchCfgCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	return ZCL_STA_SUCCESS;
}

/*********************************************************************
 * @fn      zcl_onOffSwitchCfgAttr_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffSwitchCfgAttr_save(void)
{
	nv_sts_t st = NV_SUCC;
	bool changed = false;

	zcl_onOffSwitchCfgAttr_t zcl_nv_onOffSwitchCfg[BUTTON_NUM];

	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ON_OFF_SWITCH_CFG, sizeof(zcl_onOffSwitchCfgAttr_t)*BUTTON_NUM, (u8*)&zcl_nv_onOffSwitchCfg);

	if(st == NV_SUCC){
		for(u8 ep=0;ep<BUTTON_NUM;ep++){
			if((zcl_nv_onOffSwitchCfg[ep].switchType != g_zcl_onOffSwitchCfgAttrs[ep].switchType) || (zcl_nv_onOffSwitchCfg[ep].switchAction != g_zcl_onOffSwitchCfgAttrs[ep].switchAction) ){
				changed = true;
			}
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ON_OFF_SWITCH_CFG, sizeof(zcl_onOffSwitchCfgAttr_t)*BUTTON_NUM, (u8*)&g_zcl_onOffSwitchCfgAttrs);
	}

	return st;
}

/*********************************************************************
 * @fn      zcl_onOffSwitchCfgAttr_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffSwitchCfgAttr_restore(void)
{
	nv_sts_t st = NV_SUCC;

	zcl_onOffSwitchCfgAttr_t zcl_nv_onOffSwitchCfg[BUTTON_NUM];

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ON_OFF_SWITCH_CFG, sizeof(zcl_onOffSwitchCfgAttr_t)*BUTTON_NUM, (u8*)&zcl_nv_onOffSwitchCfg);

	if(st == NV_SUCC){
		for(u8 ep=0;ep<BUTTON_NUM;ep++){
			g_zcl_onOffSwitchCfgAttrs[ep].switchType	= zcl_nv_onOffSwitchCfg[ep].switchType;
			g_zcl_onOffSwitchCfgAttrs[ep].switchAction	= zcl_nv_onOffSwitchCfg[ep].switchAction;
		}
	}else{
		for(u8 ep=0;ep<BUTTON_NUM;ep++){
			g_zcl_onOffSwitchCfgAttrs[ep].switchType	= ZCL_SWITCH_TYPE_TOGGLE;
			g_zcl_onOffSwitchCfgAttrs[ep].switchAction	= ZCL_SWITCH_ACTION_TOGGLE;
		}
	}

	return st;
}

#endif