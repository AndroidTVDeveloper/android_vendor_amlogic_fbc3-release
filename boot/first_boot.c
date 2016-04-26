#include <serial.h>
#include <common.h>
#include <reboot.h>
#include <string.h>
#include <spi_regional_division.h>

typedef int ( *secend_boot ) ( int );

int main ( void )
{
	int check_info = 0;
	clock_init();
	serial_init ( 0 );
	printf ( "first boot serial init success!\n\n" );
	unsigned boot_flag = get_boot_flag();
	memcpy ( ( void * ) ( SRAM2_BASE + BOOT_CODE_SIZE ), ( void * ) ( BOOT_DATA_BASE ), BOOT_DATA_SIZE );
	memcpy ( ( void * ) SRAM2_BASE, ( void * ) ( BOOT_CODE_BASE ), BOOT_CODE_SIZE );
	( ( secend_boot ) SRAM2_BASE ) ( boot_flag );
	return 0;
}
