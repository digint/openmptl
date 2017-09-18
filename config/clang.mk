ifdef CLANG

ifndef SIMULATION

# Consider defining CROSS_CLANG if you have an architecture dependent
# llvm/clang build:
#
#     --with-gcc-toolchain
#     --with-default-sysroot
#     --with-c-include-dirs

ifndef CROSS_CLANG

  # NOTE: llvm/clang support is very experimental, and may or may not work for your compiler.

  # NOTE: llvm linker needs compiler-rt, which is not available for our platforms.
  # see: https://github.com/ReservedField/arm-compiler-rt

  #GCC_VERSION=7.2.0
  #TRIPLE=armv7m-none-eabi
  #TOOLCHAIN_DIR=/opt/toolchain/$(TRIPLE)-$(GCC_VERSION)

  GCC_VERSION := $(shell $(CC) -dumpversion)
  TRIPLE := $(shell $(CC) -dumpmachine)
  TOOLCHAIN_DIR := $(shell which $(CC))
  TOOLCHAIN_DIR := $(shell dirname $(TOOLCHAIN_DIR))
  TOOLCHAIN_DIR := $(shell dirname $(TOOLCHAIN_DIR))

  # NOTE: these flags seem to change on each major llvm revision. Leave them all for now.
  CLANG_SYSROOT += -resource-dir $(TOOLCHAIN_DIR)
  CLANG_SYSROOT += -isysroot $(TOOLCHAIN_DIR)/$(TRIPLE)
  CLANG_SYSROOT += -nostdinc++
  CLANG_SYSROOT += -nostdlibinc

  CLANG_SYSROOT += -cxx-isystem $(TOOLCHAIN_DIR)/$(TRIPLE)/include/c++/$(GCC_VERSION)
  CLANG_SYSROOT += -cxx-isystem $(TOOLCHAIN_DIR)/$(TRIPLE)/include/c++/$(GCC_VERSION)/$(TRIPLE)
  CLANG_SYSROOT += -cxx-isystem $(TOOLCHAIN_DIR)/$(TRIPLE)/include/c++/$(GCC_VERSION)/backward

  CLANG_SYSROOT += -isystem $(TOOLCHAIN_DIR)/include
  CLANG_SYSROOT += -isystem $(TOOLCHAIN_DIR)/$(TRIPLE)/include

  # we also want to use standard include from gcc-toolchain:
  CLANG_SYSROOT += -nostdinc
  CLANG_SYSROOT += -isystem $(TOOLCHAIN_DIR)/lib/gcc/$(TRIPLE)/$(GCC_VERSION)/include

  LDFLAGS += -L$(TOOLCHAIN_DIR)/$(TRIPLE)/lib
  LDFLAGS += -L$(TOOLCHAIN_DIR)/lib/gcc/$(TRIPLE)/$(GCC_VERSION)

  # NOTE: as of clang-5.0.0, the default linker is ld.lld, which does not seem to work correctly
  LDFLAGS += -fuse-ld=bfd
  #LDFLAGS += -fuse-ld=gold -static -Wl,-plugin=/usr/lib64/llvm/4/lib64/LLVMgold.so -L${TOOLCHAIN_DIR}/${TRIPLE}/lib

  # we dont have compiler-rt build for armv7em. projects which depend on it will not work.
  LDFLAGS  += -nostdlib
  #LDFLAGS += -lstdc++

  CLANG_TARGET += -target $(TRIPLE)
  CLANG_TARGET += -ccc-gcc-name $(TRIPLE)-gcc

  # NOTE: clang-4 does not need this any more (and issues a warning)
  CLANG_TARGET += -gcc-toolchain $(TOOLCHAIN_DIR)

  CLANG_VERBOSE = -v
endif

else # SIMULATION

  ifdef CLANG_LIBSTDCPP
    CXXFLAGS  += -stdlib=libstdc++
  else
    # use libcxx by default (llvm's c++ standard library implementation, yum!)
    CXXFLAGS  += -stdlib=libc++
    LDFLAGS   += -lc++
    #LDFLAGS   += -lc++abi
  endif

endif # SIMULATION

CC           = $(CROSS_CLANG)clang $(CLANG_VERBOSE) $(CLANG_SYSROOT) $(CLANG_TARGET)
CXX          = $(CROSS_CLANG)clang++ $(CLANG_VERBOSE) $(CLANG_SYSROOT) $(CLANG_TARGET)
AS           = $(CROSS)gcc -x assembler-with-cpp
LD           = $(CROSS_CLANG)clang $(CLANG_VERBOSE) $(CLANG_TARGET)
#LD           = $(CROSS)ld.gold --verbose -static -plugin $(TOOLCHAIN_DIR)/lib/LLVMgold.so -L${TOOLCHAIN_DIR}/${TRIPLE}/lib -lc
OBJCOPY      = $(CROSS)objcopy
OBJDUMP      = $(CROSS)objdump
SIZE         = $(CROSS)size -B -d

FLAGS       += -DCONFIG_CLANG

# emit llvm bytecode by default
CLANG_EMIT_LLVM = 1

endif # CLANG
