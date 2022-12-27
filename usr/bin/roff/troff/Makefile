# Makefile for troff.

DESTDIR=

COMMON=	../common_source

CFLAGS=	-O -MD -DVMUNIX -I${COMMON}

VPATH=	${COMMON}

SRCS=	ni.c nii.c n1.c n2.c n3.c n4.c n5.c t6.c n7.c n8.c n9.c t10.c \
	hytab.c suftab.c tab3.c

OBJS=	ni.o nii.o n1.o n2.o n3.o n4.o n5.o t6.o n7.o n8.o n9.o t10.o \
	hytab.o suftab.o tab3.o

all:	troff .depend

troff:	${OBJS}
	cc -o $@ ${OBJS}

hytab.o: ${COMMON}/hytab.c
	cc -R -c ${COMMON}/hytab.c

suftab.o: ${COMMON}/suftab.c
	cc -R -c ${COMMON}/suftab.c

install: FRC
	install -c -s -o bin -g bin -m 555 troff ${DESTDIR}/usr/bin/troff

install.man: FRC
	@echo "*** need man pages for troff"

clean:	FRC
	rm -f .depend ${OBJS} troff

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
