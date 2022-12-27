# Copyright 1985, Massachusetts Institute of Technology
# Makefile for the Xlib document.

TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr
FILES1= macros.t\
	X11.protocol
FILES2= macros.t\
	X11.keysyms\
	X11.encoding\
	glossary

all: protocol appendices index

protocol: $(FILES1)
	tbl ${FILES1} | eqn |${TROFF} -ms 2> index.raw > protocol.PS

appendices: $(FILES2)
	tbl ${FILES2} | eqn |${TROFF} -ms -n74 2>> index.raw > append.PS

index:	index.raw
	sort -f -t: +1 -3 +0n -1n index.raw | awk -f fixindex.awk |awk -f block.awk > index.troff
	cat indexmacros.t index.troff | ${TROFF} -me > proto.idx.PS
	rm -f index.troff

clean:
	rm -f *~ \#* *.bak errs index.traw index.nraw

