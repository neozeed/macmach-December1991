/* this program allows the user to control the sony drive on a mac2 */

#ifndef SMALL
#include <stdio.h>
#endif /* SMALL */
#include <string.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <mac2dev/sony.h>

/* /dev/sony0 -- first internal drive
 * /dev/sony1 -- second internal drive
 * /dev/sony2 -- external drive
 */
#define SONY "/dev/sony%d"

main(int argc, char **argv)
{
  char *name;     /* name this program called with (argv[0]) */
  char sony[20];  /* sony device */
  int error;      /* apple error */
  int f;          /* fd for sony drive */
  int usage = 0;  /* usage error if non-zero */
  int format = 0; /* do format if non-zero */
  int eject = 0;  /* do eject if non-zero */
  int drive = 0;  /* sony drive number: 0, 1 or 2 */
  int mode = 0;   /* format mode */

  /* process options and arguments, set flags */
  if (name = rindex(argv[0], '/')) name++;
  else name = argv[0];
  argc--; argv++;
  if (!strcmp(name, "eject")) eject++;
  else {
    if (argc) {
      if (!strcmp(*argv, "format")) format++;
      else if (!strcmp(*argv, "eject")) eject++;
      else {
#ifndef SMALL
        fprintf(stderr, "valid options are: eject and format\n");
#endif /* SMALL */
        usage++;
      }
      argc--; argv++;
    }
    else usage++;
  }
  if (!usage && argc) {
    if ((sscanf(*argv, "%d", &drive) != 1) || (drive < 0) || (drive > 2)) {
#ifndef SMALL
      fprintf(stderr, "valid drive numbers are: 0, 1 and 2\n");
#endif /* SMALL */
      usage++;
    }
    argc--; argv++;
  }
  if (argc) usage++;

  /* if usage error flagged... */
  if (usage) {
#ifndef SMALL
    if (eject) fprintf(stderr, "usage: %s [drive#]\n", name);
    else {
      fprintf(stderr, "usage: %s [option] [drive#]\n", name);
      fprintf(stderr, "options: format, eject\n");
    }
#endif /* SMALL */
    exit(1);
  }

  /* compose sony device name */
  sprintf(sony, SONY, drive);

  /* if eject flagged... */
  if (eject) {
    if ((f = open(sony, O_RDONLY)) < 0) {
#ifndef SMALL
      perror(sony);
#endif /* SMALL */
      exit(1);
    }
    if (ioctl(f, IOC_VOID | SONY_EJECT, 0) < 0) {
#ifndef SMALL
      perror("SONY_EJECT");
#endif /* SMALL */
      exit(1);
    }
    close(f);
    exit(0);
  }

  /* if format flagged... */
  if (format) {
    if ((f = open(sony, O_RDWR)) < 0) {
#ifndef SMALL
      perror(sony);
#endif /* SMALL */
      exit(1);
    }
    if (ioctl(f, IOC_VOID | SONY_FORMAT, &mode) < 0) {
#ifndef SMALL
      perror("SONY_FORMAT");
#endif /* SMALL */
      exit(1);
    }
    close(f);
    exit(0);
  }

}
