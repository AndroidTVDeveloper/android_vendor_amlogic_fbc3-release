#include <string.h>
#include <panel.h>
#include <user_setting.h>
#include <gpio.h>
#include <project.h>
#include <vpp_api.h>
#include <reboot.h>

#include <ui.h>
#include <task.h>

int project_id = 0;

unsigned char customer_ptn = 0;

panel_config_t panel_aml = {

	.output_mode = T_1080P50HZ, .lvds_timing = TIMING_1920x1080P60, .vx1_timing = TIMING_3840x2160P60,

	.format = PANEL_NORMAL,

	.reverse = 0, .scaler = 0,

	.clk = 0x63, .repack = 1, .odd_even = 0, .hv_invert = 0, .lsb_first = 0, .pn_swap = 0, .ports = 1, .bit_size = 0, .b_select = 0, .g_select = 1, .r_select = 2, .reg_de_exten = 0, .reg_blank_align = 0,
	.lvds_swap = 7, .clk_pin_swap = 0,

	.bl_inverter = 0,

	.lane_num = 8, .byte_num = 4, .region_num = 2, .color_fmt = 4,

	.vx1_lockn_option = 0, .vx1_counter_option = 0,

	.vx1_lvds_combo_ctl0 = 0x3FFCEE12, .vx1_lvds_combo_ctl1 = 0x80000150, .vx1_lvds_combo_ctl2 = 0x0000FFFF, .vx1_lvds_combo_ctl3 = 0x00000000,

	.pwm_duty = 128, .pwm_hz = 120,

	.panel_power_on_delay = 0, .panel_power_off_delay = 0, .pwm_enable_delay = 0, .pwm_disable_delay = 0, .clock_enable_delay = 0, .clock_disable_delay = 0, .backlight_power_on_delay = 0,
	.backlight_power_off_delay = 0,

	.ManufactureID = { 0x05, 0xAc }, .ProductID = { 0xff, 0xff }, .SerialNumber = { 0xff, 0xff, 0xff, 0xff }, .ManufactureDate = { 0x00, 0x19 }, .ChipID = { 0xfb, 0x0c }, .PanelInfo = 0x03,
	.ThreeDinfo = 0x04, .SpecicalInfo = 0x05
};

panel_config_t panel_aml_vb1_3B = { .output_mode = T_2160P50HZ420, .lvds_timing = TIMING_1920x1080P60, .vx1_timing = TIMING_3840x2160P60,

									.format = PANEL_YUV420,

									.reverse = 0, .scaler = 0,

									.clk = 0x63, .repack = 1, .odd_even = 0, .hv_invert = 0, .lsb_first = 0, .pn_swap = 0, .ports = 1, .bit_size = 0, .b_select = 0, .g_select = 1, .r_select = 2, .reg_de_exten = 0, .reg_blank_align = 0,
									.lvds_swap = 7, .clk_pin_swap = 0,

									.bl_inverter = 1,

									.lane_num = 8, .byte_num = 3, .region_num = 2, .color_fmt = 4,

									.vx1_lockn_option = 0, .vx1_counter_option = 0,

									.vx1_lvds_combo_ctl0 = 0x00005E00,
									/* def value 0x00000168, def eye chart value 0x00000178 */
									.vx1_lvds_combo_ctl1 = 0x00000168,
									/* def value 0x0FF0003F, def eye chart value 0x0FF2003F */
									.vx1_lvds_combo_ctl2 = 0x0FF0003F, .vx1_lvds_combo_ctl3 = 0x00000000,

									.pwm_duty = 128, .pwm_hz = 120,

									.panel_power_on_delay = 0, .panel_power_off_delay = 0, .pwm_enable_delay = 0, .pwm_disable_delay = 0, .clock_enable_delay = 0, .clock_disable_delay = 0, .backlight_power_on_delay = 0,
									.backlight_power_off_delay = 0
								  };

