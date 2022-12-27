# Makefile for hunt.

#  Hunt
#  Copyright (c) 1985 Conrad C. Huang, Gregory S. Couch, Kenneth C.R.C. Arnold
#  San Francisco, California
#
HDR=	hunt.h
DSRC=	answer.c driver.c draw.c execute.c expl.c makemaze.c \
	shots.c terminal.c extern.c pathname.c
DOBJ=	answer.o driver.o draw.o execute.o expl.o makemaze.o \
	shots.o terminal.o extern.o
PSRC=	hunt.c connect.c playit.c pathname.c
POBJ=	hunt.o connect.o playit.o

#
# Flags are:
#	DEBUG	Don't trust everything in the code
#	INTERNET Use the Internet domain IPC instead of UNIX domain
#	BROADCAST Use internet broadcasting code when looking for driver
#	OLDIPC	Use 4.1a internet system calls (must also define
#		INTERNET but not BROADCAST)
#	RANDOM	Include doors which disperse shots randomly
#	REFLECT	Include diagonal walls that reflect shots
#	MONITOR	Include code for watching the game from the sidelines
#	OOZE	Include slime shots
#	FLY	Make people fly when walls regenerate under them
#	START_FLYING	Players enter flying (FLY must also be defined)
#	VOLCANO	Include occasional large slime explosions
#
# NOTE: if you change the domain (INTERNET vs UNIX) then "make newdomain"
#
DEFS=	-I. -DBROADCAST -DRANDOM -DREFLECT -DMONITOR -DINTERNET \
	-DOOZE -DFLY -DVOLCANO

CFLAGS=	-O -MD $(DEFS)

LDFLAGS=

PROFLAGS=

.SUFFIXES:	.uu .obj .c,v

.obj.uu:
	uuencode $*.obj < $*.obj > $*.uu

.c,v.c:
	co $*.c

all:	hunt hunt.driver .depend

#
# For testing
#
debug:	hunt.dbg hunt.driver.dbg

hunt.dbg:	$(POBJ) pathname.dbg.o
	cc $(LDFLAGS) -o hunt.dbg $(POBJ) pathname.dbg.o -lcurses -ltermlib

hunt.driver.dbg: $(DOBJ) pathname.dbg.o
	cc $(PROFLAGS) $(LDFLAGS) -o hunt.driver.dbg $(DOBJ) pathname.dbg.o

#
# Binary distribution to other sites
#
distribution:	hunt.uu hunt.driver.uu README pathname.c Makefile.dist hunt.6
	@ln Makefile.dist makefile
	shar -a README makefile pathname.c hunt.uu hunt.driver.uu hunt.6\
	  > distribution
	@rm -f makefile hunt.uu hunt.driver.uu hunt.obj hunt.driver.obj

hunt.driver.obj:	$(DOBJ) pathname.o
	ld -r -x -o hunt.driver.obj $(DOBJ)
	symstrip hunt.driver.obj pathname.o -lcurses -ltermcap

hunt.obj:	$(POBJ) pathname.o
	ld -r -x -o hunt.obj $(POBJ)
	symstrip hunt.obj pathname.o -lcurses -ltermcap

#
# System installation
#
install: FRC
	install -c -s -o games -g bin -m 500 hunt.driver $(DESTDIR)/usr/games/lib/hunt.driver
	install -c -s -o games -g bin -m 500 hunt $(DESTDIR)/usr/games/hide/hunt
	cd ${DESTDIR}/usr/games; rm -f hunt; ln -s dm hunt

hunt:	$(POBJ) pathname.o
	cc $(LDFLAGS) -o hunt $(POBJ) pathname.o -lcurses -ltermlib

hunt.driver:	$(DOBJ) pathname.o
	cc $(PROFLAGS) $(LDFLAGS) -o hunt.driver $(DOBJ) pathname.o

#
# Object file dependencies
#
$(POBJ): $(HDR)

$(DOBJ): $(HDR)
	cc $(CFLAGS) $(PROFLAGS) -c $*.c

pathname.dbg.o: pathname.c
	@echo cc $(CFLAGS) -DDEBUG -c pathname.c -o pathname.dbg.o
	@rm -f x.c
	@ln pathname.c x.c
	@cc $(CFLAGS) -DDEBUG -c x.c
	@mv x.o pathname.dbg.o
	@rm -f x.c

#
# Miscellaneous functions
#

install.man:	FRC
	install -c -o bin -g bin -m 444 hunt.man ${DESTDIR}/usr/man/man6/hunt.6

newdomain:	FRC
	rm -f hunt.o extern.o driver.o

clean:	FRC
	rm -f .depend hunt hunt.driver *.o tags errs

.depend: ${DSRC} ${PSRC}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
