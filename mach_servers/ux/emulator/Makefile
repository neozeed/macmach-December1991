#
# Mach Operating System
# Copyright (c) 1990 Carnegie-Mellon University
# Copyright (c) 1989 Carnegie-Mellon University
# Copyright (c) 1988 Carnegie-Mellon University
# Copyright (c) 1987 Carnegie-Mellon University
# Copyright (c) 1986 Carnegie-Mellon University
# All rights reserved.  The CMU software License Agreement specifies
# the terms and conditions for use and redistribution.
#  
# HISTORY
# $Log:	Makefile,v $
# Revision 2.12  91/09/03  11:11:22  jsb
# 	Added support for cross-compilation, borrowing from server and kernel
# 	KCC technology.	KCC is used to build images to be run on target
# 	machine (machine for which emulator is being built), and CC is used to
# 	build images to be run on host machine (machine upon which compilation
# 	is done). Use KCC (not CC) to build bsd_types_gen. Use CC (not KCC) to
# 	build bsd_1_user.o. Provide TARGET_MACHINE defn for emulator_base.
# 	Provide appropriate definitions for i860.
# 	[91/09/03  10:00:10  jsb]
# 
# Revision 2.11  90/11/05  15:30:56  rpd
# 	Change MACHINE to TARGET_MACHINE.
# 	[90/10/30            rwd]
# 
# Revision 2.10  90/10/25  15:06:12  rwd
# 	Add I386 (same as AT386)
# 	[90/10/24            rwd]
# 
# Revision 2.9  90/06/19  23:06:23  rpd
# 	Added -x to LDFLAGS for architectures not using xstrip.
# 	[90/06/03            rpd]
# 
# Revision 2.8  90/06/02  15:20:07  rpd
# 	Run xstrip on the emulator.
# 	[90/05/11            rpd]
# 	Process mach.defs with -DEMULATOR.
# 	[90/04/08            rpd]
# 	Added rules to run mig directly.
# 	Compile with -DMACH_IPC_COMPAT=0.
# 	[90/03/26  19:24:57  rpd]
# 
# 	Invoke emulator_base with ./emulator_base.
# 	[90/02/27  17:28:20  rpd]
# 
# Revision 2.7  90/05/29  20:22:20  rwd
# 	Added -DMAP_FILE to COPTS.
# 	[90/03/26            rwd]
# 
# Revision 2.6  90/05/21  13:45:12  dbg
# 	i386 loader doesn't look down LPATH (?!)
# 	[90/04/20            dbg]
# 
# Revision 2.5  90/03/14  21:22:30  rwd
# 	Add emul_mapped.c and define MAP_UAREA.
# 	[90/01/23            rwd]
# 
# Revision 2.4  89/11/29  15:24:39  af
# 	Mods to account for the Mips compiler specifics.
# 	[89/11/16  15:18:28  af]
# 
# Revision 2.3  89/10/17  11:23:48  rwd
# 	Added Makedep include and make
# 	[89/10/06            rwd]
# 
# Revision 2.2.1.1  89/09/21  20:35:20  dbg
# 	Remove genassym.c.
# 	[89/09/21            dbg]
# 
# Revision 2.2  89/08/09  14:35:11  rwd
# 	clean should remove *.[ch] also.
# 	[89/08/07            rwd]
# 
#
# Makefile for bsd emulator.
#
I860KCC= cc860
I860CPP= /lib/cpp
I860AS= as860
I860LD= ld860.em

MAC2CPP= cc -E

COPTS= -MD
KCC = $($(TARGET_MACHINE)KCC?$($(TARGET_MACHINE)KCC):cc)
CPP = $($(TARGET_MACHINE)CPP?$($(TARGET_MACHINE)CPP):cc -ES)
AS = $($(TARGET_MACHINE)AS?$($(TARGET_MACHINE)AS):as)
LD = $($(TARGET_MACHINE)LD?$($(TARGET_MACHINE)LD):ld)
CDEBUGFLAGS = $($(TARGET_MACHINE)CDEBUGFLAGS?$($(TARGET_MACHINE)CDEBUGFLAGS):-O)
DEFINES= -DTypeCheck=0 -DMACH_IPC_COMPAT=0 -DMAP_UAREA -DMAP_FILE
CFLAGS=	$(CDEBUGFLAGS) ${DEFINES} ${COPTS}
ASFLAGS= ${${TARGET_MACHINE}ASFLAGS}
LDFLAGS= $(${TARGET_MACHINE}LDFLAGS?${${TARGET_MACHINE}LDFLAGS}:-x -e start)

