# Makefile for mac2 libc net.

DEFS=

FLAGS=	-MD ${DEFS} -I../../mac2

CFLAGS=	-O ${FLAGS}

SRCS=	htonl.S htons.S ntohl.S ntohs.S

OBJS=	htonl.o htons.o ntohl.o ntohs.o

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

install.man: FRC
	@echo "*** need man pages for libc"

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags:	FRC
	(P=`pwd`; \
	egrep "^ENTRY(.*)|^SYSCALL(.*)" ${SRCS} | /bin/sed \
"s;\([^:]*\):\([^(]*\)(\([^, )]*\)\(.*\);\3 $$P/\1 /^\2(\3\4$$/;" \
	  >> ${TAGSFILE})

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
