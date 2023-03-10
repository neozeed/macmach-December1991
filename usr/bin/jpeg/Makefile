# Makefile for Independent JPEG Group's software

# This makefile is suitable for Unix-like systems.

# See README and edit jconfig.h before saying "make" !!

# You may need to adjust these cc options:
CFLAGS= -O -DBSD  -Wall -DHAVE_GETOPT -DMEM_STATS
LDFLAGS=
# In particular:
#   Remove -g and -Wall if not using gcc.
#   Add -DBSD if on a pure BSD system (see jinclude.h).
#   Remove -DHAVE_GETOPT if you don't have getopt(3) (see jcmain.c, jdmain.c).
#   -DMEM_STATS is optional -- it enables gathering of memory usage statistics.
#   You may also want to add -DTWO_FILE_COMMANDLINE or -D switches for other
#   symbols listed in jconfig.h, if you prefer not to change jconfig.h.
# If your compiler is non-ANSI, also see the .c.o rule below.

# On HP-UX (and probably other SysV systems) the alternate malloc(3X) is a lot
# faster than the standard malloc; this makes a noticeable difference in the
# startup time when handling big noninterleaved images.  I say "-lmalloc" to
# get the alternate allocator.  On most non-SysV systems you can just
# define LDLIBS as empty.
#LDLIBS= -lmalloc

# miscellaneous OS-dependent stuff
LN= $(CC)	# linker
RM= rm -f	# file deletion command
AR= ar rc	# library (.a) file creation command
AR2= ranlib	# second step in .a creation (use "touch" if not needed)


# source files
INCLUDES= jinclude.h jconfig.h jpegdata.h 
SOURCES= jbsmooth.c jcarith.c jccolor.c jcdeflts.c jcexpand.c \
	 jchuff.c jcmain.c jcmaster.c jcmcu.c jcpipe.c jcsample.c \
	 jdarith.c jdcolor.c jdhuff.c jdmain.c jdmaster.c jdmcu.c \
	 jdpipe.c jdsample.c jerror.c jfwddct.c jquant1.c jquant2.c \
	 jrdjfif.c jrdgif.c jrdppm.c jrevdct.c jutils.c jvirtmem.c \
	 jwrjfif.c jwrgif.c jwrppm.c egetopt.c
DOCS= README architecture codingrules
MAKEFILES= makefile.unix makefile.amiga \
	   makefile.mc5 makefile.mc6 makcjpeg.lnk makdjpeg.lnk \
	   makefile.pwc makcjpeg.cf makdjpeg.cf makljpeg.cf
TESTFILES= testorig.jpg testimg.ppm testimg.jpg
DISTFILES= $(DOCS) $(MAKEFILES) ansi2knr.c $(SOURCES) $(INCLUDES) $(TESTFILES)

# compression objectfiles
COBJECTS = jcmain.o jcmaster.o jcdeflts.o jcarith.o jccolor.o jcexpand.o \
	   jchuff.o jcmcu.o jcpipe.o jcsample.o jfwddct.o \
	   jrdgif.o jrdppm.o jwrjfif.o \
	   jutils.o jvirtmem.o jerror.o
# decompression objectfiles
DOBJECTS = jdmain.o jdmaster.o jbsmooth.o jdarith.o jdcolor.o jdhuff.o \
	   jdmcu.o jdpipe.o jdsample.o jquant1.o jquant2.o jrevdct.o \
	   jrdjfif.o jwrgif.o jwrppm.o \
	   jutils.o jvirtmem.o jerror.o
# These objectfiles are included in libjpeg.a (all but jcmain.o, jdmain.o)
LIBOBJECTS = jcmaster.o jcdeflts.o jcarith.o jccolor.o jcexpand.o \
	     jchuff.o jcmcu.o jcpipe.o jcsample.o jfwddct.o \
	     jrdgif.o jrdppm.o jwrjfif.o \
	     jdmaster.o jbsmooth.o jdarith.o jdcolor.o jdhuff.o \
	     jdmcu.o jdpipe.o jdsample.o jquant1.o jquant2.o jrevdct.o \
	     jrdjfif.o jwrgif.o jwrppm.o \
	     jutils.o jvirtmem.o jerror.o


all: cjpeg djpeg
# By default, libjpeg.a is not built unless you explicitly request it.

install:
	install -s -o bin -g bin -m 555 cjpeg ${DESTDIR}/usr/bin/cjpeg
	install -s -o bin -g bin -m 555 djpeg ${DESTDIR}/usr/bin/djpeg

# If you have a C compiler that doesn't understand function prototypes,
# uncomment the 5 lines below and make sure PROTO is not defined by jconfig.h.
# Then say "make ansi2knr" before "make".

