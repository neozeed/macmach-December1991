# 
# Mach Operating System
# Copyright (c) 1991 Carnegie Mellon University
# All Rights Reserved.
# 
# Permission to use, copy, modify and distribute this software and its
# documentation is hereby granted, provided that both the copyright
# notice and this permission notice appear in all copies of the
# software, derivative works or modified versions, and any portions
# thereof, and that both notices appear in supporting documentation.
# 
# CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
# CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
# ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
# 
# Carnegie Mellon requests users of this software to return to
# 
#  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
#  School of Computer Science
#  Carnegie Mellon University
#  Pittsburgh PA 15213-3890
# 
# any improvements or extensions that they make and grant Carnegie the
# rights to redistribute these changes.
# 
# HISTORY
# $Log:	Makefile,v $
# Revision 2.3  91/07/06  18:30:03  jsb
# 	Added rules for man pages.
# 
# Revision 2.2  91/07/06  14:55:54  jsb
# 	First checkin.
# 

CDEFS		= -I.
MIGFLAGS	= -MD

VPATH=.:$(INCDIR):$(INCDIR)/servers

NETMEMORYOBJS	= netmemory.o netmemory_server.o

all	: netmemoryserver

install : $(BINDIR)/netmemoryserver $(MAN1DIR)/netmemoryserver.1 \
	$(MAN3DIR)/netmemory.3

netmemoryserver : $(NETMEMORYOBJS) $(LIBXMM)
	cc $(CFLAGS) -o netmemoryserver $(NETMEMORYOBJS) \
		-lxmm -lthreads -lmach

netmemory_server.c netmemory_user.c : netmemory.defs
	mig $(MIGFLAGS) netmemory.defs \
		-server netmemory_server.c \
		-user netmemory_user.c

$(BINDIR)/netmemoryserver: netmemoryserver
	cp netmemoryserver $(BINDIR)/netmemoryserver
	strip $(BINDIR)/netmemoryserver

$(MAN1DIR)/netmemoryserver.1: netmemoryserver.1
	cp netmemoryserver.1 $(MAN1DIR)/netmemoryserver.1

$(MAN3DIR)/netmemory.3: netmemory.3
	cp netmemory.3 $(MAN3DIR)/netmemory.3

include $(MAKETOP)Makefile-common

-include Makedep

.EXIT:
	md -u Makedep -d *.d
