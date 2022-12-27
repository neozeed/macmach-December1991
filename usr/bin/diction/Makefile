# Makefile for diction.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	diction.c nwords.c end.c part.c pscan.c outp.c

OBJS=	nwords.o end.o part.o pscan.o outp.o

all:	dprog style1 style2 style3 .depend

dprog: diction.c
	cc ${CFLAGS} -DDICT=\"/usr/lib/dict.d\" diction.c -o $@

style1: nwords.o
	cc nwords.o -o $@ -ll

style2: end.o
	cc end.o -o $@ -ll

style3: part.o pscan.o outp.o
	cc part.o pscan.o outp.o -o $@ -ll

install: FRC
	-[ -d ${DESTDIR}/usr/dict ] || mkdir ${DESTDIR}/usr/dict
	-chmod 555 ${DESTDIR}/usr/dict
	-chown root.bin ${DESTDIR}/usr/dict
	install -c -s -o bin -g bin -m 555 style1 ${DESTDIR}/usr/lib
	install -c -s -o bin -g bin -m 555 style2 ${DESTDIR}/usr/lib
	install -c -s -o bin -g bin -m 555 style3 ${DESTDIR}/usr/lib
	install -c -s -o bin -g bin -m 555 dprog ${DESTDIR}/usr/lib
	install -c -o bin -g bin -m 555 style.sh ${DESTDIR}/usr/bin/style
	install -c -o bin -g bin -m 555 diction.sh ${DESTDIR}/usr/bin/diction
	install -c -o bin -g bin -m 555 explain.sh ${DESTDIR}/usr/bin/explain
	install -c -o bin -g bin -m 555 dict.d.src ${DESTDIR}/usr/lib/dict.d
	install -c -o bin -g bin -m 555 explain.d.src ${DESTDIR}/usr/lib/explain.d

install.man: FRC
	install -c -o bin -g bin -m 444 diction.man ${DESTDIR}/usr/man/man1/diction.1

clean:	FRC
	rm -f .depend ${OBJS} part.c nwords.c end.c style1 style2 style3 dprog

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
