#
# Last changed: 08/20/91, v1.1
#
# Makefile for the minimal build for an archie Prospero client.

# Your C compiler:
CC=cc

# For most systems, these OPTIONS will suffice.  Exceptions:
#
#	* If you're on a Stardent, add -43
#	* If you're running EP/IX, you may need to add -systype bsd43 ..try
#	  it without it first, though.

OPTIONS= -O -I. # -43    -systype bsd43

# For this, DEFINES is usually empty.  Exceptions:
#
#     * if you're on a system running A/UX, add		      -DAUX
#     * if you're running AIX (e.g. RS/6000), add	      -DAIX
#     * if your system doesn't have the functions index(),
#       rindex(), bcopy(), or bzero(), add                    -DFUNCS
#     * if your system doesn't have the re_comp or re_exec
#       routines (no regex(3) library), then add              -DNOREGEX
#     * if your system is lacking strspn(), add               -DSTRSPN
#     * if you want to include the debugging code (so you
#       can help with problem-solving if any crop up), add    -DDEBUG

DEFINES= -DDEBUG

#	Usually LDFLAGS is empty; if, after you build this, archie
#	 complains that it can't resolve ARCHIE.MCGILL.CA, you need
#	 to uncomment `-lresolv'.

LDFLAGS= # -lresolv

# Change this if necessary.

RM=/bin/rm

# =========================
# Yer' done....make archie.
# =========================
#
CFLAGS=$(OPTIONS) $(DEFINES)
#
OBJS=	aquery.o archie.o atalloc.o dirsend.o get_pauth.o get_vdir.o \
	p_err_text.o procquery.o ptalloc.o stcopy.o support.o \
	vlalloc.o vl_comp.o
#
all: archie

archie: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): archie.h pmachine.h pcompat.h Makefile

install:
	install -s -o bin -g bin -m 555 archie ${DESTDIR}/usr/bin/archie

install.man:
	install -c -o bin -g bin -m 444 archie.man \
	  ${DESTDIR}/usr/man/man1/archie.1

clean:
	$(RM) -f *.o archie gmon.out

gclean: clean
	$(RM) -f *~ archie.0* archie.tar*

#unx:
#	-unifdef -UXARCHIE xprocquery.c > procquery.c

FILES=	INSTALL Bugs Makefile Prospero README archie.c archie.h \
	archie.man aquery.c atalloc.c dirsend.c get_pauth.c \
	get_vdir.c p_err_text.c patchlevel.h pauthent.h pcompat.h perrno.h \
	pfs.h pmachine.h pprot.h procquery.c ptalloc.c rdgram.h regex.c \
	stcopy.c support.c uw-copyright.h vl_comp.c vlalloc.c

shar:
	makekit -narchie. $(FILES) > Manifest

tar:
	( cd .. ; tar cvf archie/archie.tar `echo $(FILES) | tr ' ' '\012' | sed -e "s/^/archie\//g"` )
