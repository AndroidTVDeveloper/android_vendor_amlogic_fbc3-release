#include <string.h>
#include <stdlib.h>
#include <serial.h>
#include <common.h>
#include <reboot.h>
#include <remote.h>
#include <clock.h>
#include <customer_key_conf.h>
#include <gpio.h>
#include <inputdata.h>

#define DEVICE_UART_PORT_0       0
#define DEVICE_UART_PORT_1       1
#define DEVICE_UART_PORT_2       2

#include <sar_adc.h>

static void udelay ( int us )
{
	register int n = ( us ) * 105;

	while ( n-- )
		;
}

void suspend_power_on ( void )
{
	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 1 ) );
	*P_PREG_PAD_GPIO0_O |= ( 1 << 1 );
}

void suspend_power_off ( void )
{
	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 1 ) );
	*P_PREG_PAD_GPIO0_O &= ( ~ ( 1 << 1 ) );
}

#if 0
void reset_sw_high ( void )
{
	*P_PERIPHS_PIN_MUX_0 &= ( ~ ( 1 << 15 ) ); /* disable UART_CTS_AO_C */
	*P_PERIPHS_PIN_MUX_0 &= ( ~ ( 1 << 22 ) ); /* disable JTAG_TDI */
	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 15 ) );
	*P_PREG_PAD_GPIO0_O |= ( 1 << 15 );
}

void reset_sw_low ( void )
{
	*P_PERIPHS_PIN_MUX_0 &= ( ~ ( 1 << 15 ) ); /* disable UART_CTS_AO_C */
	*P_PERIPHS_PIN_MUX_0 &= ( ~ ( 1 << 22 ) ); /* disable JTAG_TDI */
	*P_PREG_PAD_GPIO0_EN_N &= ( ~ ( 1 << 15 ) );
	*P_PREG_PAD_GPIO0_O &= ( ~ ( 1 << 15 ) );
}
#endif

int main ( int argc, char *argv[] )
{
	int key_value = 0;
	unsigned int pin_mux_1_value = 0;

	/* serial_init(DEVICE_UART_PORT_0); */
	/* serial_puts("enter suspend main...\n"); */

	if ( calibrate_internal_osc() ) {
		/* serial_puts("calibrate internal osc failed.\n"); */
		reboot ( 0 );
	}

	/* serial_puts("calibrate internal osc sucess.\n"); */
	pin_mux_1_value = *P_PERIPHS_PIN_MUX_1;
	*P_PERIPHS_PIN_MUX_1 = 0x0;
#ifdef SUSPEND_32K
	switch_clk_to_32k();
	ctrl_crystal_pad ( 0 );
	/* reset_sw_low(); */
#else
	switch_clk_to_24M();
#endif
	led_pwm_init();
	led_bl_level_set ( 128 );
	udelay ( 20 );
	suspend_power_off();
#ifndef SUSPEND_32K
	sar_adc_init();
	INPUTDATA inputdata;
	set_redetect_flag();
#endif
	resume_remote ( REMOTE_TYPE );
#ifdef SUSPEND_32K
	saradc_init ( 1 );
	adc_start_sample ( 0 );
#endif

	do {
#ifndef SUSPEND_32K
		udelay ( 1000 * 10 );
#endif
		/* serial_puts("."); */
		key_value = query_key_value();

		if ( customer_key_map[10][0] == key_value ) {
			break;
		}

#ifndef SUSPEND_32K

		if ( !detect_adc_key ( 1, &inputdata ) ) {
			if ( inputdata.input_type == 0 ) {
				break;
			}
		}

#endif
#ifdef SUSPEND_32K

		if ( is_adc_finished() ) {
			/* adc_key_code[0--4]:down,up,left,right,ok(enter) */
			if ( adc_key_code[4] == adc_detect_key ( 0 ) ) {
				break;

			} else {
				adc_start_sample ( 0 );
			}
		}

#endif
	} while ( 1 );

	ctrl_crystal_pad ( 1 );
	suspend_power_on();
#ifndef SUSPEND_32K
	udelay ( 30 * 1000 );
#endif
	*P_PERIPHS_PIN_MUX_1 = pin_mux_1_value;
#ifdef SUSPEND_32K
	switch_clk_to_24m();
	reset_spi();
	reset_ee();
	reset_ir();
	reboot_sw ( REBOOT_FLAG_FROM_SUSPEND );
#else
	reboot ( REBOOT_FLAG_FROM_SUSPEND );
#endif
	return 0;
}
