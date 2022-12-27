# Makefile for libc inet.

DEFS=

CFLAGS=	-O -MD ${DEFS}

SRCS=	inet_addr.c inet_network.c inet_netof.c inet_ntoa.c inet_lnaof.c \
	inet_makeaddr.c

OBJS=	inet_addr.o inet_network.o inet_netof.o inet_ntoa.o inet_lnaof.o \
	inet_makeaddr.o

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
