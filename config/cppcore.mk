#
# Input Variables:
#
# - CPPCORE_TOP  : CppCore top directory
# - CPPCORE_ARCH : Architecture (e.g. "stm32f10x")
#
#
# Output Variables:
#
# - CPPCORE_INCLUDE : compiler include flags
#

CPPCORE_DOC_DIR     = $(CPPCORE_TOP)/doc
CPPCORE_ARCH_DIR    = $(CPPCORE_TOP)/arch
CPPCORE_DRIVERS_DIR = $(CPPCORE_TOP)/drivers
CPPCORE_INCLUDE_DIR = $(CPPCORE_TOP)/include

#CPPCORE_SRC_DIRS    = $(CPPCORE_ARCH_DIR)/$(CPPCORE_ARCH)
#CPPCORE_SRC         = $(wildcard $(addsuffix /*.cpp, $(CPPCORE_SRC_DIRS)))

CPPCORE_INCLUDE     = -I $(CPPCORE_INCLUDE_DIR)
CPPCORE_INCLUDE    += -I $(CPPCORE_DRIVERS_DIR)
CPPCORE_INCLUDE    += -I $(CPPCORE_ARCH_DIR)/$(CPPCORE_ARCH)/include
