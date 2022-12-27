# /usr/src/usr/man/Makefile for MacMach

USRMAN=	${DESTDIR}/usr/man

DIRS=	man.old man1 man2 man3 man4 man5 man6 man7 man8 manx

all:	FRC

install: FRC
	install -c -o bin -g bin -m 444 man.template ${USRMAN}/man.template
	install -c -o bin -g bin -m 444 mandesc ${USRMAN}/mandesc
	install -c -o bin -g bin -m 555 manroff.sh ${USRMAN}/manroff
	sh mkwhatis.sh ${USRMAN} ${DIRS} >${USRMAN}/whatis
	chown bin.bin ${USRMAN}/whatis
	chmod 444 ${USRMAN}/whatis

install.man: FRC
	tar -chf - doc | (cd ${USRMAN}; tar -xf -)
	find ${USRMAN}/doc -type d -exec chmod 555 {} \;
	find ${USRMAN}/doc -type f -exec chmod 444 {} \;
	chown -R bin.bin ${USRMAN}/doc
	install -c -o bin -g bin -m 444 intro.man1 ${USRMAN}/man1/intro.1
	install -c -o bin -g bin -m 444 intro.man8 ${USRMAN}/man8/intro.8
	install -c -o bin -g bin -m 444 sticky.man8 ${USRMAN}/man8/sticky.8
	install -c -o bin -g bin -m 444 adduser.man8 ${USRMAN}/man8/adduser.8

clean:

FRC:
