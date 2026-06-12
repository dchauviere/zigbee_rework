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
	nv_sts_t st = NV_SUCC;
	bool changed = false;

  epconfig_attr_t l_epConfigAttr;  

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_EPCONFIG_BASE + endpoint, sizeof(epconfig_attr_t), (u8*)&l_epConfigAttr);

	if(st == NV_SUCC){
		for (u8 i = 0; i < EP_CONFIG_EVENT_OCTET_MAX; i++) {
			if (l_epConfigAttr.eventCfgRaw[i] != g_epConfigAttrs[endpoint].eventCfgRaw[i]) {
				changed = true;
				break;
			}
		}
		if(l_epConfigAttr.backlightMode != g_epConfigAttrs[endpoint].backlightMode) {
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_EPCONFIG_BASE + endpoint, sizeof(epconfig_attr_t), (u8*)&g_epConfigAttrs[endpoint]);
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
	nv_sts_t st = NV_SUCC;

  epconfig_attr_t l_epConfigAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_EPCONFIG_BASE + endpoint, sizeof(epconfig_attr_t), (u8*)&l_epConfigAttr);

	if(st == NV_SUCC){
		memcpy(g_epConfigAttrs[endpoint].eventCfgRaw, l_epConfigAttr.eventCfgRaw, EP_CONFIG_EVENT_OCTET_MAX);
		g_epConfigAttrs[endpoint].eventCfgLen = epConfigEvent_decode(endpoint, g_epConfigAttrs[endpoint].eventCfgList, EP_CONFIG_EVENT_MAX_COUNT);
		g_epConfigAttrs[endpoint].backlightMode			  = l_epConfigAttr.backlightMode;
	}else{
		g_epConfigAttrs[endpoint].eventCfgRaw[0] = 0x08;
		g_epConfigAttrs[endpoint].eventCfgRaw[1] = EPCONFIG_EVENT_CLICK;
		g_epConfigAttrs[endpoint].eventCfgRaw[2] = 0x01;
		g_epConfigAttrs[endpoint].eventCfgRaw[3] = EPCONFIG_CMD_ONOFF_TOGGLE;
		g_epConfigAttrs[endpoint].eventCfgRaw[4] = 0xFE;
		g_epConfigAttrs[endpoint].eventCfgRaw[5] = 0xFF;
		g_epConfigAttrs[endpoint].eventCfgRaw[6] = 0x00;
		g_epConfigAttrs[endpoint].eventCfgRaw[7] = EPCONFIG_RELAY_ATTACHED;
		g_epConfigAttrs[endpoint].eventCfgRaw[8] = 0;
		g_epConfigAttrs[endpoint].eventCfgLen = epConfigEvent_decode(endpoint, g_epConfigAttrs[endpoint].eventCfgList, EP_CONFIG_EVENT_MAX_COUNT);
		g_epConfigAttrs[endpoint].backlightMode			  = ZCL_EPCONFIG_BACKLIGHT_MODE_ONOFF;
  	st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_EPCONFIG_BASE + endpoint, sizeof(epconfig_attr_t), (u8*)&g_epConfigAttrs[endpoint]);
	}

	return st;
}

u8 epConfigEvent_encode(u8 endpoint, epConfigEventCfg_t *list, u8 count)
{
    if (count > EP_CONFIG_EVENT_MAX_COUNT) {
        count = EP_CONFIG_EVENT_MAX_COUNT;
    }

    u8 *p = &g_epConfigAttrs[endpoint].eventCfgRaw[1];
    g_epConfigAttrs[endpoint].eventCfgRaw[0] = count * sizeof(epConfigEventCfg_t);

    for (u8 i = 0; i < count; i++) {
        *p++ = list[i].event;
				*p++ = list[i].nbClicks;
        *p++ = list[i].cmd;
        *p++ = list[i].dstAddr & 0xFF;
        *p++ = list[i].dstAddr >> 8;
        *p++ = list[i].dstEndpoint;
				*p++ = list[i].extra[0];
				*p++ = list[i].extra[1];
    }

    return count;
}

u8 epConfigEvent_decode(u8 endpoint, epConfigEventCfg_t *outList, u8 maxCount)
{
    u8 len = g_epConfigAttrs[endpoint].eventCfgRaw[0];
    u8 count = len / sizeof(epConfigEventCfg_t);
    if (count > maxCount) count = maxCount;

    const u8 *p = &g_epConfigAttrs[endpoint].eventCfgRaw[1];

    for (u8 i = 0; i < count; i++) {
        outList[i].event       = *p++;
        outList[i].nbClicks    = *p++;
        outList[i].cmd         = *p++;
        outList[i].dstAddr     = p[0] | (p[1] << 8);
        p += 2;
        outList[i].dstEndpoint = *p++;
				outList[i].extra[0]   = *p++;
				outList[i].extra[1]   = *p++;
    }

    return count;
}
