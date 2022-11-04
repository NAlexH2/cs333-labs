# 
# Alex Harris - CS 333 Winter 2022

CC = gcc
DEBUG = -g
DEFINES = 


CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
		-Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
		-Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized \
		$(DEFINES)

PROG1 = primesMT

PROGS = $(PROG1)

all: $(PROGS)

# $(PROG1): $(PROG1).o $(CC) 
#	$(CFLAGS) -o $@ $^ 
#	chmode a+rx,g-w $@

$(PROG1).o: $(PROG1).c $(PROG1).h Makefile
	$(CC) $(CFLAGS) -c $<

clean cls:
	rm -f $(PROGS) *.o *~ \#* *.out *.err *.log *.txt.gz

#primtesMT:

#primtesMT.o:

tar: clean
	tar cvfz ${LOGNAME}-$(PROG1).tar.gz *.[ch] ?akefile
