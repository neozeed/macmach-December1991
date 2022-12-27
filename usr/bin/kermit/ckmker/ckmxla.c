/*  C K M X L A  */

/*  C-Kermit tables and functions supporting character set translation.  */
/*  Macintosh-only version */
/*
 Author: Frank da Cruz (fdc@columbia.edu, FDCCU@CUVMA.BITNET),
 Columbia University Center for Computing Activities.
 Copyright (C) 1989, Trustees of Columbia University in the City of New York.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided
 this copyright notice is retained. 
*/

#ifndef NULL
#define NULL 0
#endif

#include "ckcdeb.h"			/* Includes... */
#include "ckcker.h"

#ifdef COMMENT
#include "ckucmd.h"			/* PWP: feh!  Either it's a ckc file, or it's not. */
#endif

#include "ckcxla.h"

/* Character set translation data and functions */

extern int zincnt;
extern CHAR *zinptr;

int tslevel  = TS_L0;			/* Transfer syntax level (0,1,2) */
int tcharset = TC_USASCII;		/* Transfer syntax character set */
int fcharset = FC_USASCII;		/* Local file character set */
int language = L_USASCII;		/* Language */

/* Nota Bene: I6/100 is the proper designation for Latin-1, but MS-DOS */
/* Kermit 3.00 uses I2/100 (this will be fixed in 3.01).  This table */
/* allows C-Kermit to recognize either one when receiving a file, but */
/* when sending, C-Kermit uses I6/100, which MS-DOS Kermit 3.00 will not */
/* recognize.  Workaround: in MS-DOS Kermit, SET TRANSF CHAR LATIN1, */
/* SET ATTR CHAR OFF, or apply the MSKERMIT.PCH patch. */

/* This list is indexed by the transfer character set number. */
/* The third entry is a kludge explained immediately above, and must be */
/* removed before additional transfer character sets are added. */

struct csinfo tcsinfo[] = {		/* Transfer character-set info */
    "Transparent text",		256, TC_TRANSP,  "",		/* 0 */
    "ASCII 7-bit text",		256, TC_USASCII, "",		/* 1 */        
    "Latin-1 ISO 8859-1 text",	256, TC_1LATIN, "I6/100",	/* 2 */
    "Latin-1 ISO 8859-1 text",	256, TC_1LATIN, "I2/100"	/* kludge */
};
int ntcsets = (sizeof(tcsinfo) / sizeof(struct csinfo));

/* Grrr... Had to back off on moving this to ckuxla.h because that file */
/* is included by more than one module, so link complains about multiple */
/* definitions of _fcsinfo, _fcstab, etc. */

/* File character set information structure, indexed by character set code, */
/* as defined immediately above.  This table must be in order of character */
/* set number! */ 

struct csinfo fcsinfo[] = { /* File character set information... */
  /* Descriptive Name              Size  Designator */
    "US ASCII",                     128, FC_USASCII, NULL,
    "ISO Latin-1",                  256, FC_1LATIN,  NULL,
    "US Macintosh",                 256, FC_USMAC,  NULL
};    

/* Local file character sets */
/* Includes 7-bit National Replacement Character Sets of ISO 646 */
/* Plus ISO Latin-1 and DEC Multinational Character Set (MCS) */

#ifdef COMMENT

struct keytab fcstab[] = { /* Keyword table for 'set file character-set' */

/* Keyword             Value       Flags */
    "ascii",            FC_USASCII, 0,	/* ASCII */
    "latin-1",          FC_1LATIN,  0,	/* ISO Latin Alphabet 1 */
    "macintosh",        FC_USMAC,   0,	/* Norwegian and Danish NRC */
};
int nfilc = (sizeof(fcstab) / sizeof(struct keytab)); /* size of this table */

#endif

/*
 Languages:

 This table serves two purposes.  First, it allows C-Kermit to have a
 SET LANGUAGE command, which automatically selects the associated file
 character set and transfer character set.  Second, it allows the program
 to apply special language-specific rules when translating from a character
 set that contains national characters into plain ASCII, like German umlaut-a
 becomes ae.
*/

struct langinfo langs[] = {
/*  Language code   File Charset Xfer Charset Name */
    L_USASCII,      FC_USASCII,  TC_USASCII,  "ASCII (American English)",
    L_BRITISH,      FC_USMAC,  TC_1LATIN,   "British (English)",
    L_DANISH,       FC_USMAC,  TC_1LATIN,   "Danish",
    L_DUTCH,        FC_USMAC,  TC_1LATIN,   "Dutch",
    L_FINNISH,      FC_USMAC,  TC_1LATIN,   "Finnish",
    L_FRENCH,       FC_USMAC,  TC_1LATIN,   "French",
    L_FR_CANADIAN,  FC_USMAC,  TC_1LATIN,   "French-Canadian",
    L_GERMAN,       FC_USMAC,  TC_1LATIN,   "German",
    L_HUNGARIAN,    FC_USMAC,  TC_1LATIN,   "Hungarian",
    L_ITALIAN,      FC_USMAC,  TC_1LATIN,   "Italian",
    L_NORWEGIAN,    FC_USMAC,  TC_1LATIN,   "Norwegian",
    L_PORTUGUESE,   FC_USMAC,  TC_1LATIN,   "Portuguese",
    L_SPANISH,      FC_USMAC,  TC_1LATIN,   "Spanish",
    L_SWEDISH,      FC_USMAC,  TC_1LATIN,   "Swedish",
    L_SWISS,        FC_USMAC,  TC_1LATIN,   "Swiss"
};

/* Translation tables ... */

/*
  Note, many more can and should be added, space permitting!
  Presently we have only ASCII and Latin-1 as transfer character sets
  and ASCII, Latin-1, and the Adobe-extended-Macintosh set as file character sets.
  For each pair of (file,transfer) character sets, we need two translation
  functions, one for sending, one for receiving.  It is recommended that
  functions and tables for all computers be included in this file, perhaps
  within #ifdef's, so that corrections need be made only in one place.
*/

/* Here is the first table, fully annotated... */

