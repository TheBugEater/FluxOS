CC_ASM = nasm
CC_CPP = g++ 
CC_C = gcc

INC =-I $(SOURCEDIR)/include

ASM_Flags = -f elf32 $(ASM_EXTRA)
C_Flags = -fno-stack-protector -w -std=c99 -fno-builtin -m32 $(INC) $(EXTRA)
CPP_Flags = -m32 -ffreestanding -O2 -Wall -Wextra -fno-builtin -fno-exceptions -fno-rtti $(EXTRA)
LNK_Flags = -m elf_i386 

TOOLSDIR = tools
SOURCEDIR = src
BUILDDIR = build
EXECUTABLEDIR = bin


C_SOURCES = $(wildcard $(SOURCEDIR)/**/*.c)
CPP_SOURCES = $(wildcard $(SOURCEDIR)/**/*.cpp)
ASM_SOURCES = $(wildcard $(SOURCEDIR)/**/*.s)

C_OBJECTS = $(C_SOURCES:.c=.o)
CPP_OBJECTS = $(CPP_SOURCES:.cpp=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

EXECUTABLE = FluxOS.bin

all: init compile create_ramdisk create_grub

compile:$(ASM_OBJECTS) $(C_OBJECTS) $(CPP_OBJECTS) 
	@echo "Linking the Object files..."
	@ld -T linker.ld $(LNK_Flags) $(foreach file,$(^F),$(BUILDDIR)/$(file)) -o $(EXECUTABLEDIR)/$(EXECUTABLE) 

debug: all
	objcopy --only-keep-debug $(EXECUTABLEDIR)/$(EXECUTABLE) $(BUILDDIR)/kernel.sym

.cpp.o:
	@echo "Compiling C++ File" $<
	$(CC_CPP) $(CPP_Flags) -c $< -o $(BUILDDIR)/$(@F)

.c.o:
	@echo "Compiling C File" $< 
	@$(CC_C) $(C_Flags) -c $< -o $(BUILDDIR)/$(@F) 

.s.o:
	@echo "Compiling Assembly File" $<
	@$(CC_ASM) $(ASM_Flags) $< -o $(BUILDDIR)/$(@F)

init:
	@echo "Creating Build Directories" $<
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(EXECUTABLEDIR)

create_ramdisk:
	@echo "Building Ramdisk..."
	@gcc -std=gnu99 $(TOOLSDIR)/ramdisk_creator.c -o $(BUILDDIR)/ramdisk
	@echo "Running Ramdisk..."
	@./$(BUILDDIR)/ramdisk

create_grub:
	@echo "Creating Bootable Image" $<
	@mkdir -p $(BUILDDIR)/isodir/boot/grub
	@mkdir -p $(BUILDDIR)/isodir/modules
	@cp $(EXECUTABLEDIR)/$(EXECUTABLE) $(BUILDDIR)/isodir/boot/$(EXECUTABLE)
	@cp grub.cfg $(BUILDDIR)/isodir/boot/grub/grub.cfg
	@mv initrd.img $(BUILDDIR)/isodir/modules/initrd.img
	grub-mkrescue -o myos.iso $(BUILDDIR)/isodir

run:
	@echo "Running..." $<
	@qemu-system-i386 -cdrom myos.iso

run_debug:
	@echo "Running in Debug Mode..." $<
	@qemu-system-i386 -s -S -d int,cpu_reset -cdrom myos.iso

clean:
	@echo "Cleaning Build Directories and Files..." $<
	@rm -f initrd.img
	@rm -R -f $(BUILDDIR)
	@rm -R -f $(EXECUTABLEDIR)

