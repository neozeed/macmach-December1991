# Makefile for grep.

PROGS=	egrep old.ucb.grep old.bin.grep old.fgrep old.egrep

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/usr/ucb

LIBDIR=	${DESTDIR}/usr/lib

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

# -DEGREPSTD=path	location of std egrep (normally /usr/bin/egrep).
EGREPSTD=\"${LIBDIR}/old.egrep\"

# -DGREPSTD=path	location of std grep (normally /bin/grep).
GREPSTD=\"${LIBDIR}/old.bin.grep\"

# -DFGREPSTD=path	location of std fgrep (normally /usr/bin/fgrep).
FGREPSTD=\"${LIBDIR}/old.fgrep\"

# -Dstrrchr=rindex, -Dstrchr=index	for troglodytes.
# -DSLOWSYS		invoke xread() for system time quirk on PDP, others? 
# -DNOKANJI		default is for Japanese Unix.  undef only for raw
#			 parity-marked search capability, not standard w/grep.
# -DCHINESE		for systems using EUC Chinese2 codes


CFLAGS=	-O -MD -Dstrrchr=rindex -Dstrchr=index -DNOKANJI \
	-DEGREPSTD=${EGREPSTD} -DGREPSTD=${GREPSTD} -DFGREPSTD=${FGREPSTD}

IFLAGS=	-o bin -g bin -m 555

SRCS=	egrep.c old.ucb.grep.c old.bin.grep.c old.fgrep.c old.egrep.c

all:	${PROGS} .depend

egrep: egrep.c
	cc -o $@ ${CFLAGS} egrep.c

oegrep: oegrep.c ${LIBC}
	cc -o $@ ${CFLAGS} oegrep.c

old.ucb.grep: ${LIBC}
	cc -o $@ ${CFLAGS} old.ucb.grep.c

old.bin.grep: ${LIBC}
	cc -o $@ ${CFLAGS} old.bin.grep.c

old.fgrep: ${LIBC}
	cc -o $@ ${CFLAGS} old.fgrep.c

old.egrep: old.egrep.c ${LIBC}
	cc -o $@ ${CFLAGS} old.egrep.c

old.egrep.c: old.egrep.y
	yacc old.egrep.y
	mv y.tab.c old.egrep.c

install: FRC
	install -c -s ${IFLAGS} egrep ${BINDIR}/egrep
	rm -f ${BINDIR}/grep; ln -s egrep ${BINDIR}/grep
	rm -f ${BINDIR}/fgrep; ln -s egrep ${BINDIR}/fgrep
	install -c -s ${IFLAGS} old.bin.grep ${LIBDIR}/old.bin.grep
	install -c -s ${IFLAGS} old.ucb.grep ${LIBDIR}/old.ucb.grep
	install -c -s ${IFLAGS} old.fgrep ${LIBDIR}/old.fgrep
	install -c -s ${IFLAGS} old.egrep ${LIBDIR}/old.egrep

install.man: FRC
	install -c -o bin -g bin -m 444 grep.man ${MANDIR}/grep.${MANEXT}
	rm -f ${MANDIR}/egrep.${MANEXT}
	ln -s grep.${MANEXT} ${MANDIR}/egrep.${MANEXT}
	rm -f ${MANDIR}/fgrep.${MANEXT}
	ln -s grep.${MANEXT} ${MANDIR}/fgrep.${MANEXT}

clean:	FRC
	rm -f .depend ${OBJS} core ${PROGS} old.egrep.c

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
