#include "tl_common.h"
#include "zclApp.h"

epconfig_attr_t g_epConfigAttrs[BUTTON_NUM];


/*********************************************************************
 * @fn      saveEPConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t saveEPConfig(u8 endpoint)
{
  u8 idx = endpoint - 1;
	nv_sts_t st = NV_SUCC;
	bool changed = false;

  epconfig_attr_t l_epConfigAttr;  

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_EPCONFIG_BASE + idx, sizeof(epconfig_attr_t), (u8*)&l_epConfigAttr);

	if(st == NV_SUCC){
		if((l_epConfigAttr.simpleClickDevice != g_epConfigAttrs[idx].simpleClickDevice) || (l_epConfigAttr.doubleClickDevice != g_epConfigAttrs[idx].doubleClickDevice) ||
		   	(l_epConfigAttr.longPressDevice != g_epConfigAttrs[idx].longPressDevice) || (l_epConfigAttr.relayMode != g_epConfigAttrs[idx].relayMode) ||
			(l_epConfigAttr.backlightMode != g_epConfigAttrs[idx].backlightMode) ){
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_EPCONFIG_BASE + idx, sizeof(epconfig_attr_t), (u8*)&g_epConfigAttrs[idx]);
	}

	return st;
}

/*********************************************************************
 * @fn      restoreEPConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t restoreEPConfig(u8 endpoint)
{
  u8 idx = endpoint - 1;
	nv_sts_t st = NV_SUCC;

  epconfig_attr_t l_epConfigAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_EPCONFIG_BASE + idx, sizeof(epconfig_attr_t), (u8*)&l_epConfigAttr);

	if(st == NV_SUCC){
		g_epConfigAttrs[idx].simpleClickDevice		= l_epConfigAttr.simpleClickDevice;
		g_epConfigAttrs[idx].doubleClickDevice		= l_epConfigAttr.doubleClickDevice;
		g_epConfigAttrs[idx].longPressDevice			= l_epConfigAttr.longPressDevice;
		g_epConfigAttrs[idx].relayMode				      = l_epConfigAttr.relayMode;
		g_epConfigAttrs[idx].backlightMode			  = l_epConfigAttr.backlightMode;
	}else{
		g_epConfigAttrs[idx].simpleClickDevice		= 0xFFFE;
		g_epConfigAttrs[idx].doubleClickDevice		= 0xFFFE;
		g_epConfigAttrs[idx].longPressDevice			= 0xFFFE;
		g_epConfigAttrs[idx].relayMode				      = ZCL_EPCONFIG_RELAY_MODE_ATTACHED;
		g_epConfigAttrs[idx].backlightMode			  = ZCL_EPCONFIG_BACKLIGHT_MODE_ONOFF;
  	st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_EPCONFIG_BASE + idx, sizeof(epconfig_attr_t), (u8*)&g_epConfigAttrs[idx]);
	}

	return st;
}
