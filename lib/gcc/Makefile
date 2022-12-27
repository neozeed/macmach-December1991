# Makefile for gcc.

CFLAGS=	-O

DESTDIR=

MANEXT=	1

BINDIR=	${DESTDIR}/bin

LIBDIR=	${DESTDIR}/lib

INCDIR=	${LIBDIR}/gcc-include

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

INFODIR= $(DESTDIR)/usr/man/info

INFO =	cpp.info cpp.info-1 cpp.info-2 \
	gcc.info gcc.info-1 gcc.info-2 gcc.info-3 gcc.info-4 gcc.info-5 \
	gcc.info-6 gcc.info-7 gcc.info-8 gcc.info-9 gcc.info-10

INCS=	stddef.h assert.h va-i860.h va-mips.h va-pyr.h va-sparc.h \
	va-spur.h limits.h proto.h float.h

all:
	@make -f Makefile.orig \
	  CFLAGS="${CFLAGS}" \
	  bindir="${BINDIR}" \
	  libdir="${LIBDIR}" \
	  all

install:
	-[ -d ${INCDIR} ] || mkdir ${INCDIR}
	chown bin.bin ${INCDIR}
	chmod 555 ${INCDIR}
	install -c -s -o bin -g bin -m 555 cc1 ${LIBDIR}/gcc-cc1
	install -c -s -o bin -g bin -m 555 cpp ${LIBDIR}/gcc-cpp
	install -c -s -o bin -g bin -m 555 gcc ${BINDIR}/gcc
	install -c -o bin -g bin -m 444 ${INCS} ${LIBDIR}/gcc-include
	install -c -o bin -g bin -m 444 gvarargs.h \
	  ${LIBDIR}/gcc-include/varargs.h
	install -c -o bin -g bin -m 444 gstdarg.h \
	  ${LIBDIR}/gcc-include/stdarg.h
	install -c -o bin -g bin -m 444 gnulib ${LIBDIR}/gcc-gnulib
	ranlib ${LIBDIR}/gcc-gnulib
	rm -f ${BINDIR}/cc; ln -s gcc ${BINDIR}/cc
	rm -f ${LIBDIR}/cpp; ln -s gcc-cpp ${LIBDIR}/cpp

install.man:
	install -c -o bin -g bin -m 444 gcc.1 ${MANDIR}/gcc.${MANEXT}
	install -c -o bin -g bin -m 444 cc.man ${MANDIR}/cc.${MANEXT}
	install -c -o bin -g bin -m 444 ${INFO} ${INFODIR}

clean:
	make -f Makefile.orig clean
