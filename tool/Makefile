GCC = gcc

OBJ_TOOLS = audio_param
OBJ_TOOLS += bin_wrapper
OBJ_TOOLS += bin_op
OBJ_TOOLS += add_check_info2bin

TOOLS_SRCS = audio_param.c
TOOLS_SRCS += bin_wrapper.c
TOOLS_SRCS += bin_op.c
TOOLS_SRCS += add_check_info2bin.c
TOOLS_SRCS += add_partition_info.c

CODESIZE = $(shell echo `stat -c %s ./../rom_code_orig.bin`)
DATASIZE = $(shell echo `stat -c %s ./../rom_data_orig.bin`)

CFLAGS += -DPROJECT_ID=$(PANEL_ID)
CFLAGS += -DPANEL_MODULE=\"$(PANEL_MODULE)\"

CFLAGS += -DPQ_BINARY_START=$(PQ_BINARY_START)
CFLAGS += -DPQ_BINARY_UNIT_SIZE=$(PQ_BINARY_UNIT_SIZE)

CFLAGS += -DFBC_USER_START=$(FBC_USER_START)
CFLAGS += -DFBC_USER_SIZE=$(FBC_USER_SIZE)

CFLAGS += -DUSER_HDCPKEY=$(USER_HDCPKEY)
CFLAGS += -DUSER_HDCPKEY_SIZE=$(USER_HDCPKEY_SIZE)

CFLAGS += -DUSER_SETTING_START=$(USER_SETTING_START)
CFLAGS += -DUSER_SETTING_SIZE=$(USER_SETTING_SIZE)

CFLAGS += -DUSER_WB_SETTING_START=$(USER_WB_SETTING_START)
CFLAGS += -DUSER_WB_SETTING_SIZE=$(USER_WB_SETTING_SIZE)

CFLAGS += -DUSER_PIC_SETTING_START=$(USER_PIC_SETTING_START)
CFLAGS += -DUSER_PIC_SETTING_SIZE=$(USER_PIC_SETTING_SIZE)

CFLAGS += -DUSER_CRI_DATA_SIZE=$(USER_CRI_DATA_SIZE)
CFLAGS += -DUSER_CRI_DATA_START=$(USER_CRI_DATA_START)

audio_param: audio_param.o
	$(GCC) $^ -o $@

bin_wrapper: bin_wrapper.o
	$(GCC) $^ -o $@

add_check_info2bin: add_check_info2bin.o
	$(GCC) $^ -o $@

config2bin: config2bin.o
	$(GCC) $^ -o $@

rom_map:
	echo "$(CODESIZE)" > rom_map.dat
	echo "$(DATASIZE)" >> rom_map.dat

all: clean rom_map $(OBJ_TOOLS)

clean:
	@$(RM) *.o
	@$(RM) rom_audioparam
	@$(RM) $(OBJ_TOOLS)

.c.o:
	$(GCC) -c $<