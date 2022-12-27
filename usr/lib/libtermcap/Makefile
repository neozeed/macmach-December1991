# Makefile for libtermcap.

CFLAGS=	-O -MD -DCM_N -DCM_GT -DCM_B -DCM_D

SRCS=	termcap.c tgoto.c tputs.c
OBJS=	termcap.o tgoto.o tputs.o

all:	termcap.a termcap_p.a .depend

termcap.a termcap_p.a: ${OBJS}
	ar cr termcap.a ${OBJS}
	cd profiled; ar cr ../termcap_p.a ${OBJS}

test:	FRC
	ranlib termcap.a
	cc ${CFLAGS} -o tc1 tc1.c termcap.a
	cc ${CFLAGS} -o tc2 tc2.c termcap.a
	cc ${CFLAGS} -o tc3 tc3.c termcap.a

.c.o:
	cc ${CFLAGS} -c -p $*.c
	ld -x -r -o profiled/$*.o $*.o
	cc ${CFLAGS} -c $*.c
	ld -X -r $*.o
	mv a.out $*.o

install: FRC
	install -c -o bin -g bin -m 644 termcap.a ${DESTDIR}/usr/lib/libtermcap.a
	ranlib ${DESTDIR}/usr/lib/libtermcap.a
	install -c -o bin -g bin -m 644 termcap_p.a ${DESTDIR}/usr/lib/libtermcap_p.a
	ranlib ${DESTDIR}/usr/lib/libtermcap_p.a
	install -c -o bin -g bin -m 444 termcap.man ${DESTDIR}/usr/man/man3/termcap.3
	rm -f ${DESTDIR}/usr/lib/libtermlib.a
	ln -s libtermcap.a ${DESTDIR}/usr/lib/libtermlib.a
	rm -f ${DESTDIR}/usr/lib/libtermlib_p.a
	ln -s libtermcap_p.a ${DESTDIR}/usr/lib/libtermlib_p.a

install.man: FRC
	rm -f ${DESTDIR}/usr/man/man3/termlib.3
	ln -s termcap.3 ${DESTDIR}/usr/man/man3/termlib.3

clean:	FRC
	rm -f .depend ${OBJS} profiled/*.o termcap.a termcap_p.a

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
