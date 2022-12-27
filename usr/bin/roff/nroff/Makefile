# Makefile for nroff.

DESTDIR=

COMMON=	../common_source

CFLAGS=	-O -MD -DNROFF -DVMUNIX -I${COMMON}

VPATH=	${COMMON}

SRCS=	ni.c nii.c n1.c n2.c n3.c n4.c n5.c n6.c n7.c n8.c n9.c n10.c \
	hytab.c ntab.c suftab.c

OBJS=	ni.o nii.o n1.o n2.o n3.o n4.o n5.o n6.o n7.o n8.o n9.o n10.o \
	hytab.o ntab.o suftab.o

all:	nroff .depend

nroff:	${OBJS}
	$(CC) -o $@ ${OBJS}

hytab.o: ${COMMON}/hytab.c
	cc -R -c ${COMMON}/hytab.c

suftab.o: ${COMMON}/suftab.c
	cc -R -c ${COMMON}/suftab.c

ntab.o: ntab.c
	cc -R -c ntab.c

install: FRC
	install -c -s -o bin -g bin -m 555 nroff ${DESTDIR}/usr/bin/nroff

clean:	FRC
	rm -f .depend ${OBJS} nroff

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
