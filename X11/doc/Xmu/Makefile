#TROFF= ptroff
TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr
FILES= ../Xlib/macros.t Xmu.ms

all: xmu

xmu: $(FILES)
	${TROFF} -ms ${FILES} 2> index.raw > xmu.PS

clean:
	rm -f *~ \#* *.bak errs index.raw
