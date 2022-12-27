/******************************************************************************
 * TTY.C -- TTY version 1.0 Copyright (c) 1990 FWR Group
 ******************************************************************************/

/* tty.c edit log:
 * 12/05/90 created version 1.0 zw
 */

/******************************************************************************
 * TERMINAL WINDOW MANAGER
 * tty_enable(on) -- turn terminal window manager on/off
 * tty_menu(menu, header1, header2, footer) -- select item from menu
 * tty_yesno(question) -- solicit yes/no response to question
 ******************************************************************************/

/* define CURSES to display via the curses library */
#define CURSES

#include <ctype.h>
#ifdef CURSES
#include <curses.h>
#endif

#include "salloc.h"
#include "tty.h"

/* search source for TTYERR(#) to locate error */
#define TTYERR(N) N

/* define QUICK_SELECT to make quick command actually select the item */
#define QUICK_SELECT

/* SALLOC() stack memory limits per function */
#define TTY_MENU_LIMIT 0x2000      /* menu list structure */

/* general purpose returned data buffer size */
#define STUFF 0x800

/* standard display parameters, see headfoot() and clear_data() */
#define DATA_Y 3                   /* top data line */
#define DATA_LINES (tty_max_y - 6) /* number of data lines */

/* standard command characters */
#define CMD_EXIT (' ')             /* exit without selecting anything */
#define CMD_SELECT ('M' - '@')     /* select item(s) */
#define CMD_UP ('P' - '@')         /* move up */
#define CMD_DOWN ('N' - '@')       /* move down */
#define CMD_LEFT ('B' - '@')       /* move left */
#define CMD_RIGHT ('F' - '@')      /* move right */
#define CMD_NEXT ('I' - '@')       /* move to next item */
#define CMD_PREV 127               /* move to previous item */
#define CMD_DELETE ('H' - '@')     /* delete character */

/* tty_menu() initial display parameters */
#define MENU_SEPERATION 2          /* spaces between items */
#define MENU_LMARGIN 1             /* left margin */
#define MENU_RMARGIN 1             /* right margin */

/* tty_form() data types */
#define FORM_FIXNUM 0              /* form data type is FIXNUM */
#define FORM_FLONUM 1              /* form data type is FLONUM */
#define FORM_STRING 2              /* form data type is STRING */

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/* exported global variables */
char tty_error[0x800] = "";        /* error message buffer */
char *tty_version = TTY_VERSION;   /* this labels programs and objects */
int tty_max_x = 0;                 /* 0 <= x < tty_max_x */
int tty_max_y = 0;                 /* 0 <= y < tty_max_y */

/* local global variables */
static char stuff[STUFF];          /* returned data buffer */
static tty_enable_count = 0;       /* count calls to tty_enable() */

/******************************************************************************
 * MACROS -- FORMAT TTY_ERROR[] AND GOTO ERROR LABEL IF ERROR
 ******************************************************************************/

/* salloc() a buffer */
static char *salloc_error_format = "TTYERR(%d), salloc %d";
#define SALLOC(buffer, length, err) \
  if (!((char *)(buffer) = salloc(length))) { \
    sprintf(tty_error, salloc_error_format, err, length); \
    goto error; \
  }

/******************************************************************************
 * TTY ACCESS ROUTINES -- INTERFACE TO MACHINE DEPENDENT FUNCTIONS
 ******************************************************************************/

/* enable/disable tty display */
void tty_enable(const int on)
{
#ifdef CURSES
  if (on) {
    if (tty_enable_count <= 0) {
      (void)initscr(); clear(); raw(); noecho(); refresh();
      tty_max_y = LINES; tty_max_x = COLS;
      tty_enable_count = 0;
    }
    tty_enable_count++;
  }
  else {
    tty_enable_count--;
    if (tty_enable_count <= 0) {
      clear(); move(LINES - 1, 0); refresh(); echo(); noraw(); endwin();
      tty_enable_count = 0;
    }
  }
#endif
} /* tty_enable() */

/* move cursor to (x,y), top left is (0,0) */
static void cursor(const int x, const int y)
{
#ifdef CURSES
  move((y < tty_max_y) ? y : (tty_max_y - 1),
    (x < tty_max_x) ? x : (tty_max_x - 1));
#endif
}

/* clear specified line */
static void clear_line(const int y)
{
#ifdef CURSES
  int i;

  cursor(0, y);
  for (i = 0; i < tty_max_x; i++) addch(' ');
#endif
}

/* turn highlight on and off */
static void highlight(const int on)
{
#ifdef CURSES
  if (on) standout();
  else standend();
#endif
}

/* write char at current cursor position */
static void display(const char c)
{
#ifdef CURSES
  addch(c);
#endif
}

/* input next char from keyboard, does not echo */
static char keyboard()
{
#ifdef CURSES
  refresh();
  return getch();
#endif
}

/******************************************************************************
 * STANDARD TTY DISPLAY TOOLS FOR A CONSISTANT USER INTERFACE
 ******************************************************************************/

/* convert a command char to a printable string */
static char *show_cmd(const char c)
{
  static char buffer[4];

  if (c == ' ') sprintf(buffer, "'%c'", c);
  else if (c < ' ') sprintf(buffer, "^%c", c + '@');
  else sprintf(buffer, "%c", c);
  return buffer;

} /* show_cmd() */

/* display string in center of line, clear left/right margin */
static center(const char *s, const int y)
{
  const char *c;
  int length, start, i;

  if ((length = strlen(s)) > tty_max_x) length = tty_max_x;
  start = (tty_max_x - length) / 2;
  cursor(0, y);
  for (i = 0; i < start; i++) display(' ');
  for (i = start, c = s; *c && i < tty_max_x; i++) display(*c++);
  for (i = start + length; i < tty_max_x; i++) display(' ');

} /* center */

/* display standard headers and footers above and below data area */
static headers(const char *h1, const char *h2, const char *f1, const char *f2)
{

  /* first header line */
  center(h1, DATA_Y - 3);

  /* second header line -- highlighted */
  highlight(1);
  center(h2, DATA_Y - 2);
  highlight(0);

  /* blank line above data area */
  clear_line(DATA_Y - 1);

  /* blank line below data area */
  clear_line(DATA_Y + DATA_LINES);

  /* first footer line -- highlighted */
  highlight(1);
  center(f1, DATA_Y + DATA_LINES + 1);
  highlight(0);

  /* second footer line */
  center(f2, DATA_Y + DATA_LINES + 2);

} /* headers() */

/* clear standard data area */
static clear_data()
{
  int i;

  for (i = 0; i < DATA_LINES; i++) clear_line(i + DATA_Y);

} /* clear_data() */

/* display data field of specified width at (x,y) in data area */
static void data_field(const int x, const int y, const int w, const char *s)
{
  const char *c;
  int i, width;

  cursor(x, ((y < DATA_LINES) ? y : DATA_LINES - 1) + DATA_Y);
  width = ((x + w) > tty_max_x) ? (tty_max_x - x) : w;
  for (c = s, i = 0; *c && (i < width); i++) display(*c++);
  while (i++ < width) display(' ');

} /* data_field() */

/******************************************************************************
 * TTY_MENU() -- INTERACTIVELY SELECT AN ITEM FROM A MENU
 ******************************************************************************/

/* compose tty_menu() status line */
static char *menu_status(const int more_above, const int more_below)
{
  static char buffer[80];
  char up[4], down[4], left[4], right[4], select[4], exit[4], *more;

  strcpy(up, show_cmd(CMD_UP));
  strcpy(down, show_cmd(CMD_DOWN));
  strcpy(left, show_cmd(CMD_LEFT));
  strcpy(right, show_cmd(CMD_RIGHT));
  strcpy(select, show_cmd(CMD_SELECT));
  strcpy(exit, show_cmd(CMD_EXIT));
  more = ((more_above && more_below) ? ", More above and below." :
    (more_above ? ", More above." : (more_below ? ", More below." : "")));
  sprintf(buffer, "up %s, down %s, left %s, right %s, select %s, exit %s%s",
    up, down, left, right, select, exit, more);
  return buffer;

} /* menu_status() */

