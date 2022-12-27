.\" Copyright (c) 1989 O'Reilly and Associates, Inc.
.\" See xtetris.c for complete rights and liability information.
.\"
.TH XTETRIS 1 "August 15, 1989"
.UC 5
.SH NAME
xtetris \- spacial reasoning game
.SH SYNOPSIS
xtetris 
[-pw \fIpixmapwidthincells\fR]
[-pixmapwidth \fIpixmapwidthincells\fR]
[-ph \fIpixmapheightincells\fR]
[-pixmapheight \fIpixmapheightincells\fR]
[-c \fIcellsizeinpixels\fR]
[-cellsize \fIcellsizeinpixels\fR]
[-level (1-9)]
(plus usual X Toolkit options)
.SH DESCRIPTION
.I xtetris
is a version of the game
.I tetris
available on Macintosh\(tm computers.
.I Tetris
is a trademark of AcademySoft-ELORG, copyright and trademark licensed
to Andromeda Software Ltd.  The original concept of the game is
by Alexi Pazhitnov and Vadim Gerasimov.
.LP
Blocks of various shapes fall through space (the window) onto the
ground (the bottom of the window).  You are able to steer the blocks 
(with the left and right 
buttons) and rotate them (with the center button), but not to stop 
them falling.  Blocks pile up on blocks already on the ground.
The goal is to pack them as efficiently as possible.  
Whenever you complete a row, the row is removed, and all blocks 
above fall down one row.  When the rows of blocks reach the top of 
the window, you lose.  Meanwhile, you get 1 point for each block 
safely guided to earth.  Obviously, then, the strategy is to complete 
(and remove) as many rows as possible so that there are always as few 
blocks on the ground as possible.  The challenge is to figure out
how to fit the shape that is falling into the shapes of the piled up
blocks on the ground.
.LP
The pace of the game gradually increases with time.  Its initial pace
is governed by the -level command line option or level resource.  
When you have positioned a block over the desired 
piece of real estate, you can "drop" the block by holding down the 
Shift key and then pressing the middle button.  You get an extra 
point each time you drop a block.  However, it gets harder to have time
to drop blocks as the pace increases.
.LP
Most of the application is implemented as a single widget; it can be
installed in any X Toolkit application.  All the command line
settings can also be set from any resource database file.  The 
application has buttons to start a new game, quit the game, pause the 
game, and show the current tally of high scores.
.LP
A keyboard interface is supplied.  The game is easier to play with the
keyboard because they can usually be clicked faster than mouse buttons.
The default keyboard interface is as follows:
.sp .5
.nf
Move Left:	j key or left arrow
Move Right:	l key or right arrow
Rotate Block:	k key or up arrow
Drop Block:	, key or space bar or down arrow
.fi
.sp .5
These interfaces are the same as in the Macintosh version of the game,
but can be changed by simply modifying the accelerator table in the 
application defaults file, or modifying a copy of it into another 
resource file that is merged later (such as .Xdefaults or one in the 
server set with
.I xrdb ).
.SH AUTHOR
Adrian Nye of O'Reilly and Associates, Inc.
.SH BUGS
High score table has bugs.  Also, it should save the high scores
in a file and read the file on startup.  Ideally, this should be done
in such a way that an entire network of machines shares one 
high-scores file.
