#include <string.h>
#include <stdlib.h>
#include <serial.h>
#include <common.h>
#include <spi_flash.h>
#include <update.h>
#include <reboot.h>
#include <uart_api.h>
#include <spi_regional_division.h>

#include <update.h>

#ifndef _USE_WITHOUT_UI_UPDATE_
#include <timer.h>
#include <panel.h>
#include <osd.h>
#include <fonts_update.h>
#include <timer.h>
#include <vpp.h>
#endif

#define DEVICE_UART_PORT_0       0
#define DEVICE_UART_PORT_1       1
#define DEVICE_UART_PORT_2       2

#ifndef _USE_WITHOUT_UI_UPDATE_
/*extern void panel_init(void);
*/
int gTitleMsgHandle = -1;
int gUpdateMsgHandle = -1;
int gUpdateTimeHandle = -1;

void init_osd(void)
{
	int i;
	/* hide_logo(); */
	OSD_Enable(0);
	if (IS_1080P(panel_param->output_mode)) {
		OSD_Initial(1920, 1080, 0, 0, 1919, 1079);
		OSD_SetFontScale(1, 1);
	} else {
		OSD_Initial(3840, 2160, 0, 0, 3839, 2159);
		OSD_SetFontScale(2, 2);
	}
	OSD_Set3DMode(OSD_3D_MODE_NORMAL);
	/* OSD_SetFontScale(1, 1); */
	/* OSD_SetMirror(1); */
	if (1 == panel_param->reverse)
		OSD_SetMirror(1);
	else
		OSD_SetMirror(0);

	OSD_SetSpacing(2, 2, 2, 2);
	if (OSD_GetMirror() == 1) {
		OSD_ConfigFonts(FONT_NUM, FONT_WIDTH, FONT_HEIGHT,
				sosd_font_lib_lut_hvflip, font_mapping, 1);
	} else if (OSD_GetMirror() == 0) {
		OSD_ConfigFonts(FONT_NUM, FONT_WIDTH, FONT_HEIGHT,
				sosd_font_lib_lut, font_mapping, 1);
	}
	for (i = 0; i < FONT_NUM; i++)
		OSD_SetFontCut(i, sosd_cut_table[i]);
	for (i = 0; i < 16; i++)
		OSD_SetColor(i, nRGBA[i][0], nRGBA[i][1], nRGBA[i][2],
			     nRGBA[i][3]);
	OSD_SetBackground(1, FONT_BG_COLOR_INDEX);
	/* osd_init = 1; */
	OSD_CleanScreen(NULL, 0);
	/* OSD_InitialRegionSimple(1, 1, "test update",
	FONT_NORMAL_COLOR_INDEX, FONT_BG_COLOR_INDEX); */
	OSD_Enable(1);
}

int show_msg(int *ui_handle, int msg_type, int line, int row, const char *msg)
{
	int forecolor = FONT_NORMAL_COLOR_INDEX;
	int bgcolor = FONT_BG_COLOR_INDEX;

	if (msg_type == 0)
		forecolor = FONT_NORMAL_COLOR_INDEX;
	else if (msg_type == 1)
		forecolor = FONT_HILIGHT_COLOR_INDEX;

	if (*ui_handle < 0) {
		*ui_handle =
		    OSD_InitialRegionSimple(line, row, (char *)msg, forecolor,
					    FONT_BG_COLOR_INDEX);
	} else {
		OSD_UpdateRegionContentSimple(*ui_handle, (char *)msg,
					      forecolor, FONT_BG_COLOR_INDEX);
	}

	return 0;
}

int show_title_msg(int msg_type, const char *msg)
{
	return show_msg(&gTitleMsgHandle, msg_type, 0, 100, msg);
}

int show_update_msg(int msg_type, const char *msg)
{
	return show_msg(&gUpdateMsgHandle, msg_type, 3, 100, msg);
}

#define CLOCKS_PER_SECOND (1000000)
int show_update_time(unsigned int tm_lapse)
{
	char msg_buf[256];

	tm_lapse = tm_lapse / CLOCKS_PER_SECOND;
	sprintf(msg_buf, "Time lapse: %02d:%02d:%02d", tm_lapse / 3600,
		tm_lapse / 60, tm_lapse % 60);
	return show_msg(&gUpdateTimeHandle, 0, 5, 100, msg_buf);
}

