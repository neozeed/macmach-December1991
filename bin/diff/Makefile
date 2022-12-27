# Makefile for diff.

PROG1=	diff
PROG2=	diffh

DESTDIR=

MANEXT=1

BINDIR=	${DESTDIR}/bin
LIBDIR=${DESTDIR}/usr/lib

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

DIFF=	${BINDIR}/diff
DIFFH=	${LIBDIR}/diffh
PR=	${BINDIR}/pr

CFLAGS= -O -MD -DDIFF='"${DIFF}"' -DDIFFH='"${DIFFH}"' -DPR='"${PR}"'

SRCS=	diff.c diffdir.c diffreg.c diffh.c

OBJS1=	diff.o diffdir.o diffreg.o

OBJS2=	diffh.o

all:	${PROG1} ${PROG2} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	install -c -s ${IFLAGS} ${PROG2} ${LIBDIR}/${PROG2}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	rm -f ${MANDIR}/${PROG2}.${MANEXT}
	ln -s ${PROG1}.${MANEXT} ${MANDIR}/${PROG2}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} ${PROG1} ${PROG2}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
