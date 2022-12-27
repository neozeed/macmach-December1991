# Makefile for moria.

DESTDIR=

# LIBDIR is where the other files (scoreboard, news, hours) will be put
# LIBDIR must be the same directory defined in config.h

LIBDIR=	${DESTDIR}/usr/games/lib/moria

IFLAGS=	-o bin -g games -m 500

CFLAGS=	-O -MD -DKEY_BINDINGS=ROGUE_LIKE

SRCS=	main.c misc1.c misc2.c store1.c files.c io.c create.c desc.c\
	generate.c sets.c dungeon.c creature.c death.c eat.c help.c magic.c\
	potions.c prayer.c save.c staffs.c wands.c scrolls.c spells.c\
	wizard.c store2.c signals.c moria1.c moria2.c monsters.c\
	treasure1.c treasure2.c variables.c

OBJS=	main.o misc1.o misc2.o store1.o files.o io.o create.o desc.o\
	generate.o sets.o dungeon.o creature.o death.o eat.o help.o magic.o\
	potions.o prayer.o save.o staffs.o wands.o scrolls.o spells.o\
	wizard.o store2.o signals.o moria1.o moria2.o monsters.o\
  	treasure1.o treasure2.o variables.o

LIBS=	 -lm -lcurses -ltermcap

all:	moria .depend

moria: ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

install:
	install -c -s -o games -g bin -m 500 moria ${DESTDIR}/usr/games/hide
	cd ${DESTDIR}/usr/games; rm -f moria; ln -s dm moria
	-[ -d ${LIBDIR} ] || mkdir ${LIBDIR}
	chown games.bin ${LIBDIR}; chmod 700 ${LIBDIR}
	install -c -o games -g bin -m 444 Moria_hours ${DESTDIR}/etc/newconfig
	install -c -o games -g bin -m 444 Moria_news ${DESTDIR}/etc/newconfig
	install -c -o games -g bin -m 444 /dev/null \
	  ${DESTDIR}/etc/newconfig/Moria_scores

install.man:
	@echo "*** need man page for moria"

clean:
	rm -f .depend ${OBJS} ${PROG}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend
