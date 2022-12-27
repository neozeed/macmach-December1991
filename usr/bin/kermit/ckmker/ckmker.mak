# MPW make file for MacKermit 0.9(36) created by "Build"
#
#   File:       Kermit.make
#   Target:     Kermit
#   Sources:    ckcfn2.c
#               ckcfns.c
#               ckcmai.c
#               ckcpro.w
#				ckmco2.c
#               ckmcon.c
#               ckmfio.c
#				ckmini.c
#               ckmker.r
#               ckmkey.c
#				ckmpri.c
#               ckmrem.c
#               ckmsav.c
#               ckmscr.c
#               ckmsfp.c
#               ckmtio.c
#               ckmusr.c
#               ckmutl.c
#   Created:    Sonntag, 4. Oktober 1987 18:24:44 Uhr
#	Modified:	Tuesday, December 1, 1987 15:35:13 by PWP:
#			make the .c.o files dependent on the appropriate
#			.h files also

# -b : put string constants into code
# -mbg ch8 : create v2.0 compat. MacsBug symbols
# -r : warn on calling an undefined function
# -d MAC : Macintosh version of CKermit
# -d MPW : We are using the MPW 3.0 C compiler
# -w do not display warnings
# -q2 no external memory changes
# -u PROFILE : if you want to do profiling, then change this to -d PROFILE,
#   and add the commented out lines in the link, further down in this
#   file.
# -d TLOG : Include transaction logging code in Kermit
#
CKCDefns = -d MAC -d TLOG -u PROFILE -d DEBUG -d IFDEBUG -d DYNAMIC -d NOICP -u NETCONN -d NOCMDL -d NOCCTRAP
COptions = -w -mbg ch8 -b -d MPW {CKCDefns}

ckcfn2.c.o � ckcfn2.c ckcsym.h ckcker.h ckcdeb.h ckcxla.h ckmxla.h ckcasc.h
	C -s ckcfn2 {COptions} ckcfn2.c

ckcfn3.c.o � ckcfn3.c ckcdeb.h ckcasc.h ckcker.h ckcxla.h ckmxla.h
	C -s ckcfn3 {COptions} ckcfn3.c

ckcfns.c.o � ckcfns.c ckcsym.h ckcker.h ckcdeb.h ckcxla.h ckmxla.h ckcasc.h
	C -s ckcfns {COptions} ckcfns.c

ckcmai.c.o � ckcmai.c ckcsym.h ckcker.h ckcdeb.h ckcxla.h ckmxla.h ckcasc.h
	C -s ckmini {COptions} ckcmai.c

ckmxla.c.o � ckmxla.c ckcsym.h ckcker.h ckcdeb.h ckcxla.h ckmxla.h
	C -s ckmxla {COptions} ckmxla.c

ckmcon.c.o � ckmcon.c ckcdeb.h ckmdef.h ckmasm.h ckmcon.h
	C -s ckmcon {COptions} ckmcon.c

ckmco2.c.o � ckmco2.c ckcdeb.h ckmdef.h ckmasm.h ckmcon.h
	C -s ckmcon {COptions} ckmco2.c

ckcpro.c � ckcpro.w wart
	wart ckcpro.w ckcpro.c
	SetFile ckcpro.c -t TEXT -c 'MPS '

### ckcpro.c should be compiled into the same segment as ckcfns.c so that the check
### in decode() for what routine to call to write a character works right.  If we
### do this call across segments, the test fails.
ckcpro.c.o � ckcpro.c ckcker.h ckcdeb.h ckcasc.h
	C -s ckcfns {COptions} ckcpro.c

ckmfio.c.o � ckmfio.c ckcker.h ckcdeb.h ckmdef.h ckmasm.h ckmres.h
	C -s ckmfio {COptions} ckmfio.c

ckmini.c.o � ckmini.c ckcker.h ckcdeb.h ckmdef.h ckmasm.h ckmres.h
	C -s ckmini {COptions} ckmini.c

ckmkey.c.o � ckmkey.c ckcker.h ckcdeb.h ckmdef.h ckmres.h
	C -s ckmkey {COptions} ckmkey.c