panel_config_t panel_aml_vb1_4B = { .output_mode = T_1080P50HZ44410BIT, .lvds_timing = TIMING_1920x1080P60, .vx1_timing = TIMING_3840x2160P60,

									.format = PANEL_YUV420,

									.reverse = 0, .scaler = 0,

									.clk = 0x63, .repack = 1, .odd_even = 0, .hv_invert = 0, .lsb_first = 0, .pn_swap = 0, .ports = 1, .bit_size = 0, .b_select = 0, .g_select = 1, .r_select = 2, .reg_de_exten = 0, .reg_blank_align = 0,
									.lvds_swap = 7, .clk_pin_swap = 0,

									.bl_inverter = 1,

									.lane_num = 8, .byte_num = 4, .region_num = 2, .color_fmt = 4,

									.vx1_lockn_option = -1, .vx1_counter_option = 0,

									.vx1_lvds_combo_ctl0 = 0x00005E00,
									/* def value 0x00000168, def eye chart value 0x00000178 */
									.vx1_lvds_combo_ctl1 = 0x00000168,
									/* def value 0x0FF0003F, def eye chart value 0x0FF2003F */
									.vx1_lvds_combo_ctl2 = 0x0FF0003F, .vx1_lvds_combo_ctl3 = 0x00000000,

									.pwm_duty = 128, .pwm_hz = 120,

									.panel_power_on_delay = 200,
									/* panel spec min is 1s */
									.panel_power_off_delay = 1000, .pwm_enable_delay = 0, .pwm_disable_delay = 0, .clock_enable_delay = 0, .clock_disable_delay = 150, .backlight_power_on_delay = 150, .backlight_power_off_delay =
										150,

									.ManufactureID = { 0x05, 0xAc }, .ProductID = { 0xff, 0xff }, .SerialNumber = { 0xff, 0xff, 0xff, 0xff }, .ManufactureDate = { 0x00, 0x19 }, .ChipID = { 0xfb, 0x0c }, .PanelInfo = 0x01,
									.ThreeDinfo = 0x00, .SpecicalInfo = 0x00,
								  };

panel_config_t panel_aml_vb1_4k1k_4B = { .output_mode = T_2160P50HZ42010BIT, .lvds_timing = TIMING_1920x1080P60, .vx1_timing = TIMING_3840x2160P60,

										 .format = PANEL_YUV420,

										 .reverse = 0, .scaler = 0,

										 .clk = 0x63, .repack = 1, .odd_even = 0, .hv_invert = 0, .lsb_first = 0, .pn_swap = 0, .ports = 1, .bit_size = 0, .b_select = 0, .g_select = 1, .r_select = 2, .reg_de_exten = 0, .reg_blank_align = 0,
										 .lvds_swap = 7, .clk_pin_swap = 0,

										 .bl_inverter = 1,

										 .lane_num = 8, .byte_num = 4, .region_num = 2, .color_fmt = 4,

										 .vx1_lockn_option = 0, .vx1_counter_option = 0,

										 .vx1_lvds_combo_ctl0 = 0x00005E00,
										 /* def value 0x00000168, def eye chart value 0x00000178 */
										 .vx1_lvds_combo_ctl1 = 0x00000168,
										 /* def value 0x0FF0003F, def eye chart value 0x0FF2003F */
										 .vx1_lvds_combo_ctl2 = 0x0FF0003F, .vx1_lvds_combo_ctl3 = 0x00000000,

										 .pwm_duty = 128, .pwm_hz = 120,

										 .panel_power_on_delay = 0, .panel_power_off_delay = 0, .pwm_enable_delay = 0, .pwm_disable_delay = 0, .clock_enable_delay = 0, .clock_disable_delay = 0, .backlight_power_on_delay = 0,
										 .backlight_power_off_delay = 0
									   };

