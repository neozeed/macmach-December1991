# Makefile for libmach_sa, Mach 3.0 version.

# Note that DEFS.h, crt0.c, setjmp.S and _setjmp.S are symbolic links
# into the libc sources.

DESTDIR=

SYS=	../../../mach_kernel/kernel

DEFS=	-DMACH_IPC_COMPAT=0 -DSTANDALONE -DTypeCheck=0

CFLAGS=	-O -MD -I${target_machine} ${DEFS}

MIGFLAGS= ${DEFS}

LINKS=	crt0.c _setjmp.S bcopy.S bzero.S mach_traps.S

MIGS=	mach_user.c \
	mach_port_user.c \
	mach_host_user.c \
	device_user.c \
	device_request_user.c \
	device_reply_server.c \
	exc_user.c exc_server.c \
	mach_debug_user.c

GENS=	exc.h device_request.h device_reply.h mach_debug.h

SRCS=	exit.c mach_init.c mach_msg_destroy.c mach_msg_server.c \
	mach_syscalls.c mig_strncpy.c mig_support.c msg.c printf.c \
	strcat.c strcmp.c strcpy.c strlen.c strncpy.c ${LINKS} ${MIGS}

OBJS=	crt0.o exit.o mach_user.o mach_traps.o mig_support.o \
	mach_msg_destroy.o  mach_msg_server.o \
	bcopy.o strcmp.o strlen.o strncpy.o msg.o exc_server.o \
	mach_host_user.o mach_port_user.o mach_syscalls.o \
	device_user.o device_request_user.o device_reply_server.o \
	_setjmp.o bzero.o strcat.o strcpy.o mach_init.o mig_strncpy.o \
	mach_debug_user.o

libmach_sa.a: links ${OBJS}
	ar cu $@ ${OBJS}
	ranlib $@

links:	FRC
	rm -f mach; ln -s ${SYS}/mach mach
	for i in ${LINKS}; do \
	  rm -f $$i; \
	  ln -s ${target_machine}/$$i $$i; \
	done

mach_user.c : ${SYS}/mach/mach.defs
	mig ${MIGFLAGS} \
		-user mach_temp \
		-header mach_interface.h \
		-server /dev/null \
		$?
	sed 	-e 's/vm_allocate/mig_vm_allocate/' \
		-e 's/vm_map/mig_vm_map/' \
		-e 's/vm_deallocate/mig_vm_deallocate/' \
		-e 's/task_create/mig_task_create/' \
		-e 's/task_terminate/mig_task_terminate/' \
		-e 's/task_suspend/mig_task_suspend/' \
		-e 's/task_set_special_port/mig_task_set_special_port/' \
		< mach_temp > $@
	rm -f mach_temp

mach_port_user.c: ${SYS}/mach/mach_port.defs
	mig ${MIGFLAGS} \
		-user mach_port_temp \
		-header mach_port.h \
		-server /dev/null \
		$?
	sed	-e 's/mach_port_allocate/mig_mach_port_allocate/' \
		-e 's/mach_port_deallocate/mig_mach_port_deallocate/' \
		-e 's/mach_port_insert_right/mig_mach_port_insert_right/' \
		< mach_port_temp > $@
	rm -f mach_port_temp

mach_host_user.c: ${SYS}/mach/mach_host.defs
	mig ${MIGFLAGS} -user $@ -server /dev/null $?

device_user.c: ${SYS}/device/device.defs
	mig ${MIGFLAGS} -user $@ -header device.h -server /dev/null $?

device_request_user.c: ${SYS}/device/device_request.defs
	mig ${MIGFLAGS} -user $@ -server /dev/null $?

device_reply_server.c: ${SYS}/device/device_reply.defs
	mig ${MIGFLAGS} -user /dev/null -server $@ $?

exc_user.c exc_server.c: ${SYS}/mach/exc.defs
	mig ${MIGFLAGS} -user exc_user.c -server exc_server.c $?

mach_debug_user.c: ${SYS}/mach_debug/mach_debug.defs
	mig ${MIGFLAGS} -user mach_debug_user.c -server /dev/null $?

install: FRC
	install -c -o bin -g bin -m 644 libmach_sa.a ${DESTDIR}/usr/lib/libmach_sa.a
	ranlib ${DESTDIR}/usr/lib/libmach_sa.a

install.man: FRC

clean:	FRC
	rm -f .depend mach ${LINKS} ${MIGS} ${GENS} ${OBJS} libmach_sa.a
	rm -f mach_interface.h mach_port.h device.h mach_host.h

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS} ${MIGS} ${LINKS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
