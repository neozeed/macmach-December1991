.EH ''''
.OH ''''
.EF ''''
.OF ''''
.XS ii
Table of Contents
.XE
.XS iii
Acknowledgments
.XE
\&
.sp 1
.ce 3
\s+1\fBAcknowledgments\fP\s-1
.sp 2
.na
.LP
The design and implementation of the first 10 versions of X
were primarily the work of three individuals: Robert Scheifler of the
MIT  Laboratory for Computer Science and Jim Gettys of Digital
Equipment Corporation and Ron Newman of MIT, both at MIT
Project Athena. 
X version 11, however, is the result of the efforts of 
dozens of individuals at almost as many locations and organizations.
At the risk of offending some of the players by exclusion, 
we would like to acknowledge some of the people who deserve special credit 
and recognition.
Our apologies to anyone inadvertently overlooked.
.LP
First, our thanks goes to Phil Karlton
and Scott McGregor, both of Digital, for their considerable contributions
to the specification of the version 11 protocol.
Susan Angebranndt, Raymond Drewry, Todd Newman, and Phil Karlton of Digital 
worked long and hard to produce the sample server implementation.  
.LP
Next, our thanks goes to Ralph Swick (Project Athena and Digital) who kept 
it all together for us.
He handled literally thousands of requests from people everywhere
and saved the sanity of at least one of us.
His calm good cheer was a foundation on which we could build.
.LP
Our thanks also goes to Todd Brunhoff (Tektronix) who was ``loaned'' 
to Project Athena at exactly the right moment to provide very capable 
and much-needed assistance during the alpha and beta releases.
He was responsible for the successful integration of sources
from multiple sites;
we would not have had a release without him.
.LP
Our thanks also goes to Al Mento and Al Wojtas of Digital's ULTRIX 
Documentation Group.
With good humor and cheer,
they took a rough draft and made it an infinitely better and more useful 
document.
The work they have done will help many everywhere.
We also would like to thank Hal Murray (Digital SRC) and
Peter George (Digital VMS) who contributed much
by proofreading the early drafts of this document.
.LP
Our thanks also goes to Jeff Dike (Digital UEG), Tom Benson, 
Jackie Granfield, and Vince Orgovan (Digital VMS) who helped with the 
library utilities implementation;
to Hania Gajewska (Digital UEG-WSL) who, along with Ellis Cohen (CMU and Siemens),
was instrumental in the semantic design of the window manager properties;
and to Dave Rosenthal (Sun Microsystems) who also contributed to the protocol 
and provided the sample generic color frame buffer device-dependent code.
.LP
The alpha and beta test participants deserve special recognition and thanks as well.
It is significant
that the bug reports (and many fixes) during alpha and beta test came almost
exclusively from just a few of the alpha testers, mostly hardware vendors
working on product implementations of X.  
The continued public
contribution of vendors and universities is certainly to the benefit 
of the entire X community.
.\".LP
.\"The Version 11 protocol was authored by Bob Scheifler of MIT's Laboratory
.\"for computer science.
.\"Contributors to the design were:
.\".LP
.\".DS
.\".R
.\"Dave Carver (Digital HPW)
.\"Branko Gerovac (Digital HPW)
.\"Jim Gettys (MIT/Project Athena, Digital)
.\"Phil Karlton (Digital WSL)
.\"Scott McGregor (Digital SSG)
.\"Ram Rao (Digital UEG)
.\"David Rosenthal (Sun)
.\"Dave Winchell (Digital UEG)
.\".DE
.\".LP
.\"Invited reviewers who provided useful input:
.\".DS
.\".R
.\"Andrew Cherenson (Berkeley)
.\"Burns Fisher (Digital)
.\"Dan Garfinkel (HP)
.\"Leo Hourvitz (Next)
.\"Brock Krizan (HP)
.\"David Laidlaw (Stellar)
.\"Dave Mellinger (Interleaf)
.\"Ron Newman (MIT)
.\"John Ousterhout (Berkeley)
.\"Andrew Palay (ITC CMU)
.\"Ralph Swick (MIT/Project Athena, Digital)
.\"Craig Taylor (Sun)
.\"Jeffery Vroom (Stellar)
.\".DE
.LP
Our special thanks must go to Sam Fuller, Vice-President of Corporate
Research at Digital, who has remained committed to the widest public 
availability of X and who made it possible to greatly supplement MIT's
resources with the Digital staff in order to make version 11 a reality.
Many of the people mentioned here are part of the Western
Software Laboratory (Digital UEG-WSL) of the ULTRIX Engineering group
and work for Smokey Wallace, who has been vital to the project's success. 
Others not mentioned here worked on the toolkit and are acknowledged 
in the X Toolkit documentation.
.LP
Of course, 
we must particularly thank Paul Asente, formerly of Stanford University
and now of Digital UEG-WSL, who wrote W, the predecessor to X,
and Brian Reid, formerly of Stanford University and now of Digital WRL,
who had much to do with W's design.
.LP
Finally, our thanks goes to MIT,  Digital Equipment Corporation,
and IBM for providing the environment where it could happen.
.LP
.Ds 0
.TA 1.5i 3i
.ta 1.5i 3i
.R

Jim Gettys
Cambridge Research Laboratory
Digital Equipment Corporation

Robert W. Scheifler
Laboratory for Computer Science
Massachusetts Institute of Technology

Ron Newman
Project Athena
Massachusetts Institute of Technology

.DE
.bp 1
