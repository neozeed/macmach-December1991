# Makefile for libc net named.

DEFS=

CFLAGS= -O ${DEFS} 

SRCS=	gethostnamadr.c sethostent.c

OBJS=	gethostnamadr.o sethostent.o

TAGSFILE= tags

all:	${OBJS} .depend

link:	all
	(cd ../../library; rm -f ${OBJS})
	(cd ../../profiled; rm -f ${OBJS})
	cp ${OBJS} ../../library
	(cd profiled; cp ${OBJS} ../../../profiled)

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
