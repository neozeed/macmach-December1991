# /usr/src/etc/named/tools/Makefile for MacMach

CFLAGS=	-O
RES=
LIBC=	/lib/libc.a
SRCS=	nsquery.c nstest.c
OBJS=	nsquery.o nstest.o

all:	nsquery nstest nslookup .depend

nsquery: ${LIBC}
	${CC} -o $@ nsquery.c ${RES}

# test programs and utilities
nstest: ${LIBC}
	@echo "The resolver routines in libc must be compiled with the -DDEBUG"
	@echo "option or nstest will not have any output."
	${CC} -o $@ nstest.c ${RES}

nslookup:	FRC
	cd nslookup; make ${MFLAGS} RES=${RES}

install: FRC
	cd nslookup; make ${MFLAGS} DESTDIR=${DESTDIR} install

clean:	FRC
	cd nslookup; make ${MFLAGS} clean
	rm -f .depend ${OBJS} core nsquery nstest

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
