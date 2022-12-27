# Makefile for lpr vfilters.

CFLAGS=	-O -MD

SRCS=	necf.c railmag.c rvcat.c rvsort.c sidebyside.c vcat.c \
	vdmp.c vpf.c vplotf.c vpltdmp.c vpsf.c vsort.c

OBJS=	necf.o railmag.o rvcat.o rvsort.o sidebyside.o vcat.o \
	vdmp.o vpf.o vplotf.o vpltdmp.o vpsf.o vsort.o

STD=	necf railmag rvcat rvsort sidebyside vcat \
	vdmp vpf vplotf vpltdmp vpsf vsort

all:	${STD} .depend

necf:	necf.c
	cc -O -o $@ necf.c

railmag: railmag.c
	cc -O -o $@ railmag.c

rvcat:	rvcat.c
	cc -O -o $@ rvcat.c

rvsort:	rvsort.c
	cc -O -o $@ rvsort.c

sidebyside: sidebyside.c
	cc -O -o $@ sidebyside.c

vcat:	vcat.c
	cc -O -o $@ vcat.c

vdmp:	vdmp.c
	cc -O -o $@ vdmp.c

vpf:	vpf.c
	cc -O -o $@ vpf.c

vplotf:	vplotf.c chrtab.c
	cc -O -o $@ vplotf.c chrtab.c

vpltdmp: vpltdmp.c
	cc -O -o $@ vpltdmp.c

vpsf:	vpsf.c
	cc -O -o $@ vpsf.c

vsort:	vsort.c
	cc -O -o $@ vsort.c

install: FRC
	install -c -s -o bin -g bin -m 755 necf ${DESTDIR}/usr/lib/necf
	install -c -s -o bin -g bin -m 755 railmag ${DESTDIR}/usr/lib/railmag
	install -c -s -o bin -g bin -m 755 rvcat ${DESTDIR}/usr/lib/rvcat
	install -c -s -o bin -g bin -m 755 rvsort ${DESTDIR}/usr/lib/rvsort
	install -c -s -o bin -g bin -m 755 sidebyside ${DESTDIR}/usr/lib/sidebyside
	install -c -s -o bin -g bin -m 755 vcat ${DESTDIR}/usr/lib/vcat
	install -c -s -o bin -g bin -m 755 vdmp ${DESTDIR}/usr/lib/vdmp
	install -c -s -o bin -g bin -m 755 vpf ${DESTDIR}/usr/lib/vpf
	rm -f ${DESTDIR}/usr/lib/vpfW
	ln -s vpf ${DESTDIR}/usr/lib/vpfW
	install -c -s -o bin -g bin -m 755 vplotf ${DESTDIR}/usr/lib/vplotf
	install -c -s -o bin -g bin -m 755 vpltdmp ${DESTDIR}/usr/lib/vpltdmp
	install -c -s -o bin -g bin -m 755 vpsf ${DESTDIR}/usr/lib/vpsf
	install -c -s -o bin -g bin -m 755 vsort ${DESTDIR}/usr/lib/vsort

clean:	FRC
	rm -f .depend ${OBJS} core ${STD} chrtab.o

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
