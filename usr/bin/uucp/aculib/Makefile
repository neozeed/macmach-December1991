# /usr/src/usr.bin/uucp/aculib/Makefile for MacMach

CFLAGS=	-O -MD
LIBC=	/lib/libc.a
SRCS=	bsdtcp.c df2.c df12.c dk.c dn.c hys.c hysq.c mic.c nov.c pen.c pnet.c \
	rvmacs.c sy.c unet.c va212.c va811.c va820.c vad.c vent.c vmacs.c \
	hys24.c cds224.c att2224.c
OBJS=	bsdtcp.o df2.o df12.o dk.o dn.o hys.o hysq.o mic.o nov.o pen.o pnet.o \
	rvmacs.o sy.o unet.o va212.o va811.o va820.o vad.o vent.o vmacs.o \
	hys24.o cds224.o att2224.o
P=

all:	aculib.a .depend

aculib.a: $(P) ${OBJS}
	ar cr aculib.a ${OBJS}
	-ranlib aculib.a 2>&1 | sed '/no symbol table/d'

install: FRC

clean:	FRC
	rm -f .depend ${OBJS} core aculib.a

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
