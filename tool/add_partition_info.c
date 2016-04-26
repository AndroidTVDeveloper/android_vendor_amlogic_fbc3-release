
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "../include/spi_regional_division.h"
#include "../rsa_key/signature2048_1.h"

struct partition_info_t {
	unsigned code_offset;
	unsigned code_size;
	unsigned data_offset;
	unsigned data_size;
	unsigned bss_offset;
	unsigned bss_size;
	unsigned readonly_offset;
	unsigned readonly_size;
	unsigned char signature[256];
};

#define HAS_INPUT_FILE		(0x1 << 0)
#define HAS_OUTPUT_FILE	(0x1 << 1)
#define HAS_TYPE_INFO		(0x1 << 2)

static int cmd_flag;
static char *optstring = "i:o:t:";

static struct option long_options[] = {
	{"input", 1, NULL, 'i'},
	{"output", 1, NULL, 'o'},
	{"tpye", 1, NULL, 't'},
};

struct rsa_public_key {
	/* Length of n[] in number of uint32_t */
	unsigned int len;
	/* modulus as little endian array */
	unsigned int n[64];
	/* R^2 as little endian array */
	unsigned int rr[64];
	/* -1 / n[0] mod 2^32 */
	unsigned int n0inv;
};

struct rsa_public_key rsa_key =
#include "../rsa_key/rsa2048_pkey_1.h"

	int main ( int argc, char *argv[] )
{
	int i = 0, flag = 0, val = 0;
	unsigned info_size = 0;
	struct partition_info_t partition_info = { 0 };
	unsigned int code_len = 0, data_len = 0;
	unsigned char data_buff[64 + 1], *buff = NULL;
	unsigned char *temp1 = NULL, *temp2 = NULL;
	char *if_name = NULL, *of_name = NULL, *tp_name = NULL;
	FILE *out = NULL, *in = NULL;

	if ( argc != 7 ) {
		printf ( "Usage example ./%s ", argv[0] );
		printf ( "-t first_boot -i boot/size.data" );
		printf ( " -o par_info.bin" );
		goto EXT_0;
	}

	while ( ( val = getopt_long ( argc, argv,
								  optstring, long_options, NULL ) ) != -1 ) {
		switch ( val ) {
			case 'i':
				cmd_flag |= HAS_INPUT_FILE;
				if_name = optarg;
				break;

			case 'o':
				cmd_flag |= HAS_OUTPUT_FILE;
				of_name = optarg;
				break;

			case 't':
				cmd_flag |= HAS_TYPE_INFO;
				tp_name = optarg;
				break;
		}
	}

	if ( cmd_flag & HAS_INPUT_FILE ) {
		printf ( "if_name: %s\n", if_name );
		in = fopen ( if_name, "r" );

		if ( NULL == in ) {
			printf ( "open %s failed!\n", if_name );
			goto EXT_0;
		}

		clearerr ( in );
		memset ( data_buff, '\0', sizeof ( data_buff ) );
		fread ( data_buff, 1, sizeof ( data_buff ), in );
		temp1 = strchr ( data_buff, '\n' );
		*temp1 = '\0';
		code_len = atoi ( data_buff );
		temp2 = ++temp1;
		temp1 = strchr ( temp1, '\n' );
		*temp1 = '\0';
		data_len = atoi ( temp2 );
		printf ( "data_len:%d, code_len:%d\n", data_len, code_len );
	}

	if ( cmd_flag & HAS_TYPE_INFO ) {
		printf ( "tp_name: %s\n", tp_name );

		if ( !strcmp ( "first_boot", tp_name ) ) {
			info_size = PARTITION_INFO_OFFSET + PARTITION_INFO_SIZE;

		} else {
			info_size = PARTITION_INFO_OFFSET + PARTITION_INFO_SIZE;
		}

		/* goto EXT_1; */
		printf ( "info_size = %08x\n", info_size );
	}

	if ( cmd_flag & HAS_OUTPUT_FILE ) {
		printf ( "of_name: %s\n", of_name );
		out = fopen ( of_name, "wb" );

		if ( NULL == out ) {
			printf ( "open %s failed!\n", of_name );
			goto EXT_1;
		}

		clearerr ( out );
	}

	buff = ( unsigned char * ) malloc ( info_size );

	if ( NULL == buff ) {
		printf ( "malloc buff failure\n" );
		goto EXT_2;
	}

	if ( !strcmp ( "first_boot", tp_name ) ) {
		memset ( &partition_info, 0, sizeof ( partition_info ) );
		partition_info.code_offset = FIRST_BOOT_CODE_BASE - SPI_BASE;
		partition_info.code_size = FIRST_BOOT_CODE_SIZE;
		partition_info.data_offset = FIRST_BOOT_DATA_BASE - SPI_BASE;
		partition_info.data_size = FIRST_BOOT_DATA_SIZE;

	} else {
		printf ( "Now we only support first_boot\n" );
		goto EXT_2;
	}

	memcpy ( partition_info.signature,
			 rsa_key_signature2048_1, sizeof ( rsa_key_signature2048_1 ) );
	memset ( buff, '\0', info_size );
	memcpy ( buff + PARTITION_INFO_OFFSET,
			 &partition_info, sizeof ( partition_info ) );
	memcpy ( buff, &rsa_key, sizeof ( rsa_key ) );
	fwrite ( buff, info_size, 1, out );
	fflush ( out );
EXT_2:
	free ( buff );
	buff = NULL;
	fclose ( out );
EXT_1:
	fclose ( in );
EXT_0:
	return 0;
}
