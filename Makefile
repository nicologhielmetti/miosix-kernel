##
## Makefile for Miosix embedded OS
##
MAKEFILE_VERSION := 1.07
## Path to kernel directory (edited by init_project_out_of_git_repo.pl)
KPATH := miosix
## Path to config directory (edited by init_project_out_of_git_repo.pl)
CONFPATH := $(KPATH)
include $(CONFPATH)/config/Makefile.inc




##
## List here subdirectories which contains makefiles
##
SUBDIRS := $(KPATH)

##
## List here your source files (both .s, .c and .cpp)
##
SRC :=                                  \
main.cpp				\
NN/Src/network.c			\
NN/Src/network_data.c			\
NN/Src/aeabi_memclr.c			\
NN/Src/aeabi_memcpy.c			\
ActiveObject/ActiveObject.cpp	\
NN/NeuralNetwork.cpp			\
IKS01A2_classes/lps22hb.cpp      
#NN/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
#NN/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
#NN/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c \
#NN/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c 


##
## List here additional static libraries with relative path
##
 LIBS := NN/Middlewares/ST/AI/Lib/NetworkRuntime410_CM4_GCC.a	    \
	 NN/Middlewares/ST/AI/Lib/libarm_cortexM4lf_math.a

##
## List here additional include directories (in the form -Iinclude_dir)
##
INCLUDE_DIRS := -I./NN/Inc -I./NN/Middlewares/ST/AI/Inc -I./NN/Middlewares/ST/AI/Lib -I./NN -I./IKS01A2_classes
	##-I./miosix/arch/common/CMSIS/Device/ST/STM32F4xx/Include -I./NN/Drivers/STM32F4xx_HAL_Driver/Inc
	##-I./miosix/arch/common/CMSIS/Device/ST/STM32F4xx/Include
	##-I./NN/Drivers/CMSIS/Device/ST/STM32F4xx/Include  
	##-I./NN/Drivers/CMSIS/Include

##############################################################################
## You should not need to modify anything below                             ##
##############################################################################

ifeq ("$(VERBOSE)","1")
Q := 
ECHO := @true
else
Q := @
ECHO := @echo
endif

## Replaces both "foo.cpp"-->"foo.o" and "foo.c"-->"foo.o"
OBJ := $(addsuffix .o, $(basename $(SRC)))

## Includes the miosix base directory for C/C++
## Always include CONFPATH first, as it overrides the config file location

CXXFLAGS := $(CXXFLAGS_BASE) -I$(CONFPATH) -I$(CONFPATH)/config/$(BOARD_INC)  \
            -I. -I$(KPATH) -I$(KPATH)/arch/common -I$(KPATH)/$(ARCH_INC)      \
            -I$(KPATH)/$(BOARD_INC) $(INCLUDE_DIRS)
CFLAGS   := $(CFLAGS_BASE)   -I$(CONFPATH) -I$(CONFPATH)/config/$(BOARD_INC)  \
            -I. -I$(KPATH) -I$(KPATH)/arch/common -I$(KPATH)/$(ARCH_INC)      \
            -I$(KPATH)/$(BOARD_INC) $(INCLUDE_DIRS)
AFLAGS   := $(AFLAGS_BASE)
LFLAGS   := $(LFLAGS_BASE)
DFLAGS   := -MMD -MP

#LINK_LIBS := $(LIBS) -L$(KPATH) -Wl,--start-group -lmiosix -lstdc++ -lc \
             -lm -lgcc -Wl,--end-group
#LINK_LIBS := $(LIBS) -Wl,--start-group -lmiosix $(ARCH_LIBS) -Wl,--end-group
LINK_LIBS := $(LIBS) -L$(KPATH) -Wl,--start-group -lmiosix $(ARCH_LIBS) -Wl,--end-group

all: all-recursive main

clean: clean-recursive clean-topdir

program:
	$(PROGRAM_CMDLINE)

all-recursive:
	$(foreach i,$(SUBDIRS),$(MAKE) -C $(i)                               \
	  KPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(KPATH))       \
	  CONFPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(CONFPATH)) \
	  || exit 1;)

clean-recursive:
	$(foreach i,$(SUBDIRS),$(MAKE) -C $(i)                               \
	  KPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(KPATH))       \
	  CONFPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(CONFPATH)) \
	  clean || exit 1;)

clean-topdir:
	-rm -f $(OBJ) main.elf main.hex main.bin main.map $(OBJ:.o=.d)

main: main.elf
	$(ECHO) "[CP  ] main.hex"
	$(Q)$(CP) -O ihex   main.elf main.hex
	$(ECHO) "[CP  ] main.bin"
	$(Q)$(CP) -O binary main.elf main.bin
	$(Q)$(SZ) main.elf

main.elf: $(OBJ) all-recursive
	$(ECHO) "[LD  ] main.elf"
	$(Q)$(ARM_NONE_EABI)/bin/arm-none-eabi-gcc $(LFLAGS) -o main.elf $(OBJ) $(KPATH)/$(BOOT_FILE) $(LINK_LIBS)
	##$(Q)$(CXX) $(LFLAGS) -o main.elf $(OBJ) $(KPATH)/$(BOOT_FILE) 

%.o: %.s
	$(ECHO) "[AS  ] $<"
	$(Q)$(AS)  $(AFLAGS) $< -o $@

%.o : %.c
	$(ECHO) "[CC  ] $<"
	$(Q)$(CC)  $(DFLAGS) $(CFLAGS) $< -o $@

%.o : %.cpp
	$(ECHO) "[CXX ] $<"
	$(Q)$(CXX) $(DFLAGS) $(CXXFLAGS) $< -o $@

#pull in dependecy info for existing .o files
-include $(OBJ:.o=.d)
