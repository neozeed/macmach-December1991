# Makefile for phantasia.

# Makefile for Phantasia 3.3.2
#
# To create game:
#
# 1)  Set up a directory where the game and its support files will live.
#        (It is suggested that the source be kept somewhere else.)
#
# 2)  Set up the variables in Makefile CAREFULLY to reflect your
#        particular situation.
#
# 3)  Check out ok_to_play() at the end of main.c if you want to include
#        code to restrict access at certain times.
#
# 4)  'make install' and sit back and watch.
#
# NOTE:  'setup' MUST be executed before trying to run 'phantasia'.
#	 The install procedure does this for you.
#
#
# To convert from 3.3.1 or 3.3.1+ to 3.3.2:
#
# 1)	'make convert'  (3.3.1+: 'make PLUS=-DPHANTPLUS convert')
#
# 2)	Move 'newcharacs' to 'characs'.
#

RM =	-rm
SHAR =	/d2/estes/bin/shar
# Tell 'make' to use /bin/sh for all commands.
SHELL =	/bin/sh

# PLOTDEVICE is used for plotting the map.
# Change as appropriate.
PLOTDEVICE =	> /dev/tty

# Add '-ljobs' to LDFLAGS if 4.1bsd; add -ltermlib if not SVR2.
# Need separate i/d space on small machines.
# May need '-f' on machines without floating point hardware.
LIBS= -lm -lcurses -ltermlib

# DEST is where the program and its support files reside
DEST =	/usr/games/lib/phantasia
DESTR =	/usr/games

MDIR=	/usr/man/man6

.SUFFIXES: .6 .0

# The following are program constants which are implementation dependent.
#
# DEST is passed on.  Note that there is no closing quote here.
# WIZARD is the login of the one who will clean up things.
# UID is the uid of game wizard.
# define OK_TO_PLAY to restrict playing access at certain times.
#	Also see function ok_to_play() in main.c, and tailor to your own needs.
# define ENEMY to include code for checking of a 'hit list' of resricted 
#	accounts.  The list of logins goes in the file 'enemy'.
# SHELL is the default shell for shell escapes.  If SHELL is not defined,
#	shell escapes are not enabled.
# define BSD41 for 4.1bsd
# define BSD42 for 4.2bsd
# define SYS3 for System III, or similar (like Version 7)
# define SYS5 for System V
# If you don't have 'strchr()', put '-Dstrchr=index' below.
# May need '-I/usr/include/sys' for 4.2bsd.
CFLAGS = -O -traditional \
	-DDEST=\"${DEST} \
	-DDESTR=\"${DESTR} \
	-DWIZARD=\"root\" \
	-DUID=0 \
	-DENEMY \
	-DBSD42

SRCS=	main.c fight.c io.c interplayer.c gamesupport.c misc.c phantglobs.c
OBJS=	main.o fight.o io.o interplayer.o gamesupport.o misc.o phantglobs.o
HFILES =	phantdefs.h phantstruct.h macros.h include.h
LIBC=	/lib/libc.a

all:	phantasia setup phantasia.0 .depend

# Make the game.
phantasia: ${OBJS} ${LIBC}
	${CC} ${OBJS} -o $@ ${LIBS}

${OBJS}: ${HFILES}

# Make the initialization program.
setup:	phantglobs.o setup.o monsters.asc ${LIBC}
	${CC} phantglobs.o setup.o -o $@ ${LIBS}

setup.o: ${HFILES}

# Make documentation.
# The flags below on tbl and nroff are to make a line printable version.
.6.0:
	tbl -TX phantasia.6 | /usr/man/manroff > phantasia.0

# Install game and documentation.  Initialize.
install: FRC
	-[ -d ${DESTDIR}/usr/games/lib/phantasia ] || mkdir ${DESTDIR}/usr/games/lib/phantasia
	chmod 700 ${DESTDIR}/usr/games/lib/phantasia
	chown games.bin ${DESTDIR}/usr/games/lib/phantasia
	install -c -s -o games -g bin -m 500 phantasia ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f phantasia; ln -s dm phantasia
	install -c -o games -g bin -m 400 phantasia.0 ${DESTDIR}/usr/games/lib/phantasia/phant.help
	./setup
	chown games.bin ${DESTDIR}/usr/games/lib/phantasia/*

install.man: FRC
	tbl -TX phantasia.6 >${DESTDIR}${MDIR}/phantasia.6

# Make Phantasia map.
# Change the map commands reflect your installation.
# Pre-System III installations may use 'plot' instead of 'tplot'.
map:	map.c
	cc -O map.c -lplot -o map
	map | tplot -t4014 ${PLOTDEVICE}

# Convert old character file to new format.
# 'convert' is meant to be run only once.
# PLUS = PHANTPLUS for conversion from 3.3.1+.
convert: convert.c oldplayer.h phantstruct.h
	${CC} ${CFLAGS} ${PLUS} convert.c ${LDFLAGS} -o convert
	convert
	@echo "New data is in newcharacs."

# Clean up directory after installation.
clean:	FRC
	${RM} -f .depend *.o core phantasia phant.help phantasia.0 setup

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