CHAR
yl1toas[] = {  /* ISO 8859-1 Latin-1 to ascii */
      /*  Source character    Description               => Translation */
      /*  Dec row/col Set                                           */
  0,  /*  000  00/00  C0 NUL  Ctrl-@                    =>  (self)  */
  1,  /*  001  00/01  C0 SOH  Ctrl-A                    =>  (self)  */
  2,  /*  002  00/02  C0 STX  Ctrl-B                    =>  (self)  */
  3,  /*  003  00/03  C0 ETX  Ctrl-C                    =>  (self)  */
  4,  /*  004  00/04  C0 EOT  Ctrl-D                    =>  (self)  */
  5,  /*  005  00/05  C0 ENQ  Ctrl-E                    =>  (self)  */
  6,  /*  006  00/06  C0 ACK  Ctrl-F                    =>  (self)  */
  7,  /*  007  00/07  C0 BEL  Ctrl-G                    =>  (self)  */
  8,  /*  008  00/08  C0 BS   Ctrl-H                    =>  (self)  */
  9,  /*  009  00/09  C0 HT   Ctrl-I                    =>  (self)  */
 10,  /*  010  00/10  C0 LF   Ctrl-J                    =>  (self)  */
 11,  /*  011  00/11  C0 VT   Ctrl-K                    =>  (self)  */
 12,  /*  012  00/12  C0 FF   Ctrl-L                    =>  (self)  */
 13,  /*  013  00/13  C0 CR   Ctrl-M                    =>  (self)  */
 14,  /*  014  00/14  C0 SO   Ctrl-N                    =>  (self)  */
 15,  /*  015  00/15  C0 SI   Ctrl-O                    =>  (self)  */
 16,  /*  016  01/00  C0 DLE  Ctrl-P                    =>  (self)  */
 17,  /*  017  01/01  C0 DC1  Ctrl-Q                    =>  (self)  */
 18,  /*  018  01/02  C0 DC2  Ctrl-R                    =>  (self)  */
 19,  /*  019  01/03  C0 DC3  Ctrl-S                    =>  (self)  */
 20,  /*  020  01/04  C0 DC4  Ctrl-T                    =>  (self)  */
 21,  /*  021  01/05  C0 NAK  Ctrl-U                    =>  (self)  */
 22,  /*  022  01/06  C0 SYN  Ctrl-V                    =>  (self)  */
 23,  /*  023  01/07  C0 ETB  Ctrl-W                    =>  (self)  */
 24,  /*  024  01/08  C0 CAN  Ctrl-X                    =>  (self)  */
 25,  /*  025  01/09  C0 EM   Ctrl-Y                    =>  (self)  */
 26,  /*  026  01/10  C0 SUB  Ctrl-Z                    =>  (self)  */
 27,  /*  027  01/11  C0 ESC  Ctrl-[                    =>  (self)  */
 28,  /*  028  01/12  C0 FS   Ctrl-\                    =>  (self)  */
 29,  /*  029  01/13  C0 GS   Ctrl-]                    =>  (self)  */
 30,  /*  030  01/14  C0 RS   Ctrl-^                    =>  (self)  */
 31,  /*  031  01/15  C0 US   Ctrl-_                    =>  (self)  */
 32,  /*  032  02/00     SP   Space                     =>  (self)  */
 33,  /*  033  02/01  G0 !    Exclamation mark          =>  (self)  */
 34,  /*  034  02/02  G0 "    Doublequote               =>  (self)  */
 35,  /*  035  02/03  G0 #    Number sign               =>  (self)  */
 36,  /*  036  02/04  G0 $    Dollar sign               =>  (self)  */
 37,  /*  037  02/05  G0 %    Percent sign              =>  (self)  */
 38,  /*  038  02/06  G0 &    Ampersand                 =>  (self)  */
 39,  /*  039  02/07  G0 '    Apostrophe                =>  (self)  */
 40,  /*  040  02/08  G0 (    Left parenthesis          =>  (self)  */
 41,  /*  041  02/09  G0 )    Right parenthesis         =>  (self)  */
 42,  /*  042  02/10  G0 *    Asterisk                  =>  (self)  */
 43,  /*  043  02/11  G0 +    Plus sign                 =>  (self)  */
 44,  /*  044  02/12  G0 ,    Comma                     =>  (self)  */
 45,  /*  045  02/13  G0 -    Hyphen, minus sign        =>  (self)  */
 46,  /*  046  02/14  G0 .    Period, full stop         =>  (self)  */
 47,  /*  047  02/15  G0 /    Slash, solidus            =>  (self)  */
 48,  /*  048  03/00  G0 0    Digit 0                   =>  (self)  */
 49,  /*  049  03/01  G0 1    Digit 1                   =>  (self)  */
 50,  /*  050  03/02  G0 2    Digit 2                   =>  (self)  */
 51,  /*  051  03/03  G0 3    Digit 3                   =>  (self)  */
 52,  /*  052  03/04  G0 4    Digit 4                   =>  (self)  */
 53,  /*  053  03/05  G0 5    Digit 5                   =>  (self)  */
 54,  /*  054  03/06  G0 6    Digit 6                   =>  (self)  */
 55,  /*  055  03/07  G0 7    Digit 7                   =>  (self)  */
 56,  /*  056  03/08  G0 8    Digit 8                   =>  (self)  */
 57,  /*  057  03/09  G0 9    Digit 9                   =>  (self)  */
 58,  /*  058  03/10  G0 :    Colon                     =>  (self)  */
 59,  /*  059  03/11  G0 ;    Semicolon                 =>  (self)  */
 60,  /*  060  03/12  G0 <    Less-than sign            =>  (self)  */
 61,  /*  061  03/13  G0 =    Equals sign               =>  (self)  */
 62,  /*  062  03/14  G0 >    Greater-than sign         =>  (self)  */
 63,  /*  063  03/15  G0 ?    Question mark             =>  (self)  */
 64,  /*  064  04/00  G0 @    Commercial at sign        =>  (self)  */
 65,  /*  065  04/01  G0 A    Letter A                  =>  (self)  */
 66,  /*  066  04/02  G0 B    Letter B                  =>  (self)  */
 67,  /*  067  04/03  G0 C    Letter C                  =>  (self)  */
 68,  /*  068  04/04  G0 D    Letter D                  =>  (self)  */
 69,  /*  069  04/05  G0 E    Letter E                  =>  (self)  */
 70,  /*  070  04/06  G0 F    Letter F                  =>  (self)  */
 71,  /*  071  04/07  G0 G    Letter G                  =>  (self)  */
 72,  /*  072  04/08  G0 H    Letter H                  =>  (self)  */
 73,  /*  073  04/09  G0 I    Letter I                  =>  (self)  */
 74,  /*  074  04/10  G0 J    Letter J                  =>  (self)  */
 75,  /*  075  04/11  G0 K    Letter K                  =>  (self)  */
 76,  /*  076  04/12  G0 L    Letter L                  =>  (self)  */
 77,  /*  077  04/13  G0 M    Letter M                  =>  (self)  */
 78,  /*  078  04/14  G0 N    Letter N                  =>  (self)  */
 79,  /*  079  04/15  G0 O    Letter O                  =>  (self)  */
 80,  /*  080  05/00  G0 P    Letter P                  =>  (self)  */
 81,  /*  081  05/01  G0 Q    Letter Q                  =>  (self)  */
 82,  /*  082  05/02  G0 R    Letter R                  =>  (self)  */
 83,  /*  083  05/03  G0 S    Letter S                  =>  (self)  */
 84,  /*  084  05/04  G0 T    Letter T                  =>  (self)  */
 85,  /*  085  05/05  G0 U    Letter U                  =>  (self)  */
 86,  /*  086  05/06  G0 V    Letter V                  =>  (self)  */
 87,  /*  087  05/07  G0 W    Letter W                  =>  (self)  */
 88,  /*  088  05/08  G0 X    Letter X                  =>  (self)  */
 89,  /*  089  05/09  G0 Y    Letter Y                  =>  (self)  */
 90,  /*  090  05/10  G0 Z    Letter Z                  =>  (self)  */
 91,  /*  091  05/11  G0 [    Left square bracket       =>  (self)  */
 92,  /*  092  05/12  G0 \    Reverse slash             =>  (self)  */
 93,  /*  093  05/13  G0 ]    Right square bracket      =>  (self)  */
 94,  /*  094  05/14  G0 ^    Circumflex accent         =>  (self)  */
 95,  /*  095  05/15  G0 _    Underline, low line       =>  (self)  */
 96,  /*  096  06/00  G0 `    Grave accent              =>  (self)  */
 97,  /*  097  06/01  G0 a    Letter a                  =>  (self)  */
 98,  /*  098  06/02  G0 b    Letter b                  =>  (self)  */
 99,  /*  099  06/03  G0 c    Letter c                  =>  (self)  */
100,  /*  100  06/04  G0 d    Letter d                  =>  (self)  */
101,  /*  101  06/05  G0 e    Letter e                  =>  (self)  */
102,  /*  102  06/06  G0 f    Letter f                  =>  (self)  */
103,  /*  103  06/07  G0 g    Letter g                  =>  (self)  */
104,  /*  104  06/08  G0 h    Letter h                  =>  (self)  */
105,  /*  105  06/09  G0 i    Letter i                  =>  (self)  */
106,  /*  106  06/10  G0 j    Letter j                  =>  (self)  */
107,  /*  107  06/11  G0 k    Letter k                  =>  (self)  */
108,  /*  108  06/12  G0 l    Letter l                  =>  (self)  */
109,  /*  109  06/13  G0 m    Letter m                  =>  (self)  */
110,  /*  110  06/14  G0 n    Letter n                  =>  (self)  */
111,  /*  111  06/15  G0 o    Letter o                  =>  (self)  */
112,  /*  112  07/00  G0 p    Letter p                  =>  (self)  */
113,  /*  113  07/01  G0 q    Letter q                  =>  (self)  */
114,  /*  114  07/02  G0 r    Letter r                  =>  (self)  */
115,  /*  115  07/03  G0 s    Letter s                  =>  (self)  */
116,  /*  116  07/04  G0 t    Letter t                  =>  (self)  */
117,  /*  117  07/05  G0 u    Letter u                  =>  (self)  */
118,  /*  118  07/06  G0 v    Letter v                  =>  (self)  */
119,  /*  119  07/07  G0 w    Letter w                  =>  (self)  */
120,  /*  120  07/08  G0 x    Letter x                  =>  (self)  */
121,  /*  121  07/09  G0 y    Letter y                  =>  (self)  */
122,  /*  122  07/10  G0 z    Letter z                  =>  (self)  */
123,  /*  123  07/11  G0 {    Left curly bracket        =>  (self)  */
124,  /*  124  07/12  G0 |    Vertical bar              =>  (self)  */
125,  /*  125  07/13  G0 }    Right curly bracket       =>  (self)  */
126,  /*  126  07/14  G0 ~    Tilde                     =>  (self)  */
127,  /*  127  07/15     DEL  Delete, Rubout            =>  (self)  */
128,  /*  128  08/00  C1      Ctrl-Meta-@               =>  (self)  */
129,  /*  129  08/01  C1      Ctrl-Meta-A               =>  (self)  */
130,  /*  130  08/02  C1      Ctrl-Meta-B               =>  (self)  */
131,  /*  131  08/03  C1      Ctrl-Meta-C               =>  (self)  */
132,  /*  132  08/04  C1 IND  Ctrl-Meta-D               =>  (self)  */
133,  /*  133  08/05  C1 NEL  Ctrl-Meta-E               =>  (self)  */
134,  /*  134  08/06  C1 SSA  Ctrl-Meta-F               =>  (self)  */
135,  /*  135  08/07  C1 ESA  Ctrl-Meta-G               =>  (self)  */
136,  /*  136  08/08  C1 HTS  Ctrl-Meta-H               =>  (self)  */
137,  /*  137  08/09  C1      Ctrl-Meta-I               =>  (self)  */
138,  /*  138  08/10  C1      Ctrl-Meta-J               =>  (self)  */
139,  /*  139  08/11  C1      Ctrl-Meta-K               =>  (self)  */
140,  /*  140  08/12  C1      Ctrl-Meta-L               =>  (self)  */
141,  /*  141  08/13  C1 RI   Ctrl-Meta-M               =>  (self)  */
142,  /*  142  08/14  C1 SS2  Ctrl-Meta-N               =>  (self)  */
143,  /*  143  08/15  C1 SS3  Ctrl-Meta-O               =>  (self)  */
144,  /*  144  09/00  C1 DCS  Ctrl-Meta-P               =>  (self)  */
145,  /*  145  09/01  C1      Ctrl-Meta-Q               =>  (self)  */
146,  /*  146  09/02  C1      Ctrl-Meta-R               =>  (self)  */
147,  /*  147  09/03  C1 STS  Ctrl-Meta-S               =>  (self)  */
148,  /*  148  09/04  C1      Ctrl-Meta-T               =>  (self)  */
149,  /*  149  09/05  C1      Ctrl-Meta-U               =>  (self)  */
150,  /*  150  09/06  C1 SPA  Ctrl-Meta-V               =>  (self)  */
151,  /*  151  09/07  C1 EPA  Ctrl-Meta-W               =>  (self)  */
152,  /*  152  09/08  C1      Ctrl-Meta-X               =>  (self)  */
153,  /*  153  09/09  C1      Ctrl-Meta-Y               =>  (self)  */
154,  /*  154  09/10  C1      Ctrl-Meta-Z               =>  (self)  */
155,  /*  155  09/11  C1 CSI  Ctrl-Meta-[               =>  (self)  */
156,  /*  156  09/12  C1 ST   Ctrl-Meta-\               =>  (self)  */
157,  /*  157  09/13  C1 OSC  Ctrl-Meta-]               =>  (self)  */
158,  /*  158  09/14  C1 PM   Ctrl-Meta-^               =>  (self)  */
159,  /*  159  09/15  C1 APC  Ctrl-Meta-_               =>  (self)  */
 32,  /*  160  10/00  G1      No-break space            =>  SP      */
 33,  /*  161  10/01  G1      Inverted exclamation      =>  !       */
UNK,  /*  162  10/02  G1      Cent sign                 =>  UNK     */
UNK,  /*  163  10/03  G1      Pound sign                =>  UNK     */
UNK,  /*  164  10/04  G1      Currency sign             =>  UNK     */
UNK,  /*  165  10/05  G1      Yen sign                  =>  UNK     */
124,  /*  166  10/06  G1      Broken bar                =>  |       */
UNK,  /*  167  10/07  G1      Paragraph sign            =>  UNK     */
 34,  /*  168  10/08  G1      Diaeresis                 =>  "       */
 67,  /*  169  10/09  G1      Copyright sign            =>  C       */
UNK,  /*  170  10/10  G1      Feminine ordinal          =>  UNK     */
 34,  /*  171  10/11  G1      Left angle quotation      =>  "       */
126,  /*  172  10/12  G1      Not sign                  =>  ~       */
 45,  /*  173  10/13  G1      Soft hyphen               =>  -       */
 82,  /*  174  10/14  G1      Registered trade mark     =>  R       */
UNK,  /*  175  10/15  G1      Macron                    =>  UNK     */
UNK,  /*  176  11/00  G1      Degree sign, ring above   =>  UNK     */
UNK,  /*  177  11/01  G1      Plus-minus sign           =>  UNK     */
UNK,  /*  178  11/02  G1      Superscript two           =>  UNK     */
UNK,  /*  179  11/03  G1      Superscript three         =>  UNK     */
 39,  /*  180  11/04  G1      Acute accent              =>  '       */
117,  /*  181  11/05  G1      Micro sign                =>  u       */
UNK,  /*  182  11/06  G1      Pilcrow sign              =>  UNK     */
UNK,  /*  183  11/07  G1      Middle dot                =>  UNK     */
 44,  /*  184  11/08  G1      Cedilla                   =>  ,       */
UNK,  /*  185  11/09  G1      Superscript one           =>  UNK     */
UNK,  /*  186  11/10  G1      Masculine ordinal         =>  UNK     */
 34,  /*  187  11/11  G1      Right angle quotation     =>  "       */
UNK,  /*  188  11/12  G1      One quarter               =>  UNK     */
UNK,  /*  189  11/13  G1      One half                  =>  UNK     */
UNK,  /*  190  11/14  G1      Three quarters            =>  UNK     */
 63,  /*  191  11/15  G1      Inverted question mark    =>  ?       */
 65,  /*  192  12/00  G1      A grave                   =>  A       */
 65,  /*  193  12/01  G1      A acute                   =>  A       */
 65,  /*  194  12/02  G1      A circumflex              =>  A       */
 65,  /*  195  12/03  G1      A tilde                   =>  A       */
 65,  /*  196  12/04  G1      A diaeresis               =>  A       */
 65,  /*  197  12/05  G1      A ring above              =>  A       */
 65,  /*  198  12/06  G1      A with E                  =>  A       */
 67,  /*  199  12/07  G1      C Cedilla                 =>  C       */
 69,  /*  200  12/08  G1      E grave                   =>  E       */
 69,  /*  201  12/09  G1      E acute                   =>  E       */
 69,  /*  202  12/10  G1      E circumflex              =>  E       */
 69,  /*  203  12/11  G1      E diaeresis               =>  E       */
 73,  /*  204  12/12  G1      I grave                   =>  I       */
 73,  /*  205  12/13  G1      I acute                   =>  I       */
 73,  /*  206  12/14  G1      I circumflex              =>  I       */
 73,  /*  207  12/15  G1      I diaeresis               =>  I       */
UNK,  /*  208  13/00  G1      Icelandic Eth             =>  UNK     */
 78,  /*  209  13/01  G1      N tilde                   =>  N       */
 79,  /*  210  13/02  G1      O grave                   =>  O       */
 79,  /*  211  13/03  G1      O acute                   =>  O       */
 79,  /*  212  13/04  G1      O circumflex              =>  O       */
 79,  /*  213  13/05  G1      O tilde                   =>  O       */
 79,  /*  214  13/06  G1      O diaeresis               =>  O       */
120,  /*  215  13/07  G1      Multiplication sign       =>  x       */
 79,  /*  216  13/08  G1      O oblique stroke          =>  O       */
 85,  /*  217  13/09  G1      U grave                   =>  U       */
 85,  /*  218  13/10  G1      U acute                   =>  U       */
 85,  /*  219  13/11  G1      U circumflex              =>  U       */
 85,  /*  220  13/12  G1      U diaeresis               =>  U       */
 89,  /*  221  13/13  G1      Y acute                   =>  Y       */
UNK,  /*  222  13/14  G1      Icelandic Thorn           =>  UNK     */
115,  /*  223  13/15  G1      German sharp s            =>  s       */
 97,  /*  224  14/00  G1      a grave                   =>  a       */
 97,  /*  225  14/01  G1      a acute                   =>  a       */
 97,  /*  226  14/02  G1      a circumflex              =>  a       */
 97,  /*  227  14/03  G1      a tilde                   =>  a       */
 97,  /*  228  14/04  G1      a diaeresis               =>  a       */
 97,  /*  229  14/05  G1      a ring above              =>  a       */
 97,  /*  230  14/06  G1      a with e                  =>  a       */
 99,  /*  231  14/07  G1      c cedilla                 =>  c       */
101,  /*  232  14/08  G1      e grave                   =>  e       */
101,  /*  233  14/09  G1      e acute                   =>  e       */
101,  /*  234  14/10  G1      e circumflex              =>  e       */
101,  /*  235  14/11  G1      e diaeresis               =>  e       */
105,  /*  236  14/12  G1      i grave                   =>  i       */
105,  /*  237  14/13  G1      i acute                   =>  i       */
105,  /*  238  14/14  G1      i circumflex              =>  i       */
105,  /*  239  14/15  G1      i diaeresis               =>  i       */
UNK,  /*  240  15/00  G1      Icelandic eth             =>  UNK     */
110,  /*  241  15/01  G1      n tilde                   =>  n       */
111,  /*  242  15/02  G1      o grave                   =>  o       */
111,  /*  243  15/03  G1      o acute                   =>  o       */
111,  /*  244  15/04  G1      o circumflex              =>  o       */
111,  /*  245  15/05  G1      o tilde                   =>  o       */
111,  /*  246  15/06  G1      o diaeresis               =>  o       */
 47,  /*  247  15/07  G1      Division sign             =>  /       */
111,  /*  248  15/08  G1      o oblique stroke          =>  o       */
117,  /*  249  15/09  G1      u grave                   =>  u       */
117,  /*  250  15/10  G1      u acute                   =>  u       */
117,  /*  251  15/11  G1      u circumflex              =>  u       */
117,  /*  252  15/12  G1      u diaeresis               =>  u       */
121,  /*  253  15/13  G1      y acute                   =>  y       */
UNK,  /*  254  15/14  G1      Icelandic thorn           =>  UNK     */
121   /*  255  15/15  G1      y diaeresis               =>  y       */
};