PMAXCDEBUGFLAGS = -O2 -g3
PMAXASFLAGS= -nocpp
PMAXLDFLAGS= -e __start
AT386LDFLAGS= -x -e __start -L${MKLIB}
I386LDFLAGS= -x -e __start -L${MKLIB}
MAC2LDFLAGS= -x -e start -L${MKLIB}
I860LDFLAGS=	/usr/i860/lib/ieee_div.o \
		/usr/i860/lib/misc.o \
		/usr/i860/lib/libc.a \
		/usr/i860/lib/crt0.o \
		-L${MKLIB}

# emul_vector.o must be first so that jmp to EFAULT handler is first
OBJS=	emul_vector.o \
	bsd_user_side.o emul_init.o emul_stack_alloc.o emul_generic.o \
	allocator.o mach_user.o syscall_table.o \
	emul_machdep.o bsd_1_user.o emul_cache.o \
	emul_mapped.o

SRCS=	bsd_user_side.c emul_init.c emul_stack_alloc.c emul_generic.c \
	allocator.c syscall_table.c emul_cache.c emul_mapped.c \
	machine/emul_machdep.c machine/emul_vector.s \
	uxkern/bsd_1_user.c

emulator: ${OBJS} emulator_base
	$(LD) -z -o emulator -T `./emulator_base` ${LDFLAGS} ${OBJS} \
		-lthreads -lmach_sa
	-xstrip emulator
	md -u Makedep -d *.d

.s.o:
	$(CPP) -MD ${DEFINES} $*.s > $*.as
	$(AS) ${ASFLAGS} -o $*.o $*.as
	rm -f $*.as

.c.o:
	$(KCC) $(CFLAGS) -c $< 

emulator_base: emulator_base.c
	$(CC) $(CFLAGS) -o emulator_base emulator_base.c -D$(TARGET_MACHINE)=1

bsd_1_user.o: uxkern/bsd_1_user.c
	$(KCC) $(CFLAGS) -c -o bsd_1_user.o uxkern/bsd_1_user.c

bsd_user_side.o: uxkern/bsd_1.h

#
#  Mach IPC-based interfaces
#

MAKE_UXKERN = [ -d uxkern ] || mkdir uxkern

MIG = mig
MIGFLAGS = -DMACH_IPC_COMPAT=0

BSD_FILES = uxkern/bsd_1.h uxkern/bsd_1_user.c

$(BSD_FILES): uxkern/bsd_types_gen.h ../server/uxkern/bsd_1.defs
	-$(MAKE_UXKERN)
	$(MIG) -MD $(MIGFLAGS) -header uxkern/bsd_1.h -user uxkern/bsd_1_user.c -server /dev/null ../server/uxkern/bsd_1.defs

#
# We have to tell Mig to generate a header file, even though we don't really
# want it, because the user file includes the header file from the current
# directory.
#

mach_user.c: mach/mach.defs
	$(MIG) -MD $(MIGFLAGS) -DEMULATOR -header mach_interface.h -user mach_user.c -server /dev/null mach/mach.defs

uxkern/bsd_types_gen.h : bsd_types_gen
	-$(MAKE_UXKERN)
	./bsd_types_gen > uxkern/bsd_types_gen.h

bsd_types_gen : bsd_types_gen.o
	${CC} -o bsd_types_gen bsd_types_gen.o

bsd_types_gen.o : uxkern/bsd_types_gen.c
	${CC} -c ${CFLAGS} uxkern/bsd_types_gen.c

clean:
	rm -rf emulator ${OBJS} uxkern migfiles *.[chdo] bsd_types_gen* Makedep

-include Makedep
