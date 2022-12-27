# Makefile for me.

MELIB=	/usr/lib/me
TMLIB=	/usr/lib/tmac
MESRCS=	acm.me chars.me deltext.me eqn.me float.me footnote.me \
	index.me letterhead.me local.me null.me refer.me sh.me tbl.me thesis.me
TMSRCS=	tmac.e

all:	FRC

install: FRC
	-[ -d ${DESTDIR}${MELIB} ] || mkdir ${DESTDIR}${MELIB}
	-[ -d ${DESTDIR}${TMLIB} ] || mkdir ${DESTDIR}${TMLIB}
	chown bin.bin ${DESTDIR}${MELIB} ${DESTDIR}${TMLIB}
	chmod 755 ${DESTDIR}${MELIB} ${DESTDIR}${TMLIB}
	for i in ${MESRCS}; do \
	  ./strip.sed < $$i > ${DESTDIR}${MELIB}/$$i; \
	  chown bin.bin ${DESTDIR}${MELIB}/$$i; \
	  chmod 444 ${DESTDIR}${MELIB}/$$i; \
	done
	for i in ${TMSRCS}; do \
	  ./strip.sed < $$i > ${DESTDIR}${TMLIB}/$$i; \
	  chown bin.bin ${DESTDIR}${TMLIB}/$$i; \
	  chmod 444 ${DESTDIR}${TMLIB}/$$i; \
	done
	install -c -o bin -g bin -m 444 revisions ${DESTDIR}${MELIB}/revisions

install.man: FRC

clean:	FRC

FRC:
