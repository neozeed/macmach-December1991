TEXTFILES = vis.nr
REFER = refer -e -n -p vis.refs -s
EQN = eqn
PIC = pic
TROFF = psroff -ms -t

vis.ps:	vis.nr vis.refs xsd.c xsud.c
	$(REFER) $(TEXTFILES) | $(EQN) | $(PIC) | $(TROFF) >vis.ps
