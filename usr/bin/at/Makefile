# Makefile for at.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	atq.c atrm.c at.c atrun.c

OBJS=	atq.o atrm.o at.o atrun.o

ATRUNDIR=	/usr/lib
SPOOLDIRS=	/usr/spool/at /usr/spool/at/past
SPOOLFILES=	/usr/spool/at/lasttimedone

all:	atq atrm at atrun .depend

at:	at.c
	cc ${CFLAGS} -o $@ at.c

atq:	atq.c
	cc ${CFLAGS} -o $@ atq.c

atrm:	atrm.c
	cc ${CFLAGS} -o $@ atrm.c

atrun:	atrun.c
	cc ${CFLAGS} -o $@ atrun.c

install: FRC
	-for i in ${SPOOLDIRS}; do \
	  mkdir ${DESTDIR}/$$i > /dev/null 2>&1; \
	  chown bin ${DESTDIR}/$$i; \
	  chgrp daemon ${DESTDIR}/$$i; \
	  chmod 575 ${DESTDIR}/$$i; \
	done
	-for i in ${SPOOLFILES}; do \
	  if [ ! -f ${DESTDIR}/$$i ]; then \
	    echo 0 > ${DESTDIR}/$$i; \
	  fi; \
	  chown bin ${DESTDIR}/$$i; \
	  chgrp daemon ${DESTDIR}/$$i; \
	done
	install -c -s -o bin -g bin -m 555 atrun ${DESTDIR}/${ATRUNDIR}
	install -c -s -o bin -g daemon -m 4555 atq ${DESTDIR}/usr/bin
	install -c -s -o bin -g daemon -m 4555 at ${DESTDIR}/usr/bin
	install -c -s -o bin -g daemon -m 4555 atrm ${DESTDIR}/usr/bin

install.man: FRC
	install -c -o bin -g bin -m 444 at.man ${DESTDIR}/usr/man/man1/at.1

clean:	FRC
	rm -f .depend ${OBJS} atq atrm at atrun

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
