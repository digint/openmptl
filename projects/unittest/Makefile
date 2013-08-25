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
OPENMPTL_TOP = ../..

#CROSS        = x86_64-pc-linux-gnu-
CROSS        = /usr/x86_64-pc-linux-gnu/gcc-bin/4.8.1/x86_64-pc-linux-gnu-

# needed by some unittests
OPENMPTL_ARCH = arm/cortex/stm32/f4

FLAGS       += -DOPENMPTL_SIMULATION
FLAGS       += -DCONFIG_DUMP_REGISTER_ACCESS
FLAGS       += -DCONFIG_DUMP_REGISTER_BITFIELD
FLAGS       += -DUNITTEST_MUST_FAIL_ENABLED

SIMULATION = 1

#------------------------------------------------------------------------------
# input/output setup
#

SRC_DIR     = src

INCLUDE      += -I $(SRC_DIR)

OBJ_DIR      = obj
SRC_FAIL_DIR = var

SRCS         = $(wildcard $(addsuffix /*.cpp, $(SRC_DIR)))

FAIL_TEST_SRCS = $(shell perl -- create_fail_src.pl -o $(SRC_FAIL_DIR) $(SRCS))

vpath %.cpp $(SRC_DIR)


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
# unittest executables go to OBJ_DIR (flat!)

OBJS        := $(notdir $(SRCS) )
OBJS        := $(OBJS:.cpp= )
OBJS        := $(patsubst %, $(OBJ_DIR)/%, $(OBJS))

OBJS_FAIL   := $(FAIL_TEST_SRCS:.cpp=.o)

#------------------------------------------------------------------------------
# flags
#

# commmon flags propagated to CFLAGS, CXXFLAGS, ASFLAGS (not LDFLAGS)
FLAGS       += $(CPUFLAGS)
FLAGS       += $(INCLUDE)
FLAGS       += -MMD

CXXFLAGS    += $(FLAGS)
CXXFLAGS    += $(OPTIMIZER)
CXXFLAGS    += -std=c++11
CXXFLAGS    += -g
CXXFLAGS    += -fno-exceptions
CXXFLAGS    += -fno-rtti
CXXFLAGS    += -funsigned-bitfields
CXXFLAGS    += -fshort-enums

CXXFLAGS    += -Wall -Wextra
CXXFLAGS    += -Winline
CXXFLAGS    += -Wpointer-arith
CXXFLAGS    += -Wredundant-decls
CXXFLAGS    += -Wctor-dtor-privacy
CXXFLAGS    += -Wshadow
CXXFLAGS    += -Wcast-qual
CXXFLAGS    += -Wcast-align
#CXXFLAGS    += -pedantic


.PHONY: all clean clean_fail test test_fail

all: test test_fail

test: $(OBJ_DIR) $(OBJS)

test_fail: $(SRC_FAIL_DIR) $(OBJS_FAIL)

$(SRC_FAIL_DIR)/%.o: $(SRC_FAIL_DIR)/%.cpp
	@echo -e "---\n--- compiling $@\n---"
	$(CXX) $(CXXFLAGS) -o $@ $< || true
	@if [ -e $@ ] ; then echo "--- compilation did NOT fail, but should have!" ; false ; else echo -e "---\n--- compilation failed, good!\n---" ; fi

$(OBJ_DIR)/%: %.cpp
	@echo -e "---\n--- compiling $@\n---"
	$(CXX) $(CXXFLAGS) -o $@ $<
	@echo -e "---\n--- running $@\n---"
	$@

$(OBJ_DIR):
	@$(MKDIR_P) $(OBJ_DIR)

$(SRC_FAIL_DIR):
	@$(MKDIR_P) $(SRC_FAIL_DIR)

clean: clean_fail
	$(RM) $(OBJ_DIR)/*

clean_fail:
	$(RM) $(SRC_FAIL_DIR)/*


-include $(wildcard $(OBJ_DIR)/*.d)
