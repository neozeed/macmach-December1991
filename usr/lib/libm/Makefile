# Makefile for libm.

# This high quality math library is intended to run on either a VAX in
# D_floating format or a machine that conforms to the IEEE standard 754
# for double precision floating-point arithmetic.

# WARNING: On machines other than the ones mentioned above, run the original
# Version 7 math library, if nothing better is available.

# MACHINE indicates the type of floating point hardware you are using; legal
# values are:

# vax		- for the VAX D_floating format, the default.
# tahoe		- for the TAHOE double format.
# national	- for those IEEE machines whose floating point implementation
#		  has similar byte ordering as the NATIONAL 32016 with 32081.
# ieee		- for other IEEE machines, we hope.

CFLAGS=	-O -MD -D${MACHINE}

SUBDIR=	common ieee

# Files comprising the standard Math library;
# actually there are more under ${MACHINE}/ subdirectory.

SRCS=	acosh.c asincos.c asinh.c atan.c atanh.c cosh.c erf.c \
	exp.c exp__E.c expm1.c floor.c lgamma.c j0.c j1.c jn.c \
	log.c log10.c log1p.c log__L.c pow.c sinh.c tanh.c

OBJS=	acosh.o asincos.o asinh.o atan.o atanh.o cosh.o erf.o \
	exp.o exp__E.o expm1.o floor.o lgamma.o j0.o j1.o jn.o \
	log.o log10.o log1p.o log__L.o pow.o sinh.o tanh.o 

all:	libm.a libm_p.a .depend

libm.a libm_p.a: ${OBJS} more.${MACHINE}
	cd profiled; ar cru ../libm_p.a ${OBJS}
	ar cru libm.a ${OBJS}

more.ieee: FRC
	for i in common ieee; do (cd $$i; make ${MFLAGS}); done

more.national: FRC
	for i in common national; do (cd $$i; make ${MFLAGS}); done

more.tahoe: FRC
	for i in common tahoe; do (cd $$i; make ${MFLAGS}); done

more.vax: FRC
	cd vax; make "CFLAGS=${CFLAGS}"

more.mac2: more.ieee

.c.o:
####	generate additional code for profiling (-p)
	cc -p ${CFLAGS} -c $*.c
####	generate relocation bits (-r) & preserve symbols that begin with L (-X)
	-ld -X -r $*.o
	mv a.out profiled/$*.o
	cc ${CFLAGS} -c $*.c
####	generate relocation bits (-r) but don't preserve local symbols (-x)
	-ld -x -r $*.o
	mv a.out $*.o

install: FRC
	install -c -o bin -g bin -m 444 libm.a ${DESTDIR}/usr/lib/libm.a
	ranlib ${DESTDIR}/usr/lib/libm.a
	install -c -o bin -g bin -m 444 libm_p.a ${DESTDIR}/usr/lib/libm_p.a
	ranlib ${DESTDIR}/usr/lib/libm_p.a

install.man:
	install -c -o bin -g bin -m 444 math.man ${DESTDIR}/usr/man/man3/math.3

clean:	FRC
	rm -f .depend ${OBJS} profiled/*.o core libm.a libm_p.a
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

tags:	FRC
	ctags ${SRCS}
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} tags); done

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
