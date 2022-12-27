# Makefile for tzone.

# If you want something other than Eastern United States time used on your
# system, change the line below (after finding the zone you want in the
# time zone files, or adding it to a time zone file).
# Alternately, if you discover you've got the wrong time zone, you can just
#	zic -l rightzone
LOCALTIME=	US/Eastern

# If you want code inspired by certain emerging standards, add
#	-DSTD_INSPIRED
# to the end of the "CFLAGS=" line.

CFLAGS=	-O
LIBC=	/lib/libc.a
TZSRCS=zic.c scheck.c ialloc.c
TZCOBJS=zic.o scheck.o ialloc.o
TZDSRCS=zdump.c ialloc.c
TZDOBJS=zdump.o ialloc.o
DOCS=	Theory README Makefile newctime.3 tzfile.5 zic.8 zdump.8
SRCS=	zic.c zdump.c scheck.c ialloc.c

# If you want to handle solar-time-based time zones, remove the
# "#define NOSOLAR" from the include file usr/include/tzfile.h.
# (and add solar87 to the DATA= line below).
#DATA=	asia australasia europe etcetera northamerica pacificnew systemv
DATA=	etcetera northamerica pacificnew systemv

all:	zdump zic .depend

zdump:	${TZDOBJS} ${LIBC}
	${CC} ${CFLAGS} ${TZDOBJS} -o $@

zic:	${TZCOBJS} ${LIBC}
	${CC} ${CFLAGS} ${TZCOBJS} -o $@

install:
	./zic ${DATA}
	./zic -d ${DESTDIR}/etc/zoneinfo -l ${LOCALTIME} ${DATA}

install.man:
	@echo "*** need man page for tzone"

clean:
	rm -f .depend ${TZDOBJS} ${TZCOBJS} core zdump zic

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend
