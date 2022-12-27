# Makefile for mac2 libc.

DESTDIR=

DEFS=

SUBDIR=	csu sys

TAGSFILE=tags

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS} DEFS=${DEFS}

install: FRC
	cd csu; make ${MFLAGS} DESTDIR=${DESTDIR} install

clean:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

tags:	FRC
	for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} TAGSFILE=../${TAGSFILE} tags); \
	done

FRC:
