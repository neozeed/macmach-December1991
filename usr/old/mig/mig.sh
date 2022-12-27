#!/bin/sh
# Mach Operating System
# Copyright (c) 1989 Carnegie-Mellon University
# All rights reserved.  The CMU software License Agreement specifies
# the terms and conditions for use and redistribution.
#
# HISTORY
# $Log:	mig.sh,v $
# Revision 1.2  90/02/18  20:17:44  bww
# 	Purged absolute pathnames.
# 	[90/02/18  20:17:10  bww]
# 
# Revision 1.1  90/02/18  20:12:43  bww
# 	Mach Release 2.5
# 	[90/02/18  20:12:32  bww]
# 
# Revision 1.4  89/05/20  22:35:20  mrt
# 	Cleanup for Mach 2.5
# 
#
# 27-May-87  Richard Draves (rpd) at Carnegie-Mellon University
#	Created.
#

CPP=`wh -Lq cpp`
MIGCOM=`wh -Lq old.migcom`

cppflags=
migflags=
files=

until [ $# -eq 0 ]
do
    case $1 in
	-[qQvVtTrRsSi] ) migflags="$migflags $1"; shift;;
	-user   ) migflags="$migflags $1 $2"; shift; shift;;
	-server ) migflags="$migflags $1 $2"; shift; shift;;
	-header ) migflags="$migflags $1 $2"; shift; shift;;

	-MD ) sawMD=1; cppflags="$cppflags $1"; shift;;
	-* ) cppflags="$cppflags $1"; shift;;
	* ) files="$files $1"; shift;;
    esac
done

for file in $files
do
    base="`basename "$file" .defs`"
    $CPP $cppflags "$file" - ${sawMD+"$base".d~} | $MIGCOM $migflags || exit
    if [ $sawMD ]
    then
	sed 's/^'"$base"'.o/'"$base"'.h '"$base"'User.c '"$base"'Server.c/' \
		< "$base".d~ > "$base".d
	rm -f "$base".d~
    fi
done

exit 0
