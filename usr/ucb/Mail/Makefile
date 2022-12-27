# Makefile for Mail.

PROG1=	Mail
PROG2=	fmt
PROG3=	mail

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

LIBDIR=	${DESTDIR}/usr/lib

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o bin -g bin -m 555

SRCS=	aux.c cmd1.c cmd2.c cmd3.c cmdtab.c collect.c config.c edit.c fio.c \
	getname.c head.c v7.local.c lex.c list.c main.c names.c optim.c \
	popen.c quit.c send.c strings.c temp.c tty.c vars.c version.c

OBJS1=	version.o aux.o cmd1.o cmd2.o cmd3.o cmdtab.o collect.o config.o \
	edit.o fio.o getname.o head.o v7.local.o lex.o list.o main.o \
	names.o optim.o popen.o quit.o send.o strings.o temp.o tty.o \
	vars.o str.o flsbuf.o

OBJS2=	fmt.o xhead.o

all:	${PROG1} ${PROG2} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

.c.o:
	cc -E ${CFLAGS} $*.c | xstr -c -
	cc -c -o $*.o ${CFLAGS} x.c
	@rm -f x.c

version.o: version.c
	cc -c -R version.c

xhead.c:
	ln -s head.c xhead.c

xhead.o:
	cc -c ${CFLAGS} xhead.c

fmt.o:
	cc -c ${CFLAGS} fmt.c

str.o: strings
	xstr
	cc -R -c -o str.o xs.c
	rm -f xs.c

cmdtab.o: cmdtab.c
	cc -R -c ${CFLAGS} cmdtab.c

install: FRC
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	install -c -s ${IFLAGS} ${PROG2} ${BINDIR}/${PROG2}
	install -c -o bin -g bin -m 444 misc/Mail.* ${LIBDIR}
	rm -f ${BINDIR}/${PROG3}; ln -s ${PROG1} ${BINDIR}/${PROG3}

install.man: FRC
	rm -f ${MANDIR}/${PROG1}.${MANEXT}
	ln -s ${PROG2}.${MANEXT} ${MANDIR}/${PROG1}.${MANEXT}
	install -c -o bin -g bin -m 444 ${PROG2}.man \
	  ${MANDIR}/${PROG2}.${MANEXT}
	install -c -o bin -g bin -m 444 ${PROG3}.man \
	  ${MANDIR}/${PROG3}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} ${PROG1} ${PROG2}
	rm -f x.c xs.c xhead.c strings

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
