# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O2

all	: bin/cl
	@echo -n ""

# optimized version
bin/cl	: main_opt.o lib
			$(CC) $(OPTFLAGS)  main_opt.o -ltm_usage -Llib -o bin/cl
main_opt.o 	   	: src/CellLegalization.cpp
			$(CC) $(CFLAGS) $< -Ilib -o $@

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*

