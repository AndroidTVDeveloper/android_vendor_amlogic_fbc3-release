#include <board_config.h>

const char* switch_p = ((void*)0);
int bit10_mode = 0;

void init_configures(void)
{
	switch_p = CONFIG_SWITCH_P;
	bit10_mode = CONFIG_BIT_MODE;
}