panel_config_t panel_aml_vb1_4B_ext1 = { .output_mode = T_2160P50HZ42210BIT, .lvds_timing = TIMING_1920x1080P60, .vx1_timing = TIMING_3840x2160P60,

										 .format = PANEL_YUV420,

										 .reverse = 0, .scaler = 0,

										 .clk = 0x63, .repack = 1, .odd_even = 0, .hv_invert = 0, .lsb_first = 0, .pn_swap = 0, .ports = 1, .bit_size = 0, .b_select = 0, .g_select = 1, .r_select = 2, .reg_de_exten = 0, .reg_blank_align = 0,
										 .lvds_swap = 7, .clk_pin_swap = 0,

										 .bl_inverter = 1,

										 .lane_num = 8, .byte_num = 4, .region_num = 2, .color_fmt = 4,

										 .vx1_lockn_option = -1, .vx1_counter_option = 0,

										 .vx1_lvds_combo_ctl0 = 0x00005E00,
										 .vx1_lvds_combo_ctl1 = 0x00000168, //def value 0x00000168, def eye chart value 0x00000178
										 .vx1_lvds_combo_ctl2 = 0x0FF0003F, //def value 0x0FF0003F, def eye chart value 0x0FF2003F
										 .vx1_lvds_combo_ctl3 = 0x00000000,

										 .pwm_duty = 128, .pwm_hz = 120,

										 .panel_power_on_delay = 200, .panel_power_off_delay = 1000, /* panel spec min is 1s */
										 .pwm_enable_delay = 0, .pwm_disable_delay = 0, .clock_enable_delay = 0, .clock_disable_delay = 150, .backlight_power_on_delay = 150, .backlight_power_off_delay = 150,

										 .ManufactureID = { 0x05, 0xAc }, .ProductID = { 0xff, 0xff }, .SerialNumber = { 0xff, 0xff, 0xff, 0xff }, .ManufactureDate = { 0x00, 0x19 }, .ChipID = { 0xfb, 0x0c }, .PanelInfo = 0x01,
										 .ThreeDinfo = 0x00, .SpecicalInfo = 0x00,
									   };

panel_config_t panel_aml_vb1444_4B = { .output_mode = T_2160P50HZ444, .lvds_timing = TIMING_1920x1080P60, .vx1_timing = TIMING_3840x2160P60,

									   .format = PANEL_YUV444,

									   .reverse = 0, .scaler = 0,

									   .clk = 0x63, .repack = 1, .odd_even = 0, .hv_invert = 0, .lsb_first = 0, .pn_swap = 0, .ports = 1, .bit_size = 0, .b_select = 0, .g_select = 1, .r_select = 2, .reg_de_exten = 0, .reg_blank_align = 0,
									   .lvds_swap = 7, .clk_pin_swap = 0,

									   .bl_inverter = 1,

									   .lane_num = 8, .byte_num = 4, .region_num = 2, .color_fmt = 4,

									   .vx1_lockn_option = -1, .vx1_counter_option = 0,

									   .vx1_lvds_combo_ctl0 = 0x00005E00,
									   .vx1_lvds_combo_ctl1 = 0x00000168, //def value 0x00000168, def eye chart value 0x00000178
									   .vx1_lvds_combo_ctl2 = 0x0FF0003F, //def value 0x0FF0003F, def eye chart value 0x0FF2003F
									   .vx1_lvds_combo_ctl3 = 0x00000000,

									   .pwm_duty = 128, .pwm_hz = 120,

									   .panel_power_on_delay = 20, .panel_power_off_delay = 0, .pwm_enable_delay = 0, .pwm_disable_delay = 0, .clock_enable_delay = 0, .clock_disable_delay = 0, .backlight_power_on_delay = 500,
									   .backlight_power_off_delay = 500,

									   .ManufactureID = { 0x05, 0xAc }, .ProductID = { 0xff, 0xff }, .SerialNumber = { 0xff, 0xff, 0xff, 0xff }, .ManufactureDate = { 0x00, 0x19 }, .ChipID = { 0xfb, 0x0c }, .PanelInfo = 0x01,
									   .ThreeDinfo = 0x00, .SpecicalInfo = 0x00,
									 };

#define panel_power(id, action) panel_power_##action##_aml();
#define backlight_power(id, action) backlight_power_##action##_aml();

#define CC_PANEL_MAX_COUNT	(35)

int get_panel_max_count ( void )
{
	return CC_PANEL_MAX_COUNT;
}

