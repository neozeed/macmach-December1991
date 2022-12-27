# Makefile for ieee libm.

CFLAGS=	-O -MD

SRCS=	cabs.c cbrt.c support.c

OBJS=	cabs.o cbrt.o support.o

.c.o:
	cc -p ${CFLAGS} -c $*.c
	-ld -X -r $*.o
	mv a.out ../profiled/$*.o
	cc ${CFLAGS} -c $*.c
	-ld -x -r $*.o
	mv a.out $*.o

all:	../libm.a ../libm_p.a .depend

../libm.a ../libm_p.a: ${OBJS}
	cd ../profiled; ar cru ../libm_p.a ${OBJS}
	ar cru ../libm.a ${OBJS}

clean:	FRC
	rm -f .depend ${OBJS} a.out

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