/* Translation tables for ISO Latin Alphabet 1 to local file character sets */

/*
 * These tables were devoped by Andr'e Pirard <A-PIRARD@BLIULG11.BITNET>, modeled
 * after a first stab by Paul Placeway, the things that Apple File Exchange does,
 * and the desire to translate back and forth to an ISO Latin-1 speaking PC in a
 * completely byte-wize revertable fasion.
 */

CHAR
yl1toam[] = {  /* Latin-1 to Apple (adobe-extended) Macintosh Character Set */
  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,

	0xA5,	/* 80 Center Box Bar Vert D ==> bullet */
	0xAA,	/* 81 Center Box Bar Horiz D ==> trade mark */
	0xAD,	/* 82 Upp Left Box Corner D ==> not equal */
	0xB0,	/* 83 Upper Right Box Corn D ==> infinity */
	0xB3,	/* 84 Low Left Box Corner D ==> greater than or equal to */
	0xB7,	/* 85 Lower Right Box Corn D ==> Uppercase Sigma (Summation) */
	0xBA,	/* 86 Left Box Side Double ==> integral */
	0xBD,	/* 87 Right Box Side Double ==> Uppercase Omega */
	0xC3,	/* 88 Middle Box Top Double ==> radical (square root) */
	0xC5,	/* 89 Middle Box Bottom D ==> approx equal */
	0xC9,	/* 8A Box Intersection Doubl ==> elipsis (...) */
	0xD1,	/* 8B Solid Fill Ch,Top Half ==> em dash */
	0xD4,	/* 8C Solid Fill Ch,Bottom H ==> left singlequote ( ` ) */
	0xD9,	/* 8D Solid Fill Character ==> Y dieresis */
	0xDA,	/* 8E Small Solid Square ==> divide (a / with less slope) */
	0xDC,	/* 8F Double Underscore ==> single left guil (like < ) */
	0xDD,	/* 90 Center Box Bar Vertic ==> single left guil (like > ) */
	0xE0,	/* 91 Center Box Bar Horiz ==> double dagger */
	0xE2,	/* 92 Upper Left Box Corner ==> baseline single close quote */
	0xE3,	/* 93 Upper Right Box Corner ==> baseline double close quote */
	0xE4,	/* 94 Lower Left Box Corner ==> per thousand */
	0xF0,	/* 95 Lower Right Box Corner ==> (closed) Apple */
	0xF6,	/* 96 Left Middle Box Side ==> circumflex */
	0xF7,	/* 97 Right Middle Box Side ==> tilde */
	0xF9,	/* 98 Middle Box Top ==> breve */
	0xFA,	/* 99 Middle Box Bottom ==> dot accent */
	0xFB,	/* 9A Box Intersection ==> ring accent */
	0xFD,	/* 9B Fill Character, Light ==> Hungarian umlaut */
	0xFE,	/* 9C Fill Character, Medium ==> ogonek */
	0xFF,	/* 9D Fill Character, Heavy ==> caron */
	0xF5,	/* 9E i dotless small ==> dotless i */
	0xC4,	/* 9F Florin Sign ==> florin */
	0xCA,	/* A0 required space ==> non-printing space */
	0xC1,	/* A1 exclamation point inv ==> inverted ! */
	0xA2,	/* A2 cent sign ==> cent */
	0xA3,	/* A3 pound sign ==> sterling */
	0xDB,	/* A4 int. currency symbol ==> generic curency */
	0xB4,	/* A5 Yen sign ==> yen */
	0xA0,	/* A6 Vertical Line, Broken ==> dagger */
	0xA4,	/* A7 section/paragraph symb ==> section */
	0xAC,	/* A8 diaeresis,umlaut acc ==> dieresis (AKA umlaut) */
	0xA9,	/* A9 Copyright sign ==> copyright   ( (C) ) */
	0xBB,	/* AA ordinal indicator fem ==> feminine ordinal */
	0xC7,	/* AB left angle quotes ==> left guillemot (like << ) */
	0xC2,	/* AC logical NOT, EOL symb ==> logical not */
	0xD0,	/* AD Syllabe Hyphen ==> en dash */
	0xA8,	/* AE Regist.Trade Mark sym ==> registered  ( (R) ) */
	0xF8,	/* AF overline ==> macron */
	0xA1,	/* B0 Degree Symbol ==> superscript ring */
	0xB1,	/* B1 plus or minus sign ==> plus minus */
	0xD3,	/* B2 2 superscript ==> right doublequote ( '' ) */
	0xD2,	/* B3 3 superscript ==> left doublequote ( `` ) */
	0xAB,	/* B4 acute accent ==> acute accent */
	0xB5,	/* B5 micro symbol ==> greek lowercase mu */
	0xA6,	/* B6 paragraph symbol USA ==> paragraph */
	0xE1,	/* B7 Middle dot accent ==> centered (small) dot */
	0xFC,	/* B8 cedilla accent ==> cedilla */
	0xD5,	/* B9 1 superscript ==> right singlequote ( ' ) */
	0xBC,	/* BA ordinal indicator mas ==> masculine ordinal */
	0xC8,	/* BB right angle quotes ==> right guillemot (like >> ) */
	0xB9,	/* BC one quarter ==> lowercase pi */
	0xB8,	/* BD one half ==> Uppercase Pi (Power) */
	0xB2,	/* BE three quarters ==> less than or equal to */
	0xC0,	/* BF Question mark inverted ==> inverted ? */
	0xCB,	/* C0 A grave capital ==> A grave */
	0xE7,	/* C1 A acute capital ==> A accute */
	0xE5,	/* C2 A circumflex capital ==> A circumflex */
	0xCC,	/* C3 A tilde capital ==> A tilde */
	0x80,	/* C4 A diaeresis capital ==> A dieresis */
	0x81,	/* C5 A overcircle capital ==> A ring */
	0xAE,	/* C6 AE diphthong capital ==> AE */
	0x82,	/* C7 C cedilla capital ==> C cedilla */
	0xE9,	/* C8 E grave capital ==> E grave */
	0x83,	/* C9 E acute capital ==> E accute */
	0xE6,	/* CA E circumflex capital ==> E circumflex */
	0xE8,	/* CB E diaeresis capital ==> E dieresis */
	0xED,	/* CC I grave capital ==> I grave */
	0xEA,	/* CD I acute capital ==> I accute */
	0xEB,	/* CE I circumflex capital ==> I circumflex */
	0xEC,	/* CF I diaeresis capital ==> I dieresis */
	0xC6,	/* D0 Eth islandic capital ==> Uppercase Delta */
	0x84,	/* D1 N tilde capital ==> N tilde */
	0xF1,	/* D2 O grave capital ==> O grave */
	0xEE,	/* D3 O acute capital ==> O accute */
	0xEF,	/* D4 O circumflex capital ==> O circumflex */
	0xCD,	/* D5 O tilde capital ==> O tilde */
	0x85,	/* D6 O diaeresis capital ==> O dieresis */
	0xD7,	/* D7 Multiply sign ==> lozenge (open diamond) */
	0xAF,	/* D8 O slash capital ==> O slash */
	0xF4,	/* D9 U grave capital ==> U grave */
	0xF2,	/* DA U acute capital ==> U accute */
	0xF3,	/* DB U circumflex capital ==> U circumflex */
	0x86,	/* DC U diaeresis capital ==> U dieresis */
	0xDF,	/* DD Y acute Capital ==> fl */
	0xCE,	/* DE Thorn islandic capital ==> OE */
	0xA7,	/* DF sharp s small ==> Es-sed (German double s) */
	0x88,	/* E0 a grave small ==> a grave */
	0x87,	/* E1 a acute small ==> a accute */
	0x89,	/* E2 a circumflex small ==> a circumflex */
	0x8B,	/* E3 a tilde small ==> a tilde */
	0x8A,	/* E4 a diaeresis small ==> a dieresis */
	0x8C,	/* E5 a overcircle small ==> a ring */
	0xBE,	/* E6 ae diphthong small ==> ae */
	0x8D,	/* E7 c cedilla small ==> c cedilla */
	0x8F,	/* E8 e grave small ==> e grave */
	0x8E,	/* E9 e acute small ==> e accute */
	0x90,	/* EA e circumflex small ==> e circumflex */
	0x91,	/* EB e diaeresis small ==> e dieresis */
	0x93,	/* EC i grave small ==> i grave */
	0x92,	/* ED i acute small ==> i accute */
	0x94,	/* EE i circumflex small ==> i circumflex */
	0x95,	/* EF i diaeresis small ==> i dieresis */
	0xB6,	/* F0 Eth Islandic small ==> partial */
	0x96,	/* F1 n tilde small ==> n tilde */
	0x98,	/* F2 o grave small ==> o grave */
	0x97,	/* F3 o acute small ==> o accute */
	0x99,	/* F4 o circumflex small ==> o circumflex */
	0x9B,	/* F5 o tilde small ==> o tilde */
	0x9A,	/* F6 o diaeresis small ==> o dieresis */
	0xD6,	/* F7 Divide sign ==> divide */
	0xBF,	/* F8 o slash small ==> o slash */
	0x9D,	/* F9 u grave small ==> u grave */
	0x9C,	/* FA u acute small ==> u accute */
	0x9E,	/* FB u circumflex small ==> u circumflex */
	0x9F,	/* FC u diaeresis small ==> u dieresis */
	0xDE,	/* FD y acute small ==> fi */
	0xCF,	/* FE Thorn islandic small ==> oe */
	0xD8	/* FF y diaeresis small ==> y dieresis */
};

