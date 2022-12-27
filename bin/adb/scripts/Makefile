# Makefile for standard adb scripts.

DESTDIR=

SCRIPTDIR= ${DESTDIR}/usr/lib/adb

SCRIPTS= buf callout callout.next clist clist.nxt dino dir dir.nxt \
	dmap file filsys findinode findinode.nxt findproc host.nxt hosts \
	hosts.nxt hosttable idp ifaddr ifnet ifrw imp in_ifaddr inode inpcb \
	iovec ipreass ipreass.nxt mact mact.nxt mbstat mbuf mbuf.nxt \
	mbufchain mbufs mbufs.nxt mount nameidata nspcb nsrtentry packetchain \
	protosw quota rawcb route rtentry rusage setproc setproc.done socket \
	spp sppcb stat tcpcb tcpip tcpreass tcpreass.nxt text traceall \
	traceall.nxt u u.all ubadev uio un unpcb urppcb vtimes

all:

install:
	-[ -d ${SCRIPTDIR} ] || mkdir ${SCRIPTDIR}
	chown bin.bin ${SCRIPTDIR}
	chmod 555 ${SCRIPTDIR}
	rm -rf ${SCRIPTDIR}/*
	install -c -o bin -g bin -m 444 ${SCRIPTS} ${SCRIPTDIR}

install.man:

clean:
