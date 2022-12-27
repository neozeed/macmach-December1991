# Makefile for learn.

DESTDIR=

# Flag BSD4_2 means 4.2 or later.
CFLAGS=	-O -MD -DBSD4_2

LLIB=	${DESTDIR}/usr/lib/learn/bin

SRCS=	copy.c dounit.c getlesson.c learn.c list.c mem.c makpipe.c maktee.c \
	mysys.c selsub.c selunit.c start.c whatnow.c wrapup.c

OBJS=	copy.o dounit.o getlesson.o learn.o list.o mem.o makpipe.o maktee.o \
	mysys.o selsub.o selunit.o start.o whatnow.o wrapup.o

all:	learn lrntee lcount getline.o getnum.o .depend

learn:	${OBJS}
	cc -o $@ ${OBJS}

lcount: lcount.c
	cc ${CFLAGS} -o $@ lcount.c

lrntee: lrntee.c
	cc ${CFLAGS} -o $@ lrntee.c

install: FRC
	-[ -d ${LLIB} ] || mkdir ${LLIB}
	chown bin.bin ${LLIB}
	chmod 555 ${LLIB}
	install -c -s -o bin -g bin -m 555 learn ${DESTDIR}/usr/bin/learn
	install -c -s -o bin -g bin -m 555 learn ${LLIB}/learn
	install -c -s -o bin -g bin -m 555 lrntee ${LLIB}/lrntee
	install -c -s -o bin -g bin -m 555 lcount ${LLIB}/lcount
	install -c -o bin -g bin -m 444 getline.o ${LLIB}/getline.o
	install -c -o bin -g bin -m 444 getnum.o ${LLIB}/getnum.o
# assume LLIB is ${DESTDIR}/usr/lib/learn/bin
	cd ${LLIB}; rm -f ../C/get*.o; \
	  ln -s ../bin/getline.o ../C/getline.o; \
	  ln -s ../bin/getnum.o ../C/getnum.o
	install -c -o bin -g bin -m 444 getline.c ${LLIB}/../C/getline.c
	install -c -o bin -g bin -m 444 getnum.c ${LLIB}/../C/getnum.c
	chmod 555 ${LLIB}/../*/Init

install.man: FRC
	install -c -o bin -g bin -m 444 learn.man ${DESTDIR}/usr/man/man1/learn.1

clean:	FRC
	rm -f .depend ${OBJS} learn lrntee lcount getline.o getnum.o

.depend: lcount.c lrntee.c getline.c getnum.c
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
