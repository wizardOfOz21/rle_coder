SHELL=cmd

OS_RM    = del 
.SUFFIXES:


TARGET   = test.abs
TARGETD  = testd.abs

CC       = nmc-gcc
AS       = nmc-gcc
CXXFLAGS =  -O2 -Wall -mnmc4 -g -std=c99
CXXFLAGSD=  -O0 -Wall -mnmc4 -g -std=c99
LIBS     =  -lc
LIBSD    =  -lc 
LDFLAGS  = -Wl,-Map=test.map  -Wl,-Tqemu.lds
LDFLAGSD = -Wl,-Map=testd.map -Wl,-Tqemu.lds 
INC_DIRS = -I../../include -I..
LIB_DIRS = 
SOURCES  = ../test.S

.DEFAULT_GOAL := all

all: $(TARGET) $(TARGETD)

$(TARGET): $(SOURCES) Makefile qemu.lds 
	@echo ------ build release --------------
	$(CC)  -o$(TARGET) $(CXXFLAGS) $(LDFLAGS)  $(INC_DIRS) $(LIB_DIRS)  $(SOURCES) $(LIBS) $(ERRECHO)
	nmc-objdump -D $(TARGET) > dasm.txt

$(TARGETD): $(SOURCES) Makefile qemu.lds
	@echo ------ build debug ----------------
	$(CC)  -o$(TARGETD) $(CXXFLAGSD) $(LDFLAGSD) $(INC_DIRS) $(LIB_DIRS) $(SOURCES) $(LIBSD) $(ERRECHO)
	nmc-objdump -D $(TARGETD) > dasmd.txt


run:  $(TARGET)
	nmc-qemu $(TARGET) 

rund: $(TARGETD)
	nmc-qemu -g 10000 $(TARGETD) 


	
gdb:
	nmc-gdb -iex "set tcp connect-timeout unlimited"  -ex "target remote :10000" $(TARGETD)


clean: 
	-$(OS_RM) *.abs *.elf *.map *.txt
	

