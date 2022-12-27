# /usr/src/etc/netmsg/utils/Makefile for MacMach

DESTDIR=

DEFS=	-DMACH3 -DMACH3_UNIX -DMACH_IPC_COMPAT

IDEFS=	-I../conf -I../server

MFLAGS= ${IDEFS} ${DEFS}

CFLAGS=	-O ${IDEFS} ${DEFS}

SRCS=	dummy_kds.c new_ns_port.c nmcontrol.c nmprofexit.c \
	nmprotocol.c nmtest.c nmtime.c nmxlog.c nmxportstat.c \
	new_ns_port.c old_ns_port.c \
	../server/logstat.c ../server/mach3_msg.c ../server/km_procs.c \
	../server/km_utils.c ../server/multperm.c ../server/ls_rec.c

GENS=	kmUser.c logstatUser.c

STD=	nmcontrol nmprofexit nmprotocol nmtest nmtime nmxlog nmxportstat \
	dummy_kds new_ns_port old_ns_port

all:	${STD} .depend

nmcontrol: nmcontrol.c logstatUser.o ls_rec.o mach3_msg.o
	cc ${CFLAGS} -o $@ nmcontrol.c logstatUser.o ls_rec.o mach3_msg.o -lmach

nmprofexit: nmprofexit.c mach3_msg.o
	cc ${CFLAGS} -o $@ nmprofexit.c mach3_msg.o -lmach

nmprotocol: nmprotocol.c logstatUser.o mach3_msg.o
	cc ${CFLAGS} -o $@ nmprotocol.c logstatUser.o mach3_msg.o -lmach

nmtest: nmtest.c mach3_msg.o port_enable.c
	cc ${CFLAGS} -o $@ nmtest.c mach3_msg.o port_enable.c -lmach -lthreads

nmtime: nmtime.c mach3_msg.o port_enable.o
	cc ${CFLAGS} -o $@ nmtime.c mach3_msg.o port_enable.o -lmach

nmxlog: nmxlog.c
	cc ${CFLAGS} -o $@ nmxlog.c

nmxportstat: nmxportstat.c
	cc ${CFLAGS} -o $@ nmxportstat.c

dummy_kds: dummy_kds.c
	cc ${CFLAGS} -o $@ dummy_kds.c

new_ns_port: new_ns_port.c
	cc ${CFLAGS} -o $@ new_ns_port.c -lmach

old_ns_port: old_ns_port.c
	cc ${CFLAGS} -o $@ old_ns_port.c -lmach

install: FRC
	install -c -s -o bin -g bin -m 555 ${STD} ${DESTDIR}/usr/bin

clean:	FRC
	rm -f .depend *.o ${STD} core
	rm -f logstatUser.c logstat.h

#############################################################################

logstatUser.c: ../server/logstat.defs
	old.mig ${MDEFS} -user $@ -header logstat.h -server /dev/null $?

logstatUser.o: logstatUser.c
	cc ${CFLAGS} -c -o $@ $?

mach3_msg.o: ../server/mach3_msg.c
	cc ${CFLAGS} -c -o $@ $?

ls_rec.o: ../server/ls_rec.c
	cc ${CFLAGS} -c -o $@ ../server/ls_rec.c

#############################################################################

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
