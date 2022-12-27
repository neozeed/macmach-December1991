# Makefile for sendmail src.

SRCS=	alias.c arpadate.c clock.c collect.c conf.c convtime.c daemon.c \
	deliver.c domain.c envelope.c err.c headers.c macro.c main.c \
	parseaddr.c queue.c readcf.c recipient.c savemail.c srvrsmtp.c \
	stab.c stats.c sysexits.c trace.c usersmtp.c util.c Version.c
OBJS=	alias.o arpadate.o clock.o collect.o conf.o convtime.o daemon.o \
	deliver.o domain.o envelope.o err.o headers.o macro.o main.o \
	parseaddr.o queue.o readcf.o recipient.o savemail.o srvrsmtp.o \
	stab.o stats.o sysexits.o trace.o usersmtp.o util.o

CFLAGS=	-O -MD -I. -I../include -DVMUNIX -DMXDOMAIN
ASMSED=	../include/asm.sed.${MACHINE}

GET=	sccs get
DELTA=	sccs delta
WHAT=	what
PRT=	sccs prt

all:	sendmail .depend

sendmail: ${OBJS} Version.o
	cc ${COPTS} -o $@ Version.o ${OBJS} -ldbm
	size sendmail; ls -l sendmail; ${WHAT} < Version.o

dsendmail: ${OBJS} Version.o
	cc ${COPTS} -o sendmail Version.o ${OBJS} -ldbm -lresolv
	size sendmail; ls -l sendmail; ${WHAT} < Version.o

.c.o:
	cc -S ${CFLAGS} $*.c
	sed -f ${ASMSED} $*.s > x.s && as x.s -o $*.o
	rm -f $*.s x.s

install: FRC
	install -c -s -o root -g bin -m 6755 sendmail ${DESTDIR}/usr/lib
	install -c -o bin -g bin -m 644 /dev/null ${DESTDIR}/usr/lib/sendmail.fc
	install -c -o bin -g bin -m 666 /dev/null ${DESTDIR}/usr/lib/sendmail.st
	rm -f ${DESTDIR}/usr/ucb/newaliases
	ln -s ../lib/sendmail ${DESTDIR}/usr/ucb/newaliases
	rm -f ${DESTDIR}/usr/ucb/mailq
	ln -s ../lib/sendmail ${DESTDIR}/usr/ucb/mailq

#version: newversion ${OBJS} Version.c

#newversion:
#	@rm -f SCCS/p.version.c version.c
#	@${GET} -e SCCS/s.version.c
#	@${DELTA} -s SCCS/s.version.c
#	@${GET} -t -s SCCS/s.version.c

#Version.c: version.c
#	@echo generating Version.c from version.c
#	@cp version.c Version.c
#	@chmod 644 Version.c
#	@echo "" >> Version.c
#	@echo "# ifdef COMMENT" >> Version.c
#	@${PRT} SCCS/s.version.c >> Version.c
#	@echo "" >> Version.c
#	@echo "code versions:" >> Version.c
#	@echo "" >> Version.c
#	@${WHAT} ${OBJS} >> Version.c
#	@echo "" >> Version.c
#	@echo "# endif COMMENT" >> Version.c

clean:	FRC
	rm -f .depend ${OBJS} core sendmail a.out Version.o

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
