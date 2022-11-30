# R Jesse Chaney
# 

CC = gcc
DEBUG = -g
DEFINES = -DDEBUG

CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
        -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
        -Wdeclaration-after-statement $(DEFINES)

PROG1 = mystat
#PROG2 = mystat_ORIG
PROG3 = funny_socket

PROGS = $(PROG1) $(PROG2) $(PROG3)


all: $(PROGS)

$(PROG1): $(PROG1).o
	$(CC) $(CFLAGS) -o $@ $^
	chmod a+rx,g-w $@

$(PROG1).o: $(PROG1).c
	$(CC) $(CFLAGS) -c $<

#$(PROG2): $(PROG2).o
#	$(CC) $(CFLAGS) -o $@ $^
#	chmod a+rx,g-w $@

#$(PROG2).o: $(PROG2).c
#	$(CC) $(CFLAGS) -c $<

$(PROG3): $(PROG3).o
	$(CC) $(CFLAGS) -o $@ $^
	chmod a+rx,g-w,ug+s $@

$(PROG3).o: $(PROG3).c
	$(CC) $(CFLAGS) -c $<

cls clean:
	rm -f $(PROGS) *.o *~ \#* *.zip *.tar.gz

ci:
	if [ ! -d RCS ] ; then mkdir RCS; fi
	ci -t-none -l -m"lazy commit" [Mm]akefile *.[ch]

git get gat:
	if [ ! -d .git ] ; then git init; fi
	git add *.[ch] ?akefile
	git commit -m"git on with it"

TAR_FILE=cs333-${LOGNAME}-Lab4.tar.gz
tar:
	rm -f $(TAR_FILE)
	tar cvzf $(TAR_FILE) *.[ch] [Mm]akefile

zip:
	zip cs201-${LOGNAME}.zip *.[ch] ?akefile

funny: $(PROG3)
	rm -rf FUNNY*
	mkfifo FUNNYpipe
	mkdir FUNNYdir
	chmod +t FUNNYdir
	touch FUNNYregfile
	ln -s FUNNYregfile FUNNYsymlink
	touch JUNK
	ln -s JUNK FUNNYbroken
	rm JUNK
	ln FUNNYregfile FUNNYhardlink
	./funny_socket

