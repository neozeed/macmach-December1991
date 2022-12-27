# Makefile for libmach, Mach 3.0 version.

DESTDIR=

INC=	../../include

SYS=	../../../mach_kernel/kernel

CFLAGS=	-O -MD -I${target_machine} -I${INC}/servers

LINKS=	fork.S mach_traps.S

MIGS=	mach_user.c \
	mach_port_user.c \
	mach_host_user.c \
	device_user.c \
	device_request_user.c \
	device_reply_server.c \
	notify_user.c notify_server.c \
	seqnos_notify_server.c \
	exc_user.c exc_server.c \
	default_pager_object_user.c \
	memory_object_server.c \
	seqnos_memory_object_server.c \
	memory_object_default_server.c \
	seqnos_memory_object_default_server.c \
	mach_debug_user.c \
	netname_user.c netname_server.c \
	service_user.c service_server.c \
	machid_user.c machid_server.c \
	machid_debug_user.c machid_debug_server.c

SRCS=	error_codes.c mach_error.c mach_init.c mach_msg.c \
	mach_msg_destroy.c mach_msg_receive.c mach_msg_send.c \
	mach_msg_server.c mach_privileged_ports.c mach_syscalls.c \
	mig_strncpy.c mig_support.c Ovfork.c sbrk.c slot_name.c \
	machid_lib.c ${LINKS} ${MIGS}

OBJS_MK = mach_traps.o \
	mig_support.o mig_strncpy.o \
	mach_msg.o mach_msg_send.o mach_msg_receive.o \
	mach_msg_destroy.o mach_msg_server.o \
	device_user.o device_request_user.o device_reply_server.o \
	mach_user.o mach_port_user.o mach_host_user.o \
	mach_debug_user.o \
	default_pager_object_user.o \
	memory_object_server.o seqnos_memory_object_server.o \
	memory_object_default_server.o seqnos_memory_object_default_server.o \
	exc_user.o exc_server.o \
	notify_user.o notify_server.o seqnos_notify_server.o

OBJS_MU = mach_init.o mach_syscalls.o\
	slot_name.o error_codes.o mach_error.o machid_lib.o \
	mach_privileged_ports.o \
	netname_user.o netname_server.o \
	service_user.o service_server.o \
	machid_user.o machid_server.o \
	machid_debug_user.o machid_debug_server.o

OBJS_LIBC = sbrk.o fork.o Ovfork.o 

OBJS = $(OBJS_MK) $(OBJS_MU) $(OBJS_LIBC)

all:	libmach.a .depend

libmach.a: links ${OBJS}
	ar cu $@ ${OBJS}
	ranlib $@

links:	FRC
	for i in ${LINKS}; do \
	  rm -f $$i; \
	  ln -s ${target_machine}/$$i $$i; \
	done

mach_user.c: ${SYS}/mach/mach.defs
	mig \
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
	mig \
		-user mach_port_temp \
		-server /dev/null \
		$?
	sed	-e 's/mach_port_allocate/mig_mach_port_allocate/' \
		-e 's/mach_port_deallocate/mig_mach_port_deallocate/' \
		-e 's/mach_port_insert_right/mig_mach_port_insert_right/' \
		< mach_port_temp > $@
	rm -f mach_port_temp

mach_host_user.c: ${SYS}/mach/mach_host.defs
	mig -user $@ -header mach_host.h -server /dev/null $?

device_user.c: ${SYS}/device/device.defs
	mig -user $@ -header device.h -server /dev/null $?

device_request_user.c: ${SYS}/device/device_request.defs
	mig -user $@ -server /dev/null $?

device_reply_server.c: ${SYS}/device/device_reply.defs
	mig -user /dev/null -server $@ $?

notify_user.c notify_server.c notify_user.h: ${SYS}/mach/notify.defs
	mig \
	  -header notify_user.h \
	  -user notify_user.c \
	  -server notify_server.c \
	  $?

seqnos_notify_server.c: ${SYS}/mach/notify.defs
	mig -DSEQNOS -header /dev/null -user /dev/null -server $@ $?

exc_user.c exc_server.c: ${SYS}/mach/exc.defs
	mig -user exc_user.c -server exc_server.c $?

default_pager_object_user.c: ${SYS}/mach/default_pager_object.defs
	mig -user $@ -server /dev/null $?

memory_object_server.c memory_object_user.h: ${SYS}/mach/memory_object.defs
	mig \
	  -header memory_object_user.h \
	  -user /dev/null \
	  -server memory_object_server.c \
	  $?

seqnos_memory_object_server.c: ${SYS}/mach/memory_object.defs
	mig -DSEQNOS -header /dev/null -user /dev/null -server $@ $?

memory_object_default_server.c: ${SYS}/mach/memory_object_default.defs
	mig -user /dev/null -server $@ $?

seqnos_memory_object_default_server.c: ${SYS}/mach/memory_object_default.defs
	mig -DSEQNOS -header /dev/null -user /dev/null -server $@ $?

mach_debug_user.c: ${SYS}/mach_debug/mach_debug.defs
	mig -user $@ -server /dev/null $?

netname_user.c netname_server.c: ${INC}/servers/netname.defs
	mig -user netname_user.c -server netname_server.c $?

service_user.c service_server.c: ${INC}/servers/service.defs
	mig -user service_user.c -server service_server.c $?

machid_user.c machid_server.c: ${INC}/servers/machid.defs
	mig -user machid_user.c -server machid_server.c $?

machid_debug_user.c machid_debug_server.c: ${INC}/servers/machid_debug.defs
	mig -user machid_debug_user.c -server machid_debug_server.c $?

install: FRC
	install -c -o bin -g bin -m 444 libmach.a ${DESTDIR}/usr/lib/libmach.a
	ranlib ${DESTDIR}/usr/lib/libmach.a

install.man: FRC

clean:	FRC
	rm -f .depend ${LINKS} ${MIGS} ${OBJS} libmach.a
	rm -f mach_interface.h device.h notify_user.h memory_object_user.h
	rm -f mach_host.h service.h netname.h memory_object_default.h
	rm -f mach_debug.h exc.h default_pager_object.h
	rm -f device_reply.h device_request.h mach_port.h
	rm -f machid.h machid_debug.h

tags:	FRC
	ctags ${SRCS}

.depend: ${SRCS} ${MIGS} ${LINKS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
