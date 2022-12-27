#TROFF= ptroff
TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr

all: ddx r4 gdz strat

ddx: ddx.tbl.ms
	tbl ddx.tbl.ms | ${TROFF} -ms 2> index.raw > ddx.PS

r4: r4.tbl.ms
	tbl r4.tbl.ms | ${TROFF} -ms 2> index.raw > r4.PS

gdz: gdz.tbl.ms
	tbl gdz.tbl.ms | ${TROFF} -ms 2> index.raw > gdz.PS

strat: strat.ms
	${TROFF} -ms strat.ms 2> index.raw > strat.PS

clean:
	rm -f *~ \#* *.bak errs index.raw