/* adjust margins and seperation for optimum menu item display organization */
/* line looks like: LMAR WID SEP WID SEP ... WID SEP WID RMAR */
void menu_margins(const int width, int *seperation, int *rmargin, int *lmargin)
{
  int n;
  int extra_width;
  int extra_seperation;
  int extra_margin;

  /* number of items on a line */
  n = (tty_max_x - MENU_RMARGIN - MENU_LMARGIN + MENU_SEPERATION) /
    (width + MENU_SEPERATION);

  /* extra width available using default margins and seperation */
  extra_width = (tty_max_x - MENU_RMARGIN - MENU_LMARGIN + MENU_SEPERATION) -
    n * (width + MENU_SEPERATION);

  /* extra seperation between items */
  extra_seperation = extra_width / (n - 1);

  /* extra left and right margin */
  extra_margin = (extra_width - ((n - 1) * extra_seperation))/2;

  /* balanced seperation between items */
  *seperation = MENU_SEPERATION + extra_seperation;

  /* balanced left margin */
  *lmargin = MENU_LMARGIN + extra_margin;

  /* balanced right margin */
  *rmargin = MENU_RMARGIN + extra_margin;

} /* menu_margins() */

/* display menu with header, return user selection */
/* menu looks like: ((<value> "<name>")(<value> "<name>")...) */
/* selection looks like: (<code> <value>) */
char *tty_menu(const char *menu,
  const char *header1, const char *header2, const char *footer)
{
  SALLOC_SPACE(TTY_MENU_LIMIT);  /* reserve space for salloc() */
  const char *m;                 /* pointer walks through menu[] */
  struct menu {                  /* SALLOC menu entries */
    char *name;                  /* name of menu item */
    char *value;                 /* return value of menu item */
    struct menu *next;           /* next menu item in list */
    int x;                       /* display x position in data area */
    int y;                       /* display y position in data area */
    char quick;                  /* quick select command char or zero */
  };
  struct menu *list;             /* list of menu items */
  struct menu *new;              /* for building new menu items */
  struct menu *i;                /* pointer for walking menu list */
  struct menu *old_top;          /* menu item at top of display */
  struct menu *top;              /* new top item */
  struct menu *old_item;         /* currently highlighted menu item */
  struct menu *item;             /* new menu item */
  char quick;                    /* quick select command char or zero */
  char buffer[0x800];            /* scanning  buffer */
  char *b;                       /* pointer to walk through buffer */
  char *value;                   /* value of menu item */
  char *name;                    /* name of menu item */
  int x;                         /* display X position, 0 to tty_max_x */
  int y;                         /* display Y position, 0 to tty_max_y */
  int n;                         /* space counter */
  int width;                     /* width of biggest menu item */
  int above;                     /* if non-zero, more items above */
  int below;                     /* if non-zero, more items below */
  char cmd;                      /* current command char */
  int seperation;                /* seperation between items */
  int rmargin;                   /* right margin */
  int lmargin;                   /* left margin */

  /* grab display -- remember to undo this if error */
  tty_enable(1);

  /* scan menu items, create menu list */
  /* menu looks like: ((<value> "name")(<value> "name")...) */
  /* space between <value> and "name" is important */
  list = 0;
  m = menu;
  if (!m) goto bad_menu;
  if (*m++ != '(') goto bad_menu;
  while (*m && *m != ')') {

    /* create menu record, add to list */
    SALLOC(new, sizeof(*new), TTYERR(0));
    new->next = list; list = new;

    /* scan a (<value> "name") */
    while (*m && *m++ != '('); if (!*m) goto bad_menu;
    for (b = buffer; *m && *m != ' '; ) *b++ = *m++; *b = 0;
    SALLOC(new->value, strlen(buffer) + 1, TTYERR(1));
    strcpy(new->value, buffer);
    while (*m && *m++ != '"'); if (!*m) goto bad_menu;
    for (b = buffer; *m && *m != '"'; ) *b++ = *m++; *b = 0;
    SALLOC(new->name, strlen(buffer) + 4, TTYERR(2));
    sprintf(new->name, "   %s", buffer);
    if (*m++ != '"') goto bad_menu;
    while (*m && *m++ != ')'); if (!*m) goto bad_menu;

  } /* while (*m && *m != ')') */
  if (*m++ != ')') goto bad_menu;
  if (*m) goto bad_menu;

  /* determine item width, assign quick select command chars */
  for (quick = '0', width = 0, i = list; i; i = i->next) {

    /* quick select command chars are 0-9, A-Z, otherwise zero */
    i->quick = quick;
    if (quick) { i->name[0] = quick; i->name[1] = ')'; }
    if (quick == '9') quick = 'A';
    else if (quick == 'Z') quick = 0;
    else if (quick) quick++;

    /* find widest width */
    if (strlen(i->name) > width) width = strlen(i->name);
    if (width >= tty_max_x) {
      sprintf(tty_error, "TTYERR(3) (%d >= %d) name too big \"%s\"",
        width, tty_max_x, i->name);
      goto error;
    }

  } /* for (quick = '0', width = 0, i = list; i; i = i->next) */

  /* decide where to display items */
  menu_margins(width, &seperation, &rmargin, &lmargin);
  for (x = lmargin, y = 0, i = list; i; i = i->next) {
    if ((x + width) > (tty_max_x - rmargin)) { x = lmargin; y++; }
    i->x = x; i->y = y; x += width + seperation;
  }

  /* command loop */
  old_top = old_item = 0; /* trigger initial display refresh */
  top = item = list;
  while (1) {

    /* check if display must be changed to highlight selection */
    if (old_top) {
      if ((item->y < old_top->y) || (item->y >= (old_top->y + DATA_LINES))) {
        for (i = list; i; i = i->next)
          if ((item->y - i->y) < DATA_LINES / 2) { top = i; break; }
      }
    }

    /* if new top, display menu items in data area, update headers */
    if (top != old_top) {
      clear_data();
      above = below = 0;
      for (i = top; i; i = i->next) {
        if ((i->y - top->y) >= DATA_LINES) { below++; break; }
        data_field(i->x, i->y - top->y, width, i->name);
      }
      if (top != list) above++;
      headers(header1, header2, menu_status(above, below), footer);
    }

    /* otherwise, if new selection, remove old highlight */
    else if (old_item && (item != old_item))
      data_field(old_item->x, old_item->y - top->y, width, old_item->name);

    /* highlight new item */
    if (item != old_item) {
      highlight(1);
      data_field(item->x, item->y - top->y, width, item->name);
      highlight(0);
    }

    /* remember current highlighted item and current display top item */
    old_item = item;
    old_top = top;

    /* get command character */
    cmd = keyboard();

    /* exit menu -- return "()" */
    if (cmd == CMD_EXIT) { item = 0; break; }

    /* select current highlighted item -- return item value */
    else if (cmd == CMD_SELECT) break;

    /* move left */
    else if (cmd == CMD_LEFT) {
      if (old_item->x > 0) for (i = list; i; i = i->next)
        if (i->next == old_item) {
          item = i; break;
        }
    }

    /* move right */
    else if (cmd == CMD_RIGHT) {
      if ((old_item->next) && (old_item->next->y == old_item->y))
        item = old_item->next;
    }

    /* move up */
    else if (cmd == CMD_UP) {
      for (i = list; i; i = i->next)
        if (i->y == old_item->y - 1 && i->x == item->x) {
          item = i; break;
        }
    }

    /* move down */
    else if (cmd == CMD_DOWN) {
      for (i = list; i; i = i->next)
        if ((i->y == old_item->y + 1) && (i->x == old_item->x)) {
          item = i; break;
        }
    }

    /* move to next item in list */
    else if (cmd == CMD_NEXT) { if (old_item->next) item = old_item->next; }

    /* move to previous item in list */
    else if (cmd == CMD_PREV) {
      for (i = list; i; i = i->next) if (i->next == old_item) {
        item = i; break;
      }
    }

    /* check for quick select command */
    else {
      if (islower(cmd)) cmd = toupper(cmd);
      for (item = 0, i = list; i; i = i->next) if (i->quick == cmd) {
        item = i; break;
      }
      if (!item) item = old_item;
#ifdef QUICK_SELECT
      else { strcpy(stuff, item->value); break; }
#endif
    }

  } /* while (1) */

  /* release display -- caller must clear */
  tty_enable(0);

  /* compose response */
  if (!item) strcpy(stuff, "()");
  else strcpy(stuff, item->value);

  /* return response */
  return stuff;

  /* bad menu format error */
bad_menu:
  sprintf(tty_error, "TTYERR(4) bad menu \"%s\"", menu ? menu : "");

  /* undo tty_enable() and return zero if error */
error:
  tty_enable(0);
  return 0;

} /* tty_menu() */

/******************************************************************************
 * TTY_YESNO() -- ASK YES/NO QUESTION
 ******************************************************************************/

/* ask yes-no question, trashes middle of display */
/* returns zero if no, non-zero if yes */
int tty_yesno(const char *question)
{
  int width;
  int i;
  int x;
  const char *c;
  char buffer[0x800];
  char yes;

  /* grab display */
  tty_enable(1);

  width = strlen(question) + 4;
  if (width > tty_max_x) width = tty_max_x - 4;
  x = (tty_max_x / 2) - (width / 2);

  /* display question in middle of data area, highlighted */
  highlight(1);
  for (i = 0; i < width; i++) buffer[i] = ' ';
  data_field(x, (DATA_LINES / 2) - 1, width, buffer);
  for (i = 2, c = question; i < (width - 2); i++) buffer[i] = *c++;
  data_field(x, (DATA_LINES / 2) + 0, width, buffer);
  for (i = 0; i < width; i++) buffer[i] = ' ';
  data_field(x, (DATA_LINES / 2) + 1, width, buffer);
  cursor(x + width - 2, DATA_Y + (DATA_LINES / 2));
  highlight(0);

  /* get Y or N response */
  while (yes = keyboard()) {
    if ((yes == 'y') || (yes == 'Y')) { yes = 'Y'; break; }
    if ((yes == 'n') || (yes == 'N')) { yes = 'N'; break; }
  }

  /* release display -- caller must clear */
  tty_enable(0);

  /* return non-zero if Y */
  return yes == 'Y';

} /* tty_yesno() */

/******************************************************************************
 * TTY_FORM() -- FILL IN A FORM
 ******************************************************************************/

#ifdef NOTDEF

/* display menu with header, return user selection */
/* form looks like: (("<name>" <x> <y> <width> <type> <default>)...) */
/* type is one of: FIXNUM FLONUM STRING */
/* returns: (<value>...) */
char *tty_form(const char *form, const char *header1, const char *header2,
  const char *footer)
{
  SALLOC_SPACE(TTY_FORM_LIMIT);  /* reserve space for salloc() */
  const char *f;                 /* pointer walks through form[] */
  struct form {                  /* SALLOC form entries */
    char *name;                  /* name of form item */
    int x;                       /* display x position in data area */
    int y;                       /* display y position in data area */
    int width;                   /* length of value buffer */
    int type;                    /* data type */
    char *value;                 /* value buffer */
    struct form *next;           /* next form item in list */
  };
  struct form *list;             /* list of form items */
  struct form *new;              /* for building new form items */
  struct form *i;                /* pointer for walking form list */
  struct form *j;                /* pointer for walking form list */
  struct menu *old_top;          /* menu item at top of display */
  struct menu *top;              /* new top item */
  struct menu *old_item;         /* currently highlighted menu item */
  struct menu *item;             /* new menu item */
  char buffer[0x800];            /* scanning  buffer */
  char *b;                       /* pointer to walk through buffer */
  int above;                     /* if non-zero, more items above */
  int below;                     /* if non-zero, more items below */
  char cmd;                      /* current command char */

  /* grab display -- remember to undo this if error */
  tty_enable(1);

  /* scan form items, create form list */
  /* form looks like: (("<name>" <x> <y> <width> <type> <default>)...) */
  list = 0;
  f = form;
  if (!f) goto bad_form;
  if (*f++ != '(') goto bad_form;
  while (*f && *f != ')') {

    /* create form record, add to list */
    SALLOC(new, sizeof(*new), TTYERR(5));
    new->next = list; list = new;

    /* scan a ("<name>" <x> <y> <width> <type> <default>) */
    while (*m && *m++ != '('); if (!*m) goto bad_form;
    while (*m && *m++ != '"'); if (!*m) goto bad_form;
    for (b = buffer; *m && *m != '"'; ) *b++ = *m++; *b = 0;
    SALLOC(new->name, strlen(buffer) + 1, TTYERR(6));
    strcpy(new->name, buffer);
    if (*m++ != '"') goto bad_form;
    while (*m && *m++ == ' '); if (!*m) goto bad_form;
    for (b = buffer; *m && isdigit(*m); ) *b++ = *m++; *b = 0;
    new->x = atoi(buffer);
    while (*m && *m++ == ' '); if (!*m) goto bad_form;
    for (b = buffer; *m && isdigit(*m); ) *b++ = *m++; *b = 0;
    new->y = atoi(buffer);
    while (*m && *m++ == ' '); if (!*m) goto bad_form;
    for (b = buffer; *m && isdigit(*m); ) *b++ = *m++; *b = 0;
    new->width = atoi(buffer);
    while (*m && *m++ == ' '); if (!*m) goto bad_form;
    for (b = buffer; *m && (*m != ' ') && (*m != ')'); ) *b++ = *m++; *b = 0;
    if (!strcmp(buffer, "FIXNUM") new->type = FORM_FIXNUM;
    else if (!strcmp(buffer, "FLONUM") new->type = FORM_FLONUM;
    else if (!strcmp(buffer, "STRING") new->type = FORM_STRING;
    else goto bad_form;
    while (*m && *m++ == ' '); if (!*m) goto bad_form;
    for (b = buffer; *m && (*m != ')'); ) *b++ = *m++; *b = 0;
    if (strlen(buffer) > new->width) goto bad_form;
    SALLOC(new->value, new->width + 1, TTYERR(7));
    strcpy(new->value, buffer);
    while (*m && *m++ != ')'); if (!*m) goto bad_form;
    if ((new->x < 0) goto bad_form;
    if (new->x + strlen(new->name) + new->width) > tty_max_x) goto bad_form;
    if ((new->y < 0) goto bad_form;

  } /* while (*f && *f != ')') */
  if (*f++ != ')') goto bad_form;
  if (*f) goto bad_form;

  /* make sure fields do not overlap */
  for (i = list; i; i->next) for (j = list; j; j->next) if (i != j) {
    if (i->y != j->y) continue;
    if ((i->x + strlen(i->name) + i->width) >= j->x) continue;
    if ((j->x + strlen(j->name) + j->width) >= i->x) continue;
    goto bad_form;
  }

  /* command loop */
  old_top = old_item = 0; /* trigger initial display refresh */
  top = item = list;
  while (1) {
    /* check if display must be changed to show field */
    /* if new top, display form items in data area, update headers */
    /* remember current item and current display top item */
    old_item = item;
    old_top = top;

    /* get command character */
    cmd = keyboard();

    /* exit form without selecting anything */
    if (cmd == CMD_EXIT) { list = 0; break; }

    /* select current form -- return values if all are filled in */
    else if (cmd == CMD_SELECT) {
      for (i = list, item = 0; i; i = i->next) if (!strlen(i->value)) {
        item = i; break;
      }
      if (!item) break;
    }

    /* move left */
    else if (cmd == CMD_LEFT) {
      /* ??? */
    }

    /* move right */
    else if (cmd == CMD_RIGHT) {
      /* ??? */
    }

    /* move up */
    else if (cmd == CMD_UP) {
      /* ??? */
    }

    /* move down */
    else if (cmd == CMD_DOWN) {
      /* ??? */
    }

    /* move to next item in list */
    else if (cmd == CMD_NEXT) {
      /* ??? */
    }

    /* move to previous item in list */
    else if (cmd == CMD_PREV) {
      /* ??? */
    }

    /* delete character */
    else if (cmd == CMD_DELETE) {
      /* ??? */
    }

    /* insert character into item value */
    else {
      /* ??? */
    }

  } /* while (1) */

  /* release display -- caller must clear */
  tty_enable(0);

  /* compose response */
  if (!list) strcpy(stuff, "()");
  else for (i = list; i; i->next) {
    /* ??? */
  }

  /* return response */
  return stuff;

  /* bad form format error */
bad_form:
  sprintf(tty_error, "TTYERR(8) bad form \"%s\"", form ? form : "");

  /* undo tty_enable() and return zero if error */
error:
  tty_enable(0);
  return 0;

}

#endif
