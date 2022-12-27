# Makefile for plot.

DESTDIR=

CFLAGS=	-O -MD

SRCS=	atoplot.c chrtab.c crtdriver.c crtplot.c debug.c driver.c plottoa.c

OBJS=	atoplot.o chrtab.o crtdriver.o crtplot.o debug.o driver.o plottoa.o

STD=	tek t4013 t300 t300s t450 aedplot bgplot crtplot dumbplot gigiplot \
	hpplot hp7221plot implot atoplot plottoa grnplot

all:	${STD} debug .depend

tek:	driver.o
	cc ${CFLAGS} -o $@ driver.o -l4014 -lm

t4013:	driver.o
	cc ${CFLAGS} -o $@ driver.o -l4013 -lm

t300:	driver.o
	cc ${CFLAGS} -o $@ driver.o -l300 -lm

t300s:	driver.o
	cc ${CFLAGS} -o $@ driver.o -l300s -lm

t450:	driver.o
	cc ${CFLAGS} -o $@ driver.o -l450 -lm

aedplot: driver.o
	cc ${CFLAGS} -o $@ driver.o -lplotaed

bgplot: driver.o
	cc ${CFLAGS} -o $@ driver.o -lplotbg -lm

crtplot: crtdriver.o crtplot.o
	cc ${CFLAGS} -o $@ crtdriver.o crtplot.o -lcurses -ltermcap -lm

dumbplot: driver.o
	cc ${CFLAGS} -o $@ driver.o -lplotdumb -ltermcap -lm

gigiplot: driver.o
	cc ${CFLAGS} -o $@ driver.o -lplotgigi -lm

hpplot:	driver.o
	cc ${CFLAGS} -o $@ driver.o -lplot2648 -lm

hp7221plot: driver.o
	cc ${CFLAGS} -o $@ driver.o -lplot7221 -lm

implot:	driver.o
	cc ${CFLAGS} -o $@ driver.o -lplotimagen -lm

atoplot: atoplot.o
	cc ${CFLAGS} -o $@ atoplot.o -lplot -lm

plottoa: plottoa.o
	cc ${CFLAGS} -o $@ plottoa.o

grnplot: driver.o
	cc ${CFLAGS} -o $@ driver.o -lplotgrn -lm

debug:	debug.o
	cc ${CFLAGS} -o $@ debug.o

install: FRC
	for i in ${STD}; do \
	  (install -c -s -o bin -g bin -m 555 $$i ${DESTDIR}/usr/bin/$$i); \
	done
	install -c -o bin -g bin -m 555 plot.sh ${DESTDIR}/usr/bin/plot

install.man: FRC
	install -c -o bin -g bin -m 444 plot.man ${DESTDIR}/usr/man/man1/plot.1

clean:	FRC
	rm -f .depend ${OBJS} ${STD} debug

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
