/******************************************************************************
 * OSFUN.C -- Xlisp glue for DB and TTY, Copyright (c) 1990 FWR Group
 *   SALLOC version 1.0 Copyright (c) 1990 FWR Group
 *   DB version 1.0 Copyright (c) 1990 FWR Group
 *   TTY version 1.0 Copyright (c) 1990 FWR Group
 ******************************************************************************/

/* osfun.c edit log:
 * 12/05/90 created version 1.0 zw
 */

#include "xlisp.h"

#ifdef FWR_STUFF

extern LVAL true;

#include "salloc.h"
#include "db.h"
#include "tty.h"

/* reserved stack space for functions that use salloc() */
#define XDB_UPDATE_LIMIT 0x1000   /* string length of update data argument */
#define XTTY_MENU_LIMIT 0x1000    /* string length of menu list argument */

/******************************************************************************/

/* open the specified database file -- sets up internal structures */
/* the database is read-only, there is no need for a close routine */
/* argument is: "<data file name>" */
/* returns NIL if error else T */
LVAL xdb_open()
{
  char *file_name;

  /* single argument is file name string */
  file_name = (char *)getstring(xlgastring());

  /* no more arguments */
  xllastarg();

  /* apply db_open() to file name, return NIL or T */
  return db_open(file_name) ? NIL : true;

} /* xdb_open() */

/******************************************************************************/

/* load text file, create data file and name file, if the name file */
/* already exists, it will be rewritten with the added names */
/* argument is: "<text file name>" */
/* returns "<data file name>", xlfail if error */
LVAL xdb_load()
{
  char *file_name, *i, *response;
  LVAL val;

  /* protect val */
  xlstkcheck(1);
  xlsave(val);

  /* single argument is file name string */
  file_name = (char *)getstring(xlgastring());

  /* no more arguments */
  xllastarg();

  /* apply db_load() to file name, xlfail if error */
  if (!(response = db_load(file_name))) xlfail(db_error);

  /* build stream from returned string */
  val = newustream();
  xlputc(val, '"');
  for (i = response; *i; i++) xlputc(val, *i);
  xlputc(val, '"');

  /* apply xlread() to convert into expression */
  if (!xlread(val, &val, 0)) xlfail("xdb_group xlread failed");

  /* unsave val */
  xlpop();

  /* return expression */
  return val;

} /* xdb_load() */

/******************************************************************************/

/* return the group list for the specified item */
/* looks like: ((<index> "<item name>")(<index> "<item name>")...) */
/* argument is: <index number> */
/* note that only valid items for this group are listed */
/* xlfail if error */
LVAL xdb_group()
{
  long index;
  char *i, *response;
  LVAL val;

  /* protect val */
  xlstkcheck(1);
  xlsave(val);

  /* single integer argument is index */
  index = getfixnum(xlgafixnum());

  /* no more arguments */
  xllastarg();

  /* apply db_group() to index, xlfail if error */
  if (!(response = db_group(index))) xlfail(db_error);

  /* build stream from returned string */
  val = newustream();
  for (i = response; *i; i++) xlputc(val, *i);

  /* apply xlread() to convert into expression */
  if (!xlread(val, &val, 0)) xlfail("xdb_group xlread failed");

  /* unsave val */
  xlpop();

  /* return expression */
  return val;

} /* xdb_group() */

/******************************************************************************/

/* return data list for the specified item */
/* argument is: <index number> */
/* xlfail if error */
LVAL xdb_data()
{
  long index;
  char *i, *response;
  LVAL val;

  /* protect val */
  xlstkcheck(1);
  xlsave(val);

  /* single integer argument is index */
  index = getfixnum(xlgafixnum());

  /* no more arguments */
  xllastarg();

  /* apply db_data() to index, xlfail if error */
  if (!(response = db_data(index))) xlfail(db_error);

  /* build stream from returned string */
  val = newustream();
  for (i = response; *i; i++) xlputc(val, *i);

  /* apply xlread() to convert into expression */
  if (!xlread(val, &val, 0)) xlfail("xdb_data xlread failed");

  /* unsave val */
  xlpop();

  /* return expression */
  return val;

} /* xdb_data() */

