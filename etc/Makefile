# Makefile for things in /etc, Mach 2.5 version.

DESTDIR=

THINGS=	XNSrouted ac accton badsect chown chroot clri comsat cron dcheck \
	disklabel diskpart dmesg dump dumpfs edquota fastboot fasthalt \
	fingerd fsck fsdb fstat ftpd getpass gettable getty halt htable \
	icheck ifconfig inetd init kgmon machid mkhosts mklost+found \
	mknod mkpasswd mkproto mount mpolicy named ncheck netmsg newfs ping \
	pstat quot quotacheck quotaon rc reboot renice repquota restore \
	rexecd rlogind rmt route routed rshd rwhod sa savecore shutdown \
	slattach snames stacks syslogd talkd telnetd termcap tftpd timed \
	tunefs tzone update version vipw \
	etc.${target_machine} 

all:
	@for i in ${THINGS}; do ( \
	  echo "=== Building $$i."; \
	  cd $$i; make; \
	) done

install:
	@for i in ${THINGS}; do ( \
	  echo "=== Installing $$i."; \
	  cd $$i; make DESTDIR=${DESTDIR} install; \
	) done

install.man:
	@for i in ${THINGS}; do ( \
	  echo "=== Installing man pages for $$i."; \
	  cd $$i; make DESTDIR=${DESTDIR} install.man; \
	) done

clean:
	@for i in ${THINGS}; do ( \
	  echo "=== Cleaning $$i."; \
	  cd $$i; make clean; \
	) done
