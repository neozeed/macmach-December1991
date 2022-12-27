# Makefile for sendmail doc.

#
#  Sendmail
#  Copyright (c) 1983  Eric P. Allman
#  Berkeley, California
#
#	Makefile for sendmail documentation
#

OBJS=	intro.lpr op.lpr usenix.lpr
DOCS=	intro.me  op.me  usenix.me
SRCS=	$(DOCS) sendmail.8 syslog.3 syslog.8
STD=	$(OBJS) rfc819.lpr rfc821.lpr rfc822.lpr

LPR=	lpr
TROFF=	vtroff
GET=	sccs get

.SUFFIXES: .me .lpr

.me.lpr:
	nroff -me -Tlp $< > $@

troff: $(DOCS)
	for i in $(DOCS); do $(TROFF) -me $$i; done

all:	$(STD)

print: $(STD)
	$(LPR) $(STD)

sources: $(SRCS)

$(SRCS):
	$(GET) $(REL) SCCS/s.$@

clean:	FRC
	rm -f .depend $(OBJS) a.out core

FRC:
