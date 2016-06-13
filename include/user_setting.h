#ifndef USER_SETTING_H
#define USER_SETTING_H

#include <vpp_api.h>
//#include <vpp_api.h>
#include <cmd.h>


#define HDCP_KEY_PARA 0x5A

#define CC_FACTORY_SN_SIZE     (20)

#define CC_CRI_REVERSED_LEN    (112)
#define CC_CRI_DEVICE_ID_LEN   (124)
#define CC_CRI_FAC_SN_LEN      (128)

struct system_setting {

	unsigned char sleep_time;
	/* 0 direct suspend, 1 direct power on */
	unsigned char power_on_mode;
	unsigned char watch_dog;

};

struct vpp_switch {

	/*VPU_MODULE_VPU,       //vpu uint
	 VPU_MODULE_TIMGEN,
	 VPU_MODULE_PATGEN,
	 VPU_MODULE_GAMMA,
	 VPU_MODULE_WB,      //WhiteBalance
	 VPU_MODULE_BC,      //Brightness&Contrast
	 VPU_MODULE_BCRGB,   //RGB Brightness&Contrast
	 VPU_MODULE_CM2,
	 VPU_MODULE_CSC1,
	 VPU_MODULE_DNLP,
	 VPU_MODULE_CSC0,
	 VPU_MODULE_OSD,
	 VPU_MODULE_BLEND,
	 VPU_MODULE_DEMURE,  //15
	 VPU_MODULE_OUTPUT,  //LVDS/VX1 output
	 VPU_MODULE_OSDDEC,  //OSD decoder */
	/* unsigned char vpu_enable; */
	/* unsigned char timgen_enable; */
	/* unsigned char patgen_enable; */
	unsigned char gamma_enable;

	unsigned char wb_enable;

	/* unsigned char bc_enable; */
	/* unsigned char bcrgb_enable; */
	unsigned char cm2_enable;

	/* unsigned char csc1_enable; */
	unsigned char dnlp_enable;

	unsigned char nature_light_enable;

	/* unsigned char csc0_enable; */
	/* unsigned char osd_enable; */
	/* unsigned char blend_enable; */
	/* unsigned char demura_enable; */
	/* unsigned char output_enable; */
	/* unsigned char osddec_enable; */
};

struct systems {

	unsigned project_id; /* must be 1st member */
	unsigned version;

	unsigned v2;

	char device_id[CC_CRI_DEVICE_ID_LEN];

	char factory_sn[CC_FACTORY_SN_SIZE];

	struct system_setting system;

	struct vpp_switch vpp;

};

typedef struct white_balance_setting {
	unsigned char r_gain;
	unsigned char g_gain;
	unsigned char b_gain;
	unsigned char r_offset;
	unsigned char g_offset;
	unsigned char b_offset;
} white_balance_setting_t;

typedef struct white_balance_setting_ext {
	unsigned short r_gain;
	unsigned short g_gain;
	unsigned short b_gain;
	unsigned short r_offset;
	unsigned short g_offset;
	unsigned short b_offset;
} white_balance_setting_ext_t;


struct user_setting {

	unsigned char change_flag;

	struct systems *system;

	struct audio_control *audio;

	vpu_fac_pq_t *pq;

};


//save_parameter write_user_setting(unsigned char *s);
//unsigned char read_user_setting();
unsigned read_project_id();
int write_project_id ( unsigned val );
char *read_device_id();
int write_device_id ( char *device_id );
char *read_factory_sn();
int write_factory_sn ( char *pSn );
unsigned char read_power_on_mode();
int write_power_on_mode ( unsigned char mode );
char *read_HDCP_KEY();
int write_HDCP_KEY ( const char *key, int len );
void d2d3_select ( int mode );

int write_wb_setting ( vpu_colortemp_t mode, white_balance_setting_t *val );
white_balance_setting_t *read_wb_setting ( vpu_colortemp_t mode );
int write_picmod_setting ( vpu_picmod_t mode, vpu_picmod_table_t *val );
vpu_picmod_table_t *read_picmod_setting ( vpu_picmod_t mode );

int select_project_id ( unsigned int id );

int load_default_setting();
int getcurentSource();

int get_nature_lihgt_en();
void set_nature_lihgt_en ( unsigned int on_off );

extern void init_setting_task ( void );

extern int clr_default_setting ( int clr_version_flag );
extern int clr_default_wb_setting ( void );

extern white_balance_setting_ext_t *req_wb_data_getting (vpu_colortemp_t);


#endif
