ifdef CLANG

CC           = clang
CXX          = clang++
AS           = $(CROSS)gcc -x assembler-with-cpp
LD           = clang++
OBJCOPY      = $(CROSS)objcopy
OBJDUMP      = $(CROSS)objdump
SIZE         = $(CROSS)size -B -d

else

CC           = $(CROSS)gcc
CXX          = $(CROSS)g++
AS           = $(CROSS)gcc -x assembler-with-cpp
LD           = $(CROSS)g++
OBJCOPY      = $(CROSS)objcopy
OBJDUMP      = $(CROSS)objdump
SIZE         = $(CROSS)size -B -d

endif
