# Maekfile for mac2 libc csu.

#	crt0	Normal C run time startoff
#	mcrt0	C run time start off for profiling, ``prof'' conventions
#	gcrt0	C run time start off for profiling, ``gprof'' conventions

DEFS=

VERSION= -DVERSION="`cat ../../../../VERSION`"

SRCS=	crt0.c mon.c gmon.c

OBJS=	crt0.o mcrt0.o gcrt0.o mon.o gmon.o

TAGSFILE=tags

all link: ${OBJS} .depend

crt0.o: crt0.c
	cc -MD ${VERSION} ${DEFS} -DCRT0 crt0.c -c -o x.o
	ld -x -r -o crt0.o x.o
	rm -f x.o

moncrt0.o: crt0.c
	cc -MD ${VERSION} ${DEFS} -DMCRT0 -c crt0.c -o moncrt0.o

mon.o: mon.c
	cc -MD ${DEFS} -c mon.c -o x.o
	ld -x -r -o mon.o x.o
	rm -f x.o

gmon.o: gmon.c
	cc ${DEFS} -c gmon.c

gcrt0.o: moncrt0.o gmon.o
	ld -x -r -o gcrt0.o moncrt0.o gmon.o

mcrt0.o: moncrt0.o mon.o
	ld -x -r -o mcrt0.o moncrt0.o mon.o

install: FRC
	install -c -o bin -g bin -m 444 crt0.o ${DESTDIR}/lib/crt0.o
	install -c -o bin -g bin -m 444 mcrt0.o ${DESTDIR}/lib/mcrt0.o
	install -c -o bin -g bin -m 444 gcrt0.o ${DESTDIR}/usr/lib/gcrt0.o

install.man: FRC
	@echo "*** need man pages for libc"

clean:	FRC
	rm -f .depend *.o a.out core ${TAGSFILE}

tags:	FRC
	cwd=`pwd`; \
	for i in ${SRCS}; do ctags -a -f ${TAGSFILE} $$cwd/$$i; done

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
