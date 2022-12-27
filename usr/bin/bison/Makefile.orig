# Makefile for bison
#   Copyright (C) 1984, 1989 Bob Corbett and Free Software Foundation, Inc.
# 
# This file is part of Bison, the GNU Compiler Compiler.
# 
# Bison is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 1, or (at your option)
# any later version.
# 
# Bison is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Bison; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

INSTALL = install -c

# It is unwise ever to compile a program without symbols.
CFLAGS = -g

# For sysV, you must uncomment the following lines
#CFLAGS = -g -DUSG
# If you are compiling with gcc on sysV, then omit the definition of LIBS.
#LIBS = -lPW
#INSTALL = cp

DESTDIR=
# where the installed binary goes
BINDIR = $(DESTDIR)/usr/local/bin

# where the parsers go
PARSERDIR = $(DESTDIR)/usr/local/lib

# where manual pages go and what their extensions should be
MANDIR = $(DESTDIR)/usr/man/manl
MANEXT = l

# names of parser files
PFILE = bison.simple
PFILE1 = bison.hairy

PFILES = -DXPFILE=\"$(PARSERDIR)/$(PFILE)\" \
	 -DXPFILE1=\"$(PARSERDIR)/$(PFILE1)\"

OBJECTS = LR0.o allocate.o closure.o conflicts.o derives.o files.o	\
	  getargs.o gram.o lalr.o lex.o					\
	  main.o nullable.o output.o print.o reader.o reduce.o symtab.o	\
	  warshall.o version.o						\
	  getopt.o getopt1.o

start: bison

clean:
	rm -f *.o core bison

install: bison
	$(INSTALL) bison $(BINDIR)
	-cd $(PARSERDIR); rm -f $(PFILE) $(PFILE1)
# Copy bison.simple, inserting directory name into the #line commands.
	awk -f mergedir.awk dir=$(PARSERDIR) $(PFILE) > $(PARSERDIR)/$(PFILE)
	cp $(PFILE1) $(PARSERDIR)
	$(INSTALL) bison.1 $(MANDIR)/bison.$(MANEXT)
	chmod a+r $(PARSERDIR)/$(PFILE) $(PARSERDIR)/$(PFILE1) 
	chmod a+r $(MANDIR)/bison.$(MANEXT)

bison: $(OBJECTS)
	$(CC) $(LDFLAGS) -o bison $(OBJECTS) $(LIBS)

dist:
	echo bison-`sed -e '/version_string/!d' -e 's/[^0-9.]*\([0-9.]*\).*/\1/' -e q < version.c` > .fname
	-rm -rf `cat .fname`
	mkdir `cat .fname`
	ln  COPYING ChangeLog Makefile REFERENCES bison.1 \
	    bison.simple bison.hairy mergedir.awk \
	    LR0.c allocate.c closure.c conflicts.c derives.c \
	    files.c getargs.c gram.c lalr.c lex.c main.c nullable.c \
	    output.c print.c reader.c reduce.c symtab.c version.c \
	    warshall.c files.h gram.h lex.h machine.h new.h state.h \
	    symtab.h system.h types.h bison.cld build.com vmsgetargs.c \
	    vmshlp.mar README bison.texinfo bison.info* \
	  `cat .fname`
	cp -p ../lib/getopt.[ch] ../lib/getopt1.c `cat .fname`
	tar chZf `cat .fname`.tar.Z `cat .fname`
	-rm -rf `cat .fname` .fname
.PHONY: dist

TAGS: *.c *.h
	etags *.c *.h

# This file is different to pass the parser file names
# to the compiler.
files.o: files.c files.h new.h gram.h
	$(CC) -c $(CFLAGS) $(PFILES) files.c

LR0.o: machine.h new.h gram.h state.h
closure.o: machine.h new.h gram.h
conflicts.o: machine.h new.h files.h gram.h state.h
derives.o: new.h types.h gram.h
getargs.o: files.h
lalr.o: machine.h types.h state.h new.h gram.h
lex.o: files.h symtab.h lex.h
main.o: machine.h
nullable.o: types.h gram.h new.h
output.o: machine.h new.h files.h gram.h state.h
print.o: machine.h new.h files.h gram.h state.h
reader.o: files.h new.h symtab.h lex.h gram.h
reduce.o: files.h new.h machine.h gram.h
symtab.o: new.h symtab.h gram.h
warshall.o: machine.h
