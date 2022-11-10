# 
# Alex Harris - CS 333 Winter 2022

CC = gcc
DEBUG = -g
DEFINES = 
LDFLAGS = -lm
CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
		-Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
		-Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized \
		-pthread -Werror -Wunsafe-loop-optimizations $(DEFINES)

PROG = primesMT

all: $(PROG)

$(PROG): $(PROG).o
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $(PROG)

primesMT.o: primesMT.c $(PROG).h
	$(CC) $(CFLAGS) -c primesMT.c

clean cls:
	rm -f $(PROG) *.o *~ \#* *.out *.err *.log *.txt.gz

tar: clean
	tar cvfz ${LOGNAME}-$(PROG).tar.gz *.[ch] ?akefile
