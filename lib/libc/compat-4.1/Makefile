# Makefile for libc compat-4.1.

DEFS=

CFLAGS=	-O -MD ${DEFS}

STDSRCS=ftime.c gtty.c nice.c pause.c rand.c stty.c tell.c \
	times.c utime.c vlimit.c vtimes.c

STD=	ftime.o gtty.o nice.o pause.o rand.o stty.o tell.o \
	times.o utime.o vlimit.o vtimes.o

# these should just be pitched, but in the interest of compatibility...
TRASHSRC=getpw.c
TRASH=	getpw.o 

SRCS=	${STDSRCS} ${TRASHSRCS}

OBJS=	${STD} ${TRASH}

TAGSFILE=tags

all:	${OBJS} .depend

link:	all
	(cd ../library; rm -f ${OBJS})
	(cd ../profiled; rm -f ${OBJS})
	cp ${OBJS} ../library
	(cd profiled; cp ${OBJS} ../../profiled)

.c.o:
	@cc -p ${CFLAGS} -c $*.c
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
