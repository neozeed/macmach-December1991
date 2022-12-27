# /usr/src/ucb/vlp/vlp.vax/Makefile for MacMach

# Makefile for VLP

# files required:
#	vlp.l	- lisp program to read source and add troff control
#		characters.
#	vlpmacs - troff macros used by vlp.
#
# vlp and vlpmacs are put in DESTDIR
# If DESTDIR is changed, then the source (vlp.l) must be modified since
# it explicitly points to vlpmacs.

DESTDIR =
.DEFAULT:vlp

vlp:	vlp.l
	/usr/ucb/liszt -qar -o vlp vlp.l

install: vlp vlpmacs
	install -c vlp ${DESTDIR}/usr/ucb
	install -c -m 664 vlpmacs ${DESTDIR}/usr/lib

clean:	FRC
	rm -f .depend vlp

FRC:
