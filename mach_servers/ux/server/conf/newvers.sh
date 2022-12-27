#!/bin/sh -
#
# Mach Operating System
# Copyright (c) 1989 Carnegie-Mellon University
# All rights reserved.  The CMU software License Agreement specifies
# the terms and conditions for use and redistribution.
#
#
# HISTORY
# $Log:	newvers.sh,v $
# Revision 2.3  90/09/14  13:11:00  rwd
# 	Remove major.minor from version printout.
# 	[90/09/14            rwd]
# 
# Revision 2.2  89/11/29  09:40:16  af
# 	Dropped Bersekeley version id, use Mach's one.
# 
# 
# Revision 2.1  89/08/04  15:29:17  rwd
# Created.
# 
# Revision 2.7  89/02/25  17:41:51  gm0w
# 	Changes for cleanup.
# 
# Revision 2.6  88/10/18  03:17:50  mwyoung
# 	Removed the original form of this script that was left in comments.
# 	Removed old history.
# 	[88/10/15            mwyoung]
# 
# Revision 2.5  88/10/11  10:08:21  rpd
# 	Changed version_variant & version_patch to open arrays.
# 	Fixed the definition of CONFIG to use the last component of the
# 	working directory without cutting off at underscores.
# 	[88/10/04  06:55:37  rpd]
# 
# Revision 2.4  88/07/20  16:23:53  rpd
# Made version string open-sized.
# 
# 15-Nov-86  Mike Accetta (mja) at Carnegie-Mellon University
#	Removed increment of version number which is now done by the
#	Makefile and which depends only on changes to the source files
#	and not the build directory.
#

#
# Copyright (c) 1980, 1986 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	@(#)newvers.sh	7.1 (Berkeley) 6/5/86
#
#
edit="$5"; major="$2"; minor="$3"; variant="$4"; patch="$6"; copyright="$1"
v="VERSION(${variant}${edit}${patch})" d=`pwd` h=`hostname` t=`date`
if [ -z "$d" -o -z "$h" -o -z "$t" ]; then
    exit 1
fi
CONFIG=`expr "$d" : '.*/\([^/]*\)$'`
d=`expr "$d" : '.*/\([^/]*/[^/]*\)$'`
(
  /bin/echo "int  version_major      = ${major};" ;
  /bin/echo "int  version_minor      = ${minor};" ;
  /bin/echo "char version_variant[]  = \"${variant}\";" ;
  /bin/echo "char version_patch[]    = \"${patch}\";" ;
  /bin/echo "int  version_edit       = ${edit};" ;
  /bin/echo "char version[] = \"Mach 3.0 ${v}: ${t}; $d ($h)\\n\";" ;
  /bin/echo "char cmu_copyright[] = \"\\" ;
  sed <$copyright -e '/^#/d' -e 's;[ 	]*$;;' -e '/^$/d' -e 's;$;\\n\\;' ;
  /bin/echo "\";";
) > vers.c
if [ -s vers.suffix -o ! -f vers.suffix ]; then
    echo ".${variant}${edit}${patch}.${CONFIG}" >vers.suffix
fi
exit 0
