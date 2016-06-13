#ifndef __FBC_REBOOT_H__
#define __FBC_REBOOT_H__

#define REBOOT_FLAG_NORMAL			0x00000000
#define REBOOT_FLAG_UPGRADE1		0x80808080
#define REBOOT_FLAG_UPGRADE2		0x88888888
#define REBOOT_FLAG_SUSPEND			0x12345678
#define REBOOT_FLAG_LITE_UPGRADE1	0x73737373
#define REBOOT_FLAG_LITE_UPGRADE2	0x74747474
#define REBOOT_FLAG_BOOT_ERROR		0x75757575
#define REBOOT_FLAG_MAIN_ERROR		0x76767676
#define REBOOT_FLAG_FROM_SUSPEND	0x87654321
#define REBOOT_FLAG_FROM_UPGRADE	0x87654320
#define REBOOT_FLAG_FROM_WATCHDOG	0x87654322


#define FIR_BOOT_STAGE				0x1
#define SEC_BOOT_STAGE				0x2
#define MAIN_STAGE					0x3

int reboot ( unsigned reson );
unsigned get_boot_flag();
int set_boot_flag ( unsigned int num );
unsigned get_boot_stage();
int set_boot_stage ( unsigned int stage );

void reset_spi();
void reset_iosc();
void reset_ir();
void reset_ee();
int reboot_sw ( unsigned reson );

int reset_watchdog();	//feed the dog
int enable_watchdog();
int enable_watchdog_interrupt();
int disable_watchdog();
int disable_watchdog_interrupt();
int set_watchdog_threshold ( unsigned short th );
unsigned get_watchdog_threshold();
unsigned get_watchdog_current_count();
typedef unsigned int ( * reboot_timming ) ();
void registerRebootTimming ( reboot_timming func );

#endif	//__FBC_REBOOT_H__
