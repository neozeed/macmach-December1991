gcc_compiled.:
.globl _copyright
.data
_copyright:
	.ascii "@(#) Copyright (c) 1988 Regents of the University of California.\12 All rights reserved.\12\0"
_sccsid:
	.ascii "@(#)mconnect.c\11"
	.ascii "5.4 (Berkeley) 6/29/88\0"
LC0:
	.ascii "socket\0"
LC1:
	.ascii "tcp\0"
LC2:
	.ascii "smtp\0"
LC3:
	.ascii "hp:r\0"
LC4:
	.ascii "usage: mconnect [-hr] [-p port] [host]\12\0"
LC5:
	.ascii "localhost\0"
LC6:
	.ascii "mconnect: unknown host %s\15\12\0"
LC7:
	.ascii "connecting to host %s (0x%lx), port 0x%x\15\12\0"
LC8:
	.ascii "connect\0"
LC9:
	.ascii "connection open\0"
LC10:
	.ascii "fork\0"
LC11:
	.ascii "w\0"
LC12:
	.ascii "r\0"
.text
	.even
.globl _main
_main:
	link a6,#-1016
	moveml #0x3c30,sp@-
	movel a6@(8),d2
	movel a6@(12),a3
	clrl d5
	pea _TtyBuf
	clrl sp@-
	jbsr _gtty
	pea _finis
	pea 2:w
	jbsr _signal
	clrl sp@-
	pea 1:w
	pea 2:w
	jbsr _socket
	movel d0,d4
	addw #28,sp
	jge L2
	pea LC0
	jbsr _perror
	pea -1:w
	jbsr _exit
	addqw #8,sp
L2:
	pea LC1
	pea LC2
	jbsr _getservbyname
	movel d0,a0
	addqw #8,sp
	tstl a0
	jeq L3
	movew a0@(10),a6@(-14)
L3:
	lea _TtyBuf+4,a2
	jra L4
L14:
	cmpb #104,d0
	jeq L4
	jle L11
	cmpb #112,d0
	jeq L8
	cmpb #114,d0
	jeq L9
	jra L11
L8:
	movel _optarg,sp@-
	jbsr _atoi
	movel d0,sp@-
	jbsr _htons
	movew d0,a6@(-14)
	addqw #8,sp
	jra L4
L9:
	moveq #1,d5
	andw #-17,a2@
	pea _TtyBuf
	clrl sp@-
	jbsr _stty
	orw #16,a2@
	addqw #8,sp
	jra L4
L11:
	pea __iob+40
	pea LC4
	jbsr _fputs
	pea -1:w
	jbsr _exit
	addw #12,sp
L4:
	pea LC3
	movel a3,sp@-
	movel d2,sp@-
	jbsr _getopt
	addw #12,sp
	moveq #-1,d1
	cmpl d0,d1
	jne L14
	subl _optind,d2
	movel _optind,d0
	asll #2,d0
	addl d0,a3
	tstl d2
	jeq L15
	movel a3@,a3
	jra L16
L15:
	lea LC5,a3
L16:
	moveb a3@,d0
	extbl d0
	lea __ctype_+1,a0
	btst #2,a0@(d0:l)
	jeq L17
	movel a3,sp@-
	jbsr _inet_addr
	movel d0,a6@(-12)
	addqw #4,sp
	jra L18
L17:
	movel a3,sp@-
	jbsr _gethostbyname
	movel d0,a2
	addqw #4,sp
	tstl a2
	jne L19
	movel a3,sp@-
	pea LC6
	pea __iob+40
	jbsr _fprintf
	jbsr _finis
	addw #12,sp
L19:
	movel a2@(12),sp@-
	pea a6@(-12)
	movel a2@(16),a0
	movel a0@,sp@-
	jbsr _bcopy
	addw #12,sp
L18:
	movew #2,a6@(-16)
	clrl d0
	movew a6@(-14),d0
	movel d0,sp@-
	movel a6@(-12),sp@-
	movel a3,sp@-
	pea LC7
	jbsr _printf
	pea 16:w
	pea a6@(-16)
	movel d4,sp@-
	jbsr _connect
	addw #28,sp
	tstl d0
	jge L20
	pea LC8
	jbsr _perror
	pea -1:w
	jbsr _exit
	addqw #8,sp
L20:
	pea LC9
	jbsr _puts
	addqw #4,sp
	jbsr _fork
	moveq #-1,d1
	cmpl d0,d1
	jeq L22
	tstl d0
	jeq L23
	jra L29
L22:
	pea LC10
	jbsr _perror
	pea -1:w
	jbsr _exit
	addqw #8,sp
L23:
	pea LC11
	movel d4,sp@-
	jbsr _fdopen
	movel d0,d3
	addqw #8,sp
	jra L24
L28:
	tstl d5
	jne L26
	moveq #10,d1
	cmpl d2,d1
	jne L26
	movel d3,sp@-
	pea 13:w
	jbsr _fputc
	addqw #8,sp
L26:
	movel d3,sp@-
	movel d2,sp@-
	jbsr _fputc
	addqw #8,sp
	moveq #10,d1
	cmpl d2,d1
	jne L24
	movel d3,sp@-
	jbsr _fflush
	addqw #4,sp
L24:
	pea __iob
	jbsr _fgetc
	movel d0,d2
	addqw #4,sp
	jge L28
L29:
	pea LC12
	movel d4,sp@-
	jbsr _fdopen
	movel d0,d3
	addqw #8,sp
	movel a6,d2
	addl #-1016,d2
	movel #__iob+20,d4
	jra L30
L32:
	movel d4,sp@-
	movel d2,sp@-
	jbsr _fputs
	movel d4,sp@-
	jbsr _fflush
	addw #12,sp
L30:
	movel d3,sp@-
	pea 1000:w
	movel d2,sp@-
	jbsr _fgets
	addw #12,sp
	tstl d0
	jne L32
	jbsr _finis
	moveml a6@(-1040),#0xc3c
	unlk a6
	rts
	.even
.globl _finis
_finis:
	link a6,#0
	pea _TtyBuf
	clrl sp@-
	jbsr _stty
	clrl sp@-
	jbsr _exit
	unlk a6
	rts
.lcomm _TtyBuf,6
