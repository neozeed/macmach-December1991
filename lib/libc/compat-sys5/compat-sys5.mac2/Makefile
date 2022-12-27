# Makefile for mac2 libc compat-sys5.

DEFS=

FLAGS=	-I../../mac2 ${DEFS}

CFLAGS=	-O -MD ${FLAGS}

SSRCS=

CSRCS=

SRCS=	${SSRCS} ${CSRCS}

OBJS=

TAGSFILE=tags

all:	${OBJS} .depend

link:	all
	(cd ../../library; rm -f ${OBJS})
	(cd ../../profiled; rm -f ${OBJS})
#	cp ${OBJS} ../../library
#	(cd profiled; cp ${OBJS} ../../../profiled)

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
#	cwd=`pwd`; \
#	for i in ${SRCS}; do \
#	  ctags -a -f ${TAGSFILE} $$cwd/$$i; \
#	done
#	(P=`pwd`; \
#	egrep "^ENTRY(.*)|^SYSCALL(.*)" ${SSRCS} | sed \
#"s;\([^:]*\):\([^(]*\)(\([^, )]*\)\(.*\);\3 $$P/\1 /^\2(\3\4$$/;" \
#	>> ${TAGSFILE})

.depend: ${CSRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
