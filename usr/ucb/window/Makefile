# Makefile for window.

PROG=	window

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD -R

IFLAGS=	-o bin -g bin -m 555

SRCS=	char.c cmd.c cmd1.c cmd2.c cmd3.c cmd4.c cmd5.c cmd6.c cmd7.c \
	context.c error.c lcmd.c lcmd1.c lcmd2.c main.c mloop.c parser1.c \
	parser2.c parser3.c parser4.c parser5.c scanner.c startup.c string.c \
	ttf100.c ttgeneric.c tth19.c tth29.c ttinit.c ttoutput.c tttermcap.c \
	tttvi925.c ttwyse60.c ttwyse75.c ttzentec.c \
	var.c win.c wwadd.c wwalloc.c wwbox.c wwchild.c \
	wwclose.c wwclreol.c wwclreos.c wwcursor.c wwdata.c wwdelchar.c \
	wwdelete.c wwdelline.c wwdump.c wwend.c wwenviron.c wwerror.c \
	wwflush.c wwframe.c wwgets.c wwinit.c wwinschar.c wwinsline.c \
	wwiomux.c wwlabel.c wwmisc.c wwmove.c wwopen.c wwprintf.c wwpty.c \
	wwputc.c wwputs.c wwredraw.c wwredrawwin.c wwrint.c wwscroll.c \
	wwsize.c wwspawn.c wwsuspend.c wwtty.c wwunframe.c wwupdate.c \
	wwwrite.c

OBJS=	char.o cmd.o cmd1.o cmd2.o cmd3.o cmd4.o cmd5.o cmd6.o cmd7.o \
	context.o error.o lcmd.o lcmd1.o lcmd2.o main.o mloop.o parser1.o \
	parser2.o parser3.o parser4.o parser5.o scanner.o startup.o string.o \
	ttf100.o ttgeneric.o tth19.o tth29.o ttinit.o ttoutput.o tttermcap.o \
	tttvi925.o ttwyse60.o ttwyse75.o ttzentec.o \
	var.o win.o wwadd.o wwalloc.o wwbox.o wwchild.o \
	wwclose.o wwclreol.o wwclreos.o wwcursor.o wwdata.o wwdelchar.o \
	wwdelete.o wwdelline.o wwdump.o wwend.o wwenviron.o wwerror.o \
	wwflush.o wwframe.o wwgets.o wwinit.o wwinschar.o wwinsline.o \
	wwiomux.o wwlabel.o wwmisc.o wwmove.o wwopen.o wwprintf.o wwpty.o \
	wwputc.o wwputs.o wwredraw.o wwredrawwin.o wwrint.o wwscroll.o \
	wwsize.o wwspawn.o wwsuspend.o wwtty.o wwunframe.o wwupdate.o \
	wwwrite.o

LIB=	-ltermcap

all:	${PROG} .depend

${PROG}: ${OBJS}
	cc -o $@ ${OBJS} ${LIB}

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
