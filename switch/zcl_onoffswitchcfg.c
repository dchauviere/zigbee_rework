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

 /**********************************************************************
  * INCLUDES
  */
 #include "tl_common.h"
 #include "zb_api.h"
 #include "zcl_include.h"
 #include "endpointCfg.h"
 #include "zclApp.h"
 
switchAttr_t g_switchAttr[BUTTON_NUM];


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
 * @fn      saveSwitchConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t saveSwitchConfig(u8 sw)
{
	nv_sts_t st = NV_SUCC;
	bool changed = false;

	switchAttr_t l_switchAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_APP_SWITCH_BASE + sw, sizeof(switchAttr_t), (u8*)&l_switchAttr);

	if(st == NV_SUCC){
		if ((l_switchAttr.transitionTime != g_switchAttr[sw].transitionTime)
		|| (l_switchAttr.switchAction != g_switchAttr[sw].switchAction)
		|| (l_switchAttr.switchMode != g_switchAttr[sw].switchMode)) {
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_SWITCH_BASE + sw, sizeof(switchAttr_t), (u8*)&g_switchAttr[sw]);
	}

	return st;
}

/*********************************************************************
 * @fn      restoreSwitchConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t restoreSwitchConfig(u8 sw)
{
	nv_sts_t st = NV_SUCC;

	switchAttr_t l_switchAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_SWITCH_BASE + sw, sizeof(switchAttr_t), (u8*)&l_switchAttr);

	if(st == NV_SUCC){
		g_switchAttr[sw].transitionTime	= l_switchAttr.transitionTime;
		g_switchAttr[sw].switchAction = l_switchAttr.switchAction;
		g_switchAttr[sw].switchMode = l_switchAttr.switchMode;
	}else{
		g_switchAttr[sw].transitionTime = 0x0A;
		g_switchAttr[sw].switchAction = ZCL_SWITCH_ACTION_ON_OFF;
		g_switchAttr[sw].switchMode = ZCL_SWITCH_TYPE_MULTIFUNCTION;
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_SWITCH_BASE + sw, sizeof(switchAttr_t), (u8*)&g_switchAttr[sw]);
	}

	return st;
}
