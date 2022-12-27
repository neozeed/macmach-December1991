# Makefile for sendmail.

DESTDIR=

SUBDIR=	lib src aux

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

install: FRC
	-[ -d ${DESTDIR}/usr/spool/mqueue ] || mkdir ${DESTDIR}/usr/spool/mqueue
	chown root.wheel ${DESTDIR}/usr/spool/mqueue
	chmod 775 ${DESTDIR}/usr/spool/mqueue
	for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done

install.man: FRC

clean:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

FRC:
