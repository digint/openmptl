#
# OpenMPTL - C++ Microprocessor Template Library
#
# Copyright 2013 Axel Burri <axel@tty0.ch>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#------------------------------------------------------------------------------
# project setup
#

PROJECT        = stm32f4discovery-ledtest

OPENMPTL_TOP   = ../..

OPENMPTL_ARCH  = arm/cortex/stm32/f4

ifdef SIMULATION
  CROSS        = /usr/x86_64-pc-linux-gnu/gcc-bin/4.8.1/x86_64-pc-linux-gnu-
  FLAGS       += -DOPENMPTL_SIMULATION
  FLAGS       += -DCONFIG_REGISTER_REACTION
  FLAGS       += -DCONFIG_DUMP_REGISTER_ACCESS
  FLAGS       += -DCONFIG_DUMP_REGISTER_BITFIELD
#  FLAGS       += -DCONFIG_DUMP_CURRENT_REGISTER_VALUE
else
  CROSS        = armv7m-none-eabi-
  CPUFLAGS     = -mcpu=cortex-m4 -mthumb
endif

#FLAGS += -DHIGH_LEVEL

# HINT: g++ -Q -O2 --help=optimizers
OPTIMIZER      = -Os

LD_SCRIPT      = stm32f40x_flash.ld

OPENOCD_CONFIG = -f board/stm32f4discovery.cfg


#------------------------------------------------------------------------------
# input/output setup
#

OBJ_DIR     := obj
ifdef SIMULATION
  OBJ_DIR   := $(OBJ_DIR)/sim
endif

BIN          = $(PROJECT)
ELF          = $(PROJECT).elf
HEX          = $(PROJECT).hex
MAP          = $(PROJECT).map
LSS          = $(PROJECT).lss

