#include <serial.h>
#include <command.h>
#include <common.h>
#include <reboot.h>
#include <relocate.h>
#include <mini_console.h>
#include <inputdata.h>
#include <spi_regional_division.h>

#include <version.h>

#include <clock.h>
#include <sar_adc.h>

#define SF_UPGRADE_CMD    "ug"
#define RF_UPGRADE_RES    "ok"
#define RF_NOUPGRADE_RES    "ng"
#define CONFIG_UPDATE_KEY	16

#define _ADCKEY_TEST_

void jump_0(void) {
}

typedef void (*main_entry)(void);

int __attribute__ ((section(".second.boot.entry"))) second_boot(int boot_flag) {
	unsigned int check_info = 0;
	lvds_phy_disable();

	/* copy_data_from_spi_to_sram2(BOOT_DATA_BASE, BOOT_DATA_SIZE>>2); */

	serial_init(0);
	serial_init(2);
	current_serial_device = default_serial_console();

#if (BOOT_DEBUG_VERSION == 1)
	printf("\nenter the second boot!\nbpflag = 0x%x\n", boot_flag);
#endif

	set_boot_stage(SEC_BOOT_STAGE);

	sar_adc_init();

#ifdef _ADCKEY_TEST_
	int i, j;
	INPUTDATA inputdata;
	set_redetect_flag();
	if (boot_flag == REBOOT_FLAG_NORMAL) {
		for (i = 0; i < 3; i++) {
			if (!detect_adc_key(1, &inputdata)) {
				printf("data: 0x%x, type: %d\n", inputdata.input_data,
						inputdata.input_type);
				if (inputdata.input_type
						== 0&& adc2key(1, &inputdata) == CONFIG_UPDATE_KEY) {char rec[10] = {0,};
				int n;

				serial_init(2);
				struct serial_device *sdev1 = get_serial_device(1);
				while (1)
				{
					sdev1->puts(SF_UPGRADE_CMD);
					if (sdev1->tstc() >= 2)
					{
						for (n=0; n<2; n++)
						{
							rec[n] = sdev1->getc();
						}

						rec[n] = 0;

						if (!strcmp(RF_UPGRADE_RES, rec))
						{
							boot_flag = REBOOT_FLAG_UPGRADE2;
							break;
						} else if(!strcmp(RF_NOUPGRADE_RES, rec))
						{
							boot_flag = REBOOT_FLAG_FROM_SUSPEND;
							break;
						}
					}

				}

				break;
			}
		}

		for (j = 100 * 1000; j > 0; j--)
		;

	}
}
#endif

#if (BOOT_DEBUG_VERSION == 1)
				printf("fbc boot code version:\n");
				print_build_version_info();
#endif

	reset_watchdog();

	if (REBOOT_FLAG_SUSPEND == boot_flag) {
		serial_puts("enter suspend!\n");
		copy_code_from_spi_to_ICCM(SUSPEND_CODE_BASE, SUSPEND_CODE_SIZE >> 2);
		copy_data_from_spi_to_DCCM(SUSPEND_DATA_BASE, SUSPEND_DATA_SIZE >> 2);
		disable_watchdog();
		return 0;

	}

	if (REBOOT_FLAG_UPGRADE1 == boot_flag
			|| REBOOT_FLAG_UPGRADE2 == boot_flag) {
		typedef int (*update)();

		printf("upgrade code base: 0x%x, value: 0x%x\n", UPDATE_CODE_BASE,
				UPDATE_CODE_SIZE);
		printf("upgrade data base: 0x%x, value: 0x%x\n", UPDATE_DATA_BASE,
				UPDATE_DATA_SIZE);
		copy_code_from_spi_to_ICCM(UPDATE_CODE_BASE, UPDATE_CODE_SIZE >> 2);
		printf("copy upgrade code done!\n");
		copy_data_from_spi_to_DCCM(UPDATE_DATA_BASE, UPDATE_DATA_SIZE >> 2);
		printf("copy upgrade data done!\n");
		disable_watchdog();
		set_boot_flag(boot_flag);
		((update) ICCM_BASE)();

	}

	if (bootdelay > 0 && abortboot(bootdelay)) {
		disable_watchdog();
		do_wait_cmd();
		return 0;
	} else {
		check_info = check_image_info(MAIN_H_CHECK_INFO_BASE,
				MAIN_CHECK_INFO_BASE);
		if (check_info) {
			printf("main img is bad.\n");
			reboot(REBOOT_FLAG_MAIN_ERROR);
		}
#ifndef HAS_ROM_MAP
		printf("start copy main code, size %d bytes.\n", MAIN_CODE_SIZE);
		copy_code_from_spi_to_ICCM(MAIN_CODE_BASE, MAIN_CODE_SIZE >> 2);
		printf("start copy main data, size %d bytes.\n", (MAIN_DATA_SIZE));
		copy_data_from_spi_to_DCCM(MAIN_DATA_BASE, MAIN_DATA_SIZE >> 2);
#else
		unsigned mcode_size = 0;
		unsigned mdata_size = 0;
		memcpy(&mcode_size, (unsigned char *)MAIN_CODE_SIZE_INFO_BASE,
				sizeof(mcode_size));
		memcpy(&mdata_size, (unsigned char *)MAIN_DATA_SIZE_INFO_BASE,
				sizeof(mdata_size));

		/* backward compatibility and fault tolerance */
		if (mcode_size == 0 || mdata_size == 0
				|| mcode_size > MAIN_CODE_SIZE
				|| mdata_size > (MAIN_DATA_SIZE)) {
			mcode_size = MAIN_CODE_SIZE;
			mdata_size = MAIN_DATA_SIZE;
		}

		if (mcode_size % sizeof(unsigned)) {
			mcode_size +=
			(sizeof(unsigned) - mcode_size % sizeof(unsigned));
		}

		if (mdata_size % sizeof(unsigned)) {
			mdata_size +=
			(sizeof(unsigned) - mdata_size % sizeof(unsigned));
		}
#if (BOOT_DEBUG_VERSION == 1)
		printf("start copy main code, size %d.\n", mcode_size);
#endif
		copy_code_from_spi_to_ICCM(MAIN_CODE_BASE, mcode_size >> 2);

#if (BOOT_DEBUG_VERSION == 1)
		printf("start copy main data. size %d\n", mdata_size);
#endif
		memset((int *)DCCM_BASE, 0, (DCCM_SIZE));
		copy_data_from_spi_to_DCCM(MAIN_DATA_BASE, mdata_size >> 2);
#endif

#if (BOOT_DEBUG_VERSION == 1)
		printf("finish copy main code and run main code.\n");
#endif
		set_boot_flag(boot_flag);
		((main_entry) ICCM_BASE)();
		return 0;
	}

}
