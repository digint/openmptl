ifdef CLANG

ifndef SIMULATION

# if you did not build llvm/clang with:
#
#     --with-gcc-toolchain
#     --with-default-sysroot
#     --with-c-include-dirs
#
# you might want to uncomment the following:
#

GCC_VERSION=4.8.1
TRIPLE=armv7m-none-eabi

TOOLCHAIN_DIR=/opt/toolchain/$(TRIPLE)-$(GCC_VERSION)

ifdef CLANG_CROSS_BUILD    # use prefixed clang
  CROSS_CLANG = $(CROSS)
else
  CLANG_SYSROOT += -resource-dir $(TOOLCHAIN_DIR)
  CLANG_SYSROOT += -isysroot $(TOOLCHAIN_DIR)/$(TRIPLE)
  CLANG_SYSROOT += -isystem $(TOOLCHAIN_DIR)/lib/gcc/$(TRIPLE)/$(GCC_VERSION)/include
  CLANG_SYSROOT += -isystem $(TOOLCHAIN_DIR)/$(TRIPLE)/include
  CLANG_SYSROOT += -isystem $(TOOLCHAIN_DIR)/$(TRIPLE)/include/c++/$(GCC_VERSION)
  CLANG_SYSROOT += -isystem $(TOOLCHAIN_DIR)/$(TRIPLE)/include/c++/$(GCC_VERSION)/$(TRIPLE)
  CLANG_SYSROOT += -isystem $(TOOLCHAIN_DIR)/$(TRIPLE)/include/c++/$(GCC_VERSION)/backward

  CLANG_TARGET += -target $(TRIPLE)
  CLANG_TARGET += -ccc-gcc-name $(TRIPLE)-g++
  CLANG_TARGET += -gcc-toolchain $(TOOLCHAIN_DIR)

  CROSS_CLANG =
endif

endif # SIMULATION

CC           = $(CROSS_CLANG)clang $(CLANG_SYSROOT) $(CLANG_TARGET)
CXX          = $(CROSS_CLANG)clang++ $(CLANG_SYSROOT) $(CLANG_TARGET)
AS           = $(CROSS)gcc -x assembler-with-cpp
LD           = $(CROSS_CLANG)clang++ $(CLANG_TARGET)
#LD           = $(CROSS_CLANG)g++ -fuse-ld=gold
#LD           = $(CROSS)ld.gold --verbose -static -plugin $(TOOLCHAIN_DIR)/lib/LLVMgold.so -L${TOOLCHAIN_DIR}/${TRIPLE}/lib -lc
OBJCOPY      = $(CROSS)objcopy
OBJDUMP      = $(CROSS)objdump
SIZE         = $(CROSS)size -B -d

FLAGS       += -DCONFIG_CLANG

# emit llvm bytecode by default
CLANG_EMIT_LLVM = 1

ifdef SIMULATION
  # use libcxx (llvm's c++ standard library implementation, yum!)
  CXXFLAGS  += -stdlib=libc++
  LDFLAGS   += -lc++
endif

endif # CLANG
