# Makefile for timed.

PROG1=	timed
PROG2=	timedc

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

USRETC=	${DESTDIR}/usr/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

# optional flags are: MEASURE TESTING DEBUG
CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	acksend.c candidate.c correct.c master.c networkdelta.c readmsg.c \
	slave.c timed.c cmds.c cmdtab.c timedc.c byteorder.c measure.c \
	cksum.${MACHINE}.c ocsum.s

OBJS1=	acksend.o candidate.o correct.o master.o networkdelta.o readmsg.o \
	slave.o timed.o byteorder.o measure.o cksum.${MACHINE}.o ocsum.o

OBJS2=	cmds.o cmdtab.o timedc.o byteorder.o measure.o cksum.${MACHINE}.o \
	ocsum.o

all:	${PROG1} ${PROG2} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

cksum.${MACHINE}.o: FRC
	cc ${CFLAGS} -c $*.c

ocsum.o: ocsum.s
	as ocsum.s -o ocsum.o

install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${USRETC}/${PROG1}
	rm -f ${BINDIR}/${PROG1}; ln -s ../usr/etc/${PROG1} ${BINDIR}/${PROG1}
	install -c -s ${IFLAGS} ${PROG2} ${USRETC}/${PROG2}
	rm -f ${BINDIR}/${PROG2}; ln -s ../usr/etc/${PROG2} ${BINDIR}/${PROG2}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	install -c -o bin -g bin -m 444 ${PROG2}.man \
	  ${MANDIR}/${PROG2}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} ${PROG1} ${PROG2}

.depend: FRC
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
