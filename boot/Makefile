ifeq ($(ARC_COMPILER), GNU)
  include ../makefile_gnu
else
  ifeq ($(ARC_COMPILER), METAWARE)
    include ../makefile_metaware
  endif
endif

LD_LIB_PATH=$(ARC_TOOLCHAIN_PATH)/../lib/a6
LD_PRE_BOOT_PATH=preboot
LITE_CONF_HFILE=lite_conf.h
CC_FLAGS+= -I./../include/vpp/
CC_FLAGS+= -I./../project/include/
CC_FLAGS += -DBOOT_DEBUG_VERSION=$(BOOT_DEBUG_VERSION)
CC_FLAGS += -DPROJECT_ID=$(PANEL_ID)
CC_FLAGS += -DENABLE_AVMUTE_CONTROL=$(ENABLE_AVMUTE_CONTROL)
CC_FLAGS += -DUSER_CRI_DATA_START=$(USER_CRI_DATA_START)
ASM_FLAGS+= -I./../include/vpp/
ASM_FLAGS+= -I./../project/include/

FIRST_BOOT_LIB = fbc_first_boot.a
BOOT_LIB = fbc_second_boot.a
SUSPEND_LIB = fbc_suspend.a
UPDATE_LIB = fbc_update.a

OBJ_SRCS = ./first_boot.c
OBJ_SRCS += ./relocate.c
OBJ_SRCS += ./mini_console.c

FIRST_SSRCS	 = boot_rom.s
FIRST_SRCS = ./$(LD_PRE_BOOT_PATH)/first_boot.c
FIRST_SRCS += ./$(LD_PRE_BOOT_PATH)/relocate.c
FIRST_SRCS += ./$(LD_PRE_BOOT_PATH)/mini_console.c

FIRST_SOBJS    = $(FIRST_SSRCS:%.s=%.o$(SUFFIX))
FIRST_OBJS     = $(FIRST_SRCS:%.c=%.o$(SUFFIX))

SRCS     = boot.c
SRCS     += relocate.c
SRCS     += mini_console.c
ifeq ($(wildcard ../map.c),)
else
SRCS     += ../map.c
CC_FLAGS_ADD += -DHAS_ROM_MAP
endif
SRCS += ../common/build_version.c

OBJS     = $(SSRCS:%.s=%.o$(SUFFIX)) $(SRCS:%.c=%.o$(SUFFIX))

SUS_SSRCS	 = boot_rom.s
SUS_SRCS     = suspend.c
SUS_SRCS    += ../project/customer_key_conf.c
SUS_OBJS     = $(SUS_SSRCS:%.s=%.o$(SUFFIX)) $(SUS_SRCS:%.c=%.o$(SUFFIX))

USRCS = update_main.c
USRCS += update.c
USRCS += ../project/panel.c
USSRCS = ../driver/vtable.s
UOBJS = $(USSRCS:%.s=%.o$(SUFFIX)) $(USRCS:%.c=%.o$(SUFFIX))

export CC_FLAGS_ADD
export ASM_FLAGS_ADD
export LD_FLAGS_ADD

export_symbols_lib:
	echo "#ifndef RLINK_H" > ../export_symbols.h
	echo "#define RLINK_H" >> ../export_symbols.h
	printf "\textern  const char* switch_p;\n" >> ../export_symbols.h
	printf "\textern  int bit10_mode;\n" >> ../export_symbols.h
	echo "#endif" >> ../export_symbols.h

	echo "#include \"export_symbols.h\"" > ../export_symbols.c
	printf "\n" >> ../export_symbols.c
	echo "const char* switch_p = \"$(p_switch)\";" >> ../export_symbols.c
	echo "int bit10_mode = $(BIT10_MODE);" >> ../export_symbols.c

	$(CC) $(CC_FLAGS) ../export_symbols.c -o ../export_symbols.o

all : clean second_boot.elf suspend.elf update.elf

first_boot.elf: pre_boot_src $(FIRST_SOBJS) $(FIRST_OBJS) $(FIRST_BOOT_LIB)
	$(LD) $(FIRST_SOBJS) $(FIRST_OBJS) $(FIRST_BOOT_LIB) $(LD_FLAGS) first_boot.cmd -o first_boot.elf
	$(DUMPELF) $(DUMP_FLAGS) first_boot.elf > first_boot.asm
	$(STRIP) $(STRIP_FALGS) first_boot.elf
	$(OBJCOPY) $(COPY_FLAGS) $@ -o first_boot_code.bin -ctl
	$(OBJCOPY) $(COPY_FLAGS) $@ -o first_boot_data.bin -cd
	$(DUMPELF) $(DUMP_FLAGS) $@ > first_boot.asm

