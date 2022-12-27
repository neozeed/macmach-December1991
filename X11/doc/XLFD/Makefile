#TROFF= ptroff
TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr

all: xlfd

xlfd: xlfd.tbl.ms
	tbl xlfd.tbl.ms | ${TROFF} -ms 2> index.raw > xlfd.PS

clean:
	rm -f *~ \#* *.bak errs index.raw
