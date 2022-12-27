# Makefle for tip.

# make file for intermachine communications package
#
# Files are:
#	/etc/remote		remote host description file
#	/etc/phones		phone number file, owned by ${OWNER} and
#				  mode 6??
#	${ADM}/aculog		ACU accounting file, owned by ${OWNER} and
#				  mode 6?? {if ACULOG defined}
# Presently supports:
#	BIZCOMP
#	DEC DF02-AC, DF03-AC
#	DEC DN-11/Able Quadracall
#	HAYES and Hayes emulators
#	USR COURIER (2400 baud)
#	VENTEL 212+
#	VADIC 831 RS232 adaptor
#	VADIC 3451
# (drivers are located in aculib.a)
#
# Configuration defines:
#	DF02, DF03, DN11	ACU's supported
#	  BIZ1031, BIZ1022, VENTEL, V831, V3451, HAYES, COURIER
#	ACULOG			turn on tip logging of ACU use
#	PRISTINE		no phone #'s put in ACU log file
#	CONNECT			worthless command
#	DEFBR			default baud rate to make connection at
#	DEFFS			default frame size for FTP buffering of
#				writes on local side
#	BUFSIZ			buffer sizing from stdio, must be fed
#				explicitly to remcap.c if not 1024

ADM=	usr/adm

CONFIG=	-DV831 -DVENTEL -DHAYES -DCOURIER

CFLAGS=	-O -MD -DDEFBR=1200 -DDEFFS=BUFSIZ -DACULOG -DPRISTINE

SRCS=	acu.c acutab.c cmds.c cmdtab.c cu.c hunt.c log.c partab.c remcap.c \
	remote.c tip.c tipout.c uucplock.c value.c vars.c

OBJS=	acu.o acutab.o cmds.o cmdtab.o cu.o hunt.o log.o partab.o remcap.o \
	remote.o tip.o tipout.o uucplock.o value.o vars.o

all:	aculib/aculib.a tip .depend

tip:	${OBJS}
	cc -o $@ ${OBJS} aculib/aculib.a

remcap.o: remcap.c
	cc -c -DBUFSIZ=1024 ${CFLAGS} remcap.c

# acutab is configuration dependent, and so depends on the makefile
acutab.o: Makefile
acutab.o: acutab.c
	cc -c ${CFLAGS} ${CONFIG} acutab.c

# remote.o depends on the makefile because of DEFBR and DEFFS
# log.o depends on the makefile because of ACULOG
log.o remote.o: Makefile

aculib/aculib.a:
	cd aculib; make ${MFLAGS}

install: FRC
	cd aculib; make ${MFLAGS} install
	install -c -s -o uucp -g daemon -m 4755 tip ${DESTDIR}/usr/bin/tip
	rm -f ${DESTDIR}/usr/bin/cu
	ln -s tip ${DESTDIR}/usr/bin/cu
#	cp /dev/null ${DESTDIR}/${ADM}/aculog
#	chown ${OWNER} ${DESTDIR}/${ADM}/aculog
#	chmod 600 ${DESTDIR}/${ADM}/aculog
#	@echo "create /etc/remote and /etc/phones"
	install -c -o bin -g bin -m 444 remote ${DESTDIR}/etc/newconfig

install.man: FRC
	install -c -o bin -g bin -m 444 tip.man ${DESTDIR}/usr/man/man1/tip.1
	rm -f ${DESTDIR}/usr/man/man1/cu.1
	ln -s tip.1 ${DESTDIR}/usr/man/man1/cu.1

clean:	FRC
	rm -f .depend ${OBJS} core tip
	cd aculib; make ${MFLAGS} clean

.depend:
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
