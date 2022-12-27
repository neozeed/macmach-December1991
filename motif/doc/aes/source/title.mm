.\"********************************************************************
.\"********************************************************************
.\"
.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.\"  
.\"  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
.\"  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
.\"  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
.\"  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
.\"  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
.\"  SOFTWARE IS HEREBY TRANSFERRED.
.\"  
.\"  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
.\"  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
.\"  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
.\"  
.\"  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
.\"  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
.\"  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
.\"  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
.\"  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
.\"  FITNESS FOR A PARTICULAR PURPOSE.
.\"  
.\"  Notice:  Notwithstanding any other lease or license that may pertain to,
.\"  or accompany the delivery of, this computer software, the rights of the
.\"  Government regarding its use, reproduction and disclosure are as set
.\"  forth in Section 52.227-19 of the FARS Computer Software-Restricted
.\"  Rights clause.
.\"  
.\"  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
.\"  rights reserved under the Copyright laws of the United States.
.\"  
.\"  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
.\"  Government is subject to the restrictions as set forth in subparagraph
.\"  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
.\"  at DFARS 52.227-7013.
.\"  
.\"  Open Software Foundation, Inc.
.\"  11 Cambridge Center
.\"  Cambridge, MA   02142
.\"  (617)621-8700
.\"  
.\"  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
.\"  "restricted rights."  Use, duplication or disclosure is subject to the
.\"  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
.\"  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
.\"  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
.\"  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
.\"  then the "Alternate III" clause applies.
.\"  
.\"  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
.\"  ALL RIGHTS RESERVED 
.\"  
.\"  
.\"Open Software Foundation is a trademark of The Open Software Foundation, Inc.
.\"OSF is a trademark of Open Software Foundation, Inc.
.\"OSF/Motif is a trademark of Open Software Foundation, Inc.
.\"Motif is a trademark of Open Software Foundation, Inc.
.\"DEC is a registered trademark of Digital Equipment Corporation
.\"DIGITAL is a registered trademark of Digital Equipment Corporation
.\"X Window System is a trademark of the Massachusetts Institute of Technology
.\"
.\"**********************************************************************
.\"**********************************************************************
.PH ""
.PF ""
.sp 2
.ad r
\&
.S 20
.sp |2.8i
.B "Application Environment Specification (AES)"
.sp .5
.B "User Environment Volume"
.S
.S 12
.SP 4
Revision B
.sp |9i
Open Software Foundation
.br
11 Cambridge Center
.br
Cambridge, MA 02142
.S
.ad
.bp
.ad b
The information contained within this document is subject to change
without notice.
.SP .8
.S 9
OSF MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS MATERIAL,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
.br
.S
.SP .5
OSF shall not be liable for errors contained herein or for any direct
or indirect, incidental, special or consequential damages in
connection with the furnishing, performance, or use of this material.
.br
.S 9
.SP .5
Copyright\(co 1990, 1991 Open Software Foundation, Inc.
...\"*********************************************************************
...\"       Select or add the copyrights that are appropriate for your
...\"       document.
...\"*********************************************************************
.br
Copyright\(co 1989 Digital Equipment Corporation
.br
Copyright\(co 1987, 1988, 1989 Hewlett-Packard Company
.br
Copyright\(co 1988 Massachusetts Institute of Technology 
.br
Copyright\(co 1988 Microsoft Corporation
.br
ALL RIGHTS RESERVED
.br
Printed in the U.S.A.
.SP .8
.S
.S 8
THIS DOCUMENT AND THE SOFTWARE DESCRIBED HEREIN ARE FURNISHED UNDER A
LICENSE, AND MAY BE USED AND COPIED ONLY IN ACCORDANCE WITH
THE TERMS OF SUCH LICENSE AND WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.
...\" Add the following lines if the document is only available to source
...\" licensees:
...\" THE DOCUMENT IS GOVERNED BY THOSE TERMS OF SUCH LICENSE APPLICABLE TO
...\" SOURCE CODE.  THIS DOCUMENT OR ANY OTHER COPIES THEREOF MAY NOT BE
...\" PROVIDED TO ANY OTHER PERSON WHO IS NOT ELIGIBLE TO RECEIVE SOURCE
...\" CODE COPIES OF THE ASSOCIATED SOFTWARE.
TITLE TO AND OWNERSHIP OF THE DOCUMENT AND SOFTWARE REMAIN WITH OSF OR
ITS LICENSORS.
.br
.S
.S 9
.SP .8
Open Software Foundation, OSF, the OSF logo, OSF/1, OSF/Motif, and Motif
are trademarks of Open Software Foundation, Inc.
...\"*********************************************************************
...\"       Select or add the trademarks that are appropriate for your
...\"       document.
...\"*********************************************************************
.br
DEC and DIGITAL are registered trademarks of Digital Equipment Corporation.
.br
X Window System is a trademark of the Massachusetts Institute of
Technology.
...\"************NEW TEXT - MANDATORY***********************************
.SP
.S
.S 8
FOR U.S. GOVERNMENT CUSTOMERS REGARDING THIS DOCUMENTATION AND THE 
ASSOCIATED SOFTWARE
.br
.S
.S 9
.SP .5
These notices shall be marked on any reproduction of this data, in whole 
or in part.
.SP.5
NOTICE:  Notwithstanding any other lease or license that may pertain to, 
or accompany the delivery of, this computer software, the rights of the 
Government regarding its use, reproduction and disclosure are as set 
forth in Section 52.227-19 of the FARS Computer Software-Restricted 
Rights clause.
.SP .5
RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the 
Government is subject to the restrictions as set forth in subparagraph 
(c)(1)(ii) of the Rights in Technical Data and Computer Software clause 
at DFARS 52.227-7013.
.SP .5
RESTRICTED RIGHTS LEGEND:  Use, duplication or disclosure by the 
Government is subject to restrictions as set forth in paragraph (b) 
(3)(B) of the rights in Technical Data and Computer Software clause in 
DAR 7-104.9(a).  This computer software is submitted with "restricted 
rights."  Use, duplication or disclosure is subject to the restrictions 
as set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial 
Computer Software-Restricted Rights (April 1985)."  If the 
contract contains the Clause at 18-52.227-74 "Rights in Data General" 
then the "Alternate III" clause applies.
.SP .5
US Government Users Restricted Rights - Use, duplication or disclosure 
restricted by GSA ADP Schedule Contract.
.br
Unpublished - All rights reserved under the Copyright Laws of the United
States.
.br
This notice shall be marked on any reproduction of this data, in whole or in 
part.
.S
...\"*****************END NEW TEXT*********************************
