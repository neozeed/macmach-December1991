# Makefile for csh.

# C Shell with process control; VM/UNIX VAX Makefile
# Bill Joy UC Berkeley; Jim Kulp IIASA, Austria

PROG=	csh

CFGS= csh.login csh.cshrc

DESTDIR=

MANEXT=1

BINDIR=	${DESTDIR}/bin

CFGDIR=${DESTDIR}/etc/newconfig

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o bin -g bin -m 555

# To profile, put -DPROF and -pg in CFLAGS, and recompile.
CFLAGS= -O -MD -DTELL -DVFORK -DFILEC

SRCS=	alloc.c doprnt.c sh.c sh.char.c sh.dir.c sh.dol.c sh.err.c \
	sh.exec.c sh.exp.c sh.file.c sh.func.c sh.glob.c sh.hist.c \
	sh.init.c sh.lex.c sh.misc.c sh.parse.c sh.print.c sh.proc.c \
	sh.sem.c sh.set.c sh.time.c

OBJS=	alloc.o doprnt.o sh.o sh.char.o sh.dir.o sh.dol.o sh.err.o \
	sh.exec.o sh.exp.o sh.file.o sh.func.o sh.glob.o sh.hist.o \
	sh.init.o sh.lex.o sh.misc.o sh.parse.o sh.print.o sh.proc.o \
	sh.sem.o sh.set.o sh.time.o

all:	${PROG} .depend

# strings.o must be last since it can change when previous files compile
csh:	${OBJS} strings.o
	cc ${CFLAGS} ${OBJS} strings.o -o $@

.c.o:	FRC
	cc -E ${CFLAGS} $*.c | xstr -c -
	cc -c ${CFLAGS} x.c 
	mv -f x.o $*.o
	rm -f x.c

strings.o: strings
	xstr
	cc -c -R xs.c
	mv -f xs.o strings.o
	rm -f xs.c

sh.char.o sh.init.o:	FRC
	cc -E ${CFLAGS} $*.c | xstr -c -
	cc ${CFLAGS} -c -R x.c
	mv -f x.o $*.o
	rm -f x.c

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}
	install -c -o bin -g bin -m 444 ${CFGS} ${CFGDIR}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}
	rm -f ${MANDIR}/limit.${MANEXT}
	ln -s ${PROG}.${MANEXT} ${MANDIR}/limit.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG} strings strings.o x.c xs.c errs

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