ckmpri.c.o � ckmpri.c ckcker.h ckcdeb.h ckmdef.h ckmres.h
	C -s ckmpri {COptions} ckmpri.c

ckmrem.c.o � ckmrem.c ckcker.h ckcdeb.h ckmdef.h ckmres.h ckcasc.h
	C -s ckmrem {COptions} ckmrem.c

ckmsav.c.o � ckmsav.c ckcker.h ckcdeb.h ckmdef.h ckmres.h
	C -s ckmsav {COptions} ckmsav.c

ckmscr.c.o � ckmscr.c ckcker.h ckcdeb.h ckmdef.h ckmres.h
	C -s ckmscr {COptions} ckmscr.c

ckmsfp.c.o � ckmsfp.c ckcker.h ckcdeb.h ckmdef.h ckmres.h
	C -s ckmsfp {COptions} ckmsfp.c

ckmtio.c.o � ckmtio.c ckcdeb.h ckmdef.h
	C -s ckmtio {COptions} ckmtio.c

ckmusr.c.o � ckmusr.c ckcker.h ckcdeb.h ckmdef.h ckmasm.h ckmres.h
	C -s ckmusr {COptions} ckmusr.c

ckmutl.c.o � ckmutl.c ckcker.h ckcdeb.h ckmdef.h ckmasm.h ckmres.h ckmcon.h ckcasc.h
	C -s ckmutl {COptions} ckmutl.c


wart �� ckwart.c.o
	Link -w -c 'MPS ' -t MPST �
		ckwart.c.o �
		"{Libraries}"stubs.o �
		"{CLibraries}"CRuntime.o �
		"{Libraries}"Interface.o �
		"{CLibraries}"StdCLib.o �
		"{CLibraries}"CSANELib.o �
		"{CLibraries}"Math.o �
		"{CLibraries}"CInterface.o �
		"{Libraries}"ToolLibs.o �
		-o wart

ckwart.c.o � ckwart.c
	C -d MAC -d MPW ckwart.c

Kermit.res �� ckmker.r ckmfnt.r
    Delete -i Kermit.res
	Rez ckmker.r -d TLOG -o Kermit.res -t 'rsrc' -c 'RSED'
	Rez -append ckmfnt.r -o Kermit.res

Kermit �� Kermit.make �
		Kermit.res �
		ckcfn2.c.o �
		ckcfn3.c.o �
		ckcfns.c.o �
		ckcmai.c.o �
		ckmxla.c.o �
		ckcpro.c.o �
		ckmcon.c.o �
		ckmco2.c.o �
		ckmfio.c.o �
		ckmini.c.o �
		ckmkey.c.o �
		ckmpri.c.o �
		ckmrem.c.o �
		ckmsav.c.o �
		ckmscr.c.o �
		ckmsfp.c.o �
		ckmtio.c.o �
		ckmusr.c.o �
		ckmutl.c.o
	Delete -i Kermit
	Duplicate -y Kermit.res Kermit
	Rez -o Kermit -a ckmkr2.r
	Link -map -mf -ra =resProtected -msg nodup -o Kermit -t APPL -c '????' -l �
		ckcfn2.c.o �
		ckcfn3.c.o �
		ckcfns.c.o �
		ckcmai.c.o �
		ckmxla.c.o �
		ckcpro.c.o �
		ckmcon.c.o �
		ckmco2.c.o �
		ckmfio.c.o �
		ckmini.c.o �
		ckmkey.c.o �
		ckmpri.c.o �
		ckmrem.c.o �
		ckmsav.c.o �
		ckmscr.c.o �
		ckmsfp.c.o �
		ckmtio.c.o �
		ckmusr.c.o �
		ckmutl.c.o �
		"{CLibraries}"CRuntime.o �
		"{Libraries}"Interface.o �
		"{CLibraries}"StdCLib.o �
#		"{CLibraries}"CSANELib.o �
#		"{CLibraries}"Math.o �
		"{CLibraries}"CInterface.o �
#		"{Libraries}"PerformLib.o �
		> kermit.linkmap
	SetFile Kermit -t APPL -c KR09 -a B
	
Clean �
    Delete -i Kermit.res wart Perform.out
    Delete -i �.o
	Delete -i ckcpro.c
