# Makefile for things in /usr/bin, Mach 3.0 version.

DESTDIR=

THINGS=	Xinstall ar11 at basename bc bison cal calendar cb checkeq col comm \
	crypt dc deroff diction diff3 efl emacs eqn file find fixit graph \
	hash_info ipc_perf ipc_test join kermit learn less lex lint \
	look lorder m4 macintosh massign mcreate mesg mig mkdep nohup patch \
	pinfo plot prof ptx ranlib rcs refer rev roff sc sleep sort spell \
	spline split struct sum sup tabs tac tbl tc thstate tip tk top touch \
	tr tsort tty uemacs uniq units uucp vminfo vmread vmsearch wh xlisp \
	xptest xsend xstrip yacc zprint 

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

