# Makefile for learn.

LESSONS=C editor eqn files macros morefiles
FILES=	Linfo Xinfo

all:	FRC

install: FRC
	-[ -d ${DESTDIR}/usr/lib/learn ] || mkdir ${DESTDIR}/usr/lib/learn
	-[ -d ${DESTDIR}/usr/lib/learn/log ] || mkdir ${DESTDIR}/usr/lib/learn/log
	-[ -d ${DESTDIR}/usr/lib/learn/bin ] || mkdir ${DESTDIR}/usr/lib/learn/bin
	chmod 755 ${DESTDIR}/usr/lib/learn ${DESTDIR}/usr/lib/learn/log \
		${DESTDIR}/usr/lib/learn/bin
	chown root.staff ${DESTDIR}/usr/lib/learn ${DESTDIR}/usr/lib/learn/log \
		${DESTDIR}/usr/lib/learn/bin
	install -c -o bin -g bin -m 644 Linfo ${DESTDIR}/usr/lib/learn/Linfo
	install -c -o bin -g bin -m 644 Xinfo ${DESTDIR}/usr/lib/learn/Xinfo
	tar chf - ${LESSONS} | (cd ${DESTDIR}/usr/lib/learn; tar xpf -);
	find ${DESTDIR}/usr/lib/learn -type d -exec chmod 755 {} \;
	find ${DESTDIR}/usr/lib/learn -type f -exec chmod 644 {} \;
	chown -R bin.bin ${DESTDIR}/usr/lib/learn

install.man: FRC

clean:	FRC

FRC:
