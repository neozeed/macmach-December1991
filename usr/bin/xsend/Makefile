# /usr/src/usr.bin/xsend/Makefile for MacMach

CFLAGS=	-O -MD
LIBC=	/lib/libc.a
SRCS=	enroll.c lib.c xget.c xsend.c
OBJS=	enroll.o lib.o xget.o xsend.o

all:	enroll xsend xget .depend

enroll: enroll.o lib.o ${LIBC}
	cc enroll.o lib.o -lmp -o $@

xget: xget.o lib.o ${LIBC}
	cc xget.o lib.o -lmp -o $@

xsend: xsend.o lib.o ${LIBC}
	cc xsend.o lib.o -lmp -o $@

install: FRC
	install -c -s -o bin -g bin -m 755 enroll ${DESTDIR}/usr/bin/enroll
	install -c -s -o bin -g bin -m 755 xget ${DESTDIR}/usr/bin/xget
	install -c -s -o bin -g bin -m 755 xsend ${DESTDIR}/usr/bin/xsend

install.man: FRC
	install -c -o bin -g bin -m 444 xsend.man ${DESTDIR}/usr/man/man1/xsend.1
	rm -f ${DESTDIR}/usr/man/man1/enroll.1
	ln -s xsend.1 ${DESTDIR}/usr/man/man1/enroll.1
	rm -f ${DESTDIR}/usr/man/man1/xget.1
	ln -s xsend.1 ${DESTDIR}/usr/man/man1/xget.1

clean:	FRC
	rm -f .depend ${OBJS} core enroll xsend xget

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
