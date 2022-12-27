# /usr/src/usr.bin/emacs/Makefile for MacMach

# make all	to compile and build Emacs
# make install	to install it
# make install.sysv  to install on system V.
# make install.xenix  to install on Xenix
# make tags	to update tags tables
#
# make distclean	to delete everything that wasn't in the distribution
#	This is a very dangerous thing to do!
# make CLEAN
#       This is a little less dangerous.

SHELL = /bin/sh

# Where to install things
# Note that on system V you must change MANDIR to /use/local/man/man1.
LIBDIR= ${DESTDIR}/usr/lib/emacs
BINDIR= ${DESTDIR}/usr/bin
MANDIR= ${DESTDIR}/usr/man/man1
INFODIR= ${DESTDIR}/usr/man
LOCKDIR= ${DESTDIR}/usr/spool/emacs

# Flags passed down to subdirectory makefiles.
MFLAGS=

# Subdirectories to make recursively.  `lisp' is not included
# because the compiled lisp files are part of the distribution
# and you cannot remake them without installing Emacs first.
SUBDIR= etc src

# Subdirectories to install
COPYDIR= etc lisp

# Subdirectories to clean
CLEANDIR= ${COPYDIR} lisp/term info

all:	src/paths.h ${SUBDIR}

src/paths.h: Makefile src/paths.h-dist
	/bin/sed 's;/usr/local/emacs;${LIBDIR};g' < src/paths.h-dist > src/paths.h

src:	etc

.RECURSIVE: ${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS} all

# assumes INFODIR=/usr/man
install: mkdir lockdir
	-if [ `/bin/pwd` != `(cd ${LIBDIR}; /bin/pwd)` ] ; then \
		tar chf - ${COPYDIR} | (cd ${LIBDIR}; umask 0; tar xBf - ) ;\
		tar chf - info | (cd ${INFODIR}; umask 0; tar xBf - ) ;\
		find ${LIBDIR} ${INFODIR}/info -type d -exec chmod 555 {} \; ;\
		find ${LIBDIR} ${INFODIR}/info -type f -exec chmod ugo+r-w {} \; ;\
		chown -R bin.bin ${LIBDIR} ${INFODIR}/info ;\
		rm -rf ${LIBDIR}/info ;\
		ln -s ../../man/info ${LIBDIR}/info ;\
		for i in ${CLEANDIR}; do \
			(rm -rf ${LIBDIR}/$$i/RCS; \
			 rm -f ${LIBDIR}/$$i/\#*; \
			 rm -f ${LIBDIR}/$$i/*~); \
		done \
	else true; \
	fi
	install -c -s -m 755 etc/emacsclient ${BINDIR}/emacsclient
	install -c -s -m 755 etc/etags ${BINDIR}/etags
	install -c -s -m 755 etc/ctags ${BINDIR}/ctags
	install -c -s -m 1755 src/xemacs ${BINDIR}/xemacs
	install -c -m 444 etc/emacs.1 ${MANDIR}/emacs.1
	-rm -f ${BINDIR}/emacs
	mv ${BINDIR}/xemacs ${BINDIR}/emacs

install.man:

install.sysv: all mkdir lockdir
	-if [ `/bin/pwd` != `(cd ${LIBDIR}; /bin/pwd)` ] ; then \
		find ${COPYDIR} -print | cpio -pdum ${LIBDIR} ;\
		for i in ${CLEANDIR}; do \
			(rm -rf ${LIBDIR}/$$i/RCS; \
			 rm -f ${LIBDIR}/$$i/\#*; \
			 rm -f ${LIBDIR}/$$i/*~); \
		done \
	else true; \
	fi
	-cpset etc/emacsclient ${BINDIR}/emacsclient 755 bin bin
	-cpset etc/etags ${BINDIR}/etags 755 bin bin
	-cpset etc/ctags ${BINDIR}/ctags 755 bin bin
	-cpset etc/emacs.1 ${MANDIR}/emacs.1 444 bin bin
	-/bin/rm -f ${BINDIR}/emacs
	-cpset src/xemacs ${BINDIR}/emacs 1755 bin bin

install.xenix: all mkdir lockdir
	if [ `pwd` != `(cd ${LIBDIR}; pwd)` ] ; then \
		tar cf - ${COPYDIR} | (cd ${LIBDIR}; umask 0; tar xpf - ) ;\
		for i in ${CLEANDIR}; do \
			(rm -rf ${LIBDIR}/$$i/RCS; \
			 rm -f ${LIBDIR}/$$i/\#*; \
			 rm -f ${LIBDIR}/$$i/*~); \
		done \
	else true; \
	fi
	cp etc/etags etc/ctags etc/emacsclient ${BINDIR}
	chmod 755 ${BINDIR}/etags ${BINDIR}/ctags ${BINDIR}/emacsclient
	cp etc/emacs.1 ${MANDIR}/emacs.1
	chmod 444 ${MANDIR}/emacs.1
	-mv -f ${BINDIR}/emacs ${BINDIR}/emacs.old
	cp src/xemacs ${BINDIR}/emacs
	chmod 1755 ${BINDIR}/emacs
	-rm -f ${BINDIR}/emacs.old

mkdir: FRC
	-[ -d ${LIBDIR} ] || mkdir ${LIBDIR}
	-chmod 755 ${LIBDIR}

distclean: FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} distclean); done

clean:	distclean
	cd src; make clean; ln -s config.h-$(MACHINE) config.h

CLEAN:
	cd src; make clean
	if [ `pwd` != `(cd ${LIBDIR}; pwd)` ] ; then \
		cd etc; make clean; \
	else true; \
	fi

lockdir: FRC
# assumes LOCKDIR=/usr/spool/emacs
	-[ -d ${LOCKDIR} ] || mkdir ${LOCKDIR}
	-chmod 777 ${LOCKDIR}
	-rm -f ${LIBDIR}/lock
	ln -s ../../spool/emacs ${LIBDIR}/lock

# the following additions are for MacMach Makefile compatability

FRC:

tags:	etc
	cd src; ../etc/etags *.[ch] ../lisp/*.el ../lisp/term/*.el
