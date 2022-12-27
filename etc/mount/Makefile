# Makefile for mount.

PROG1=	mount
PROG2=	umount

CFGS=	fstab

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

CFGDIR=	${DESTDIR}/etc/newconfig

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFGDIR= ${DESTDIR}/etc/newconfig

CFLAGS=	-O

IFLAGS=	-o root -g bin -m 544

SRCS=	${PROG1}.c ${PROG2}.c

OBJS1=	${PROG1}.o

OBJS2=	${PROG2}.o

all:	${PROG1} ${PROG2} .depend

${PROG1}: ${OBJS1}
	cc -o $@ ${OBJS1}

${PROG2}: ${OBJS2}
	cc -o $@ ${OBJS2}

install:
	install -c -s ${IFLAGS} ${PROG1} ${BINDIR}/${PROG1}
	install -c -s ${IFLAGS} ${PROG2} ${BINDIR}/${PROG2}
	install -c -o bin -g bin -m 444 ${CFGS} ${CFGDIR}

install.man:
	install -c -o bin -g bin -m 444 ${PROG1}.man \
	  ${MANDIR}/${PROG1}.${MANEXT}
	rm -f  ${MANDIR}/${PROG2}.${MANEXT}
	ln -s ${PROG1}.${MANEXT} ${MANDIR}/${PROG2}.${MANEXT}
	install -c -o bin -g bin -m 444 fstab.man \
	  ${DESTDIR}/usr/man/man5/fstab.5

clean:
	rm -f .depend ${OBJS1} ${OBJS2} ${PROG1} ${PROG2}

.depend: ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend
