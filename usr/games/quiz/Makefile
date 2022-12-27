# Makefile for quiz.

DESTDIR=

LIBDIR=	${DESTDIR}/usr/games/lib/quiz.k 

CFLAGS=	-O -MD

SRCS=	quiz.c

OBJS=	quiz.o

all:	quiz .depend

quiz: ${OBJS}
	cc -o $@ ${OBJS}

install: FRC
	install -c -s -o games -g bin -m 500 quiz ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f quiz; ln -s dm quiz
	-[ -d ${LIBDIR} ] || mkdir ${LIBDIR}
	chmod 755 ${LIBDIR}; chown games.bin ${LIBDIR}
	cd quiz.k; install -c -o games -g bin -m 444 * ${LIBDIR}

install.man: FRC
	install -c -o bin -g bin -m 444 quiz.man ${DESTDIR}/usr/man/man6/quiz.6

clean:	FRC
	rm -f .depend ${OBJS} quiz

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
