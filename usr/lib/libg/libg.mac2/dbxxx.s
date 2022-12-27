#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#ifndef lint
_sccsid:.asciz	"@(#)dbxxx.s	5.2 (Berkeley) 7/12/85"
#endif not lint

# make adb single-line stepping work properly

	.text
	.stabs	"libg.s",0x64,0,0,.
	.stabd	0x44,0,-1