second_boot.elf: clean_ex1_flags $(SOBJS) $(OBJS) $(BOOT_LIB)
	$(LD) $(SOBJS) $(OBJS) $(BOOT_LIB) $(LD_FLAGS) boot.cmd -o second_boot.elf
	$(DUMPELF) $(DUMP_FLAGS) second_boot.elf > second_boot.asm
	$(STRIP) $(STRIP_FALGS) second_boot.elf
	$(OBJCOPY) $(COPY_FLAGS) $@ -o second_boot_code.bin -ctl
	$(OBJCOPY) $(COPY_FLAGS) $@ -o second_boot_data.bin -cd
	$(DUMPELF) $(DUMP_FLAGS) $@ > second_boot.asm

suspend.elf: $(SUS_SOBJS) $(SUS_OBJS) $(SUSPEND_LIB)
	$(LD) $(SUS_SOBJS) $(SUS_OBJS) $(SUSPEND_LIB) $(LD_FLAGS) suspend.cmd -o suspend.elf
	$(DUMPELF) $(DUMP_FLAGS) suspend.elf > suspend.asm
	$(STRIP) $(STRIP_FALGS) suspend.elf
	$(OBJCOPY) $(COPY_FLAGS) $@ -o suspend_code.bin -ctl
	$(OBJCOPY) $(COPY_FLAGS) $@ -o suspend_data.bin -cd
	$(DUMPELF) $(DUMP_FLAGS) $@ > suspend.asm

update.elf: clean_ex2_flags $(UOBJS) $(UPDATE_LIB)
	$(LD) $(UOBJS) $(UPDATE_LIB) ../export_symbols.o  $(LD_LIB_PATH)/le/crt1.o $(LD_LIB_PATH)/le/crti.o $(LD_FLAGS) update.cmd -lhlt -e _start -o update.elf
	$(DUMPELF) $(DUMP_FLAGS) update.elf > update.asm
	$(STRIP) $(STRIP_FALGS) update.elf
	$(OBJCOPY) $(COPY_FLAGS) $@ -o update_code.bin -ctl
	$(OBJCOPY) $(COPY_FLAGS) $@ -o update_data.bin -cd
	$(DUMPELF) $(DUMP_FLAGS) $@ > update.asm
	@$(RM) update_main.o

rom: clean first_boot.elf second_boot.elf suspend.elf update.elf
	$(MK) -f makefile.rom
	stat -c %s first_boot_code.bin > size.data
	stat -c %s first_boot_data.bin >> size.data

pre_boot_src: add_ex1_flags
	@$(MKDIR) $(LD_PRE_BOOT_PATH)
	@echo "cp: $(CP)"
	@$(CP) $(OBJ_SRCS) ./$(LD_PRE_BOOT_PATH)

add_ex1_flags:
	$(eval CC_FLAGS_EX1 += -D_IN_PRE_BOOT_)
	$(eval ASM_FLAGS_EX1 += -D_IN_PRE_BOOT_)

clean_ex1_flags:
	$(eval CC_FLAGS_EX1 = )
	$(eval ASM_FLAGS_EX1 = )

add_ex2_flags:
	$(eval CC_FLAGS_EX2 += -D_USE_WITHOUT_UI_UPDATE_)
	$(eval ASM_FLAGS_EX2 += -D_USE_WITHOUT_UI_UPDATE_)

clean_ex2_flags:
	$(eval CC_FLAGS_EX2 = )
	$(eval ASM_FLAGS_EX2 = )

clean:
	@rm -f $(OBJS) $(UOBJS) $(SUS_OBJS)
	@rm -f *.asm
	@rm -f *.bin
	@rm -f *.elf
	@rm -f *boot*.hex
	@rm -f update*.hex
	@rm -f suspend*.hex
	@rm -rf $(LD_PRE_BOOT_PATH)
	@rm -f size.data

.c.o:
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EX1)  $(CC_FLAGS_EX2) $< -o $@

.s.o:
	$(ASM) $(ASM_FLAGS) $(ASM_FLAGS_EX1) $(ASM_FLAGS_EX2) $< -o $@
