#################################################
# MAKEFILE For FUJITSU MB9BF618T Device 		#
# (c) 20120401 Nemui Trinomius					#
# http://nemuisan.blog.bai.ne.jp				#
#################################################

# Environment Dependent!!!
# Throw path into YOUR environments
# export PATH = %SYSTEMROOT%;$(TOOLDIR)/arm-gcc/bin;$(TOOLDIR)/bin;$(MAKEDIR);$(OCDIR)

# Toolchain prefix (i.e arm-elf -> arm-elf-gcc.exe)
TCHAIN  = arm-none-eabi
OCD		= openocd
#OCDMODE = SWD
OCDMODE = JTAG

# Development Tools based on CodeSourceryG++
#DEVTOOL = RAISONANCE
DEVTOOL = SOURCERY

ifeq ($(DEVTOOL),RAISONANCE)
TOOLDIR = C:/Devz/ARM/Raisonance/Ride
REMOVAL = rm
else
TOOLDIR = /cygdrive/c/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI/bin
REMOVAL = cs-rm
endif

# MAKEDIR = C:/Devz/AVR/WinAVR/utils/bin

OCDIR	= /cygdrive/c/ocd
ifeq ($(OCDMODE),SWD)
OCD_CMD = -s $(OCDIR)/tcl					\
		  -f interface/vsllink_swd.cfg		\
          -f target/mb9bf618t_flash.cfg
else
OCD_CMD = -s $(OCDIR)/tcl					\
		  -f interface/jtagkey2.cfg 		\
          -f target/mb9bf618t_flash.cfg
endif

WSHELL  = cmd
MSGECHO = echo.exe

GDBDIR  = /cygdrive/c/arm-none-eabi-insight/bin
INSIGHT = $(GDBDIR)/arm-none-eabi-insight
# Environment Dependent!!!


# OPTIMIZE definition
OPTIMIZE		= fast
#OPTIMIZE		= s

# GCC4.6.x Specific Option
ifneq ($(OPTIMIZE),0)
USE_LTO			= -flto-partition=none -fipa-sra
#USE_LTO			= -flto -fipa-sra
endif
#ALIGNED_ACCESS	= -mno-unaligned-access

# Apprication Version
APP_VER = WorkInProgress

# Board definition
SUBMODEL		= MB9BF618T
EVAL_BOARD    	= USE_FRK_FM3
HSE_CLOCK 		= 4000000
#USE_TOUCH_SENCE = USE_ADS7843


# Use Display Device?
#USE_DISPLAY		= USE_SSD1332_SPI_OLED
#USE_DISPLAY		= USE_ST7735_SPI_TFT

# Use Display Fonts? (misaki(8x8) or M+(10x10) fonts)
#USE_FONTSIZE    = FONT8x8
#USE_FONTSIZE    = FONT10x10

# Use FreeRTOS?
OS_SUPPORT		= BARE_METAL
#OS_SUPPORT		= USE_FREERTOS


# Synthesis makefile Defines
DEFZ = $(SUBMODEL)   $(EVAL_BOARD)   $(MPU_DENSITY)  $(PERIF_DRIVER)    $(VECTOR_START)    \
	   $(OS_SUPPORT) $(USE_EXT_SRAM)
# Defines if Display and Font Drivers
DEFZ += $(USE_DISPLAY) $(USE_FONTSIZE) $(USE_KANJI) $(USE_TOUCH_SENCE)  $(USE_XMSTN)		\
        $(USE_JPEG_LIB) $(USE_PNG_LIB)

#define USE UART
DEFZ += USE_CH03

SYNTHESIS_DEFS	= $(addprefix -D,$(DEFZ)) 							\
				 -DPACK_STRUCT_END=__attribute\(\(packed\)\) 		\
				 -DALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
				 -DMPU_SUBMODEL=\"$(SUBMODEL)\"						\
				 -DAPP_VERSION=\"$(APP_VER)\"						\
				 -DHSE_VALUE=$(HSE_CLOCK)UL 

# TARGET definition
TARGET 		= main
TARGET_ELF  = $(TARGET).elf
TARGET_SREC = $(TARGET).s19
TARGET_HEX  = $(TARGET).hex
TARGET_BIN  = $(TARGET).bin
TARGET_LSS  = $(TARGET).lss
TARGET_SYM  = $(TARGET).sym

# define Cortex-M3 LIBRARY PATH
FWLIB  		= ./lib/Drivers
USBLIB 		=
CM3LIB 		= ./lib/CMSIS
CM3_DEVICE 	= $(CM3LIB)/Device/FUJITSU/MB9BF61xT
CM3_CORE	= $(CM3LIB)/Include

