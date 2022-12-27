# Makefile for nroff_term.

# Possible options:
#	make		compile source for all terminal tables below
#	make <term>	compile table for a specific <term>
#	make install	move tables to ${DEST}
#	make clean	remove tab*.o files lying around

DEST=	${DESTDIR}/usr/lib/term

SRCS=	tab300-12.c tab300.c tab302-12.c tab302.c tab37.c tab382-12.c \
	tab382.c tab450-12.c tab450.c tab833-12.c tab833.c tabepson.c \
	tabitoh.c tabitoh12.c tablpr.c tabnec-t.c tabnec.c tabnec12.c \
	tabnec25-t.c tabqume.c tabqume12.c tabx-ecs.c tabx-ecs12.c \
	tabxerox.c tabxerox12.c
OBJS=	tab300-12.o tab300.o tab302-12.o tab302.o tab37.o tab382-12.o \
	tab382.o tab450-12.o tab450.o tab833-12.o tab833.o tabepson.o \
	tabitoh.o tabitoh12.o tablpr.o tabnec-t.o tabnec.o tabnec12.o \
	tabnec25-t.o tabqume.o tabqume12.o tabx-ecs.o tabx-ecs12.o \
	tabxerox.o tabxerox12.o
STD=	37 lpr 300 300-12 302 302-12 382 382-12 450 450-12 833 833-12 \
	epson itoh itoh12 nec nec12 nec-t nec25-t qume qume12 xerox \
	xerox12 x-ecs x-ecs12

all:	${STD} .depend

37:		tab37.o
lpr:		tablpr.o
300:		tab300.o 
300-12:		tab300-12.o 
302:		tab302.o 
302-12:		tab302-12.o 
382:		tab382.o 
382-12:		tab382-12.o 
450:		tab450.o 
450-12:		tab450-12.o 
833:		tab833.o 
833-12:		tab833-12.o 
epson:		tabepson.o
itoh:		tabitoh.o 
itoh12:		tabitoh12.o 
nec:		tabnec.o 
nec12:		tabnec12.o 
nec-t:		tabnec-t.o
nec25-t:	tabnec25-t.o
qume:		tabqume.o
qume12:		tabqume12.o
xerox:		tabxerox.o 
xerox12:	tabxerox12.o 
x-ecs:		tabx-ecs.o 
x-ecs12:	tabx-ecs12.o 

install: FRC
	for file in tab*.o; do \
	  install -c -s -o bin -g bin -m 555 $$file ${DEST}/`basename $$file .o`; \
	done
	rm -f ${DEST}/tabtn300; ln -s tablpr ${DEST}/tabtn300
	rm -f ${DEST}/tabcrt; ln -s tablpr ${DEST}/tabcrt
	rm -f ${DEST}/tab300s; ln -s tab302 ${DEST}/tab300s
	rm -f ${DEST}/tab300s-12; ln -s tab302-12 ${DEST}/tab300s-12
	rm -f ${DEST}/tabdtc; ln -s tab302 ${DEST}/tabdtc
	rm -f ${DEST}/tabdtc12; ln -s tab302-12 ${DEST}/tabdtc12
	rm -f ${DEST}/tabipsi; ln -s tab450 ${DEST}/tabipsi
	rm -f ${DEST}/tabipsi12; ln -s tab450-12 ${DEST}/tabipsi12
	install -c -o bin -g bin -m 644 README ${DEST}/README

install.man: FRC
	@echo "*** need man pages for nroff term"

clean:	FRC
	rm -f .depend ${OBJS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
