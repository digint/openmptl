CC           = $(CROSS)gcc
CXX          = $(CROSS)clang++
AS           = $(CROSS)gcc -x assembler-with-cpp
LD           = $(CROSS)clang++
OBJCOPY      = $(CROSS)objcopy
OBJDUMP      = $(CROSS)objdump
SIZE         = $(CROSS)size -B -d
