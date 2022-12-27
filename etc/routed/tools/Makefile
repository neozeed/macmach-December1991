# /usr/src/etc/routed/tools/Makefile for MacMach

CFLAGS=	-O
SRCS=	query.c trace.c

all:	query trace .depend

# test programs and utilities
query:	FRC
	${CC} ${CFLAGS} -o $@ query.c

trace:	FRC
	${CC} ${CFLAGS} -o $@ trace.c

install: FRC

clean:	FRC
	rm -f .depend core query trace tags

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
