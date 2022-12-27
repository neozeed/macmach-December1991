# /usr/src/usr.bin/sup/Makefile for MacMach

# NOTE: doprnt.c and vsprintf.c are used because the MacMach libc does not
# do things the way that sup expects...

CFLAGS=	-O -MD -I/usr/include/cmu -UCMU
LIBC=	/lib/libc.a
SRCS=	supcmain.c supcvers.c supcparse.c supcname.c supcmisc.c supcmeat.c \
	scm.c scmio.c stree.c log.c supmsg.c supscan.c stree.c scan.c \
	doprnt.c vsprintf.c
SUP_OBJS= supcmain.o supcvers.o supcparse.o supcname.o supcmisc.o supcmeat.o \
	scm.o scmio.o stree.o log.o supmsg.o doprnt.o vsprintf.o
SUPFILESRV_OBJS= supfilesrv.o scan.o scm.o scmio.o stree.o log.o supmsg.o \
	doprnt.o vsprintf.o
SUPSCAN_OBJS= supscan.o stree.o scan.o
OBJS=	${SUP_OBJS} ${SUPFILESRV_OBJS} ${SUPSCAN_OBJS}

all:	sup supfilesrv supscan .depend

sup:	${SUP_OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${SUP_OBJS}

supfilesrv: ${SUPFILESRV_OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${SUPFILESRV_OBJS}

supscan: ${SUPSCAN_OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${SUPSCAN_OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 sup ${DESTDIR}/usr/bin
	install -c -s -o bin -g bin -m 755 supscan ${DESTDIR}/usr/bin
	rm -f ${DESTDIR}/etc/supfilesrv ${DESTDIR}/usr/etc/supfilesrv
	install -c -s -o bin -g bin -m 755 supfilesrv ${DESTDIR}/usr/etc
	ln -s ../usr/etc/supfilesrv ${DESTDIR}/etc/supfilesrv

install.man: FRC
	install -c -o bin -g bin -m 444 supservers.man ${DESTDIR}/usr/man/man8/supservers.8
	rm -f ${DESTDIR}/usr/man/man8/supfilesrv.8
	ln -s supservers.8 ${DESTDIR}/usr/man/man8/supfilesrv.8

clean:	FRC
	rm -f .depend ${OBJS} core sup supfilesrv supscan

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