/* Local file character sets to ISO Latin Alphabet 1 */

CHAR
yastol1[] = {  /* ASCII to Latin-1 */
  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127
};


CHAR
yamtol1[] = {  /* Apple Macintosh Character Set to Latin-1 */
  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,

	0xC4,	/* 80 A dieresis ==> A diaeresis capital */
	0xC5,	/* 81 A ring ==> A overcircle capital */
	0xC7,	/* 82 C cedilla ==> C cedilla capital */
	0xC9,	/* 83 E accute ==> E acute capital */
	0xD1,	/* 84 N tilde ==> N tilde capital */
	0xD6,	/* 85 O dieresis ==> O diaeresis capital */
	0xDC,	/* 86 U dieresis ==> U diaeresis capital */
	0xE1,	/* 87 a accute ==> a acute small */
	0xE0,	/* 88 a grave ==> a grave small */
	0xE2,	/* 89 a circumflex ==> a circumflex small */
	0xE4,	/* 8A a dieresis ==> a diaeresis small */
	0xE3,	/* 8B a tilde ==> a tilde small */
	0xE5,	/* 8C a ring ==> a overcircle small */
	0xE7,	/* 8D c cedilla ==> c cedilla small */
	0xE9,	/* 8E e accute ==> e acute small */
	0xE8,	/* 8F e grave ==> e grave small */
	0xEA,	/* 90 e circumflex ==> e circumflex small */
	0xEB,	/* 91 e dieresis ==> e diaeresis small */
	0xED,	/* 92 i accute ==> i acute small */
	0xEC,	/* 93 i grave ==> i grave small */
	0xEE,	/* 94 i circumflex ==> i circumflex small */
	0xEF,	/* 95 i dieresis ==> i diaeresis small */
	0xF1,	/* 96 n tilde ==> n tilde small */
	0xF3,	/* 97 o accute ==> o acute small */
	0xF2,	/* 98 o grave ==> o grave small */
	0xF4,	/* 99 o circumflex ==> o circumflex small */
	0xF6,	/* 9A o dieresis ==> o diaeresis small */
	0xF5,	/* 9B o tilde ==> o tilde small */
	0xFA,	/* 9C u accute ==> u acute small */
	0xF9,	/* 9D u grave ==> u grave small */
	0xFB,	/* 9E u circumflex ==> u circumflex small */
	0xFC,	/* 9F u dieresis ==> u diaeresis small */
	0xA6,	/* A0 dagger ==> Vertical Line, Broken */
	0xB0,	/* A1 superscript ring ==> Degree Symbol */
	0xA2,	/* A2 cent ==> cent sign */
	0xA3,	/* A3 sterling ==> pound sign */
	0xA7,	/* A4 section ==> section/paragraph symb */
	0x80,	/* A5 bullet ==> Center Box Bar Vert D */
	0xB6,	/* A6 paragraph ==> paragraph symbol USA */
	0xDF,	/* A7 Es-sed (German double s) ==> sharp s small */
	0xAE,	/* A8 registered  ( (R) ) ==> Regist.Trade Mark sym */
	0xA9,	/* A9 copyright   ( (C) ) ==> Copyright sign */
	0x81,	/* AA trade mark ==> Center Box Bar Horiz D */
	0xB4,	/* AB acute accent ==> acute accent */
	0xA8,	/* AC dieresis (AKA umlaut) ==> diaeresis,umlaut acc */
	0x82,	/* AD not equal ==> Upp Left Box Corner D */
	0xC6,	/* AE AE ==> AE diphthong capital */
	0xD8,	/* AF O slash ==> O slash capital */
	0x83,	/* B0 infinity ==> Upper Right Box Corn D */
	0xB1,	/* B1 plus minus ==> plus or minus sign */
	0xBE,	/* B2 less than or equal to ==> three quarters */
	0x84,	/* B3 greater than or equal to ==> Low Left Box Corner D */
	0xA5,	/* B4 yen ==> Yen sign */
	0xB5,	/* B5 greek lowercase mu ==> micro symbol */
	0xF0,	/* B6 partial ==> Eth Islandic small */
	0x85,	/* B7 Uppercase Sigma (Summation) ==> Lower Right Box Corn D */
	0xBD,	/* B8 Uppercase Pi (Power) ==> one half */
	0xBC,	/* B9 lowercase pi ==> one quarter */
	0x86,	/* BA integral ==> Left Box Side Double */
	0xAA,	/* BB feminine ordinal ==> ordinal indicator fem */
	0xBA,	/* BC masculine ordinal ==> ordinal indicator mas */
	0x87,	/* BD Uppercase Omega ==> Right Box Side Double */
	0xE6,	/* BE ae ==> ae diphthong small */
	0xF8,	/* BF o slash ==> o slash small */
	0xBF,	/* C0 inverted ? ==> Question mark inverted */
	0xA1,	/* C1 inverted ! ==> exclamation point inv */
	0xAC,	/* C2 logical not ==> logical NOT, EOL symb */
	0x88,	/* C3 radical (square root) ==> Middle Box Top Double */
	0x9F,	/* C4 florin ==> Florin Sign */
	0x89,	/* C5 approx equal ==> Middle Box Bottom D */
	0xD0,	/* C6 Uppercase Delta ==> Eth islandic capital */
	0xAB,	/* C7 left guillemot (like << ) ==> left angle quotes */
	0xBB,	/* C8 right guillemot (like >> ) ==> right angle quotes */
	0x8A,	/* C9 elipsis (...) ==> Box Intersection Doubl */
	0xA0,	/* CA non-printing space ==> required space */
	0xC0,	/* CB A grave ==> A grave capital */
	0xC3,	/* CC A tilde ==> A tilde capital */
	0xD5,	/* CD O tilde ==> O tilde capital */
	0xDE,	/* CE OE ==> Thorn islandic capital */
	0xFE,	/* CF oe ==> Thorn islandic small */
	0xAD,	/* D0 en dash ==> Syllabe Hyphen */
	0x8B,	/* D1 em dash ==> Solid Fill Ch,Top Half */
	0xB3,	/* D2 left doublequote ( `` ) ==> 3 superscript */
	0xB2,	/* D3 right doublequote ( '' ) ==> 2 superscript */
	0x8C,	/* D4 left singlequote ( ` ) ==> Solid Fill Ch,Bottom H */
	0xB9,	/* D5 right singlequote ( ' ) ==> 1 superscript */
	0xF7,	/* D6 divide ==> Divide sign */
	0xD7,	/* D7 lozenge (open diamond) ==> Multiply sign */
	0xFF,	/* D8 y dieresis ==> y diaeresis small */
	0x8D,	/* D9 Y dieresis ==> Solid Fill Character */
	0x8E,	/* DA divide (a / with less slope) ==> Small Solid Square */
	0xA4,	/* DB generic curency ==> int. currency symbol */
	0x8F,	/* DC single left guil (like < ) ==> Double Underscore */
	0x90,	/* DD single left guil (like > ) ==> Center Box Bar Vertic */
	0xFD,	/* DE fi ==> y acute small */
	0xDD,	/* DF fl ==> Y acute Capital */
	0x91,	/* E0 double dagger ==> Center Box Bar Horiz */
	0xB7,	/* E1 centered (small) dot ==> Middle dot accent */
	0x92,	/* E2 baseline single close quote ==> Upper Left Box Corner */
	0x93,	/* E3 baseline double close quote ==> Upper Right Box Corner */
	0x94,	/* E4 per thousand ==> Lower Left Box Corner */
	0xC2,	/* E5 A circumflex ==> A circumflex capital */
	0xCA,	/* E6 E circumflex ==> E circumflex capital */
	0xC1,	/* E7 A accute ==> A acute capital */
	0xCB,	/* E8 E dieresis ==> E diaeresis capital */
	0xC8,	/* E9 E grave ==> E grave capital */
	0xCD,	/* EA I accute ==> I acute capital */
	0xCE,	/* EB I circumflex ==> I circumflex capital */
	0xCF,	/* EC I dieresis ==> I diaeresis capital */
	0xCC,	/* ED I grave ==> I grave capital */
	0xD3,	/* EE O accute ==> O acute capital */
	0xD4,	/* EF O circumflex ==> O circumflex capital */
	0x95,	/* F0 (closed) Apple ==> Lower Right Box Corner */
	0xD2,	/* F1 O grave ==> O grave capital */
	0xDA,	/* F2 U accute ==> U acute capital */
	0xDB,	/* F3 U circumflex ==> U circumflex capital */
	0xD9,	/* F4 U grave ==> U grave capital */
	0x9E,	/* F5 dotless i ==> i dotless small */
	0x96,	/* F6 circumflex ==> Left Middle Box Side */
	0x97,	/* F7 tilde ==> Right Middle Box Side */
	0xAF,	/* F8 macron ==> overline */
	0x98,	/* F9 breve ==> Middle Box Top */
	0x99,	/* FA dot accent ==> Middle Box Bottom */
	0x9A,	/* FB ring accent ==> Box Intersection */
	0xB8,	/* FC cedilla ==> cedilla accent */
	0x9B,	/* FD Hungarian umlaut ==> Fill Character, Light */
	0x9C,	/* FE ogonek ==> Fill Character, Medium */
	0x9D	/* FF caron ==> Fill Character, Heavy */
};

