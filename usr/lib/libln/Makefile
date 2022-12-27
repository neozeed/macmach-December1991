# Makefile for libln.

CFLAGS=	-O -MD

SRCS=	allprint.c main.c reject.c yyless.c yywrap.c

OBJS=	allprint.o main.o reject.o yyless.o yywrap.o

all:	libln.a .depend

libln.a: ${OBJS}
	ar rc $@ ${OBJS}

install: FRC
	install -c -o bin -g bin -m 444 libln.a ${DESTDIR}/usr/lib/libln.a
	rm -f ${DESTDIR}/usr/lib/libl.a
	ln -s libln.a ${DESTDIR}/usr/lib/libl.a
	ranlib ${DESTDIR}/usr/lib/libln.a

install.man: FRC

clean:	FRC
	rm -f .depend ${OBJS} core libln.a

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
