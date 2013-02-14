#
# CppCore - C++ microprocessor core library
# 
# Copyright 2012 Axel Burri <axel@tty0.ch>
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

PROJECT      = cppcore

CPPCORE_TOP  = ../..
TINYFSM_TOP  = ../../../tinyfsm

CROSS        = arm-none-eabi-
#CROSS       = /opt/toolchain/x86_64-pc-linux-gnu-gcc-4.7.0/bin/x86_64-pc-linux-gnu-

CPPCORE_ARCH = arm/cortex/stm32/f4

CPUFLAGS     = -mcpu=cortex-m4 -mthumb

FLAGS        = -DSTM32F10X_MD
#FLAGS       += -DCORE_SIMULATION

# HINT: g++ -Q -O2 --help=optimizers
OPTIMIZER    = -Os

LD_SCRIPT    = stm32f40x_flash.ld 

OPENOCD_CONFIG = -f board/stm32f4discovery.cfg


#------------------------------------------------------------------------------
# input/output setup
#

OBJ_DIR      = obj

HEX          = $(PROJECT).hex
ELF          = $(PROJECT).elf
MAP          = $(PROJECT).map
LSS          = $(PROJECT).lss

SRC_DIRS      = src

# needed to find setup.hpp
INCLUDE      = -I $(SRC_DIRS)
INCLUDE     += -I $(TINYFSM_TOP)/include


SRCS         = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
SRCS        += $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))
SRCS        += $(wildcard $(addsuffix /*.S, $(SRC_DIRS)))

vpath %.cpp $(SRC_DIRS)
vpath %.c   $(SRC_DIRS)
vpath %.S   $(SRC_DIRS)



#------------------------------------------------------------------------------
# build environment
#
include $(CPPCORE_TOP)/config/gcc.mk
include $(CPPCORE_TOP)/config/system.mk


#------------------------------------------------------------------------------
# cppcore includes
#
include $(CPPCORE_TOP)/config/cppcore.mk
SRCS        += $(CPPCORE_SRC)
INCLUDE     += $(CPPCORE_INCLUDE)

vpath %.cpp $(CPPCORE_SRC_DIRS)


#------------------------------------------------------------------------------
# boost includes
#
include $(CPPCORE_TOP)/config/boost.mk
INCLUDE     += $(BOOST_INCLUDE)
FLAGS       += $(BOOST_FLAGS)


#------------------------------------------------------------------------------
# put all OBJ's to OBJ_DIR (flat!)
#
# NOTE: all source files need distinct names
#

#OBJS = $(SRCS:$(TOP)%=$(OBJ_DIR)%)
#OBJS := $(addprefix $(OBJ_DIR)/, $SRCS)

OBJS        := $(notdir $(SRCS) )
OBJS        := $(OBJS:.cpp=.o)
OBJS        := $(OBJS:.c=.o)
OBJS        := $(OBJS:.S=.o)
#OBJS        := $(SRCS:$(TOP)%=$(OBJ_DIR)%)
OBJS        := $(patsubst %, $(OBJ_DIR)/%, $(OBJS))



#------------------------------------------------------------------------------
# flags
#

# commmon flags propagated to CFLAGS, CXXFLAGS, ASFLAGS (not LDFLAGS)
FLAGS       += $(CPUFLAGS)
FLAGS       += $(INCLUDE)
FLAGS       += -Wa,-gdwarf-2
FLAGS       += -MMD
FLAGS       += -Wa,-adhlns=$(addsuffix .lst, $(basename $@))

# TODO: find out why this produces smaller code (+document)
OPTIMIZER   += -ffunction-sections -fdata-sections  # Place each function or data item into its own section

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

CXXFLAGS     = $(FLAGS)
CXXFLAGS    += $(OPTIMIZER)
CXXFLAGS    += -std=c++11
CXXFLAGS    += -g
#CXXFLAGS    += -fomit-frame-pointer
CXXFLAGS    += -fno-exceptions
CXXFLAGS    += -fno-rtti
#CXXFLAGS    += -fno-elide-constructors
CXXFLAGS    += -funsigned-bitfields
CXXFLAGS    += -fshort-enums
# CXXFLAGS  += -finline-limit=60    # number of pseudo-instructions, default is 600
#! CXXFLAGS += -frepo     # Enable automatic template instantiation at link time
# CXXFLAGS  += -fdump-class-hierarchy
#CXXFLAGS    += -ffreestanding -nostdinc++

CXXFLAGS    += -Wall -Wextra
CXXFLAGS    += -Winline   # see non-inlined functions
CXXFLAGS    += -Wpointer-arith
CXXFLAGS    += -Wredundant-decls
#!CXXFLAGS    += -Wctor-dtor-privacy  # boost/intrusive/list.hpp does not satisfy these
#!CXXFLAGS    += -Wshadow             # boost/intrusive/list.hpp does not satisfy these
CXXFLAGS    += -Wcast-qual
CXXFLAGS    += -Wcast-align
CXXFLAGS    += -pedantic
#CXXFLAGS    += -Weffc++
#CXXFLAGS   += -Wno-long-long  # disable boost warning: ISO C++ 1998 does not support 'long long'

ASFLAGS      = $(FLAGS)

LD_FLAGS     = $(CPUFLAGS)
LD_FLAGS    += -nostartfiles
LD_FLAGS    += -Wl,-Map="$(MAP)",--cref
LD_FLAGS    += -fno-exceptions
LD_FLAGS    += -fno-rtti
#! LD_FLAGS += -frepo     # Enable automatic template instantiation at link time
LD_FLAGS    += -Wl,--gc-sections
LD_FLAGS    += -T$(LD_SCRIPT)


.PHONY: all clean

all: $(OBJ_DIR) $(ELF)

$(LSS): $(ELF)
	@echo "--- generating $@"
#	$(OBJDUMP) -dStC $(ELF) > $(LSS)
	$(OBJDUMP) -dC $(ELF) > $(LSS)

$(HEX):	$(ELF)
	@echo "--- generating $@"
	$(CP) -O ihex $< $@

$(ELF):	$(OBJS)
	@echo "--- generating $@"
	$(LD) $(OBJS) $(LD_FLAGS) -o "$(ELF)"
	$(SIZE) $@

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.S
	$(AS) -c $(ASFLAGS) -o $@ $<

$(OBJ_DIR):
	@$(MKDIR_P) $(OBJ_DIR) 

clean:
#	$(RM_R) $(OBJ_DIR)
#	$(RM_R) $(BIN_DIR)
	$(RM) $(OBJ_DIR)/*.o
	$(RM) $(OBJ_DIR)/*.d
	$(RM) $(OBJ_DIR)/*.lst

	$(RM) $(HEX)
	$(RM) $(ELF)
	$(RM) $(MAP)
	$(RM) $(LSS)

include $(CPPCORE_TOP)/config/doxygen.mk
include $(CPPCORE_TOP)/config/openocd.mk

-include $(wildcard $(OBJ_DIR)/*.d)
