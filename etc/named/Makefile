# Makefile for named.

PROG=	named

DESTDIR=

MANEXT=8

BINDIR=${DESTDIR}/etc

USRETC=${DESTDIR}/usr/etc

MANDIR=${DESTDIR}/usr/man/man${MANEXT}

IFLAGS=	-o root -g bin -m 544

CFLAGS= -O -DDEBUG -DSTATS

SRCS=	db_dump.c db_load.c db_lookup.c db_reload.c db_save.c db_update.c \
	ns_forw.c ns_init.c ns_main.c ns_maint.c ns_req.c ns_resp.c \
	ns_sort.c ns_stats.c

CGENS=	version.c

OBJS=	db_dump.o db_load.o db_lookup.o db_reload.o db_save.o db_update.o \
	ns_forw.o ns_init.o ns_main.o ns_maint.o ns_req.o ns_resp.o \
	ns_sort.o ns_stats.o version.o

all: ${PROG} .depend

${PROG}: ${CGENS} ${OBJS}
	cc -o $@ ${OBJS}

version.c: FRC
	-sh newvers.sh

install: FRC
	install -c -s ${IFLAGS} ${PROG} ${USRETC}/${PROG}
	rm -f ${BINDIR}/${PROG}; ln -s ../usr/etc/${PROG} ${BINDIR}/${PROG}

install.man: FRC
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} ${PROG} ${CGENS}

.depend: ${SRCS} ${CGENS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS} ${CGENS}

-include .depend

FRC:
