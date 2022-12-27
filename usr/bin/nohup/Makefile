# Makefile for nohup.

PROG=	nohup

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/bin

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

all:	FRC

install: FRC
	install -c ${IFLAGS} ${PROG}.sh ${BINDIR}/${PROG}

install.man: FRC
	rm -f ${MANDIR}/${PROG}.${MANEXT}
	ln -s nice.${MANEXT} ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC

FRC:
