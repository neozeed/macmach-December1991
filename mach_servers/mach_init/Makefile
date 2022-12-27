# Makefile for mach_init, Mach 3.0 version.

DESTDIR=

CFLAGS= -O -MD

SRCS=	main.c service.c test_service.c waitfor.c

OBJS1=	main.o service.o

OBJS2=	test_service.o

OBJS3=	waitfor.o

LIBS=	-lmach -lthreads

all: mach_init test_service waitfor .depend

mach_init: ${OBJS1}
	cc -o $@ ${OBJS1} ${LIBS}

test_service: ${OBJS2}
	cc -o $@ ${OBJS2} ${LIBS}

waitfor: ${OBJS3}
	cc -o $@ ${OBJS3} ${LIBS}

install: FRC
	install -c -s -o root -g bin -m 544 mach_init ${DESTDIR}/mach_servers
	install -c -s -o root -g bin -m 544 test_service ${DESTDIR}/usr/etc
	install -c -s -o root -g bin -m 544 waitfor ${DESTDIR}/usr/etc
	rm -f ${DESTDIR}/etc/test_service;
	ln -s ../usr/etc/test_service ${DESTDIR}/etc
	rm -f ${DESTDIR}/etc/waitfor;
	ln -s ../usr/etc/waitfor ${DESTDIR}/etc

install.man: FRC
	@echo "*** need man page for mach_init and test_service and waitfor"

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} ${OBJS3} mach_init test_service waitfor

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
