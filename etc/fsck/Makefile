# Makefile for fsck.

PROG=	fsck

DESTDIR=

MANEXT=	8

BINDIR=	${DESTDIR}/etc

MANDIR= ${DESTDIR}/usr/man/man${MANEXT}

CFLAGS=	-O -MD

IFLAGS=	-o root -g bin -m 544

# Note that ufs_subr.c and ufs_tables.c are in kernel sources.
# do a relative path search to find Mach 2.5 or Mach 3.0 sources.
UFS_SUBR_MACH2=../../vmunix/sys/bsd/ufs_subr.c
UFS_TABLES_MACH2=../../vmunix/sys/bsd/ufs_tables.c
UFS_SUBR_MACH3=../../mach_servers/ux/server/bsd/ufs_subr.c
UFS_TABLES_MACH3=../../mach_servers/ux/server/bsd/ufs_tables.c

SRCS=	dir.c inode.c main.c pass1.c pass1b.c pass2.c pass3.c \
	pass4.c pass5.c setup.c utilities.c

CGENS=	ufs_subr.c ufs_tables.c

OBJS=	dir.o inode.o main.o pass1.o pass1b.o pass2.o pass3.o pass4.o \
	pass5.o setup.o utilities.o ufs_subr.o ufs_tables.o

all:	${PROG} .depend

${PROG}: ${CGENS} ${OBJS}
	cc -o $@ ${OBJS}

ufs_subr.c:
	-[ -f ${UFS_SUBR_MACH2} ] && ln -s ${UFS_SUBR_MACH2} ufs_subr.c
	-[ -f ${UFS_SUBR_MACH3} ] && ln -s ${UFS_SUBR_MACH3} ufs_subr.c

ufs_tables.c:
	-[ -f ${UFS_TABLES_MACH2} ] && ln -s ${UFS_TABLES_MACH2} ufs_tables.c
	-[ -f ${UFS_TABLES_MACH3} ] && ln -s ${UFS_TABLES_MACH3} ufs_tables.c

install:
	install -c -s ${IFLAGS} ${PROG} ${BINDIR}/${PROG}

install.man:
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

clean:
	rm -f .depend ${OBJS} ${PROG} ${CGENS}

.depend: ${SRCS} ${CGENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend
