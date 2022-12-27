# /usr/src/etc/Makefile for MacMach

DESTDIR=
CFLAGS=	-O
LIBC=	/lib/libc.a

# Programs that live in subdirectories, and have makefiles of their own.
SUBDIR=	dump envmgr fsck ftpd getty htable ipcexecd mach_init \
	named newfs netmemoryserver netmsgserver restore routed talkd \
	termcap tftpd timed tzone XNSrouted etc.${MACHINE}

# Shell scripts that need only be installed and are never removed.
SCRIPT=	fastboot fasthalt mklost+found

# C sources that live in the current directory
SRCS=	ac.c accton.c bootpd.c badsect.c chown.c chroot.c clri.c comsat.c \
	cron.c dcheck.c disklabel.c diskpart.c dumpfs.c edquota.c fingerd.c \
	fsdb.c fstat.c gettable.c halt.c icheck.c ifconfig.c inetd.c mknod.c \
	mkpasswd.c mkproto.c mount.c ncheck.c quot.c quotacheck.c quotaon.c \
	reboot.c renice.c repquota.c rexecd.c rlogind.c rmt.c route.c rshd.c \
	rwhod.c sa.c savecore.c slattach.c swapon.c syslogd.c telnetd.c \
	tunefs.c umount.c update.c vipw.c arp.c dmesg.c init.c kgmon.c \
	ping.c pstat.c shutdown.c trpt.c version.c

# C programs that live in the current directory and do not need
# explicit make lines.  STD programs go to /etc, XSTD programs go
# to /usr/etc, linked to /etc
#note: the following from SUBDIR also go to /usr/etc, linked to /etc:
#      ftpd dump htable named restore routed talkd tftpd
#      timed XNSrouted
STD=	badsect bootp chown clri diskpart dumpfs fsdb halt icheck ifconfig \
	mknod mount reboot route tunefs umount
XSTD=	ac accton bootpd chroot comsat cron dcheck edquota fingerd gettable \
	inetd mkpasswd mkproto ncheck quot quotacheck quotaon renice repquota \
	rexecd rlogind rmt rshd rwhod sa slattach syslogd update version vipw

# C programs that live in the current directory and need explicit make lines.
NSTD=	swapon

XNSTD=	telnetd

# Programs that get installed with special permissions
PERM=	arp dmesg disklabel fstat init kgmon ping pstat shutdown trpt

# optional: not always made
OPTIONAL= mkhosts

all:	${SUBDIR} ${STD} ${XSTD} ${NSTD} ${XNSTD} ${PERM} ${OPTIONAL} .depend

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

${STD} ${XSTD} ${PERM}: ${LIBC}
	${CC} ${CFLAGS} -o $@ $@.c

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	-for i in ${SCRIPT}; do \
	  install -c -o bin -g bin -m 755 $$i.sh ${DESTDIR}/etc/$$i; \
	done
	install -c -s -o bin -g bin -m 755 ${STD} ${NSTD} ${DESTDIR}/etc
	-for i in ${XSTD} ${XNSTD}; do \
	  rm -f ${DESTDIR}/etc/$$i ${DESTDIR}/usr/etc/$$i; \
	  install -c -s -o bin -g bin -m 755 $$i ${DESTDIR}/usr/etc; \
	  ln -s ../usr/etc/$$i ${DESTDIR}/etc/$$i; \
	done
	-for i in ${OPTIONAL}; do \
	  [ ! -f $$i ] || \
	    install -c -s -o bin -g bin -m 755 $$i ${DESTDIR}/etc; \
	done
# programs that run with special permissions
	install -c -s -o root -g bin -m 4755 -s ping ${DESTDIR}/etc/ping
	install -c -s -o root -g operator -m 4754 shutdown ${DESTDIR}/etc/shutdown
	install -c -s -o bin -g operator -m 2754 disklabel ${DESTDIR}/etc/disklabel
	install -c -s -o bin -g kmem -m 2755 pstat ${DESTDIR}/etc/pstat
	install -c -s -o bin -g kmem -m 2755 arp ${DESTDIR}/etc/arp
	install -c -s -o bin -g kmem -m 2755 dmesg ${DESTDIR}/etc/dmesg
	install -c -s -o bin -g kmem -m 2755 fstat ${DESTDIR}/etc/fstat
	install -c -s -o bin -g kmem -m 2755 kgmon ${DESTDIR}/etc/kgmon
	install -c -s -o bin -g kmem -m 2755 trpt ${DESTDIR}/etc/trpt
	install -c -s -o bin -g bin -m 744 init ${DESTDIR}/etc/init
# directories and links
	rm -f ${DESTDIR}/usr/etc/quotaoff
	ln -s quotaon ${DESTDIR}/usr/etc/quotaoff
	rm -f ${DESTDIR}/etc/quotaoff
	ln -s ../usr/etc/quotaoff ${DESTDIR}/etc/quotaoff
	rm -f ${DESTDIR}/bin/chgrp
	ln -s ../etc/chown ${DESTDIR}/bin/chgrp

clean:	FRC
	-for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done
	rm -f .depend a.out core *.s *.o
	rm -f ${STD} ${XSTD} ${NSTD} ${XNSTD} ${PERM}

# Files listed in ${NSTD}, ${XNSTD} and ${OPTIONAL} have explicit
# make lines given below.

telnetd:
	${CC} ${CFLAGS} -o $@ telnetd.c getty/get_date.c getty/gettytab.c

swapon:
	$(CC) $(CFLAGS) -o swapon swapon.c -ltermlib -lmach

mkhosts:
	@echo "Mkhosts only works with the dbm version of the hostent routines."
	@echo "If gethosts() is not in libc, mkhosts isn't installed."
	-${CC} ${CFLAGS} -o mkhosts mkhosts.c >/dev/null 2>&1 || rm mkhosts

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
