#TROFF= ptroff
TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr

all: ctext

ctext: ctext.tbl.ms
	tbl ctext.tbl.ms | ${TROFF} -ms 2> index.raw > ctext.PS

clean:
	rm -f *~ \#* *.bak errs index.raw
