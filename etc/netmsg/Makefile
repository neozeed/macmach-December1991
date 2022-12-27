# /usr/src/etc/netmsg/Makefile for MacMach

DESTDIR=

MANEXT=	8

MANDIR=	${DESTDIR}/usr/man/man${MANEXT}

SUBDIR=	server lib utils

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	 done

install.man: FRC
	install -c -o bin -g bin -m 444 netmsg.man ${MANDIR}/netmsg.${MANEXT}

clean:	FRC
	-for i in ${SUBDIR}; do (cd $$i; make clean); done

FRC:
