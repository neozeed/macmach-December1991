# Makefile for libc compat-sys5.

# Many of these routines have been rewritten in assembly.  The assembly
# source can be found in the directory stdio.${MACHINE}.  The macro
# ${STDSRC} lists the files needed by all current implementations.  The
# macros ${VAXSRC} and ${TAHOESRC} are the supplemental files needed by
# the VAX and the TAHOE.  ${PORTSRC} is a list of all of the supplemental
# files.

DEFS=

CFLAGS=	-O -MD ${DEFS}

STDSRC=	getopt.c memccpy.c memchr.c memcmp.c memcpy.c memset.c strcspn.c \
	strpbrk.c strspn.c strtok.c tmpnam.c strcatn.c strcmpn.c strcpyn.c

STDOBJ=	getopt.o memccpy.o memchr.o memcmp.o memcpy.o memset.o strcspn.o \
	strpbrk.o strspn.o strtok.o tmpnam.o strcatn.o strcmpn.o strcpyn.o

MAC2SRC=${PORTSRC}

MAC2OBJ=${PORTOBJ}

VAXSRC=

VAXOBJ=

TAHOESRC=strchr.c strrchr.c

TAHOEOBJ=strchr.o strrchr.o

PORTSRC=strchr.c strrchr.c

PORTOBJ=strchr.o strrchr.o

TAGSFILE=tags

all:	lib.${MACHINE} .depend

link:	all link.${MACHINE}

.c.o:
	@cc -p ${CFLAGS} -c $*.c
	@-ld -X -o profiled/$*.o -r $*.o
	cc ${CFLAGS} -c $*.c
	@-ld -x -r $*.o
	@mv a.out $*.o

link.mac2: FRC
	(cd ../library; rm -f ${STDOBJ} ${MAC2OBJ})
	(cd ../profiled; rm -f ${STDOBJ} ${MAC2OBJ})
	cp ${STDOBJ} ${MAC2OBJ} ../library
	(cd profiled; cp ${STDOBJ} ${MAC2OBJ} ../../profiled)

link.vax: FRC
	(cd ../library; rm -f ${STDOBJ} ${VAXOBJ})
	(cd ../profiled; rm -f ${STDOBJ} ${VAXOBJ})
	cp ${STDOBJ} ${VAXOBJ} ../library
	(cd profiled; cp ${STDOBJ} ${VAXOBJ} ../../profiled)

link.tahoe: FRC
	(cd ../library; rm -f ${STDOBJ} ${TAHOEOBJ})
	(cd ../profiled; rm -f ${STDOBJ} ${TAHOEOBJ})
	cp ${STDOBJ} ${TAHOEOBJ} ../library
	(cd profiled; cp ${STDOBJ} ${TAHOEOBJ} ../../profiled)

lib.mac2: ${STDOBJ} ${MAC2OBJ}

lib.vax: ${STDOBJ} ${VAXOBJ}

lib.tahoe: ${STDOBJ} ${TAHOEOBJ}

install.man: FRC
	@echo "*** need man pages for libc"

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags: tags.${MACHINE}

tags.mac2: FRC
	cwd=`pwd`; \
	for i in ${STDSRC} ${MAC2SRC}; do \
	  ctags -a -f ${TAGSFILE} $$cwd/$$i; \
	done

tags.vax: FRC
	cwd=`pwd`; \
	for i in ${STDSRC} ${VAXSRC}; do \
	  ctags -a -f ${TAGSFILE} $$cwd/$$i; \
	done

tags.tahoe: FRC
	cwd=`pwd`; \
	for i in ${STDSRC} ${TAHOESRC}; do \
	  ctags -a -f ${TAGSFILE} $$cwd/$$i; \
	done

.depend: depend.${MACHINE}

depend.mac2: ${STDSRC} ${MAC2SRC}
	md -d -f -u .depend *.d 2>/dev/null

depend.vax: ${STDSRC} ${VAXSRC}
	md -d -f -u .depend *.d 2>/dev/null

depend.tahoe: ${STDSRC} ${TAHOESRC}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
