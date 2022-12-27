NROFF = nroff
PTROFF = ptroff
TROFFOPTS = -ms -t

color.PS: color.tbl.ms
	tbl $? | $(PTROFF) $(TROFFOPTS) >$@

color.doc: color.tbl.ms
	tbl $? | $(NROFF) $(TROFFOPTS) >$@
