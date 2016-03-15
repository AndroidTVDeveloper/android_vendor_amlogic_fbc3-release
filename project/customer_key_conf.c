
#include <key_const.h>
#include <customer_key_conf.h>

int REMOTE_TYPE = DECODEMODE_NEC;

int remote_coustomer_code = 0xfe01;	/* customer code */
float ADC_VOLTAGE_MAX = 3.3;

int send_key_to_soc = 1;

int send_key_to_local_task = 1;

int send_remote_to_soc = 1;

int send_remote_to_local_task = 1;

int CUSTOMER_REMOTE_NUMS = 22;

int customer_key_map[][2] = {
	{25, AMLKEY_UP},
	{26, AMLKEY_DOWN},
	{27, AMLKEY_LEFT},
	{28, AMLKEY_RIGHT},
	{29, AMLKEY_ENTER},
	{21, AMLKEY_MENU},
	{22, AMLKEY_EXIT},
	{14, AMLKEY_VOL_PLUS},
	{15, AMLKEY_VOL_MINUS},
	{67, AMLKEY_BACK},
	{16, AMLKEY_POWER},
	{23, AMLKEY_MUTE},
	{10, AMLKEY_NUM_0},
	{1, AMLKEY_NUM_1},
	{2, AMLKEY_NUM_2},
	{3, AMLKEY_NUM_3},
	{4, AMLKEY_NUM_4},
	{5, AMLKEY_NUM_5},
	{6, AMLKEY_NUM_6},
	{7, AMLKEY_NUM_7},
	{8, AMLKEY_NUM_8},
	{9, AMLKEY_NUM_9},
};

int ADC_KEY_NUM_MAX = 5;

/* array num is ADC_KEY_NUM_MAX */
/* down,up,left,right,ok(enter) */
float adc_key_vol[] = { 0.48, 0.97, 1.42, 1.86, 2.8 };

/* array num is ADC_KEY_NUM_MAX */
int adc_key_code[] = {
	AMLKEY_DOWN, AMLKEY_UP, AMLKEY_LEFT, AMLKEY_RIGHT, AMLKEY_ENTER
};

int adc_key_code1[] = {
	AMLKEY_RIGHT, AMLKEY_NOP, AMLKEY_ENTER, AMLKEY_NOP, AMLKEY_NOP
};
