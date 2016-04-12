ifeq ($(ARC_COMPILER), GNU)
  include makefile_gnu_main
else
  ifeq ($(ARC_COMPILER), METAWARE)
    include makefile_metaware_main
  endif
endif

PRODUCT = "fbc-main"
VERSION = v1.0
VERSION_FILE = version_autogenarated.h

LD_LIB_PATH=$(ARC_TOOLCHAIN_PATH)/../lib/a6
CC_FLAGS+= -I./include/vpp/
CC_FLAGS+= -I./project/include/
ASM_FLAGS+= -I./include/vpp/
ASM_FLAGS+= -I./project/include/

CC_FLAGS += -DIN_FBC_MAIN_CONFIG

CC_FLAGS += -DUI_HAVE_LOGO=$(UI_HAVE_LOGO)

CC_FLAGS += -DPROJECT_ID=$(PANEL_ID)
CC_FLAGS += -DPANEL_MODULE=\"$(PANEL_MODULE)\"

CC_FLAGS += -DENABLE_AVMUTE_CONTROL=$(ENABLE_AVMUTE_CONTROL)

CC_FLAGS += -DPQ_BINARY_START=$(PQ_BINARY_START)
CC_FLAGS += -DPQ_BINARY_UNIT_SIZE=$(PQ_BINARY_UNIT_SIZE)

CC_FLAGS += -DFBC_USER_START=$(FBC_USER_START)
CC_FLAGS += -DFBC_USER_SIZE=$(FBC_USER_SIZE)

CC_FLAGS += -DUSER_HDCPKEY=$(USER_HDCPKEY)
CC_FLAGS += -DUSER_HDCPKEY_SIZE=$(USER_HDCPKEY_SIZE)

CC_FLAGS += -DUSER_SETTING_START=$(USER_SETTING_START)
CC_FLAGS += -DUSER_SETTING_SIZE=$(USER_SETTING_SIZE)

CC_FLAGS += -DUSER_WB_SETTING_START=$(USER_WB_SETTING_START)
CC_FLAGS += -DUSER_WB_SETTING_SIZE=$(USER_WB_SETTING_SIZE)

CC_FLAGS += -DUSER_PIC_SETTING_START=$(USER_PIC_SETTING_START)
CC_FLAGS += -DUSER_PIC_SETTING_SIZE=$(USER_PIC_SETTING_SIZE)

CC_FLAGS += -DUSER_CRI_DATA_SIZE=$(USER_CRI_DATA_SIZE)
CC_FLAGS += -DUSER_CRI_DATA_START=$(USER_CRI_DATA_START)

ifeq ($(ENABLE_AUTO_BACKLIGHT_CONTROL), 1)
    CC_FLAGS += -DENABLE_AUTO_BACKLIGHT
endif

ASM_FLAGS += -DIN_FBC_MAIN_CONFIG

SSRCS	= driver/vtable.s

MAIN    = main.c
SRCS	= common/printf.c
SRCS	+= common/console.c
SRCS	+= common/command.c
SRCS	+= common/cmd_test.c
SRCS	+= common/cmd_suspend.c
SRCS	+= common/cmd_debug.c
SRCS	+= common/dbg_task.c
SRCS	+= common/cmd_reboot.c
SRCS	+= common/cmd_sf.c
SRCS    += common/cmd_parameters.c
SRCS	+= common/test_running_spi.c
SRCS	+= common/build_version.c
SRCS	+= common/cmd_cri.c

PRJ_SRCS    += project/user_setting.c
PRJ_SRCS	+= project/panel.c
PRJ_SRCS	+= project/ui.c
PRJ_SRCS    += project/customer_key_conf.c

OBJS     = $(SSRCS:%.s=%.o$(SUFFIX)) $(MAIN:%.c=%.o$(SUFFIX)) $(SRCS:%.c=%.o$(SUFFIX)) $(PRJ_SRCS:%.c=%.o$(SUFFIX))

export CC_FLAGS_ADD
export LD_FLAGS_ADD

main_mem_code_size = $(shell cat mm_size)
main_spi_code_size = $(shell cat ms_size)

export_symbols_lib:
	echo "#ifndef RLINK_H" > export_symbols.h
	echo "#define RLINK_H" >> export_symbols.h
	printf "\textern  const char* switch_p;\n" >> export_symbols.h
	printf "\textern  int bit10_mode;\n" >> export_symbols.h
	echo "#endif" >> export_symbols.h

	echo "#include \"export_symbols.h\"" > export_symbols.c
	printf "\n" >> export_symbols.c
	echo "const char* switch_p = \"$(p_switch)\";" >> export_symbols.c
	echo "int bit10_mode = $(BIT10_MODE);" >> export_symbols.c

	$(CC) $(CC_FLAGS) export_symbols.c

all : main.elf
main_libs:=fbc.a

add_flags:
	$(eval CC_FLAGS_ADD = -Ml)
	$(eval ASM_FLAGS_ADD = -Ml)
main.elf: version.info add_flags $(OBJS)
	$(LD) $(OBJS) $(main_libs) export_symbols.o $(LD_LIB_PATH)/le/crt1.o $(LD_LIB_PATH)/le/crti.o $(LD_FLAGS) main.cmd -e _start -o $@
	$(DUMPELF) $(DUMP_FLAGS) $@ > main.asm
