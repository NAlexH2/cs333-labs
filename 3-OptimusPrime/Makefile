# 
# Alex Harris - CS 333 Winter 2022

CC = gcc
DEBUG = -g
DEFINES = 
LDFLAGS = -lm
CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
		-Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
		-Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized \
		-pthread $(DEFINES)

PROG = primesMT

all: $(PROG)

$(PROG): $(PROG).o
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $(PROG) $@
	chmod og-rx $(PROG) rm *.o

# $(PROG).o: $(PROG).c $(PROG).h 
# 	$(CC) $(CFLAGS) -c

primesMT.o: primesMT.c $(PROG).h
	$(CC) $(CFLAGS) -c primesMT.c

clean cls:
	rm -f $(PROG) *.o *~ \#* *.out *.err *.log *.txt.gz

#primtesMT:

#primtesMT.o:

tar: clean
	tar cvfz ${LOGNAME}-$(PROG1).tar.gz *.[ch] ?akefile
