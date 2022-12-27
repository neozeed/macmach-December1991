# Makefile for things in /usr/ucb.

DESTDIR=

THINGS=	Mail apply biff checknr clear colcrt colrm compress ctags dsh error \
	ex expand finger fold from ftp gcore gprof grep groups head indent \
	last lastcomm leave lock logger man mkstr more msgs netstat printenv \
	quota rdist rlogin rsh ruptime rwho sccs script sendbug soelim \
	strings symorder sysline systat tail talk tcopy telnet tftp tn3270 \
	tset ul unifdef users vacation vgrind vlp vmstat w wc what whereis \
	which whoami whois window xstr yes 

all:	FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Building $$i."; \
	  cd $$i; make; \
	) done

install: FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Installing $$i."; \
	  cd $$i; make DESTDIR=${DESTDIR} install; \
	) done

install.man: FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Installing man pages for $$i."; \
	  cd $$i; make DESTDIR=${DESTDIR} install.man; \
	) done

clean: FRC
	@for i in ${THINGS}; do ( \
	  echo "=== Cleaning $$i."; \
	  cd $$i; make clean; \
	) done

FRC:
