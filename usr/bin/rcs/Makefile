# Makefile for rcs.

# Copyright (C) 1982 by Walter F. Tichy
#                       Purdue University
#                       Computer Science Department
#                       West Lafayette, IN 47907
#
# All rights reserved. No part of this software may be sold or distributed
# in any form or by any means without the prior written permission of the
# author.
# Report problems and direct all inquiries to
# tichy@purdue, decvax!purdue!tichy
#
#               INSTRUCTIONS
#               ============
# 0.  If you are porting to different hardware,
#     check the instructions in rcsbase.h
#
# Note: instructions 1-5 are normally taken care of by the calling make,
# which passes in DIFF, DIFF3, RCSDIR, OLDDIR, OS, and LOCKING.
#
# 1. Early version of diff did not support options needed by RCS.
#    Newer versions of diff, support them and diff is no longer distributed with
#    RCS. The following definitions apply only if you have both old versions
#    and new versions of diff, and you want RCS to use the new versions.
#    The default should be:
#DIFF      = /bin/diff
#DIFF3     = /usr/lib/diff3

#    Figure out where to put the modified diff and diff3 commands, and
#     install them. (Read the instructions in the Make-files).
#     Define the macros DIFF and DIFF3 accordingly (below).
DESTDIR   =
DIFF      = /bin/diff
DIFF3     = /usr/lib/diff3

# 2.  Figure out where to put the RCS commands; define RCSDIR accordingly.
RCSDIR    = /usr/bin
MERGE     = $(RCSDIR)/merge

# 3.  Figure out where to put your old RCS commands; define OLDDIR
#     accordingly. This is not needed if you don't have an old release of RCS.
OLDDIR    = /usr/old

# 4.  Set the operating system variable OS. If you run on 4.1 BSD, do not
#     define OS:
#     OS  =
#     If you run on USG Unix or UN*X, use:
#     OS  = -DUSG
#     If you run on Version 6 Unix, use:
#     OS  = -DV6
#     If you run on 4.2 or 4.3 BSD, use:
#     OS = -DV4_2BSD
OS        = -DV4_2BSD

# 5.  Decide whether you want strict locking as the default or not.
#     If you want strict locking, use:
#     LOCKING = 1
#     otherwise:
#     LOCKING = 0
LOCKING   = 1

# 6.  Decide what loader flags you need, and define LDFLAGS accordingly.
#     On the Vax you need none, on PDP-11s with split i/d you need -i.
#LDFLAGS  = -i
LDFLAGS   =

# 7.  RCS can maintain a log of the use of the co and ci commands. If you
#     want the logging, you must
#     a) define SNOOP (that's where the process that writes the log goes),
#     b) define SNOOPFILE (that's the file where SNOOP writes the log;
#        it must be owned by the same user that owns SNOOP),
#     c) include SNOOP and SNOOPFILE in the CFLAGS macro.
#     If you don't want the logging of command usage, omit the SNOOP and
#     SNOOPFILE definitions from CFLAGS.
#     The two variants of the CFLAG macros are given below.
#SNOOPFILE = /usr/log/rcslog/log
#SNOOP	= /usr/local/bin/snoop

DEFINES= $(OS) -DSTRICT_LOCKING=$(LOCKING) -DDIFF='"${DESTDIR}$(DIFF)"' -DMERGE='"${DESTDIR}$(MERGE)"' -DTARGETDIR='"${DESTDIR}$(RCSDIR)"'
CFLAGS	= -O $(DEFINES)
LINT = lint -c -u  -Dlint $(DEFINES)

# 8.  If you don't have release 2 RCS files around (e.g., if you don't
#     have release 2 of RCS), remove the word -DCOMPAT2 from whatever
#     variant of CFLAGS you selected. -DCOMPAT2 assures that release 2
#     RCS files can still be read. (After all release 2 RCS files have
#     been updated with release 3 or 4 ci or rcs, you can remake RCS without
#     -DCOMPAT2.) Also, make sure that the co commands at the end of this
#     file are commented out; otherwise, Make will complain.
#
# 9.  Now you are ready. Try to make 'all'. If all went well, make
#     'install' if you want no logging of command usage, 'installog' otherwise.
#
# 10. IMPORTANT! When you have RCS installed, rename old
#     release 2 RCS files as follows (if you have any):
#     If the working file was "f.c" and the RCS file "f.c.v", rename the
#     RCS file to "f.c,v". If the working file was "f.c" and the RCS file
#     "f.v", rename the RCS file "f.c,v". Thus, suffixes are no longer
#     dropped and RCS files end in ",v" rather than ".v".
#
# 11. If you want to maintain RCS with itself, be sure you preserve the
#     original revision numbers, dates, etc. This is done by checking the
#     files in with the -k option. When done, remove the comments from the
#     co-rules at the end of this file.
#
BCOMMANDS   =   ci ident rcs rcsdiff rcsmerge rlog co sccstorcs
# binary comands

