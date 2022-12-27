# Makefile for mac2 libg.

all:	libg.a

libg.a: dbxxx.s
	as dbxxx.s -o $@

install:
	install -c -o bin -g bin -m 444 libg.a ${DESTDIR}/usr/lib/libg.a

install.man:
	@echo "*** need man page for libg"

clean:
	rm -f .depend ${OBJS} libg.a
