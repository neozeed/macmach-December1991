# Makefile for Mach 2.5 mig, installs into /usr/old.

# make              -- build mig and dependency file
# make mig          -- build mig
# make install      -- build and install mig
# make install.man  -- install mig manual pages
# make clean        -- remove everything that can be built from source

# program name
PROG= mig

# install destination root (default is /)
DESTDIR=

# manual page extension
MANEXT=old

# actual install directory for program
BINDIR=${DESTDIR}/usr/old
LIBDIR=${DESTDIR}/usr/lib

# actual install directory for manual page
MANDIR=${DESTDIR}/usr/man/man.${MANEXT}

# install flags for program
IFLAGS=	-o bin -g bin -m 555

# CC flags
CFLAGS= -O -traditional

# source files
CSRCS= 	parser.c lexxer.c mig.c error.c string.c type.c routine.c \
	statement.c global.c header.c user.c server.c utils.c

# generated source files
CGENS=	parser.c lexxer.c
HGENS=	parser.h

# object files
OBJS= 	parser.o lexxer.o mig.o error.o string.o type.o routine.o \
	statement.o global.o header.o user.o server.o utils.o

# libraries to load
LIBS=	-ll

# default action: build program and dependency file
all: migcom .depend

# build program from objects
migcom:	${CGENS} ${OBJS}
	cc -o $@ ${OBJS} ${LIBS}

parser.c parser.h: parser.y
	yacc -d parser.y && mv y.tab.c parser.c && mv y.tab.h parser.h

lexxer.c: lexxer.l
	lex lexxer.l &&	mv lex.yy.c lexxer.c

# build and install program
install:
	install -c ${IFLAGS} ${PROG}.sh ${BINDIR}/${PROG}
	rm -f ${BINDIR}/old.${PROG}; ln -s ${PROG} ${BINDIR}/old.${PROG}
	install -c -s ${IFLAGS} migcom ${LIBDIR}/old.migcom

# install manual pages for program
install.man: ${PROG}.man
	install -c -o bin -g bin -m 444 ${PROG}.man ${MANDIR}/${PROG}.${MANEXT}

# remove program, objects and dependency file
clean:	${CSRCS}
	rm -f .depend ${OBJS} migcom ${CGENS} ${HGENS}

# create dependency file
.depend: ${CSRCS} ${CGENS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${CSRCS} ${CGENS}

# include dependency file if one exists
-include .depend