/******************************************************************************/

/* return the name list for the specified item */
/* argument is: <index number> */
/* xlfail if error */
LVAL xdb_name()
{
  long index;
  char *i, *response;
  LVAL val;

  /* protect val */
  xlstkcheck(1);
  xlsave(val);

  /* single integer argument is index */
  index = getfixnum(xlgafixnum());

  /* no more arguments */
  xllastarg();

  /* apply db_name() to index, xlfail if error */
  if (!(response = db_name(index))) xlfail(db_error);

  /* build stream from returned string */
  val = newustream();
  for (i = response; *i; i++) xlputc(val, *i);

  /* apply xlread() to convert into expression */
  if (!xlread(val, &val, 0)) xlfail("xdb_name xlread failed");

  /* unsave val */
  xlpop();

  /* return expression */
  return val;

} /* xdb_name() */

/******************************************************************************/

/* update database information for specified item */
/* arguments are: <index number><expression> */
/* expression is appended to item's data list */
/* returns T */
/* xlfail if error */
LVAL xdb_update()
{
  SALLOC_SPACE(XDB_UPDATE_LIMIT);
  LVAL data_arg, fptr, lptr, cptr;
  long index;
  char *i, *data;
  int length;

  /* protect stuff */
  xlstkcheck(4);
  xlsave(data_arg);
  xlsave(fptr);
  xlsave(lptr);
  xlsave(cptr);

  /* two arguments are <index> and <expression> */
  index = getfixnum(xlgafixnum());
  data_arg = xlgetarg();
 
  /* no more arguments */
  xllastarg();

  /* print data_arg to data buffer */
  xlprint(fptr = newustream(), data_arg, TRUE);
  for (length = 0, lptr = gethead(fptr); lptr != NIL; lptr = cdr(lptr)) {
    if (!consp(lptr) || (cptr = car(lptr)) == NIL || !charp(cptr))
      xlfail("xdb_update bad stream");
    length++;
  }
  if (!(data = salloc(length + 1))) xlfail("xdb_update salloc failed");
  for (length = 0, lptr = gethead(fptr); lptr != NIL; lptr = cdr(lptr))
    data[length++] = getchcode(car(lptr));
  data[length] = 0;

  /* apply db_update() */
  if (db_update(index, data)) xlfail(db_error);

  /* unsave stuff */
  xlpopn(4);

  /* return T */
  return true;

} /* xdb_update() */

/******************************************************************************/

/* return db status */
/* no arguments */
/* returns: ("error" "version" "signature"
/*   "data_file_name" data_max_index
/*   "name_file" name_max_index
/*   "update_file_name") */
/* xlfail if error -- can't allocate status buffer */
/* NOTE: this will also fail when status text contains unquoted '"' */
LVAL xdb_status()
{
  long index;
  char *i, *response;
  LVAL val;

  /* protect val */
  xlstkcheck(1);
  xlsave(val);

  /* no arguments */
  xllastarg();

  if (!(response = db_status())) xlfail("db_status failed");

  /* build stream from returned string */
  val = newustream();
  for (i = response; *i; i++) xlputc(val, *i);

  /* apply xlread() to convert into expression */
  if (!xlread(val, &val, 0)) xlfail("xdb_status xlread failed");

  /* unsave val */
  xlpop();

  /* return expression */
  return val;

} /* xdb_status() */

/******************************************************************************/

