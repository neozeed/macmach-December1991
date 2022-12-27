# Makefile for libthreads, Mach 3.0 version.

CFLAGS=	-O -MD -DMACH_IPC_COMPAT=0

LINKS= thread.c lock.S csw.S

SRCS=	call.c cprocs.c cthreads.c malloc.c mig_support.c stack.c sync.c \
	threadtest.c ${LINKS}

OBJS=	cprocs.o cthreads.o malloc.o \
	mig_support.o stack.o sync.o \
	thread.o lock.o csw.o

all:	libthreads.a .depend

libthreads.a: links ${OBJS}
	ar cu libthreads.a ${OBJS}
	ranlib libthreads.a

links:	FRC
	for i in ${LINKS}; do \
	  rm -f $$i; \
	  ln -s ${target_machine}/$$i $$i; \
	done

.c.o:
	cc -S ${CFLAGS} -c $*.c
	@awk -f ${target_machine}/cthread_inline.awk $*.s >$*.S
	@cc -c $*.S
	@rm $*.s $*.S

threadtest: threadtest.c libthreads.a
	cc -O -c threadtest.c
	ld -o threadtest \
	  /lib/crt0.o \
	  threadtest.o \
	  ./libthreads.a \
	  /usr/lib/libmach.a \
	  /lib/libc.a
	rm threadtest.o

install: FRC
	install -c -o bin -g bin -m 644 libthreads.a ${DESTDIR}/usr/lib/libthreads.a
	ranlib ${DESTDIR}/usr/lib/libthreads.a

install.man: FRC

clean:	FRC
	rm -f .depend *.s *.S ${LINKS} ${OBJS} libthreads.a threadtest

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
