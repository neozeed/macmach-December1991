#TROFF= ptroff
TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr

all: icccm

icccm: icccm.tbl.ms
	tbl icccm.tbl.ms | ${TROFF} -ms 2> index.raw > icccm.PS

clean:
	rm -f *~ \#* *.bak errs index.raw
