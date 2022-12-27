# Copyright 1985, Massachusetts Institute of Technology
# Makefile for the Xlib document.

#TROFF= ptroff
TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr
FILES= macros.t\
	abstract.t\
	credits.t\
	CH[01][0123456789]\
	appendixA\
	appendixB\
	appendixC\
	appendixD\
	glossary\
	postproc

CFILES = macros.t R4Xlib.tbl.ms

XLIBDOC = 
XLIBMAN = Xman
SUBDIRS = \
	$(XLIBMAN)

all: xlib index changes

xlib: $(FILES)
	tbl ${FILES} | eqn |${TROFF} -ms 2> index.raw > xlib.PS
	make TROFF=${TROFF} index

index.raw:
	tbl ${FILES} | eqn | ${TROFF} -ms > /dev/null 2> index.raw|\
	${PRINTER}

index:	index.raw
	sort -f -t: +1 -3 +0n -1n index.raw | awk -f fixindex.awk |awk -f block.awk > index.troff
	cat indexmacros.t index.troff | ${TROFF} -me > xlib.idx.PS
	rm -f index.troff

changes: $(CFILES)
	tbl ${CFILES} | ${TROFF} -ms 2> index.traw > changes.PS

install:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "installing $$i"; \
			$(MAKE) $(MFLAGS) \
				INSTALL='$(INSTALL)' \
				DESTDIR='$(DESTDIR)' install ); \
	done

clean:
	rm -f *~ \#* *.bak errs index.traw index.nraw
