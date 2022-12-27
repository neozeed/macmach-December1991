# Copyright 1985, 1986, 1987, 1988 Massachusetts Institute of Technology
# Makefile for the Installation document.

TROFF= ptroff -t
FILES= 	paper.ms

all: paper.ps

paper.lpt:  $(FILES)
	 tbl $(FILES) | nroff -ms >$@

paper.ps:  $(FILES)
	tbl $(FILES) | $(TROFF) -ms >$@	

paper.txt:  $(FILES)
	tbl $(FILES) | nroff -ms | colcrt >$@

clean:
	rm -f *~ \#* *.bak errs 