sections_info.asm : main.elf
	$(DUMPELF) -s -q main.elf > sections_info.asm

mem_code_size : sections_info.asm
	./get_section_size.sh sections_info.asm mm_size 10 .start .text .fini .init .plt

spi_code_size : sections_info.asm
	./get_section_size.sh sections_info.asm ms_size 1091194880 .running.on.spi .spi.text

spi_main: clean main.elf mem_code_size spi_code_size
	$(CP) main.elf rom.elf
	$(STRIP) $(STRIP_FALGS) rom.elf
	$(DUMPELF) $(DUMP_FLAGS) rom.elf > rom.asm
	$(ELF2BIN) rom.elf rom_code_orig.bin -b0x0 -t$(main_mem_code_size)
	$(ELF2BIN) rom.elf rs_code_orig.bin -b0x410A5000 -t$(main_spi_code_size)
	$(OBJCOPY) $(COPY_FLAGS) rom.elf -o rom_rodata_orig.bin -cl
	$(OBJCOPY) $(COPY_FLAGS) rom.elf -o rom_data_orig.bin -cd
	$(CP) ./project/$(PANEL_PQ) ./project/rom_pqparam
	$(MK) -C tool all
	$(MK) -f makefile.rom

spi_boot:
	$(MK) -C boot clean
	$(MK) -C boot rom

spi_header:
	bash openssl_operate.sh boot/first_boot_code.bin  boot/first_boot_data.bin
	gcc tool/add_partition_info.c -o add_partition_info
	./add_partition_info -t first_boot -i boot/size.data -o par_info.bin
	@echo "generate head.sha256 that write efuse, 520 = sizeof (struct rsa_public_key)"
	dd if=par_info.bin of=rsa_key.bin bs=520 count=1
	openssl dgst -sha256 -binary -out rsa_key/head.sha256 rsa_key.bin
	dd if=/dev/zero of=blocks_info.bin bs=4096 count=1

spi:export_symbols_lib spi_main spi_boot spi_header
	./tool/bin_op -i project -t pq -o pq.bin
	cat par_info.bin blocks_info.bin boot/boot.bin rom.bin >> spi.bin
	cat spi.bin pq.bin >> spi_pq.bin
	./tool/bin_op -i pq.bin -t all -o spi_all.bin
	$(MKDIR) out
	@split -b 64k -d spi.bin ./out/spi.bin.
	bin2hex spi.bin 1 > spi.hex

version.info:
	@$(RM) include/$(VERSION_FILE)
	@echo "#define PRODUCT_NAME \"$(PRODUCT)\"" > include/$(VERSION_FILE)
	@echo "#define VERSION \"$(VERSION)\"" >> include/$(VERSION_FILE)
	@echo "#define RELEASE_DATE \"$(RELEASE_DATE)\"" >> include/$(VERSION_FILE)
	@echo "#define RELEASE_TIME \"$(RELEASE_TIME)\"" >> include/$(VERSION_FILE)
	@echo "#define RELEASE_USER \"$(RELEASE_USER)\"" >> include/$(VERSION_FILE)
	@echo "#define FBC_BOOT_VERSION \"$(PRODUCT) $(VERSION) (RELEASE:$(RELEASE_USER) COMPILER:$(ARC_COMPILER)) $(RELEASE_DATE) $(RELEASE_TIME)\"" >> include/$(VERSION_FILE)
	@echo "#define FBC_GIT_BRANCH \"$(FBC_GIT_BRANCH)\"" >> include/$(VERSION_FILE)
	@echo "#define FBC_GIT_COMMIT \"$(FBC_GIT_COMMIT)\"" >> include/$(VERSION_FILE)
	@echo "#define FBC_GIT_UNCOMMIT_FILE_NUM \"$(FBC_GIT_UNCOMMIT_FILE_NUM)\"" >> include/$(VERSION_FILE)
	@echo "#define FBC_GIT_LAST_CHANGED \"$(FBC_GIT_LAST_CHANGED)\"" >> include/$(VERSION_FILE)
	@echo "#define FBC_BUILD_TIME \"$(FBC_BUILD_TIME)\"" >> include/$(VERSION_FILE)
	@echo "#define FBC_BUILD_NAME \"$(FBC_BUILD_NAME)\"" >> include/$(VERSION_FILE)
	@echo "$(FBC_BUILD_TIME)" > ./tool/version.dat

clean:
	@$(RM) include/$(VERSION_FILE)
	@$(RM) tool/*.dat
	@$(RM) $(OBJS)
	@$(RM) *.elf
	@$(RM) *.bin
	@$(RM) *.bin.*
	@$(RM) *.hex
	@$(RM) *.asm
	@$(RM) mm_size
	@$(RM) ms_size
	@$(RM) rom_map.*
	@$(RM) ./project/rom_pqparam
	$(MK) -C boot clean
	$(MK) -C tool clean
	@$(RM) -r ./out
	@$(RM) -rf rsa_key/signature2048_1* rsa_key/head.sha256
	@$(RM) add_partition_info

distclean:
	@$(RM) export_symbols*

.c.o:
	$(CC) $(CC_FLAGS) $< -o $@

.s.o:
	$(ASM) $(ASM_FLAGS) $< -o $@