int get_panel_def_id ( void )
{
#ifdef PROJECT_ID
	printf ( "id1-%d\n", PROJECT_ID );
	return PROJECT_ID;
#else /*
	   */
	printf ( "id2%d\n" );
	return 2;
#endif /*
	*/
}

panel_config_t *gPanelParams[CC_PANEL_MAX_COUNT + 1] = {

	&panel_aml, &panel_aml_vb1_3B, &panel_aml_vb1_4B, &panel_aml_vb1_4k1k_4B, &panel_aml_vb1_4B_ext1, &panel_aml_vb1444_4B, NULL,
};

panel_config_t *panel_param;
int hdmi_420Mode = 0;

static void get_panel_by_id ( int id )
{
	int tmp_params_cnt = 0;

	while ( gPanelParams[tmp_params_cnt] != NULL ) {
		tmp_params_cnt += 1;
	}

	printf ( "id (%d), panel params define count = %d, max panel count = %d.\n", id, tmp_params_cnt, CC_PANEL_MAX_COUNT );

	if ( id >= 0 && id < tmp_params_cnt && id < get_panel_max_count() ) {
		panel_param = gPanelParams[id];
		printf ( "use panel id (%d) panel param.\n", id );

	} else {
		printf ( "project id (%d) is undefined!\n", id );
		id = get_panel_def_id();
		panel_param = gPanelParams[id];
		printf ( "use default panel id (%d) panel param.\n", id );
	}

	//Qy for 4k2k 422 and 420 switch
	if (panel_param->output_mode == T_2160P50HZ42010BIT || \
		panel_param->output_mode == T_2160P50HZ420) {
		hdmi_420Mode = 1;
	} else {
		hdmi_420Mode = 0;
	}
}

vpu_outputmode_t get_output_mode ( void )
{
	return panel_param->output_mode;
}

static char *panel_format_table[] = { "1080p", "YUV420", "YUV44410bit", "YUV42010BIT", "YUV42210BIT", "yuv444" };

vpu_timing_t get_timing_mode ( void )
{
	vpu_timing_t curr_timing;
	vpu_outputmode_t out_mode;
	out_mode = get_output_mode();

	if ( out_mode == T_1080P50HZ ) {
		curr_timing = panel_param->lvds_timing;
		printf ( "the outputmode is lvds, the current timing is %d.\n", curr_timing );

	} else {
		curr_timing = panel_param->vx1_timing;
		printf ( "the outputmode is vx1, the current timing is %d.\n", curr_timing );
		printf ( "lane_num: %d, byte_num: %d, region_num: %d\n", panel_param->lane_num, panel_param->byte_num, panel_param->region_num );
		printf ( "format: %s\n", panel_format_table[panel_param->format] );
	}

	return curr_timing;
}

#ifndef IN_FBC_MAIN_CONFIG
void sleep ( int us )
{
	int n = us * ( 80 / 3 );

	while ( n-- )
		;
}
#endif

void mdelay ( int ms )
{
	sleep ( 4000 * ms );
}

void power_on_aml ( void )
{
	*P_PREG_PAD_GPIO0_EN_N |= ( 1 << 11 );

	if ( *P_PREG_PAD_GPIO0_I & ( 1 << 11 ) ) {
		printf ( "Not detect P311!!!" );
	}

	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 1 ) );
	*P_PREG_PAD_GPIO0_O |= ( 1 << 1 );
	mdelay ( 500 );
	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 12 ) );
	*P_PREG_PAD_GPIO0_O &= ( ~ ( 1 << 12 ) );
}

void power_off_aml ( void )
{
	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 1 ) );
	*P_PREG_PAD_GPIO0_O |= ( 1 << 1 );
}

void panel_power_on_aml ( void )
{
	*P_PREG_PAD_GPIO3_EN_N &= ( ~ ( 1 << 5 ) );
	*P_PREG_PAD_GPIO3_O |= ( 1 << 5 );
}

void panel_power_off_aml ( void )
{
	*P_PREG_PAD_GPIO3_EN_N &= ( ~ ( 1 << 5 ) );
	*P_PREG_PAD_GPIO3_O &= ( ~ ( 1 << 5 ) );
}

