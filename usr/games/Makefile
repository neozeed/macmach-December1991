# Makefile for things in /usr/games.

DESTDIR=

THINGS=	advent arithmetic atc backgammon banner battlestar bcd bench \
	boggle btlgammon canfield cribbage dm factor fish fortune hangman \
	hunt jivval mille monop moria nethack number ppt primes quiz rain \
	robots rogue sail snake trek wargames worm worms wump 

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
