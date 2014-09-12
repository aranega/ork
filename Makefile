CC=gcc
CXX=g++
LD=gcc
AR=ar

CFLAGS= -Wall -Werror -O2 -g
CXXFLAGS= -Wall -Werror -O2 -g
LDFLAGS=-O2 -g

ORK_OFILES=ork.o
LIBORK_OFILES=libork.o

all: ork libork

ork: $(ORK_OFILES)
	$(LD) $(LDFLAGS) $(ORK_OFILES) -o ork

libork: $(LIBORK_OFILES)
	rm -f libork.a
	$(AR) rcs libork.a $(LIBORK_OFILES)

.SUFFIXES: .c .cc .o

.c.o:
	$(CC) $(CFLAGS) -c $<

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(ORK_OFILES) $(LIBORK_OFILES)

veryclean: clean
	rm -f ork libork.a
