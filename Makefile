# Makefile template file for FLAME
#
# FLAME is the FLAME source files
# SOURCE is the user source files
#
# The assumption is that the user only provides C files
# and a model files <file>.xml
#
#===================================================
# Change this variable for the location of the 
# Message Board Library installation
#===================================================
LIBMBOARD_DIR = /usr/local/lib
LIBMBOARD_INC = $(LIBMBOARD_DIR)/include
LIBMBOARD_LIB = $(LIBMBOARD_DIR)/lib

#====================================================
# Change these for your compliler/loaded
#====================================================
SERIAL_CCOMP	= gcc
PARALLEL_CCOMP	= mpicc

SERIAL_LOADER	= gcc
PARALLEL_LOADER	= mpif77

DEFINES		=

#DEFINES	+= -DSTART_END 
#DEFINES	+= -DGRAPHICS

# C Compiler
CC      = $(SERIAL_CCOMP)
CFLAGS  =  -I$(LIBMBOARD_INC) ${DEFINES}

CFLAGS += -O3 -std=c99

LD       = $(SERIAL_LOADER)

LDFLAGS  = -L$(LIBMBOARD_LIB)

LIBS    += -lmboard_s  -lm

#FLAME source files
FLAME   = main.c memory.c xml.c messageboards.c partitioning.c rules.c timing.c

#FLAME auxilary files
AUX 	= stategraph.dot stategraph_colour.dot process_order_graph.dot Doxyfile latex.tex

# FLAME generated model files
SOURCES =  write_state.c  read_states.c  forcesLJ.c 
HEADERS = header.h low_primes.h mboard.h  Sporo_agent_header.h 

DEPS    = Makefile header.h low_primes.h

OBJECTS = $(SOURCES:.c=.o) $(FLAME:.c=.o)
EXECUTABLE = main
RM = rm -f

all: $(SOURCES) $(FLAME) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

$(OBJECTS): $(DEPS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).exe 
vclean:
	$(RM) main $(EXECUTABLE) $(EXECUTABLE).exe $(OBJECTS)  $(FLAME) $(HEADERS)  $(AUX) Makefile
format:
	astyle -A3 $(SOURCES)
	for file in $(patsubst %.c,%.h, $(SOURCES)) ; do \
	echo $$file ; \
	astyle -A3 $$file ; \
	done 
print:
	a2ps -R -f10 --column=1 $(SOURCES)