RCSCOMMANDS = merge $(BCOMMANDS)
# all commands

SOURCES = ci.c co.c ident.c maketime.c partime.c rcs.c rcsdiff.c rcsedit.c\
	rcsfcmp.c rcsfnms.c rcsgen.c rcskeep.c rcskeys.c rcslex.c rcsmerge.c\
	rcsrev.c rcssyn.c rcsutil.c rlog.c snoop.c\
	rcsbase.h time.h merge.sh sccstorcs.c

BOBJECTS = ci.o co.o ident.o rcs.o rcsdiff.o rcsedit.o\
	rcsfcmp.o rcsfnms.o rcsgen.o rcskeep.o rcskeys.o rcslex.o rcsmerge.o\
	rcsrev.o rcssyn.o rcsutil.o rlog.o snoop.o\
	rcssynTST.o rcsrevTST.o rcsfnmsTST.o rcsfcmpTST.o rcskeepTST.o \
	sccstorcs.o

all:	$(RCSCOMMANDS) .depend

INSTALL = install

$(BOBJECTS):	rcsbase.h

partime.o maketime.o co.o ci.o rcs.o rlog.o rcsutil.o:        time.h

install: FRC
#	Save the old commands if you have any.
#	-cd ${DESTDIR}$(RCSDIR); mv $(RCSCOMMANDS) ${DESTDIR}$(OLDDIR)
	${INSTALL} -c -m 775 -o bin -g bin -s ci ${DESTDIR}$(RCSDIR)
	${INSTALL} -c -m 775 -o bin -g bin -s ident ${DESTDIR}$(RCSDIR)
	${INSTALL} -c -m 775 -o bin -g bin -s rcs ${DESTDIR}$(RCSDIR)
	${INSTALL} -c -m 775 -o bin -g bin -s rcsdiff ${DESTDIR}$(RCSDIR)
	${INSTALL} -c -m 775 -o bin -g bin -s rcsmerge ${DESTDIR}$(RCSDIR)
	${INSTALL} -c -m 775 -o bin -g bin -s rlog ${DESTDIR}$(RCSDIR)
	${INSTALL} -c -m 775 -o bin -g bin -s co ${DESTDIR}$(RCSDIR)
	${INSTALL} -c -m 775 -o bin -g bin merge ${DESTDIR}$(RCSDIR)
#	${INSTALL} -c -m 775 -o bin -g bin rcsclean ${DESTDIR}$(RCSDIR)
#	${INSTALL} -c -m 775 -o bin -g bin rcsfreeze.sh ${DESTDIR}${RCSDIR}/rcsfreeze
	${INSTALL} -c -m 775 -o bin -g bin sccstorcs ${DESTDIR}$(RCSDIR)

install.man: FRC
	@echo "*** need man pages for rcs"

installog:	installsnoop install

installsnoop:	snoop
	install -c -s snoop ${DESTDIR}$(SNOOP)
	chmod u+s ${DESTDIR}$(SNOOP)
	touch ${DESTDIR}$(SNOOPFILE)
	chmod 644 ${DESTDIR}$(SNOOPFILE)
# CAUTION: You may have to do a chown of SNOOP and SNOOPFILE (if not owned by root).
lint: ci.lint co.lint ident.lint rlog.lint rcs.lint rcsdiff.lint rcsmerge.lint snoop.lint

clean:	FRC
	-rm -f .depend *.o; rm -f $(RCSCOMMANDS) snoop

CIFILES = ci.o rcslex.o rcssyn.o rcsgen.o rcsedit.o rcskeys.o rcsrev.o rcsutil.o rcsfnms.o partime.o maketime.o rcskeep.o rcsfcmp.o