# include PATH
INCPATHS	 = 	./						\
				./inc					\
				$(FWLIB)/inc  			\
				$(USBLIB)/Core/inc		\
				$(CM3_DEVICE)/Include	\
				$(CM3_CORE)				\
				$(PPD_MFSERIAL)			\
				$(PPD_MFS_UART)			\
				$(LIBINCDIRS)
INCLUDES     = $(addprefix -I ,$(INCPATHS))


# Set library PATH
LIBPATHS     = $(FWLIB) $(USBLIB) $(CM3LIB) $(DISPLAY_LIB)
LIBRARY_DIRS = $(addprefix -L,$(LIBPATHS))
# if you use math-library, put "-lm" 
MATH_LIB	 =	-lm

# LinkerScript PATH
LINKER_PATH =  ./lib/linker
LINKER_DIRS = $(addprefix -L,$(LINKER_PATH)) 

# Object definition
OBJS 	 = $(CFILES:%.c=%.o) 
LIBOBJS  = $(LIBCFILES:%.c=%.o) $(SFILES:%.s=%.o)

# C code PATH
SOURCE  = ./src
CFILES = \
 $(SOURCE)/$(TARGET).c 			\
 $(SOURCE)/systick.c	\
 $(SOURCE)/xprintf.c	\
 $(SOURCE)/uart.c	\
 $(SOURCE)/syscalls.c



#/*----- Display library PATH -----*/	
DISPLAY_LIB	= ./lib/display
ifneq ($(USE_DISPLAY),)
include $(DISPLAY_LIB)/display_cfg.mk
endif

#/*----- FONX2 Driver library PATH -----*/	
FONTX2_LIB	= ./lib/FONTX2
ifneq ($(USE_FONTSIZE),)
include $(FONTX2_LIB)/fontx2_drv.mk
endif

#/*----- xMSTN Display library PATH -----*/	
xMSTN_LIB	= ./lib/xMSTN
ifneq ($(USE_XMSTN),)
include $(xMSTN_LIB)/xmstn_drv.mk
endif

#/*----- FatFs library PATH -----*/	
FATFS = ./lib/ff
LIBINCDIRS += $(FATFS)
CFILES += \

#/*----- MB9BF61xT (P)eri(P)heral(D)rivers library PATH -----*/	
PPD_LIB	= ./lib/mb9b610t/drivers
PPD_MFSERIAL = $(PPD_LIB)/mfserial
PPD_MFS_UART = $(PPD_MFSERIAL)/uart
PPD_MFS_I2C  = $(PPD_MFSERIAL)/i2c
PPD_MFS_SIO  = $(PPD_MFSERIAL)/sio
CFILES += \
 $(PPD_MFSERIAL)/MfsDev_FM3.c	\
 $(PPD_MFS_UART)/UartDev_FM3.c	\
 $(PPD_MFS_I2C)/I2cDev_FM3.c	\
 $(PPD_MFS_SIO)/SioDev_FM3.c



#/*----- STARTUP code PATH -----*/
STARTUP_DIR = $(CM3_DEVICE)/Source/Templates/startup/gcc

ifeq ($(OS_SUPPORT),USE_FREERTOS)
SFILES += \
	./src/startup_mb9bf61xt_rtos.s
else
SFILES += \
	$(STARTUP_DIR)/startup_mb9bf61xt.s
endif



#/*----- MB9BF61x library PATH -----*/
LIBCFILES = \
 $(CM3_DEVICE)/Source/Templates/system_mb9bf61x.c

# TOOLCHAIN SETTING
CC 			= $(TCHAIN)-gcc
AS	 		= $(TCHAIN)-as
CPP 		= $(TCHAIN)-g++
OBJCOPY 	= $(TCHAIN)-objcopy
OBJDUMP 	= $(TCHAIN)-objdump
SIZE 		= $(TCHAIN)-size
AR 			= $(TCHAIN)-ar
LD 			= $(TCHAIN)-gcc
NM 			= $(TCHAIN)-nm
REMOVE		= $(REMOVAL) -f
REMOVEDIR 	= $(REMOVAL) -rf

# C and ASM FLAGS
CFLAGS  = -MD -mcpu=cortex-m3 -mtune=cortex-m3 -mfix-cortex-m3-ldrd
CFLAGS += -mthumb -mlittle-endian $(ALIGNED_ACCESS)
CFLAGS += -mapcs-frame -msoft-float -mno-sched-prolog
CFLAGS += -std=gnu99
CFLAGS += -gdwarf-2 -O$(OPTIMIZE) $(USE_LTO)
CFLAGS += -fno-strict-aliasing -fsigned-char
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -fno-schedule-insns2
CFLAGS += --param max-inline-insns-single=1000
CFLAGS += -fno-common -fno-hosted
CFLAGS += -Wall -Wno-array-bounds
#CFLAGS += -Wdouble-promotion
#CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<) 
CFLAGS += $(SYNTHESIS_DEFS)  


