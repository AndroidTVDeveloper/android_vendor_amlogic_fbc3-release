#include <spi_flash.h>
#include <crc.h>
#include <relocate.h>


partition_info_t* get_partition_info(int section, int partition)
{
	unsigned int offset;

	printf("get partition info: section=%d, partition=%d\n", section, partition);
	if (partition == PARTITION_FIRST_BOOT)
		offset = FIRST_BOOT_INFO_OFFSET;

	else if ((section < 2) && (partition < PARTITION_NUM)) {
		offset = section ? SECTION_1_INFO_OFFSET : SECTION_0_INFO_OFFSET;
		offset += PARTITION_INFO_SIZE * (partition - 1);
	}
	else
		return 0;

	printf("get partition info: info offset=0x%x\n", offset);
	return (partition_info_t *)(SPI_BASE + offset);
}

int check_image_info(unsigned h_addr, unsigned t_addr)
//int check_partition_info(int section, int partition)
{
	return 0;
}


int copy_partition_to_sram2(int section, int partition)
{
	void *des, *src;
	partition_info_t *info = get_partition_info(section, partition);
	unsigned int crc;

	if (!info)
		return -1;

	des = (void *)(SRAM2_BASE);
	src = (void *)(SPI_BASE + info->code_offset);
	memcpy(des, src, info->code_size);
	crc = crc32(0, (void *)(SPI_BASE + info->code_offset - CHECK_INFO_SIZE), info->code_size+CHECK_INFO_SIZE);
	printf("SRAM2 code: offset=0x%x, size=0x%x, crc=0x%x\n", info->code_offset, info->code_size, crc);

	des = (void *)(SRAM2_BASE + info->code_size);
	src = (void *)(SPI_BASE + info->data_offset);
	memcpy(des, src, info->data_size);
	crc = crc32(crc, des, info->data_size);
	printf("SRAM2 data: offset=0x%x, size=0x%x, crc=0x%x\n", info->data_offset, info->data_size, crc);

	if (crc == info->crc) {
		printf("crc ok!\n");
		return 0;
	}
	else {
		printf("crc error!\n");
		return -1;
	}
}


int copy_partition_to_ccm(int section, int partition)
{
	void *des, *src;
	partition_info_t *info = get_partition_info(section, partition);
	unsigned int crc;

	if (!info)
		return -1;

	des = (void *)(ICCM_BASE);
	src = (void *)(SPI_BASE + info->code_offset);
	memcpy(des, src, info->code_size);
	crc = crc32(0, des, info->code_size);
	printf("ICCM_BASE: offset=0x%x, size=0x%x, crc=0x%x\n", info->code_offset, info->code_size, crc);

	des = (void *)(DCCM_BASE);
	src = (void *)(SPI_BASE + info->data_offset);
	memcpy(des, src, info->data_size);
	crc = crc32(crc, des, info->data_size);
	printf("DCCM_BASE: offset=0x%x, size=0x%x, crc=0x%x\n", info->data_offset, info->data_size, crc);

	if (crc == info->crc) {
		printf("crc ok!\n");
		return 0;
	}
	else {
		printf("crc error!\n");
		return -1;
	}
}


int move_image(
			struct spi_flash *flash,
			unsigned s_offs,
			unsigned b_offs,
			unsigned size)
{
	char buf[SPI_BLOCK_SIZE];
	char rbbuf[SPI_BLOCK_SIZE];
	int block_num;

	block_num = size / SPI_BLOCK_SIZE;
	if (size % SPI_BLOCK_SIZE)
		block_num++;
	printf("move image: block_num=%d\n", block_num);
	int i=0;
	while (block_num--) {
		printf("%d: s_offs=0x%x, b_offs=0x%x\n", i, s_offs, b_offs);
		i++;
		spi_flash_erase(flash, b_offs, SPI_BLOCK_SIZE);
		spi_flash_read(flash, s_offs, SPI_BLOCK_SIZE, buf);
		spi_flash_write(flash, b_offs, SPI_BLOCK_SIZE, buf);
		spi_flash_read(flash, b_offs, SPI_BLOCK_SIZE, rbbuf);
		if (memcmp(rbbuf, buf, SPI_BLOCK_SIZE))
			return -1;
		s_offs += SPI_BLOCK_SIZE;
		b_offs += SPI_BLOCK_SIZE;
	}

	return 0;
}

int print_image(
			struct spi_flash *flash,
			unsigned offs,
			unsigned size)
{
	unsigned char *p = (unsigned char *)(SPI_BASE + offs);
	int line = size / 8;

	if (size % 8)
		line++;
	printf("\n");
	while (line--) {
		printf("0x%x: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n", p,
				*p, *(p+1), *(p+2), *(p+3), *(p+4), *(p+5), *(p+6), *(p+7));
		p += 8;
	}
	printf("\n");
}

int section_backup(struct spi_flash *flash)
{
	int ret;

	printf("backup section...\n");
	//print_image(flash, SECTION_0_INFO_OFFSET, 128);
	//print_image(flash, SECTION_1_INFO_OFFSET, 128);
	ret = move_image(flash, SECTION_0_OFFSET, SECTION_1_OFFSET, SECTION_SIZE);
	if (!ret)
		ret = move_image(flash, SECTION_0_INFO_OFFSET, SECTION_1_INFO_OFFSET, SECTION_INFO_SIZE);
	if (ret)
		printf("backup section failed!\n");
	else
		printf("backup section success!\n");
	//print_image(flash, SECTION_0_INFO_OFFSET, 128);
	//print_image(flash, SECTION_1_INFO_OFFSET, 128);
	return ret;
}


int section_restore(struct spi_flash *flash)
{
	int ret;

	printf("restore section...\n");
	//print_image(flash, SECTION_0_INFO_OFFSET, 128);
	//print_image(flash, SECTION_1_INFO_OFFSET, 128);
	ret = move_image(flash, SECTION_1_OFFSET, SECTION_0_OFFSET, SECTION_SIZE);
	if (!ret)
		ret = move_image(flash, SECTION_1_INFO_OFFSET, SECTION_0_INFO_OFFSET, SECTION_INFO_SIZE);
	if (ret)
		printf("restore section failed!\n");
	else
		printf("restore section success!\n");
	//print_image(flash, SECTION_0_INFO_OFFSET, 128);
	//print_image(flash, SECTION_1_INFO_OFFSET, 128);
	return ret;
}
