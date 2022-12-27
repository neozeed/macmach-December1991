#include <X11/copyright.h>

/* Copyright Massachusetts Institute of Technology 1985 */

/* $XConsortium: lncmd.h,v 10.6 88/09/06 17:20:22 jim Exp $ */
/* lncmd.h - Command sequences DEC printers, in particular LN0x laser
   printers */

/*
#define LN_RIS             "\033c"  Obsolete; causes LN03+ problems
*/
#define LN_STR             "\033[!p"
#define LN_SSU             "\033[%d I"
#define LN_PUM_SET         "\033[11h"
#define LN_PFS             "\033[%s J"
#define LN_DECSLRM         "\033[%d;%ds"
#define LN_HPA             "\033[%d`"
#define LN_VPA             "\033[%dd"
#define LN_SIXEL_GRAPHICS  "\033P%d;%d;%dq"
#define LN_ST              "\033\\"
#define LN_DECOPM_SET      "\033[?52h"
#define LN_DECOPM_RESET    "\033[?52I"
#define LN_SGR             "\033[1%dm"
#define LN_PUM             "\033[11I"
#define LN_LNM             "\033[20h"
