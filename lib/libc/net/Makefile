# Makefile for libc net.

DEFS=

CFLAGS=	-O -MD ${DEFS}

SRCS=	getnetbyaddr.c getnetent.c getnetbyname.c getproto.c getprotoent.c \
	getprotoname.c getservent.c getservbyport.c getservbyname.c \
	herror.c rcmd.c rexec.c ruserpass.c res_comp.c res_debug.c \
	res_init.c res_mkquery.c res_query.c res_send.c

OBJS=	getnetbyaddr.o getnetent.o getnetbyname.o getproto.o getprotoent.o \
	getprotoname.o getservent.o getservbyport.o getservbyname.o \
	herror.o rcmd.o rexec.o ruserpass.o res_comp.o res_debug.o \
	res_init.o res_mkquery.o res_query.o res_send.o

TAGSFILE=tags

all:	${OBJS} .depend

link:	all
	(cd ../library; rm -f ${OBJS})
	(cd ../profiled; rm -f ${OBJS})
	cp ${OBJS} ../library
	(cd profiled; cp ${OBJS} ../../profiled)

.c.o:
	@cc -p -c ${CFLAGS} $*.c
	@-ld -X -o profiled/$*.o -r $*.o
	cc ${CFLAGS} -c $*.c
	@-ld -x -r $*.o
	@mv a.out $*.o

install.man: FRC
	@echo "*** need man pages for libc"

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags:	FRC
	cwd=`pwd`; \
	for i in ${SRCS}; do ctags -a -f ${TAGSFILE} $$cwd/$$i; done

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
