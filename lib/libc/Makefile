# Makefile for libc.

# Compatibility routines are kept in directories with a prefixing
# ``compat'' (so they all sort together).

# The C run-time startup code is always machine dependent and expected
# to be located in ${MACHINE}/csu

# There are two possible methods of doing host name look up.  They are
# to use the name server or /etc/hosts. HOSTLOOKUP defines which method
# is to be compiled into libc:
#   defining HOSTLOOKUP to be "named", compiles the         
#   host lookup routines that use the BIND name server.
#   defining HOSTLOOKUP to be "hosttable", compiles
#   the host lookup routines that use /etc/hosts.
#HOSTLOOKUP=	hosttable
HOSTLOOKUP=	named

# The system has two monitoring schemes, utilizing either ``prof''
# or ``gprof''. DFLMON determines the default profiler; mon.o
# specifies that ``prof'' is the default, gmon.o specifies that
# ``gprof'' is the default.
#DFLMON= gmon.o
DFLMON=	mon.o

# All library object contain sccsid strings by default; they may be
# excluded as a space-saving measure.  To produce a library that does
# not contain these strings, remove -DLIBC_SCCS and -DSYSLIBC_SCCS
# from DEFS below.  To remove these strings from just the system call
# stubs, remove -DSYSLIBC_SCCS from DEFS below.
DEFS=

ONE=	compat-4.1 compat-4.3 compat-sys5 compat-cmu gen inet net ns stdio

TWO=	compat-4.1/compat-4.1.${MACHINE} gen/gen.${MACHINE} \
	net/net.${MACHINE} net/${HOSTLOOKUP} stdio/stdio.${MACHINE} \
	compat-sys5/compat-sys5.${MACHINE} ${MACHINE}/csu ${MACHINE}/sys

SUBDIR=	${ONE} ${TWO}

TAGSFILE=tags

all:	libc.a libc_p.a

libc.a libc_p.a:
	-[ -d library ] || mkdir library
	-[ -d profiled ] || mkdir profiled
	for i in ${SUBDIR}; do ( \
	  cd $$i; make DEFS="${DEFS}" link; \
	); done
	ls library/*.o | sort -t/ +1 > t1
	ar cr libc.a `lorder \`cat t1\` | tsort` ${MACHINE}/csu/${DFLMON}
	ar ma findiop.o libc.a exit.o
	ar ma exit.o libc.a fakcu.o
	ls profiled/*.o | sort -t/ +1 > t1
	ar cr libc_p.a `lorder \`cat t1\` | tsort` ${MACHINE}/csu/${DFLMON}
	ar ma findiop.o libc_p.a exit.o
	ar ma exit.o libc_p.a fakcu.o
	rm -f t1

install: FRC
	install -c -o bin -g bin -m 644 libc.a ${DESTDIR}/lib/libc.a
	ranlib ${DESTDIR}/lib/libc.a
	install -c -o bin -g bin -m 644 libc_p.a ${DESTDIR}/usr/lib/libc_p.a
	ranlib ${DESTDIR}/usr/lib/libc_p.a
	cd ${MACHINE}/csu; make DESTDIR=${DESTDIR} install

install.man: FRC
	for i in ${SUBDIR}; \
	  do (cd $$i; make DESTDIR=${DESTDIR} install.man); done
	cd man; make DESTDIR=${DESTDIR} install.man

clean:	FRC
	for i in ${SUBDIR} net/hosttable net/named; \
	  do (cd $$i; make ${MFLAGS} clean); done
	rm -rf libc.a libc_p.a library profiled

tags:	FRC
	for i in ${ONE}; do \
	  (cd $$i; make ${MFLAGS} TAGSFILE=../${TAGSFILE} tags); \
	done
	for i in ${TWO}; do \
	  (cd $$i; make ${MFLAGS} TAGSFILE=../../${TAGSFILE} tags); \
	done
	sort -o ${TAGSFILE} ${TAGSFILE}

FRC:
