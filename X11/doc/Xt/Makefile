# Copyright 1985, Massachusetts Institute of Technology
# Makefile for the Toolkit documents.

TROFF= psroff -t
#TROFF= ditroff -Tps
NROFF= nroff
PRINTER= lpr

INTRINSICSSRC= \
	macros.t \
	strings.mit \
	Xtk.intr.front \
	CH[01][0123456789] \
	appA \
	appB \
	appC \
	appD \
	appE

all: ps

ps: intrinsics.PS

text: intrinsics.txt

intrinsics: intrinsics.txt intrinsics.PS

intrinsics.PS: $(INTRINSICSSRC)
	rm -f index.traw intrinsics.PS index.PS
	tbl $(INTRINSICSSRC) | eqn | \
		${TROFF} -ms 2> index.traw > intrinsics.PS
	make TROFF=${TROFF} index.PS

intrinsics.txt: $(INTRINSICSSRC)
	rm -f index.nraw intrinsics.txt index.txt
	tbl $(INTRINSICSSRC) | neqn | \
		${NROFF} -ms 2> index.nraw  >intrinsics.txt
	make NROFF=${NROFF} index.txt

index.PS:
	rm -f index.PS
	sort -f -t: +1 -3 +0n -1n index.traw | awk -f fixindex.awk \
		| awk -f block.awk > index.troff
	cat intr.idxmac.t index.troff | ${TROFF} -me > \
		index.PS
	rm -f index.troff index.traw

index.txt: 
	rm -f index.txt
	sort -f -t: +1 -3 +0n -1n index.nraw | awk -f fixindex.awk \
		| awk -f block.awk > index.nroff
	cat intr.idxmac.t index.nroff | ${NROFF} -me > \
		index.txt
	rm -f index.nroff index.nraw

clean:
	rm -f *~ \#* *.bak errs index.?raw *.?roff
