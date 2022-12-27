TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr

all: shape

shape: shape.ms
	${TROFF} -ms shape.ms 2> index.raw > shape.PS

clean:
	rm -f *~ \#* *.bak errs index.raw
