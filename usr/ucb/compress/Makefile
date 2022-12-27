# Makefile for compress.

PROG1=	compress
PROG2=	uncompress
PROG3=	zcat

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD -DBSD4_2 -DSACREDMEM=256000 -DUSERMEM=`cat USERMEM`

IFLAGS=	-o bin -g bin -m 555

SRCS=	${PROG1}.c

OBJS=	${PROG1}.o

all:	${PROG1} .depend

${PROG1}: USERMEM ${OBJS}
	cc -o $@ ${OBJS}

# USERMEM may have to be set by hand.  It should contain the amount of
# available user memory in bytes.  Set it to zero, for physical memory
# less than 1 Meg.
USERMEM:
	sh usermem.sh > USERMEM

install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	rm -f ${BINDIR}/${PROG2}; ln -s ${PROG1} ${BINDIR}/${PROG2}
	rm -f ${BINDIR}/${PROG3}; ln -s ${PROG1} ${BINDIR}/${PROG3}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	rm -f ${MANDIR}/${PROG2}.${MANEXT}
	ln -s ${PROG1}.${MANEXT} ${MANDIR}/${PROG2}.${MANEXT}
	rm -f ${MANDIR}/${PROG3}.${MANEXT}
	ln -s ${PROG1}.${MANEXT} ${MANDIR}/${PROG3}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
