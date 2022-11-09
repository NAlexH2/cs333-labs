# 
# Alex Harris - CS 333 Winter 2022

CC = gcc
DEBUG = -g
DEFINES = 
DFLAGS = -lm
CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
		-Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
		-Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized \
		-pthread $(DEFINES)

PROG1 = $(PROG2).o

PROG2 = primesMT

PROGS = $(PROG1) $(PROG2)

all: $(PROGS)

$(PROG1): $(PROG1) $(CC) 
	$(CFLAGS) -o -lm $@ $^ 
	chmode a+rx,g-w $@

$(PROG1): $(PROG2).c $(PROG2).h Makefile
	$(CC) $(CFLAGS) -c $<

clean cls:
	rm -f $(PROGS) *.o *~ \#* *.out *.err *.log *.txt.gz

#primtesMT:

#primtesMT.o:

tar: clean
	tar cvfz ${LOGNAME}-$(PROG1).tar.gz *.[ch] ?akefile
