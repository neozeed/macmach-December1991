# Makefile for mac2 libc gen.

DEFS=

VPATH=	../../mac2

FLAGS=	-MD -I../../mac2 ${DEFS}

CFLAGS=	-O ${FLAGS}

CSRCS=	atof.c index.c rindex.c strcat.c strcmp.c strcpy.c strlen.c \
	strncat.c strncmp.c strncpy.c strtod.c

SSRCS=	_setjmp.S abs.S alloca.S bcmp.S bcopy.S bzero.S fabs.S ffs.S frexp.S \
	insque.S ldexp.S modf.S remque.S setjmp.S

SRCS=	${CSRCS} ${SSRCS}

OBJS=	_setjmp.o abs.o alloca.o atof.o bcmp.o bcopy.o bzero.o fabs.o ffs.o \
	frexp.o index.o insque.o ldexp.o modf.o remque.o rindex.o setjmp.o \
	strcat.o strcmp.o strcpy.o strlen.o strncat.o strncmp.o strncpy.o \
	strtod.o

TAGSFILE=tags

all:	${OBJS} .depend

link:	all
	(cd ../../library; rm -f ${OBJS})
	(cd ../../profiled; rm -f ${OBJS})
	cp ${OBJS} ../../library
	(cd profiled; cp ${OBJS} ../../../profiled)

.S.o:
	@cc -p ${FLAGS} -c $*.S
	@-ld -X -o profiled/$*.o -r $*.o
	cc ${FLAGS} -c $*.S
	@-ld -x -r $*.o
	@mv a.out $*.o

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
	(P=`pwd`; \
	egrep "^ENTRY(.*)|^SYSCALL(.*)" ${SSRCS} | sed \
"s;\([^:]*\):\([^(]*\)(\([^, )]*\)\(.*\);\3 $$P/\1 /^\2(\3\4$$/;" \
	>> ${TAGSFILE})

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
