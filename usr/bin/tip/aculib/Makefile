# Makefile for tip aculib.

# make file for tip device drivers
#
# Current drivers:
#	BIZCOMP
#	USR COURIER (2400 baud)
#	DEC DF02-AC, DF03-AC
#	DEC DN-11/Able Quadracall
#	HAYES and Hayes emulators
#	VENTEL 212+ (w/o echo)
#	VADIC 831 RS232 adaptor
#	VADIC 3451
CFLAGS=	-O -MD -I..
LIBC=	/lib/libc.a
SRCS=	biz22.c biz31.c courier.c df.c dn11.c hayes.c ventel.c v831.c v3451.c
OBJS=	biz22.o biz31.o courier.o df.o dn11.o hayes.o ventel.o v831.o v3451.o

all:	aculib.a .depend

aculib.a: ${OBJS}
	ar cu aculib.a ${OBJS}
	ranlib aculib.a

install: FRC

clean:	FRC
	rm -f .depend ${OBJS} core aculib.a

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
