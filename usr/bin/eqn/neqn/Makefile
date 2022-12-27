# Makefile for neqn.

COMMON=	../common_source

CFLAGS=	-O -MD -DNEQN -I. -I${COMMON}

YFLAGS=	-d

VPATH=	${COMMON}

SRCS=	e.c diacrit.c eqnbox.c font.c fromto.c funny.c glob.c integral.c \
	io.c lex.c lookup.c mark.c matrix.c move.c over.c paren.c pile.c \
	shift.c size.c sqrt.c text.c

OBJS=	e.o diacrit.o eqnbox.o font.o fromto.o funny.o glob.o integral.o \
	io.o lex.o lookup.o mark.o matrix.o move.o over.o paren.o pile.o \
	shift.o size.o sqrt.o text.o

all:	neqn .depend

neqn:	${OBJS}
	cc -o $@ ${OBJS}

e.c:	${COMMON}/e.y
	yacc ${YFLAGS} ${COMMON}/e.y
	mv y.tab.h e.def
	mv y.tab.c e.c

install: FRC
	install -c -s -o bin -g bin -m 755 neqn ${DESTDIR}/usr/bin/neqn

clean:	FRC
	rm -f .depend ${OBJS} core neqn e.c e.def

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
