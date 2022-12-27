# Makefile for libdbm.

CFLAGS=	-O -MD

SRCS=	dbm.c

OBJS=	dbm.o

all:	libdbm.a libdbm_p.a .depend

libdbm.a: dbm.c
	cc -c ${CFLAGS} dbm.c
	ld -r -o libdbm.a -x dbm.o
	rm -f dbm.o

libdbm_p.a: dbm.c
	cc -p -c ${CFLAGS} dbm.c
	ld -r -o libdbm_p.a -X dbm.o
	rm -f dbm.o

install: FRC
	install -c -o bin -g bin -m 644 libdbm.a ${DESTDIR}/usr/lib/libdbm.a
	install -c -o bin -g bin -m 644 libdbm_p.a ${DESTDIR}/usr/lib/libdbm_p.a
	install -c -o bin -g bin -m 444 dbm.h ${DESTDIR}/usr/include/dbm.h

install.man: FRC

clean:	FRC
	rm -f .depend ${OBJS} core libdbm.a libdbm_p.a

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
