#include <relocate.h>
#include <spi_regional_division.h>

/* boot code running in SPI, not need to copy,
 except when need to upgrade boot code */
void copy_data_from_spi_to_sram2 ( int start_addr, int length )
{
	int *des = ( int * ) SRAM2_BASE;
	int *src = ( int * ) start_addr;
	int i = 0;

	for ( i = 0; i < length; i++ ) {
		* ( des++ ) = * ( src++ );
	}
}

void copy_code_from_spi_to_ICCM ( int start_addr, int length )
{
	int *des = ( int * ) ICCM_BASE;
	int *src = ( int * ) start_addr;
	int i = 0;

	for ( i = 0; i < length; i++ ) {
#if (BOOT_DEBUG_VERSION == 1)

		if ( i % 256 == 0 ) {
			serial_putc ( '.' );
		}

#endif
		* ( des++ ) = * ( src++ );
	}

	serial_putc ( '\n' );
}

void copy_data_from_spi_to_DCCM ( int start_addr, int length )
{
	int *des = ( int * ) DCCM_BASE;
	int *src = ( int * ) start_addr;
	int i = 0;

	for ( i = 0; i < length; i++ ) {
#if (BOOT_DEBUG_VERSION == 1)

		if ( i % 256 == 0 ) {
			serial_putc ( '.' );
		}

#endif
		* ( des++ ) = * ( src++ );
	}

	serial_putc ( '\n' );
}
