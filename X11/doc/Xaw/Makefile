# Copyright 1985, Massachusetts Institute of Technology
# Makefile for the Toolkit documents.

.DIRECTORIES:

TROFF= psroff
EXTENSION= PS

#For xditview
#TROFF= ditroff -TX100
#EXTENSION= dit

NROFF= nroff
PRINTER= lpr

EVERYWHERE=macros.t strings.mit strings.xaw
CHAPTER1=CH1
CHAPTER2=CH2
CHAPTER3=CH3.intro Command Grip Label List Scrollbar Simple StripChart Toggle 
CHAPTER4=CH4.intro SimpleMenu SmeBSB SmeLine Sme MenuButton
CHAPTER5=CH5.intro TextActions TextFuncs AsciiText AsciiSource AsciiSink \
	 TextCustom Text TextSource TextSink
CHAPTER6=CH6.intro Box Dialog Form Paned Viewport
CHAPTER7=CH7.intro Template

WIDGETSRC= \
	$(EVERYWHERE) TPage_Credits \
	$(CHAPTER1) $(CHAPTER2) $(CHAPTER3) $(CHAPTER4) \
	$(CHAPTER5) $(CHAPTER6) $(CHAPTER7) 

all: ps

ps: widgets.$(EXTENSION) ConvertToR4.$(EXTENSION)

text: widgets.txt

widgets: widgets.txt widgets.$(EXTENSION)

widgets.$(EXTENSION): $(WIDGETSRC)
	rm -f index.traw widgets.$(EXTENSION) index.$(EXTENSION)
	tbl $(WIDGETSRC) | ${TROFF} -ms 2> index.traw  > widgets.$(EXTENSION)
	make index

ConvertToR4.$(EXTENSION): ConvertToR4
	rm -f ConvertToR4.$(EXTENSION)
	tbl ConvertToR4 | ${TROFF} -ms 2> /dev/null  > ConvertToR4.$(EXTENSION)

widgets.txt: $(WIDGETSRC)
	rm -f index.nraw widgets.txt index.txt
	tbl $(WIDGETSRC) | neqn | \
		${NROFF} -ms 2> index.nraw  > widgets.txt
	make index.txt

index: index.traw
	rm -f index.$(EXTENSION)
	sort -f -t: +1 -3 +0n -1n index.traw | awk -f fixindex.awk \
		| awk -f block.awk > index.troff
	cat widg.idxmac.t index.troff | ${TROFF} -me  > \
		index.$(EXTENSION)
	rm -f index.troff index.traw

index.txt: index.nraw
	rm -f index.txt
	sort -f -t: +1 -3 +0n -1n index.nraw | awk -f fixindex.awk \
		| awk -f block.awk > index.nroff
	cat widg.idxmac.t index.nroff | ${NROFF} -me > \
		index.txt
	rm -f index.nroff index.nraw

clean:
	rm -f *~ \#* *.bak errs index.?raw *.?roff

spotless:
	make clean
	rm -f *.dit *.PS

Chapter1: $(CHAPTER1)
	rm -f index.traw Chapter1.$(EXTENSION) index.$(EXTENSION)
	tbl $(EVERYWHERE) $(CHAPTER1) | \
	${TROFF} -ms 2> index.traw  > Chapter1.$(EXTENSION)

Chapter2: $(CHAPTER2)
	rm -f index.traw Chapter2.$(EXTENSION) index.$(EXTENSION)
	tbl $(EVERYWHERE) $(CHAPTER2) | \
	${TROFF} -ms 2> index.traw  > Chapter2.$(EXTENSION)

Chapter3: $(CHAPTER3)
	rm -f index.traw Chapter3.$(EXTENSION) index.$(EXTENSION)
	tbl $(EVERYWHERE) $(CHAPTER3) | \
	${TROFF} -ms 2> index.traw  > Chapter3.$(EXTENSION)

Chapter4: $(CHAPTER4)
	rm -f index.traw Chapter4.$(EXTENSION) index.$(EXTENSION)
	tbl $(EVERYWHERE) $(CHAPTER4) | \
	${TROFF} -ms 2> index.traw  > Chapter4.$(EXTENSION)

Chapter5: $(CHAPTER5)
	rm -f index.traw Chapter5.$(EXTENSION) index.$(EXTENSION)
	tbl $(EVERYWHERE) $(CHAPTER5) | \
	${TROFF} -ms 2> index.traw  > Chapter5.$(EXTENSION)

Chapter6: $(CHAPTER6)
	rm -f index.traw Chapter6.$(EXTENSION) index.$(EXTENSION)
	tbl $(EVERYWHERE) $(CHAPTER6) | \
	${TROFF} -ms 2> index.traw  > Chapter6.$(EXTENSION)

Chapter7: $(CHAPTER7)
	rm -f index.traw Chapter7.$(EXTENSION) index.$(EXTENSION)
	tbl $(EVERYWHERE) $(CHAPTER7) | \
	${TROFF} -ms 2> index.traw  > Chapter7.$(EXTENSION)

