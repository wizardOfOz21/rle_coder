SHELL=cmd

OS_RM    = del 
.SUFFIXES:


TARGET   = test.abs
TARGETD  = testd.abs

CC       = nmc-gcc
AS       = nmc-gcc
CXXFLAGS =  -O2 -Wall -mnmc4-fixed -g -std=c99
CXXFLAGSD=  -O0 -Wall -mnmc4-fixed -g -std=c99
LIBS     =  -lnm6407int -Wl,--whole-archive -lnm6407_io_nmc -lmc12101load_nm -Wl,--no-whole-archive
LIBSD    =  -lnm6407int -Wl,--whole-archive -lnm6407_io_nmc -lmc12101load_nm -lmc12101_stub_nmc4_fixed -Wl,--no-whole-archive 
LDFLAGS  = -Wl,-Map=test.map  -Wl,-Tmc12101-nmpu1.lds
LDFLAGSD = -Wl,-Map=testd.map -Wl,-Tmc12101-nmpu1.lds 
INC_DIRS = -I"$(MC12101)/include" -I../../include -I..
LIB_DIRS = -L"$(MC12101)/lib"  
SOURCES  = ../main.cpp ../Encode.S ../Encode1.S

.DEFAULT_GOAL := all

all: $(TARGET) $(TARGETD)

$(TARGET): $(SOURCES) Makefile mc12101-nmpu1.lds 
	@echo ------ build release --------------
	$(CC)  -o$(TARGET) $(CXXFLAGS) $(LDFLAGS)  $(INC_DIRS) $(LIB_DIRS)  $(SOURCES) $(LIBS) $(ERRECHO)
	nmc-objdump -D $(TARGET) > dasm.txt

$(TARGETD): $(SOURCES) Makefile mc12101-nmpu1.lds
	@echo ------ build debug ----------------
	$(CC)  -o$(TARGETD) $(CXXFLAGSD) $(LDFLAGSD) $(INC_DIRS) $(LIB_DIRS) $(SOURCES) $(LIBSD) $(ERRECHO)
	nmc-objdump -D $(TARGETD) > dasmd.txt


run:  $(TARGET) 
	mc12101run -p -R -a1 -v $(TARGET) 

rund: $(TARGETD)
	mc12101run -p -R -a1 -v $(TARGETD) 

monitor:
	mc12101_gdb_monitor 0 1 5555
	
gdb:
	nmc-gdb -ex "target remote :5555" $(TARGETD)


clean: 
	-$(OS_RM) *.abs *.elf *.map *.txt
	