#if 0
void fn(void)
{
	unsigned char buf[100];
	int i;
	serial_puts("\nenter fn\n");
	memset(buf, 0, sizeof(buf));
	int len = uart_ports_read(DEVICE_UART_PORT_1, buf, 10);
	if (len > 0)
		serial_puts("\nlen > 0\n");

	for (i = 0; i < len; i++)
		serial_putc(buf[i]);
/* destory_timer(TIMERA_INDEX, fn); */
}
#endif

#else

int show_update_msg(int msg_type, const char *msg)
{
	return 0;
}

void udelay(int us)
{
	int n = us * (80 / 3);
	while (n--)
		;
}

void mdelay(int ms)
{
	udelay(4000 * ms);
}

#endif

void osd_vpu_clock_set(void)
{
#if 1
	vpu_timing_t vpu_timing = get_timing_mode();

	if (T_1080P50HZ == get_output_mode())
		vclk_set_encl_lvds(vpu_timing, LVDS_PORTS);
	else
		vclk_set_encl_vx1(vpu_timing, VX1_LANE_NUM, VX1_BYTE_NUM);
#endif
}

int main(int argc, char *argv[])
{
	unsigned int tm_start = 0, tm_lapse = 0;
	int tmp_val = 0;

	unsigned int boot_flag = get_boot_flag();

	update_ctrl_t update_ctrl;
	serial_init(CONSOLE_CHANNEL_DEV);
	serial_puts("serial uart port 0 init success!\n");
	serial_dev = get_serial_device(CONSOLE_CHANNEL_DEV);
	serial_puts("init spi flash!\n");
	init_spi_flash();

	printfx("bpflag = 0x%x\n", boot_flag);

#ifndef _USE_WITHOUT_UI_UPDATE_
	serial_puts("Power on.\n");
	power_on_aml();
	serial_puts("Init Panel.\n");
	panel_init();
	panel_power_on_aml();
	tmp_val = get_panel_power_on_dly();
	mdelay(tmp_val);
	serial_puts("Init Vpp.\n");
	init_vpp();
	osd_vpu_clock_set();
	init_osd();
	show_title_msg(0, "Updating...");

	serial_puts("panel_resume.\n");
	panel_resume();

#if 0
	uart_ports_open(DEVICE_UART_PORT_1);

	create_timer(TIMERA_INDEX, TIMER_TICK_1MS, 10, fn);	/* 10ms */
#endif

	show_update_msg(0, "starting to update fbc.");

	tm_start = OSTimeGet();
	show_update_time(0);
#endif

	init_update_ctrl_t(&update_ctrl);

	int ret = 1;
	serial_puts("enter while.\n");
	do {
		if (REBOOT_FLAG_BOOT_ERROR == boot_flag
		    || REBOOT_FLAG_MAIN_ERROR == boot_flag) {
			serial_puts("enter restore main img.\n");

			unsigned b_offs, s_offs, size;
			if (REBOOT_FLAG_BOOT_ERROR == boot_flag) {
				b_offs = SECOND_BOOT_BASE - SPI_BASE;
				s_offs = BACKUP_BOOT_BASE - SPI_BASE;
				size = BOOT_SIZE;
			} else {
				b_offs = MAIN_CODE_BASE - SPI_BASE;
				s_offs = BACKUP_MAIN_BASE - SPI_BASE;
				size = MAIN_SIZE;
			}

			printfx("b_offs: 0x%x\n", b_offs);
			printfx("s_offs: 0x%x\n", s_offs);
			printfx("size: 0x%x\n", size);

			move_image(get_spi_flash_device(0), s_offs, b_offs,
				   size);

			break;
		} else {
			ret = handle_cmd(&update_ctrl);
		}

#ifndef _USE_WITHOUT_UI_UPDATE_
		tm_lapse = OSTimeGet() - tm_start;
		show_update_time(tm_lapse);
#endif
	} while (!ret);

	mdelay(1000);
	show_update_msg(0, "Update done.");
	mdelay(1000);

	reboot(REBOOT_FLAG_FROM_UPGRADE);

	return 0;
}
