#TROFF= ptroff
TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr

all: xdmcp

xdmcp: xdmcp.ms
	tbl xdmcp.ms | eqn | ${TROFF} -ms 2> index.raw > xdmcp.PS

clean:
	rm -f *~ \#* *.bak errs index.raw