#.c.o:
#	./ansi2knr $*.c tmpansi.c
#	$(CC) $(CFLAGS) -c tmpansi.c
#	mv tmpansi.o $*.o
#	$(RM) tmpansi.c

ansi2knr: ansi2knr.c
	$(CC) -o ansi2knr ansi2knr.c
# You may need to add one of -DBSD, -DVMS, or -DMSDOS to the line above.


cjpeg: $(COBJECTS)
	$(LN) $(LDFLAGS) -o cjpeg $(COBJECTS) $(LDLIBS)

djpeg: $(DOBJECTS)
	$(LN) $(LDFLAGS) -o djpeg $(DOBJECTS) $(LDLIBS)

# libjpeg.a is useful if you are including the JPEG software in a larger
# program; you'd include it in your link, rather than the individual modules.
libjpeg.a: $(LIBOBJECTS)
	$(RM) libjpeg.a
	$(AR) libjpeg.a  $(LIBOBJECTS)
	$(AR2) libjpeg.a

clean:
	$(RM) *.o cjpeg djpeg libjpeg.a ansi2knr core tmpansi.* testout.ppm testout.jpg

distribute:
	$(RM) jpegsrc.tar*
	tar cvf jpegsrc.tar $(DISTFILES)
	ls -l jpegsrc.tar
	compress -v jpegsrc.tar
	ls -l jpegsrc.tar*

test: cjpeg djpeg
	$(RM) testout.ppm testout.jpg
	./djpeg testorig.jpg >testout.ppm
	./cjpeg testimg.ppm >testout.jpg
	cmp testimg.ppm testout.ppm
	cmp testimg.jpg testout.jpg


jbsmooth.o : jbsmooth.c jinclude.h jconfig.h jpegdata.h 
jcarith.o : jcarith.c jinclude.h jconfig.h jpegdata.h 
jccolor.o : jccolor.c jinclude.h jconfig.h jpegdata.h 
jcdeflts.o : jcdeflts.c jinclude.h jconfig.h jpegdata.h 
jcexpand.o : jcexpand.c jinclude.h jconfig.h jpegdata.h 
jchuff.o : jchuff.c jinclude.h jconfig.h jpegdata.h 
jcmain.o : jcmain.c jinclude.h jconfig.h jpegdata.h egetopt.c 
jcmaster.o : jcmaster.c jinclude.h jconfig.h jpegdata.h 
jcmcu.o : jcmcu.c jinclude.h jconfig.h jpegdata.h 
jcpipe.o : jcpipe.c jinclude.h jconfig.h jpegdata.h 
jcsample.o : jcsample.c jinclude.h jconfig.h jpegdata.h 
jdarith.o : jdarith.c jinclude.h jconfig.h jpegdata.h 
jdcolor.o : jdcolor.c jinclude.h jconfig.h jpegdata.h 
jdhuff.o : jdhuff.c jinclude.h jconfig.h jpegdata.h 
jdmain.o : jdmain.c jinclude.h jconfig.h jpegdata.h egetopt.c
jdmaster.o : jdmaster.c jinclude.h jconfig.h jpegdata.h 
jdmcu.o : jdmcu.c jinclude.h jconfig.h jpegdata.h 
jdpipe.o : jdpipe.c jinclude.h jconfig.h jpegdata.h 
jdsample.o : jdsample.c jinclude.h jconfig.h jpegdata.h 
jerror.o : jerror.c jinclude.h jconfig.h jpegdata.h 
jfwddct.o : jfwddct.c jinclude.h jconfig.h jpegdata.h 
jquant1.o : jquant1.c jinclude.h jconfig.h jpegdata.h 
jquant2.o : jquant2.c jinclude.h jconfig.h jpegdata.h 
jrdjfif.o : jrdjfif.c jinclude.h jconfig.h jpegdata.h 
jrdgif.o : jrdgif.c jinclude.h jconfig.h jpegdata.h 
jrdppm.o : jrdppm.c jinclude.h jconfig.h jpegdata.h 
jrevdct.o : jrevdct.c jinclude.h jconfig.h jpegdata.h 
jutils.o : jutils.c jinclude.h jconfig.h jpegdata.h 
jvirtmem.o : jvirtmem.c jinclude.h jconfig.h jpegdata.h 
jwrjfif.o : jwrjfif.c jinclude.h jconfig.h jpegdata.h 
jwrgif.o : jwrgif.c jinclude.h jconfig.h jpegdata.h 
jwrppm.o : jwrppm.c jinclude.h jconfig.h jpegdata.h 

