# Makefile for ftpd.

PROG= ftpd

CFGS= ftpusers

DESTDIR=

MANEXT=8

BINDIR=${DESTDIR}/etc
USRETC=${DESTDIR}/usr/etc

# get glob.c from ftp client sources
FTP=	../../usr/ucb/ftp

CFGDIR=	${DESTDIR}/etc/newconfig

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o root -g bin -m 544

CFLAGS= -O -MD

SRCS=	${PROG}.c ${FTP}/glob.c vers.c

GENS=	ftpcmd.c

OBJS=	${PROG}.o ftpcmd.o glob.o vers.o

all: ${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS}

glob.o:	${FTP}/glob.c
	cc ${CFLAGS} -c ${FTP}/glob.c

#vers.o: newvers.sh
#	-sh newvers.sh
#	cc ${CFLAGS} -c vers.c

# build and install program
install: FRC
	install -c -s ${IFLAGS} ${PROG} ${USRETC}/${PROG}
	rm -f ${BINDIR}/${PROG}; ln -s ../usr/etc/${PROG} ${BINDIR}/${PROG}
	install -c -o bin -g bin -m 444 ${CFGS} ${CFGDIR}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG} ${GENS}

.depend: ${SRCS} ${GENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
