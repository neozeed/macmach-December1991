# /usr/src/usr.lib/mach3/Makefile for MacMach
#

DESTDIR=

MIG=	mig

SYS=	/usr/include

CFLAGS=	-O

SRCS=	console.c error_codes.c mach3_abort.c mach3_configuration.c \
	mach3_exit.c mach3_filbuf.c mach3_findiop.c mach3_flsbuf.c \
	mach3_lock.c mach3_msg.c mach3_setjmp.c mach_error.c mach_exception.c \
	mach_init.c mach_privileged_ports.c mach_time.c malloc.c port_enable.c

GENS=	mach_user.c \
	mach_port_user.c \
	mach_host_user.c \
	device_user.c \
	device_request_user.c \
	device_reply_server.c \
	notify_user.c notify_server.c \
	exc_user.c exc_server.c \
	default_pager_object_user.c \
	memory_object_server.c \
	memory_object_default_server.c \
	mach_debug_user.c \
	netnameUser.c netnameServer.c \
	serviceUser.c serviceServer.c

OBJS=	console.o error_codes.o mach3_filbuf.o \
	mach3_findiop.o mach3_flsbuf.o mach_error.o \
	mach_init.o port_enable.o mach_priv_pt.o \
	mach3_config.o mach_time.o mach3_msg.o malloc.o \
	mach3_exit.o mach3_setjmp.o mach3_abort.o \
	mach_exception.o mach3_lock.o \
	syscall.o lock.o \
	netnameUser.o serviceUser.o \
	error_codes.o mach3_flsbuf.o mach_time.o \
	mach_error.o mach3_config.o mach3_lock.o \
	lock.o malloc.o

all:	libmach3.a .depend

libmach3.a: ${OBJS}
	ar cu $@ ${OBJS}
	ranlib $@

mach_user.c: ${SYS}/mach/mach.defs
	${MIG} ${MIGFLAGS} \
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
	${MIG} ${MIGFLAGS} \
		-user mach_port_temp \
		-server /dev/null \
		$?
	sed	-e 's/mach_port_allocate/mig_mach_port_allocate/' \
		-e 's/mach_port_deallocate/mig_mach_port_deallocate/' \
		-e 's/mach_port_insert_right/mig_mach_port_insert_right/' \
		< mach_port_temp > $@
	rm -f mach_port_temp

mach_host_user.c: ${SYS}/mach/mach_host.defs
	${MIG} $(MIGFLAGS) -user $@ -server /dev/null $?

device_user.c: ${SYS}/device/device.defs
	${MIG} ${MIGFLAGS} -user $@ -header device.h -server /dev/null $?

device_request_user.c: ${SYS}/device/device_request.defs
	${MIG} ${MIGFLAGS} -user $@ -server /dev/null $?

device_reply_server.c: ${SYS}/device/device_reply.defs
	${MIG} ${MIGFLAGS} -user /dev/null -server $@ $?

notify_user.c notify_server.c notify_user.h: ${SYS}/mach/notify.defs
	${MIG} $(MIGFLAGS) \
	  -header notify_user.h \
	  -user notify_user.c \
	  -server notify_server.c \
	  $?

exc_user.c exc_server.c: ${SYS}/mach/exc.defs
	${MIG} $(MIGFLAGS) -user exc_user.c -server exc_server.c $?

default_pager_object_user.c: ${SYS}/mach/default_pager_object.defs
	${MIG} ${MIGFLAGS} -user $@ -server /dev/null $?

memory_object_server.c memory_object_user.h: ${SYS}/mach/memory_object.defs
	${MIG} $(MIGFLAGS) \
	  -header memory_object_user.h \
	  -user /dev/null \
	  -server memory_object_server.c \
	  $?

memory_object_default_server.c: ${SYS}/mach/memory_object_default.defs
	${MIG} ${MIGFLAGS} -user /dev/null -server $@ $?

mach_debug_user.c: ${SYS}/mach_debug/mach_debug.defs
	${MIG} $(MIGFLAGS) -user $@ -server /dev/null $?

netnameUser.c netnameServer.c: ${SYS}/servers/netname.defs
	${MIG} $(MIGFLAGS) $?

serviceUser.c serviceServer.c: ${SYS}/servers/service.defs
	${MIG} $(MIGFLAGS) $?

install: FRC
	install -c -o bin -g bin -m 644 libmach.a ${DESTDIR}/usr/lib/libmach.a
	ranlib ${DESTDIR}/usr/lib/libmach.a

clean:	FRC
	rm -f .depend ${OBJS} libmach.a ${GENS}
	rm -f mach_interface.h device.h notify_user.h memory_object_user.h

tags:	FRC
	ctags ${SRCS}

.depend: ${GENS} ${SRCS}
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${GENS} ${SRCS}

-include .depend

FRC:

