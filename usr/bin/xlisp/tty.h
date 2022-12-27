/******************************************************************************
 * TTY.H -- TTY version 1.0 Copyright (c) 1990 FWR Group
 ******************************************************************************/

/* tty.h edit log:
 * 12/05/90 created version 1.0 zw
 */

#ifndef __TTY_H__
#define __TTY_H__

#define TTY_VERSION "TTY version 1.0 Copyright (c) 1990 FWR Group"

char tty_error[];   /* error message buffer */
char *tty_version;  /* this labels programs and objects */
int tty_max_x;      /* 0 <= x < tty_max_x */
int tty_max_y;      /* 0 <= y < tty_max_y */

/* enable/disable tty display */
void tty_enable(const int on);

/* display menu with headers, return user selection */
/* menu looks like: ((<value> "<name>")(<value> "<name>")...) */
/* selection looks like: (<code> <value>) */
char *tty_menu(const char *menu, const char *header1, const char *header2,
  const char *footer);

/* ask yes-no question, trashes middle of display */
/* returns zero if no, non-zero if yes */
int tty_yesno(const char *question);

#endif /* __TTY_H__ */