/* Translation functions ... */

CHAR				/* The identity translation function */
ident(c) CHAR c; {
    return(c);
}

CHAR
xl1toas(c) CHAR c; {			/* Latin-1 to US ASCII... */

    switch(langs[language].id) {
      case L_GERMAN:
	switch (c) {			/* German, special rules. */
	  case 196:			/* umlaut-A -> Ae */
	    zmstuff('e');
	    return('A');
	  case 214:			/* umlaut-O -> Oe */
	    zmstuff('e');
	    return('O');
	  case 220:			/* umlaut-U -> Ue */
	    zmstuff('e');
	    return('U');
	  case 228:			/* umlaut-a -> ae */
	    zmstuff('e');
	    return('a');
	  case 246:			/* umlaut-o -> oe */
	    zmstuff('e');
	    return('o');
	  case 252:			/* umlaut-u -> ue */
	    zmstuff('e');
	    return('u');
	  case 223:			/* ess-zet -> ss */
	    zmstuff('s');
	    return('s');
	  default: return(yl1toas[c]);	/* all others by the book */
	}
      case L_DANISH:
      case L_FINNISH:
      case L_NORWEGIAN:
      case L_SWEDISH:
	switch (c) {			/* Scandanavian languages. */
	  case 196:			/* umlaut-A -> Ae */
	    zmstuff('e');
	    return('A');
	  case 214:			/* umlaut-O -> Oe */
	  case 216:			/* O-slash -> Oe */
	    zmstuff('e');
	    return('O');
	  case 220:			/* umlaut-U -> Y */
	    return('Y');
	  case 228:			/* umlaut-a -> ae */
	    zmstuff('e');
	    return('a');
	  case 246:			/* umlaut-o -> oe */
	  case 248:			/* o-slash -> oe */
	    zmstuff('e');
	    return('o');
	  case 252:			/* umlaut-u -> y */
	    return('y');
	  case 197:			/* A-ring -> Aa */
	    zmstuff('a');
	    return('A');
          case 229:			/* a-ring -> aa */
	    zmstuff('a');
	    return('a');
	  default: return(yl1toas[c]);	/* All others by the book */
	}
      default:
	return(yl1toas[c]);		/* Not German, by the table. */
    }
}


CHAR
xamtoas(c) CHAR c; {			/* Apple Mac to ASCII */
    return(yl1toas[c]);			/* for now, treat like Latin-1 */
}

CHAR
xl1toam(c) CHAR c; { /* Latin-1 to Apple Macintosh Character Set */
    return(yl1toam[c]); 
}

CHAR
xamtol1(c) CHAR c; { /* Apple Macintosh Character Set to Latin-1 */
    return(yamtol1[c]); 
}

/*
  Table of translation functions for receiving files.
  Array of pointers to functions for translating from the transfer
  syntax to the local file character set.  The first index is the
  transfer syntax character set number, the second index is the file 
  character set number.
*/

/*
 The following list of functions applies to Unix and VAX/VMS.  This
 list can't be moved to ckuxla.h without including a forward declaration
 for each function.  It's not clear to me how best to clean this up.
 Maybe whoever adapts this file to the Mac or OS/2 will have an idea.
*/

CHAR (*xlr[MAXTCSETS+1][MAXFCSETS+1])() = {
    ident,				/* 0,0 ascii to us ascii */
    ident,				/* 0,14 ascii to latin-1 */
    ident,				/* 0,15 ascii to Apple Mac native set */
    xl1toas,			/* 1,0 latin-1 to us ascii */
    ident,				/* 1,14 latin-1 to latin-1 */
    xl1toam				/* 1,15 latin-1 to Apple Mac */
};

/*
  Translation functions for sending files.
  Array of pointers to functions for translating from the local file
  character set to the transfer syntax character set.  Indexed in the same
  way as the xlr array above.
*/
CHAR (*xls[MAXTCSETS+1][MAXFCSETS+1])() = {
    ident,				/* us ascii to ascii */
    ident,				/* us ascii to latin-1 */
    ident,				/* latin-1 to latin-1 */
    xamtol1				/* Apple Mac to latin-1 */
};