CISRC = ci.c rcslex.c rcssyn.c rcsgen.c rcsedit.c rcskeys.c rcsrev.c rcsutil.c rcsfnms.c partime.c maketime.c rcskeep.c rcsfcmp.c

ci.lint:
	$(LINT) $(CISRC)
	
ci:	$(CIFILES)
	cc $(LDFLAGS) -o ci $(CIFILES)

COFILES = co.o rcslex.o rcssyn.o rcsgen.o rcsedit.o rcskeys.o rcsrev.o rcsutil.o rcsfnms.o partime.o maketime.o

COSRC = co.c rcslex.c rcssyn.c rcsgen.c rcsedit.c rcskeys.c rcsrev.c rcsutil.c rcsfnms.c partime.c maketime.c
co.lint:
	$(LINT) $(COSRC)
co:	$(COFILES)
	cc $(LDFLAGS) -o co $(COFILES)

ident:	ident.o rcskeys.o
	cc -o ident ident.o rcskeys.o

ident.lint:
	$(LINT) ident.c rcskeys.c

merge:	merge.sh
	sed -e '/^#/d' -e 's:DIFF3=.*$$:DIFF3=$(DIFF3):' merge.sh > merge
#	This takes out the comment lines and substitutes in DIFF3.
#	(Comments are not permitted in some older shells.)
	chmod 755 merge

RLOG = rlog.o rcslex.o rcssyn.o rcsrev.o rcsutil.o partime.o maketime.o rcsfnms.o
RLOGSRC = rlog.c rcslex.c rcssyn.c rcsrev.c rcsutil.c partime.c maketime.c rcsfnms.c
rlog:	$(RLOG)
	cc $(LDFLAGS) -o rlog $(RLOG)

rlog.lint:
	$(LINT) $(RLOGSRC)

RCS = rcs.o rcslex.o rcssyn.o rcsrev.o rcsutil.o rcsgen.o rcsedit.o rcskeys.o rcsfnms.o

RCSSRC = rcs.c rcslex.c rcssyn.c rcsrev.c rcsutil.c rcsgen.c rcsedit.c rcskeys.c rcsfnms.c
rcs:	$(RCS)
	cc $(LDFLAGS) -o rcs $(RCS)

rcs.lint:
	$(LINT) $(RCSSRC)

rcsclean:	rcsclean.sh
	sed -e '/^#/d' rcsclean.sh > rcsclean
#	This takes out the comments (not permitted in older shells).
	chmod 755 rcsclean

RCSDIFF = rcsdiff.o rcsutil.o rcsfnms.o rcsrev.o rcssyn.o rcslex.o
RCSDIFFSRC = rcsdiff.c rcsutil.c rcsfnms.c rcsrev.c rcssyn.c rcslex.c

rcsdiff:	$(RCSDIFF)
	cc -o rcsdiff $(RCSDIFF)

rcsdiff.lint:
	$(LINT) $(RCSDIFFSRC)
RCSMERGE = rcsmerge.o rcsutil.o rcsfnms.o rcsrev.o rcssyn.o rcslex.o
RCSMERGESRC = rcsmerge.c rcsutil.c rcsfnms.c rcsrev.c rcssyn.c rcslex.c
rcsmerge:	$(RCSMERGE)
	cc -o rcsmerge $(RCSMERGE)
rcsmerge.lint:
	$(LINT) $(RCSMERGESRC)
	
snoop:	snoop.o
	cc -o snoop snoop.o
snoop.lint:
	$(LINT) snoop.c

SCCSTORCS= sccstorcs.o
SCCSTORCSSRC = sccstorcs.c

sccstorcs:	$(SCCSTORCS)
	cc -o $@ $(SCCSTORCS)

sccstorcs.lint:
	$(LINT) $(SCCSTORCSSRC)

SOURCE=	ci.c co.c curdir.c ident.c maketime.c partime.c rcs.c \
	rcsclean.c rcsdiff.c rcsedit.c rcsfcmp.c rcsfnms.c rcsgen.c \
	rcskeep.c rcskeys.c rcslex.c rcsmerge.c rcsrev.c rcssyn.c rcsutil.c \
	rlog.c snoop.c sccstorcs.c

HFILES=	rcsbase.h time.h

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