void backlight_power_on_aml ( void )
{
	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 2 ) );
	*P_PREG_PAD_GPIO0_O &= ( ~ ( 1 << 2 ) );
}

void backlight_power_off_aml ( void )
{
	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 2 ) );
	*P_PREG_PAD_GPIO0_O |= ( 1 << 2 );
}

void set_led_onoff ( unsigned char vcValue )
{
	*P_PERIPHS_PIN_MUX_0 &= ( ~ ( 1 << 3 ) );
	*P_PERIPHS_PIN_MUX_0 &= ( ~ ( 1 << 26 ) );
	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 3 ) );

	if ( vcValue ) {
		*P_PREG_PAD_GPIO0_O |= ( 1 << 3 );

	} else {
		*P_PREG_PAD_GPIO0_O &= ( ~ ( 1 << 3 ) );
	}
}

int panel_suspend ( void )
{
	backlight_power ( project_id, off );
	mdelay ( panel_param->backlight_power_off_delay );
	set_vx1_signal_off();
	lvds_phy_disable();
	mdelay ( panel_param->clock_disable_delay );
	mdelay ( panel_param->pwm_disable_delay );
	panel_power ( project_id, off );
	mdelay ( panel_param->panel_power_off_delay );
	return 0;
}

int panel_resume ( void )
{
	/* panel_power(project_id, on); */
	mdelay ( panel_param->panel_power_on_delay );
	mdelay ( panel_param->pwm_enable_delay );
	mdelay ( panel_param->clock_enable_delay );
	backlight_power ( project_id, on );
	return 0;
}

int get_panel_power_on_dly ( void )
{
	return panel_param->panel_power_on_delay;
}

int get_panel_pwm_inverter_flag ( void )
{
	return panel_param->bl_inverter;
}

void panel_backlight_power ( char val )
{
	if ( 0 == val ) {
		backlight_power ( project_id, off );
		;/*avoid checkPatch.pl check*/

	} else {
		backlight_power ( project_id, on );
	}
}

unsigned int reboot_func ( void )
{
	lvds_phy_disable(); /* turn off data/clk */
	mdelay ( 30 );
	panel_power_off_aml(); /* turn off panel power 12v(for tcon) */
	mdelay ( 15 );
}

void get_vx1_lvds_combo_ctl ( unsigned int *ctl0, unsigned int *ctl1, unsigned int *ctl2, unsigned int *ctl3 )
{
	*ctl0 = panel_param->vx1_lvds_combo_ctl0;
	*ctl1 = panel_param->vx1_lvds_combo_ctl1;
	*ctl2 = panel_param->vx1_lvds_combo_ctl2;
	*ctl3 = panel_param->vx1_lvds_combo_ctl3;
}

void panel_init ( void )
{
	int ret = 0;
#ifdef IN_FBC_MAIN_CONFIG
	project_id = read_project_id();
#else /*
	   */
	unsigned int tmp_prj_id_addr = SPI_BASE + USER_CRI_DATA_START;
	unsigned int tmp_crc = 0, prj_id_chksum = 0;
	int panel_max_cnt = get_panel_max_count();
	project_id = * ( int * ) ( tmp_prj_id_addr );
	prj_id_chksum = * ( int * ) ( tmp_prj_id_addr + 4 );
	tmp_crc = crc32 ( 0, ( unsigned char * ) &project_id, sizeof ( unsigned int ) );

	if ( tmp_crc != prj_id_chksum ) {
		printf ( "cri data project id checksum error" );
		printf ( "(%d, 0x%08X, 0x%08X)!\n", project_id, tmp_crc, prj_id_chksum );
		project_id = get_panel_def_id();
	}

	printf ( "project id is %d, address is 0x%08x\n", project_id, tmp_prj_id_addr );
#endif /*
	*/
	/* printf("current project_id is %d\n",project_id); */
	get_panel_by_id ( project_id );
#ifdef IN_FBC_MAIN_CONFIG

	if ( UiGetHaveLogoFlag() == 1 ) {
		register_backlight_func ( panel_backlight_power );
	}

	registerRebootTimming ( reboot_func );
#endif /*
	*/
}
