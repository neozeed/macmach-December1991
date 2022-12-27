# /usr/src/usr.lib/sendmail/aux/Makefile for MacMach

CFLAGS=	-O -MD -I../src -I../include -DVMUNIX
STD=	mconnect mailstats praliases mail rmail
SRCS=	mconnect.c mail.c mailstats.c praliases.c rmail.c
OBJS=	mconnect.o mail.o mailstats.o praliases.o rmail.o

.c.o:
	cc -S ${CFLAGS} $*.c
	sed -f ../include/asm.sed.${MACHINE} $*.s > x.s && as x.s -o $*.o
	rm -f $*.s x.s

all:	${STD} .depend

mail: mail.o
	cc -o $@ mail.o

rmail: rmail.o
	cc -o $@ rmail.o

mconnect: mconnect.o
	cc -o $@ mconnect.o

mailstats: mailstats.o
	cc -o $@ mailstats.o

praliases: praliases.o
	cc -o $@ praliases.o -ldbm

install: FRC
	install -c -s -o root -g bin -m 4755 mail ${DESTDIR}/bin/mail
	install -c -s -o bin -g bin -m 755 rmail ${DESTDIR}/bin/rmail
	install -c -s -o bin -g bin -m 755 mconnect mailstats praliases ${DESTDIR}/usr/lib

clean:	FRC
	rm -f .depend core ${STD} ${OBJS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
