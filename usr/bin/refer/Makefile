# Makefile for refer.

CFLAGS=	-O -MD

DLSRCS=	deliv1.c deliv2.c

DLOBJS=	deliv1.o deliv2.o

HTSRCS=	deliv2.c glue4.c glue5.c hunt1.c hunt2.c hunt3.c hunt5.c hunt6.c \
	hunt7.c hunt8.c hunt9.c refer3.c shell.c tick.c

HTOBJS=	deliv2.o glue4.o glue5.o hunt1.o hunt2.o hunt3.o hunt5.o hunt6.o \
	hunt7.o hunt8.o hunt9.o refer3.o shell.o tick.o

INSRCS=	inv1.c inv2.c inv3.c inv5.c inv6.c deliv2.c

INOBJS=	inv1.o inv2.o inv3.o inv5.o inv6.o deliv2.o

MKSRCS=	mkey1.c mkey2.c mkey3.c deliv2.c

MKOBJS=	mkey1.o mkey2.o mkey3.o deliv2.o

RFSRCS=	deliv2.c glue1.c glue2.c glue3.c glue4.c glue5.c hunt2.c hunt3.c \
	hunt5.c hunt6.c hunt7.c hunt8.c hunt9.c mkey3.c refer0.c refer1.c \
	refer2.c refer4.c refer5.c refer6.c refer7.c refer8.c shell.c

RFOBJS=	deliv2.o glue1.o glue2.o glue3.o glue4.o glue5.o hunt2.o hunt3.o \
	hunt5.o hunt6.o hunt7.o hunt8.o hunt9.o mkey3.o refer0.o refer1.o \
	refer2.o refer4.o refer5.o refer6.o refer7.o refer8.o shell.o

WHSRCS=	what1.c what2.c what3.c what4.c shell.c mkey3.c

WHOBJS=	what1.o what2.o what3.o what4.o shell.o mkey3.o

SRCS=	addbib.c lookbib.c ${DLSRCS} ${HTSRCS} ${INSRCS} ${MKSRCS} ${RFSRCS} \
	${WHSRCS}

OBJS=	addbib.o lookbib.o sortbib.o ${DLOBJS} ${HTOBJS} ${INOBJS} ${MKOBJS} \
	${RFOBJS} ${WHOBJS}

all:	addbib deliv hunt inv lookbib mkey refer sortbib whatabout .depend

addbib: addbib.o
	cc addbib.o -o $@

deliv: ${DLOBJS} ${LIBC}
	cc ${DLOBJS} -o $@

hunt: ${HTOBJS} ${LIBC}
	cc ${HTOBJS} -o $@

inv: ${INOBJS} ${LIBC}
	cc ${INOBJS} -o $@

lookbib: lookbib.o
	cc lookbib.o -o $@

mkey: ${MKOBJS}
	cc ${MKOBJS} -o $@

refer: ${RFOBJS}
	cc ${RFOBJS} -o $@

# refpart doesn't compile
#
#RPSRCS=	deliv2.c glue4.c glue5.c mkey3.c refer0.c refer1.c refer2.c \
#	refer3.c refer4.c refer5.c refer6.c refer7.c refer8.c
#RPOBJS=	deliv2.o glue4.o glue5.o mkey3.o refer0.o refer1.o refer2.o \
#	refer3.o refer4.o refer5.o refer6.o refer7.o refer8.o
#refpart: ${RPOBJS} ${LIBC}
#	cc ${CFLAGS} ${RPOBJS} -o $@

sortbib: sortbib.o
	cc  sortbib.o -o $@

whatabout: ${WHOBJS} ${LIBC}
	cc ${WHOBJS} -o $@

install: FRC
	-[ -d ${DESTDIR}/usr/lib/refer ] || mkdir ${DESTDIR}/usr/lib/refer
	install -c -s -o bin -g bin -m 755 mkey ${DESTDIR}/usr/lib/refer/mkey
	install -c -s -o bin -g bin -m 755 inv ${DESTDIR}/usr/lib/refer/inv
	install -c -s -o bin -g bin -m 755 hunt ${DESTDIR}/usr/lib/refer/hunt
	install -c -s -o bin -g bin -m 755 refer ${DESTDIR}/usr/bin/refer
	install -c -s -o bin -g bin -m 755 addbib ${DESTDIR}/usr/bin/addbib
	install -c -s -o bin -g bin -m 755 sortbib ${DESTDIR}/usr/bin/sortbib
	install -c -s -o bin -g bin -m 755 lookbib ${DESTDIR}/usr/bin/lookbib
	install -c -o bin -g bin -m 755 roffbib.sh ${DESTDIR}/usr/bin/roffbib
	install -c -o bin -g bin -m 755 indxbib.sh ${DESTDIR}/usr/bin/indxbib
	install -c -o bin -g bin -m 755 tmac.bib ${DESTDIR}/usr/lib/tmac
	tar -chf - papers | (cd ${DESTDIR}/usr/dict; tar -xvf -)
	find ${DESTDIR}/usr/dict/papers -type f -exec chmod 644 {} \;
	find ${DESTDIR}/usr/dict/papers -type f -exec chown root.bin {} \;
	chmod 755 ${DESTDIR}/usr/dict/papers
	chown root.bin ${DESTDIR}/usr/dict/papers
	(cd ${DESTDIR}/usr/dict/papers; /bin/sh runinv)

install.man: FRC
	rm -f ${DESTDIR}/usr/man/man1/indxbib.1
	ln -s lookbib.1 ${DESTDIR}/usr/man/man1/indxbib.1
	@echo "*** need man pages for refer"

clean:	FRC
	rm -f .depend ${OBJS} core addbib deliv hunt inv lookbib mkey refer \
		refpart sortbib whatabout

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
