# Makefile for netmsg server.

DESTDIR=

BINDIR=	${DESTDIR}/etc

USRETC=	${DESTDIR}/usr/etc

DEFS=	-DMACH3 -DMACH3_UNIX -DMACH_IPC_COMPAT

IDEFS=	-I../conf

MDEFS=	${IDEFS} ${DEFS}

CFLAGS=	-O -MD -traditional ${IDEFS} ${DEFS}

IFLAGS=	-o root -g bin -m 544

SRCS=	cam_stuff.c crypt.c crypt_des.c crypt_multperm.c crypt_newdes.c \
	crypt_xor.c datagram.c deltat.c deltat_handler.c deltat_send.c \
	deltat_utils.c des.c dispatcher.c ipc.c ipc_block.c ipc_exception.c \
	ipc_in.c ipc_msg.c ipc_out.c ipc_rpc.c ipc_swap.c keyman.c \
	km_procs.c km_utils.c lock_queue.c logstat.c ls_rec.c mem.c \
	multperm.c netipc.c netipc_mach3.c netipc_mn.c netipc_so.c \
	netname.c network.c nm_enable.c nm_extra.c nm_init.c nmserver.c \
	nn_handler.c nn_procs.c pc_handler.c pc_startup.c po_message.c \
	po_notify.c po_utils.c port_test.c portcheck.c portops.c portrec.c \
	portsearch.c ps_auth.c rwlock.c sbuf.c skel.c skelp.c srr.c \
	srr_handler.c srr_utils.c tcp.c timer.c trans_test.c transport.c \
	trint_test.c uid.c vmtp1.c vmtp2.c mach3_msg.c mach3_flsbuf.c

GENS=	netnameServer.c logstatServer.c

RPCMOD=ON
RPCMOD_ON= ipc_exception.o ipc_msg.o ipc_rpc.o tcp.o vmtp2.o
RPCMOD_OFF= ipc_in.o ipc_out.o vmtp1.o
RPCMOD_OBJS= ${RPCMOD_${RPCMOD}}

MACH3_OBJS= netipc_mach3.o mach3_msg.o mach3_flsbuf.o

OBJS=	nmserver.o cam_stuff.o crypt.o crypt_des.o crypt_multperm.o \
	crypt_newdes.o crypt_xor.o datagram.o deltat.o deltat_handler.o \
	deltat_send.o deltat_utils.o des.o dispatcher.o ipc.o ipc_block.o \
	ipc_swap.o keyman.o km_procs.o km_utils.o lock_queue.o logstat.o \
	ls_rec.o mem.o multperm.o netipc.o netname.o network.o nm_enable.o \
	nm_extra.o nm_init.o nn_handler.o nn_procs.o pc_handler.o \
	pc_startup.o po_message.o po_notify.o po_utils.o portcheck.o \
	portops.o portrec.o portsearch.o ps_auth.o rwlock.o sbuf.o srr.o \
	srr_handler.o srr_utils.o timer.o transport.o uid.o netnameServer.o \
	logstatServer.o netipc_mn.o netipc_so.o \
	${RPCMOD_OBJS} \
	${MACH3_OBJS}

all:	netmsgserver .depend

netmsgserver: ${GENS} ${OBJS}
	${CC} -o $@ ${OBJS} -lmach -lthreads

netnameServer.c: netname.defs
	old.mig ${MDEFS} -header netname.h -server $@ -user /dev/null $?

logstatServer.c: logstat.defs
	old.mig ${MDEFS} -header logstat.h -server $@ -user /dev/null $?

logstat.c: logstatServer.c

install: FRC
	install -c -s ${IFLAGS} netmsgserver ${USRETC}/netmsgserver
	rm -f ${BINDIR}/netmsgserver
	ln -s ../usr/etc/netmsgserver ${BINDIR}/netmsgserver

clean:	FRC
	rm -f .depend ${OBJS} netmsgserver core
	rm -f netnameServer.c netname.h
	rm -f logstatServer.c logstat.h

.depend: ${SRCS} ${GENS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
