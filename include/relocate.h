#ifndef __FBC_RELOCATE_H__
#define __FBC_RELOCATE_H__

void copy_data_from_spi_to_sram2(int start_addr, int length);
void copy_code_from_spi_to_ICCM(int start_addr, int length);
void copy_data_from_spi_to_DCCM(int start_addr, int length);

#endif	//__FBC_RELOCATE_H__