SRC_DIRS     = src
SRCS         = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
SRCS        += $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))
SRCS        += $(wildcard $(addsuffix /*.S, $(SRC_DIRS)))

vpath %.cpp $(SRC_DIRS)
vpath %.c   $(SRC_DIRS)
vpath %.S   $(SRC_DIRS)

include $(OPENMPTL_TOP)/config/simulation.mk


#------------------------------------------------------------------------------
# compiler-rt sources (experimental)
#

# libgcc does not provide __aeabi_memset, which is referenced by a
# clang build. As a hotfix, we simply compile aeabi_memset.S from the
# compiler-rt project.

# NOTE: For this to work, you will need to download compiler-rt from
# <http://llvm.org/releases> and unpack it to OPENMPTL_TOP.

ifndef SIMULATION
  COMPILER_RT_TOP     = $(OPENMPTL_TOP)/compiler-rt
  COMPILER_RT_SRC_DIR = $(COMPILER_RT_TOP)/lib/arm

  ifdef CLANG
    SRCS        += $(COMPILER_RT_SRC_DIR)/aeabi_memset.S
    vpath %.S   $(COMPILER_RT_SRC_DIR)
  endif
endif


#------------------------------------------------------------------------------
# build environment
#
include $(OPENMPTL_TOP)/config/gcc.mk
include $(OPENMPTL_TOP)/config/clang.mk
include $(OPENMPTL_TOP)/config/system.mk


#------------------------------------------------------------------------------
# OpenMPTL includes
#
include $(OPENMPTL_TOP)/config/openmptl.mk
INCLUDE     += $(OPENMPTL_INCLUDE)


#------------------------------------------------------------------------------
# put all OBJ's to OBJ_DIR (flat!)
#
# NOTE: all source files need distinct names
#

OBJS        := $(notdir $(SRCS) )
ifdef CLANG_EMIT_LLVM
  OBJS      := $(OBJS:.cpp=.bc)
  OBJS      := $(OBJS:.c=.bc)
else
  OBJS      := $(OBJS:.cpp=.o)
  OBJS      := $(OBJS:.c=.o)
endif
OBJS        := $(OBJS:.S=.o)

OBJS        := $(patsubst %, $(OBJ_DIR)/%, $(OBJS))


#------------------------------------------------------------------------------
# flags
#

# commmon flags propagated to CFLAGS, CXXFLAGS, ASFLAGS (not LDFLAGS)
FLAGS       += $(CPUFLAGS)
FLAGS       += $(INCLUDE)
FLAGS       += -MMD
ifndef CLANG
  FLAGS     += -Wa,-gdwarf-4
  ifdef LST
    FLAGS   += -Wa,-adhlns=$(addsuffix .lst, $(basename $@))
  endif
endif

ifndef SIMULATION
  # Place each function or data item into its own section
  OPTIMIZER += -ffunction-sections -fdata-sections
endif

CFLAGS      += $(FLAGS)
CFLAGS      += $(OPTIMIZER)
CFLAGS      += -std=gnu99
CFLAGS      += -g
CFLAGS      += -Wall -Wextra
CFLAGS      += -Wimplicit
CFLAGS      += -Wcast-align
CFLAGS      += -Wpointer-arith
CFLAGS      += -Wredundant-decls
CFLAGS      += -Wshadow
CFLAGS      += -Wcast-qual
CFLAGS      += -Wcast-align
CFLAGS      += -Wnested-externs
CFLAGS      += -pedantic

CXXFLAGS    += $(FLAGS)
CXXFLAGS    += $(OPTIMIZER)
CXXFLAGS    += -std=c++11
CXXFLAGS    += -g
CXXFLAGS    += -fno-exceptions
CXXFLAGS    += -fno-rtti
ifndef CLANG
  CXXFLAGS  += -funsigned-bitfields
endif
CXXFLAGS    += -fshort-enums
#CXXFLAGS    += -fomit-frame-pointer
#CXXFLAGS    += -fno-elide-constructors
#CXXFLAGS    += -finline-limit=60    # number of pseudo-instructions, default is 600
#CXXFLAGS    += -frepo     # Enable automatic template instantiation at link time
#CXXFLAGS    += -fdump-class-hierarchy
#CXXFLAGS    += -ffreestanding
#CXXFLAGS    += -nostdinc++

CXXFLAGS    += -Wall -Wextra
CXXFLAGS    += -Winline   # see non-inlined functions
CXXFLAGS    += -Wpointer-arith
CXXFLAGS    += -Wredundant-decls
CXXFLAGS    += -Wctor-dtor-privacy
CXXFLAGS    += -Wshadow
CXXFLAGS    += -Wcast-qual
CXXFLAGS    += -Wcast-align
CXXFLAGS    += -pedantic
#CXXFLAGS    += -Weffc++
#CXXFLAGS   += -Wno-long-long  # disable boost warning: ISO C++ 1998 does not support 'long long'
ifdef SIMULATION
  CXXFLAGS  += -Wno-attributes
endif

ASFLAGS     += $(FLAGS)

LDFLAGS     += $(CPUFLAGS)
LDFLAGS     += $(OPTIMIZER)
LDFLAGS     += -fno-exceptions
LDFLAGS     += -fno-rtti
#! LDFLAGS  += -frepo     # Enable automatic template instantiation at link time
ifdef SIMULATION
  LDFLAGS   += -pthread
else
  LDFLAGS   += -nostartfiles
  LDFLAGS   += -Wl,-Map="$(MAP)",--cref
  LDFLAGS   += -Wl,--gc-sections
  LDFLAGS   += -T$(LD_SCRIPT)
endif

.PHONY: all clean

ifdef SIMULATION
all: $(OBJ_DIR) $(BIN)
else
all: $(OBJ_DIR) $(ELF)
endif

$(LSS): $(ELF)
	@echo "--- generating $@"
#	$(OBJDUMP) -dStC $(ELF) > $(LSS)
	$(OBJDUMP) -dC $(ELF) > $(LSS)

$(HEX):	$(ELF)
	@echo "--- generating $@"
	$(OBJCOPY) -O ihex $< $@

$(ELF):	$(OBJS)
	@echo "--- generating $@"
	$(LD) $(LDFLAGS) $(OBJS) -o "$(ELF)"
	$(SIZE) $@

$(BIN):	$(OBJS)
	@echo "--- generating $@"
	$(LD) $(LDFLAGS) $(OBJS) -o "$(BIN)"

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.bc: %.cpp
	$(CXX) -c -emit-llvm $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/%.bc: %.c
	$(CC) -c -emit-llvm $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: %.S
	$(AS) -c $(ASFLAGS) -o $@ $<

$(OBJ_DIR):
	@$(MKDIR_P) $(OBJ_DIR)

clean:
	$(RM) $(BIN)
	$(RM) $(ELF)
	$(RM) $(HEX)
	$(RM) $(MAP)
	$(RM) $(LSS)
	$(RM) $(OBJ_DIR)/*.bc
	$(RM) $(OBJ_DIR)/*.s
	$(RM) $(OBJ_DIR)/*.o
	$(RM) $(OBJ_DIR)/*.d
	$(RM) $(OBJ_DIR)/*.lst


include $(OPENMPTL_TOP)/config/doxygen.mk
include $(OPENMPTL_TOP)/config/openocd.mk

-include $(wildcard $(OBJ_DIR)/*.d)
