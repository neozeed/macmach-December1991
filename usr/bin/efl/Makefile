# Makefile for efl.

DESTDIR=

CFLAGS=	-O -MD -w

SRCS=	addr.c alloc.c blklab.c dcl.c dclgen.c error.c exec.c field.c free.c \
	gram.c icfile.c init.c io.c lex.c main.c misc.c mk.c namgen.c pass2.c \
	print.c simple.c struct.c symtab.c tailor.c temp.c
OBJS=	addr.o alloc.o blklab.o dcl.o dclgen.o error.o exec.o field.o free.o \
	gram.o icfile.o init.o io.o lex.o main.o misc.o mk.o namgen.o pass2.o \
	print.o simple.o struct.o symtab.o tailor.o temp.o

oops:
	@echo "*** need efl for mac2 ***"

all:	efl .depend

efl: ${OBJS}
	${CC} -o $@ ${CFLAGS} ${OBJS}

# gram.c can no longer be made on a pdp11 because of yacc limits
gram.c: gram.head gram.dcl gram.expr gram.exec tokdefs
	(sed < tokdefs "s/#define/%token/" ; \
	cat gram.head gram.dcl gram.expr gram.exec) > gram.in
	${YACC} ${YFLAGS} gram.in
	(echo "#define YYDEBUG"; cat y.tab.c) > gram.c
	rm -f gram.in y.tab.c

lex.c: fixuplex
	lex lex.l
	sh fixuplex
	mv lex.yy.c lex.c

tokdefs: tokens
	grep -n . < tokens | sed "s/\([^:]*\):\(.*\)/#define \2 \1/" > tokdefs

install: oops
#install: FRC
#	install -c -s -o bin -g bin -m 755 efl ${DESTDIR}/usr/bin/efl

install.man: FRC
	install -c -o bin -g bin -m 444 efl.man ${DESTDIR}/usr/man/man1/efl.1

clean:	FRC
	rm -f .depend ${OBJS} core efl gram.c lex.c tokdefs

TEST:	FRC
	./efl "system=gcos" efltest/Hard.e >z1 2>z2
	cmp z1 efltest/Hard.out
	./efl "system=gcos" efltest/Band.e >z1 2>z2
	cmp z1 efltest/Band.out
	./efl "system=gcos" efltest/Buram.e >z1 2>z2
	cmp z1 efltest/Buram.out
	./efl "system=gcos" efltest/Dgl.e >z1 2>z2
	cmp z1 efltest/Dgl.out
	rm -f z1 z2
	@echo TEST OK

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
