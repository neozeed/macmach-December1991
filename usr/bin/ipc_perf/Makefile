# Makefile for ipc_perf.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	icmp.c machipc.c palloc.c tcpip.c

STD=	 machipc palloc icmp tcpip

all:	${STD} .depend

${STD}:
	cc ${CFLAGS} -o $@ $@.c -lmach

install: FRC
	install -c -s -o bin -g bin -m 755 ${STD} ${DESTDIR}/usr/bin

install.man: FRC
	@echo "*** need man pages for ipc_perf"

clean:	FRC
	rm -f .depend ${STD} a.out core *.s *.o

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
