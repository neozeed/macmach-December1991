# /usr/src/etc/named/tools/nslookup/Makefile for MacMach

CFLAGS=	-O
RES=
LIBC=	/lib/libc.a
LIBS=	-ll ${RES}
CSRCS=	main.c getinfo.c debug.c send.c skip.c list.c subr.c
SRCS=	${CSRCS} commands.c
OBJS=	main.o getinfo.o debug.o send.o skip.o list.o subr.o commands.o

all:	nslookup .depend

nslookup: ${OBJS} ${LIBC}
	${CC} -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -s -o bin -g bin -m 755 nslookup ${DESTDIR}/usr/bin/nslookup
	install -c -o bin -g bin -m 444 nslookup.help ${DESTDIR}/usr/lib/nslookup.help
	install -c -o bin -g bin -m 444 nslookup.1 ${DESTDIR}/usr/man/man1/nslookup.1

clean:	FRC
	rm -f .depend ${OBJS} core nslookup commands.c nslookup.0 lex.yy.c lex.yy.o

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
