/*
 File CKUXLA.H
 Language and Character Set Support for UNIX and VAX/VMS.

 This file should be used as a template for the language support files
 for other C-Kermit implementations -- Macintosh, OS/2, Amiga, etc.

 Author: F. da Cruz, Columbia University, 1990-1991.
*/
#ifndef CKUXLA_H
#define CKUXLA_H

/* Codes for local file character sets */

#define FC_TRANSP  254 /* Transparent (magic number) */
#define FC_USASCII 0   /* US ASCII */
#define FC_UKASCII 1   /* United Kingdom ASCII */
#define FC_DUASCII 2   /* Dutch ISO 646 NRC */
#define FC_FIASCII 3   /* Finnish ISO 646 NRC */
#define FC_FRASCII 4   /* French ISO 646 NRC */
#define FC_FCASCII 5   /* French Canadian ISO 646 NRC */
#define FC_GEASCII 6   /* German ISO 646 NRC */
#define FC_HUASCII 7   /* Hungarian ISO 646 NRC */
#define FC_ITASCII 8   /* Italian *ISO 646 NRC */
#define FC_NOASCII 9   /* Norwegian and Danish ISO 646 NRC */
#define FC_POASCII 10  /* Portuguese ISO 646 NRC */
#define FC_SPASCII 11  /* Spanish ISO 646 NRC */
#define FC_SWASCII 12  /* Swedish ISO 646 NRC */
#define FC_CHASCII 13  /* Swiss ISO 646 NRC */

/* 8-bit character sets for Unix */
#define FC_1LATIN  14  /* ISO 8859-1 Latin Alphabet 1 */
#define FC_DECMCS  15  /* DEC Multinational Character Set */
#define FC_NEXT    16  /* NeXT workstation character set */
#define FC_CP437   17  /* IBM PC Code Page 437 */
#define FC_CP850   18  /* IBM PC Code Page 850 */
#define FC_APPQD   19  /* Apple Quickdraw */
#ifndef CYRILLIC
#ifndef KANJI
#define MAXFCSETS  19
#endif /* KANJI */
#endif /* CYRILLIC */

#ifdef CYRILLIC
#define FC_CYRILL  20  /* ISO 8859-5 Latin/Cyrillic */
#define FC_CP866   21  /* Microsoft CP866 Cyrillic */
#define FC_KOI7    22  /* KOI-7 = Short KOI */
#define FC_KOI8    23  /* KOI-8 */
#ifndef KANJI
#define MAXFCSETS 23
#endif /* KANJI */
#endif /* CYRILLIC */
/*
  This is truly ugly, and will only get uglier as the number of
  conditionally compiled character set groups increases.  But the
  C preprocessor does not allow recursion, nor is it portable to
  do any kind of arithmetic on the symbol definitions.
*/
#ifdef KANJI
#ifndef CYRILLIC
#define FC_JIS7    20  /* JIS-7 */
#define FC_SHJIS   21  /* Shifted JIS = CP932 */
#define FC_JEUC    22  /* Japanese EUC (JAE) */
#define FC_JDEC    23  /* Japanese DEC Kanji */
#define MAXFCSETS  23
#else /* No CYRILLIC */
#define FC_JIS7    24  /* JIS-7 */
#define FC_SHJIS   25  /* Shifted JIS = CP932 */
#define FC_JEUC    26  /* Japanese EUC (JAE) */
#define FC_JDEC    27  /* Japanese DEC Kanji */
#define MAXFCSETS  27
#endif /* CYRILLIC */
#endif /* KANJI */

#endif /* CKUXLA_H */
