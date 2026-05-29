/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "endpointCfg.h"
#include "zclApp.h"

zcl_basicAttr_t g_zcl_basicAttrs =
{
	.zclVersion 	= 0x03,
	.appVersion 	= (APP_RELEASE|APP_BUILD), //0x00,
	.stackVersion = (STACK_RELEASE|STACK_BUILD), //0x02,
	.hwVersion		= HARDWARE_REV, //0x00,
	.manuName			= ZCL_BASIC_MFG_NAME,
	.modelId			= ZCL_BASIC_MODEL_ID,
	.dateCode     = ZCL_BASIC_DATE_CODE,
	.powerSource	= POWER_SOURCE_MAINS_1_PHASE,
	.genDevClass  = 0x00,  //Lightning
	.genDevType   = 0xe1,  //Wall Switch
	.swBuildId		= ZCL_BASIC_SW_BUILD_ID,
	.deviceEnable	= TRUE,
};

/*********************************************************************
 * @fn      switch_zclBasicResetCmdHandler
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t switch_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
	}

	return ZCL_STA_SUCCESS;
}