# ASM FLAGS
AFLAGS  =

# Linker FLAGS
LDFLAGS  = -mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd
LDFLAGS += -u g_pfnVectors -Wl,-static -Wl,--gc-sections
LDFLAGS += -nostartfiles
LDFLAGS += -Wl,-Map=$(TARGET).map
LDFLAGS += $(LIBRARY_DIRS) $(LINKER_DIRS) $(MATH_LIB) -lc -lg -lstdc++ -lsupc++ -lgcc
LDFLAGS +=-T$(LINKER_PATH)/MB9BF618T-ROM.ld


# Object Copy and dfu generation FLAGS
OBJCPFLAGS = -O
OBJDUMPFLAGS = -h -S -C
 
 
# Object Size Infomations
ELFSIZE = $(SIZE) -A -x $(TARGET).elf
sizeafter:
	@$(MSGECHO) 
	@$(MSGECHO) Size After:
	$(SIZE) $(TARGET).elf
	@$(SIZE) -A -x $(TARGET).elf
	
# Display compiler version information.
gccversion : 
	@$(CC) --version
	@$(MSGECHO) "BUILD_TYPE = "$(OS_SUPPORT)
	@$(MSGECHO) "USING_DISPLAY = "$(USE_DISPLAY)
	@$(MSGECHO) 

# Build Object
all: gccversion clean build sizeafter
build: $(TARGET_ELF) $(TARGET_LSS) $(TARGET_SYM) $(TARGET_HEX) $(TARGET_SREC) $(TARGET_BIN)

.SUFFIXES: .o .c .s   

$(TARGET_LSS): $(TARGET_ELF)
	@$(MSGECHO)
	@$(MSGECHO) Disassemble: $@
	$(OBJDUMP) $(OBJDUMPFLAGS) $< > $@ 
$(TARGET_SYM): $(TARGET_ELF)
	@$(MSGECHO)
	@$(MSGECHO) Symbol: $@
	$(NM) -n $< > $@
$(TARGET).hex: $(TARGET).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) ihex $^ $@    
$(TARGET).s19: $(TARGET).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) srec $^ $@ 
$(TARGET).bin: $(TARGET).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) binary $< $@ 
$(TARGET).elf: $(OBJS) mb9bfxxxlib.a
	@$(MSGECHO) Link: $@
	$(LD) $(CFLAGS) $(LDFLAGS) $^ -o $@
	@$(MSGECHO)

mb9bfxxxlib.a: $(LIBOBJS)
	@$(MSGECHO) Archive: $@
	$(AR) cr $@ $(LIBOBJS)    
	@$(MSGECHO)
.c.o:
	@$(MSGECHO) Compile: $<
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
	@$(MSGECHO)
.s.o:
	@$(MSGECHO) Assemble: $<
	$(AS)  $(AFLAGS) -gdwarf-2 $(INCLUDES) $< -o $@    
	@$(MSGECHO)


# Flash and Debug Program
debug :
	$(WSHELL) /c start /B $(INSIGHT) $(TARGET).elf
	$(OCD) $(OCD_CMD) -c "soft_reset_halt"
program :
	$(OCD) $(OCD_CMD) -c "mt_flash $(TARGET).elf"
#	$(OCD) $(OCD_CMD) -c "eraser"


# Drop files into dust-shoot
.PHONY clean:
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).obj
	$(REMOVE) mb9bfxxxlib.a
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).s19
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(TARGET).dfu
	$(REMOVE) $(wildcard *.stackdump)
	$(REMOVE) $(OBJS)
	$(REMOVE) $(AOBJ)
	$(REMOVE) $(LIBOBJS)
	$(REMOVE) $(LST)
	$(REMOVE) $(CFILES:.c=.lst)
	$(REMOVE) $(CFILES:.c=.d)
	$(REMOVE) $(LIBCFILES:.c=.lst)
	$(REMOVE) $(LIBCFILES:.c=.d)
	$(REMOVE) $(SFILES:.s=.lst)
	$(REMOVE) $(wildcard ./lib/IOView/*.d)
	$(REMOVE) $(wildcard ./lib/IOView/*.lst)
	$(REMOVE) $(wildcard ./lib/IOView/*.o)
	$(REMOVE) $(wildcard $(CM3_DEVICE)/*.d)
	$(REMOVE) $(wildcard $(CM3_DEVICE)/*.lst)
	$(REMOVE) $(wildcard $(CM3_CORE)/*.d)
	$(REMOVE) $(wildcard $(CM3_CORE)/*.lst)
	$(REMOVEDIR) .dep
	@$(MSGECHO)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex bin lss sym clean clean_list program
