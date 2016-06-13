/*
 * Copyright (c) 2015 Amlogic, Inc. All rights reserved.
 *
 * This source code is subject to the terms and conditions defined in the
 * file 'LICENSE' which is part of this source code package.
 *
 */
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

/*
1080p:PANEL_ID is 0
4k220:PANEL_ID is 3
4k222:PANEL_ID is 4
*/
#define PANEL_ID 0
/*
PB:T111
PA:T112
*/
#define CONFIG_SWITCH_P "PB"
/*
1080: 0
4k:   1
*/
#define CONFIG_BIT_MODE 0

#define PROJECT_ID PANEL_ID
#define PANEL_MODULE "HV550QU2-305"

#define DYNAMIC_SET_PANEL_TYPE 0
#define BOOT_DEBUG_VERSION 1
#define UI_HAVE_LOGO 1
#define ENABLE_AVMUTE_CONTROL 1
#define ENABLE_AUTO_BACKLIGHT_CONTROL 0

#define CONFIG_SPI_SIZE 0x200000

#define PQ_BINARY_START 0x1A3000
#define PQ_BINARY_UNIT_SIZE 0x4000

#define FBC_FACTORY_SIZE 0x10000
#define FBC_FACTORY_START (CONFIG_SPI_SIZE - FBC_FACTORY_SIZE)

#define FBC_USER_SIZE 0x2000
#define FBC_USER_START (FBC_FACTORY_START - FBC_USER_SIZE)

#define USER_HDCPKEY FBC_USER_START
#define USER_HDCPKEY_SIZE 0x400

#define USER_SETTING_START (USER_HDCPKEY+USER_HDCPKEY_SIZE)
#define USER_SETTING_SIZE 0x100

#define USER_WB_SETTING_START (USER_SETTING_START+USER_SETTING_SIZE)
#define USER_WB_SETTING_SIZE 0x100

#define USER_PIC_SETTING_START (USER_WB_SETTING_START+USER_WB_SETTING_SIZE)
#define USER_PIC_SETTING_SIZE 0x100

#define USER_CRI_DATA_SIZE 0x400
#define USER_CRI_DATA_START (FBC_USER_START+FBC_USER_SIZE-USER_CRI_DATA_SIZE)

extern  const char* switch_p;
extern  int bit10_mode;

extern void init_configures(void);

#endif//__BOARD_CONFIG_H__
