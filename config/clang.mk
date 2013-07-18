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

#   GCC_VERSION=4.8.1
#   TRIPLE=armv7m-none-eabi
#
#   CLANG_SYSROOT    = -isysroot /opt/toolchain/$(TRIPLE)-$(GCC_VERSION)/$(TRIPLE)/
#   CLANG_SYSROOT   += -iwithsysroot /include/c++/$(GCC_VERSION)
#   CLANG_SYSROOT   += -iwithsysroot /include/c++/$(GCC_VERSION)/$(TRIPLE)
#   CLANG_SYSROOT   += -iwithsysroot /include/c++/$(GCC_VERSION)/backward
#   CLANG_SYSROOT   += -iwithsysroot /include

  # use prefixed clang
  CROSS_CLANG = $(CROSS)

endif # SIMULATION

CC           = $(CROSS_CLANG)clang $(CLANG_SYSROOT)
CXX          = $(CROSS_CLANG)clang++ $(CLANG_SYSROOT)
AS           = $(CROSS)gcc -x assembler-with-cpp
LD           = $(CROSS_CLANG)clang++ $(CLANG_SYSROOT)
OBJCOPY      = $(CROSS)objcopy
OBJDUMP      = $(CROSS)objdump
SIZE         = $(CROSS)size -B -d

FLAGS       += -DCONFIG_CLANG


ifdef SIMULATION
  # use libcxx (llvm's c++ standard library implementation, yum!)
  CXXFLAGS  += -stdlib=libc++
  LDFLAGS   += -lc++
endif

endif # CLANG
