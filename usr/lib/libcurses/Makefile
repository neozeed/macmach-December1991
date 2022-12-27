# Makefile for libcurses.

CFLAGS=	-O -MD

SRCS=	addbytes.c addch.c addstr.c box.c clear.c clrtobot.c clrtoeol.c \
	cr_put.c cr_tty.c curses.c delch.c deleteln.c delwin.c endwin.c \
	erase.c fullname.c getch.c getstr.c idlok.c id_subwins.c initscr.c \
	insch.c insertln.c longname.c move.c mvprintw.c mvscanw.c mvwin.c \
	newwin.c overlay.c overwrite.c printw.c putchar.c refresh.c scanw.c \
	scroll.c toucholap.c standout.c touchwin.c tstp.c unctrl.c

OBJS=	addbytes.o addch.o addstr.o box.o clear.o clrtobot.o clrtoeol.o \
	cr_put.o cr_tty.o curses.o delch.o deleteln.o delwin.o endwin.o \
	erase.o fullname.o getch.o getstr.o idlok.o id_subwins.o initscr.o \
	insch.o insertln.o longname.o move.o mvprintw.o mvscanw.o mvwin.o \
	newwin.o overlay.o overwrite.o printw.o putchar.o refresh.o scanw.o \
	scroll.o toucholap.o standout.o touchwin.o tstp.o unctrl.o

all:	libcurses libcurses_p .depend

libcurses libcurses_p: ${OBJS}
	@echo building profiled libcurses
	@cd profiled; ar cu ../libcurses_p ${OBJS}
	ranlib libcurses_p
	@echo building normal libcurses
	@ar cu libcurses ${OBJS}
	ranlib libcurses

.c.o:
	@cc -c -pg ${CFLAGS} $*.c
	@ld -x -o profiled/$*.o -r $*.o
	cc ${CFLAGS} -c $*.c
	@ld -x -r $*.o
	@mv a.out $*.o

install: FRC
	install -c -o bin -g bin -m 644 libcurses ${DESTDIR}/usr/lib/libcurses.a
	ranlib ${DESTDIR}/usr/lib/libcurses.a
	install -c -o bin -g bin -m 644 libcurses_p ${DESTDIR}/usr/lib/libcurses_p.a
	ranlib ${DESTDIR}/usr/lib/libcurses_p.a

install.man: FRC
	@echo "*** need man pages for libcurses"

clean:	FRC
	rm -f .depend ${OBJS} profiled/*.o libcurses libcurses_p

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
