#ifndef _PROJECT_H_
#define _PROJECT_H_

//#define PROJECT_ID 2    //define in Makefile
#define FBC_USER_WITHOUT_SAVE

#define HDMI_PORT_A		0//012
#define HDMI_PORT_B		1//210
#define HDMI_PORT_SWITCH  HDMI_PORT_A

#define POWER_ON_MODE_SUSPEND	0
#define POWER_ON_MODE_MAIN		1
#define USER_POWER_ON_MODE  	POWER_ON_MODE_MAIN

#define WB_DATA_FROM_DB	        1
#define FBC_PATTERN_MODE		PATTERN_MODE_BLUE

#define	LOCKN_TYPE_A			0
#define	LOCKN_TYPE_B			1//new method as T868's
#define	LOCKN_TYPE_C			1//new method as GXTVBB's
#define LOCKN_TYPE_SEL			LOCKN_TYPE_B

#define ENABLE_10BIT_MODE	0
#endif
