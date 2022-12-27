# Makefile for common libm stuff.

CFLAGS=	-O -MD

SRCS=	atan2.c sincos.c tan.c

OBJS=	atan2.o sincos.o tan.o

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

tags:	FRC
	ctags ${SRCS}

clean:	FRC
	rm -f .depend ${OBJS} a.out

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
