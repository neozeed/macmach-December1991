# Makefile for restore.

PROG1=	restore
PROG2=	rrestore

VPATH=	../dump

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

USRETC=	${DESTDIR}/usr/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	main.c interactive.c restore.c dirs.c symtab.c tape.c utilities.c \
	../dump/dumprmt.c

OBJS1=	main.o interactive.o restore.o dirs.o symtab.o utilities.o tape.o

OBJS2=	main.o interactive.o restore.o dirs.o symtab.o utilities.o rtape.o \
	dumprmt.o

all:	${PROG1} ${PROG2} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

rtape.o: ${LIBC} tape.c
	${CC} ${CFLAGS} -c -DRRESTORE tape.c -o rtape.o

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

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
