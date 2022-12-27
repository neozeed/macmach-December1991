# Makefile for libc compat-cmu.

DEFS=

CFLAGS=	-O -MD ${DEFS}

SRCS=	abspath.c atoh.c atoo.c atot.c boolarg.c bsearch.c chrarg.c ci.c \
	concat.c del.c dfork.c doublearg.c editor.c errmsg.c expand.c \
	fdate.c ffilecopy.c fgetpass.c filecopy.c floatarg.c fold.c \
	fwantread.c fwantwrite.c getbool.c getchr.c getdouble.c getfloat.c \
	gethex.c gethostattr.c getint.c getlong.c getname.c getoct.c \
	getpwwho.c getsearch.c getshort.c getstab.c getstr.c gtime.c \
	hexarg.c intarg.c longarg.c makepath.c movefile.c nxtarg.c octarg.c \
	openp.c path.c pathof.c prstab.c quit.c run.c salloc.c \
	searcharg.c searchp.c setpath.c shortarg.c sindex.c skipto.c \
	srchscore.c stabarg.c stablk.c stabsearch.c stlmatch.c strarg.c \
	vmnlist.c vprintf.c wantread.c wantwrite.c

OBJS=	abspath.o atoh.o atoo.o atot.o boolarg.o bsearch.o chrarg.o ci.o \
	concat.o del.o dfork.o doublearg.o editor.o errmsg.o expand.o \
	fdate.o ffilecopy.o fgetpass.o filecopy.o floatarg.o fold.o \
	fwantread.o fwantwrite.o getbool.o getchr.o getdouble.o getfloat.o \
	gethex.o gethostattr.o getint.o getlong.o getname.o getoct.o \
	getpwwho.o getsearch.o getshort.o getstab.o getstr.o gtime.o \
	hexarg.o intarg.o longarg.o makepath.o movefile.o nxtarg.o octarg.o \
	openp.o parsedate.o path.o pathof.o prstab.o quit.o run.o salloc.o \
	searcharg.o searchp.o setpath.o shortarg.o sindex.o skipto.o \
	srchscore.o stabarg.o stablk.o stabsearch.o stlmatch.o strarg.o \
	vmnlist.o wantread.o wantwrite.o

TAGSFILE=tags

all:	${OBJS} .depend

link:	all
	(cd ../library; rm -f ${OBJS})
	(cd ../profiled; rm -f ${OBJS})
	cp ${OBJS} ../library
	(cd profiled; cp ${OBJS} ../../profiled)

.c.o:
	@cc -p ${CFLAGS} -c $*.c
	@-ld -X -o profiled/$*.o -r $*.o
	cc ${CFLAGS} -c $*.c
	@-ld -x -r $*.o
	@mv a.out $*.o

parsedate.o: parsedate.y parsedate.scr
	yacc parsedate.y
	ed - y.tab.c < parsedate.scr
	mv y.tab.c $*.c
	cc -p ${CFLAGS} -c $*.c
	-ld -X -r $*.o
	mv a.out profiled/$*.o
	cc ${CFLAGS} -c $*.c
	-ld -x -r $*.o
	mv a.out $*.o
	rm -f y.tab.o y.tab.c $*.c

install.man: FRC
	@echo "*** need man pages for libc"

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE} parsedate.c

tags:	FRC
	cwd=`pwd`; \
	for i in ${SRCS}; do ctags -a -f ${TAGSFILE} $$cwd/$$i; done

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
