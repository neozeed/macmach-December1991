#ifndef CKCASC_H
#define CKCASC_H

/* Mnemonics for ASCII control characters (and Space) */

#define NUL  '\0'     /* Null */
#define SOH  001      /* Start of header */
#define STX  002      /* Ctrl-B */
#define ENQ  005      /* ENQ */
#define BEL  007      /* Bell (Beep) */
#define BS   010      /* Backspace */
#define HT   011      /* Horizontal Tab */
#define LF   012      /* Linefeed */
#define NL   '\n'     /* Newline */
#define FF   014      /* Formfeed */
#define CR   015      /* Carriage Return */
#define SO   016      /* Shift Out */
#define SI   017      /* Shift In */
#define DLE  020      /* Datalink Escape */
#define ESC  033      /* Escape */
#define XON  021      /* XON */
#define XOFF 023      /* XOFF */
#define SUB  032      /* SUB */
#define XGS  '\x1D'   /* Group Separator,  Ctrl-Rightbracket */
#define US   '\x1F'   /* Unit Separator,   Ctrl-Underscore */
#define XFS  '\x1C'   /* Field Separator,  Ctrl-Backslash */
#define XRS  036      /* Record Separator, Ctrl-Circumflex */
#define SYN  '\x16'   /* SYN, Ctrl-V */
#define SP   040      /* Space */
#define DEL  0177     /* Delete (Rubout) */
#define RUB  0177     /* Delete (Rubout) */

#endif /* CKCASC_H */