/* interactively select an item from the specified group */
/* looks like: ( <group> <item> ) -or- () */
/* arguments are: <group> "<header text>" "<footer text>" */
/* xlfail if error */
LVAL xdb_menu()
{
  long index;
  char *header, *footer, *i, *response;
  LVAL val;

  /* protect val */
  xlstkcheck(1);
  xlsave(val);

  /* first argument is group index */
  index = getfixnum(xlgafixnum());

  /* second argument is header string */
  header = (char *)getstring(xlgastring());

  /* third argument is footer string */
  footer = (char *)getstring(xlgastring());

  /* no more arguments */
  xllastarg();

  /* apply db_menu() to index, xlfail if error */
  if (!(response = db_menu(index, header, footer))) xlfail(db_error);

  /* build stream from returned string */
  val = newustream();
  for (i = response; *i; i++) xlputc(val, *i);

  /* apply xlread() to convert into expression */
  if (!xlread(val, &val, 0)) xlfail("xdb_menu xlread failed");

  /* unsave val */
  xlpop();

  /* return expression */
  return val;

} /* xdb_menu() */

/******************************************************************************/

/* enable/disable tty display */
LVAL xtty_enable()
{
  LVAL arg;

  /* argument is T or NIL */
  arg = xlgetarg();
 
  /* no more arguments */
  xllastarg();

  /* enable if T, disable if NIL */
  tty_enable(arg != NIL);

  /* return the given argument */
  return arg;

} /* xtty_enable() */

/******************************************************************************/

/* display menu with header, return user selection */
/* arugments are: (<menu> "<header1>" "<header2>" "<footer>")
/* menu looks like: ((<value> "<name>")(<value> "<name>")...) */
/* selection looks like: <value> */
LVAL xtty_menu()
{
  SALLOC_SPACE(XTTY_MENU_LIMIT);
  LVAL menu_arg, fptr, lptr, cptr, val;
  char *header1, *header2, *footer, *menu, *i, *response;
  int length;

  /* protect stuff */
  xlstkcheck(5);
  xlsave(menu_arg);
  xlsave(fptr);
  xlsave(lptr);
  xlsave(cptr);
  xlsave(val);

  /* first argument is menu list */
  menu_arg = xlgetarg();

  /* second argument is first header string */
  header1 = (char *)getstring(xlgastring());

  /* third argument is second header string */
  header2 = (char *)getstring(xlgastring());

  /* fourth argument is footer string */
  footer = (char *)getstring(xlgastring());

  /* no more arguments */
  xllastarg();

  /* print arg to menu buffer */
  xlprint(fptr = newustream(), menu_arg, TRUE);
  for (length = 0, lptr = gethead(fptr); lptr != NIL; lptr = cdr(lptr)) {
    if (!consp(lptr) || (cptr = car(lptr)) == NIL || !charp(cptr))
      xlfail("xtty_menu bad stream");
    length++;
  }
  if (!(menu = salloc(length + 1))) xlfail("xtty_menu salloc failed");
  for (length = 0, lptr = gethead(fptr); lptr != NIL; lptr = cdr(lptr))
    menu[length++] = getchcode(car(lptr));
  menu[length] = 0;

  /* apply db_menu() to index, xlfail if error */
  if (!(response = tty_menu(menu, header1, header2, footer)))
    xlfail(tty_error);

  /* build stream from returned string */
  fptr = newustream();
  for (i = response; *i; i++) xlputc(fptr, *i);

  /* apply xlread() to convert into expression */
  if (!xlread(fptr, &val, 0)) xlfail("xdb_menu xlread failed");

  /* unsave stuff */
  xlpopn(5);

  /* return expression */
  return val;

}

/******************************************************************************/

/* ask yes-no question, trashes middle of display */
/* argument is: "<question>"
/* returns NIL if no, T if yes */
LVAL xtty_yesno()
{
  char *question;

  /* first argument is question string */
  question = (char *)getstring(xlgastring());

  /* no more arguments */
  xllastarg();

  /* ask question, return NIL or T */
  return tty_yesno(question) ? true : NIL;

} /* xtty_yesno() */

#endif /* FWR_STUFF */
