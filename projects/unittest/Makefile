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

#CROSS        = x86_64-pc-linux-gnu-
CROSS       = /opt/toolchain/x86_64-pc-linux-gnu-gcc-4.7.0/bin/x86_64-pc-linux-gnu-

CPPCORE_ARCH  = arm/cortex/stm32/f1

FLAGS        = -DCORE_SIMULATION


#------------------------------------------------------------------------------
# input/output setup
#

SRC_DIR      = src

OBJ_DIR      = obj
SRC_FAIL_DIR = mustfail_src

SRCS         = $(wildcard $(addsuffix /*.cpp, $(SRC_DIR)))

FAIL_TEST_SRCS = $(shell perl -- create_fail_src.pl -o $(SRC_FAIL_DIR) $(SRCS))


#------------------------------------------------------------------------------
# build environment
#
include $(CPPCORE_TOP)/config/gcc.mk
include $(CPPCORE_TOP)/config/system.mk


#------------------------------------------------------------------------------
# cppcore includes
#
include $(CPPCORE_TOP)/config/cppcore.mk
INCLUDE     += $(CPPCORE_INCLUDE)


#------------------------------------------------------------------------------
# unittest executables go to OBJ_DIR (flat!)

OBJS        := $(patsubst %, $(OBJ_DIR)/%, $(UNITTESTS))
OBJS_FAIL   := $(FAIL_TEST_SRCS:.cpp=.o)

#------------------------------------------------------------------------------
# flags
#

# commmon flags propagated to CFLAGS, CXXFLAGS, ASFLAGS (not LDFLAGS)
FLAGS       += $(CPUFLAGS)
FLAGS       += $(INCLUDE)

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


.PHONY: all clean test test_fail

all: test test_fail
	echo $(FAIL_TESTS)

test: $(OBJ_DIR) $(OBJS)

test_fail: $(SRC_FAIL_DIR) $(OBJS_FAIL)

$(SRC_FAIL_DIR)/%.o: $(SRC_FAIL_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< || true
	@if [ -e $@ ] ; then echo "--- compilation did NOT fail, but should have!" ; false ; else echo "--- compilation failed, good!" ; fi

$(OBJ_DIR)/%: $(SRC_DIR)/%.cpp
	@echo "--- compiling $@"
	$(CXX) $(CXXFLAGS) -o $@ $<
	@echo "--- running $@"
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
