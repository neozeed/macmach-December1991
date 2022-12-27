/* This program returns the termcap entry for the MacMach console. */

/* This also sets the correct erase character and window size */

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/signal.h>

#define TIMEOUT 2

#define SH  1
#define CSH 2

static struct sgttyb saved_sgttyb;
static int ttyfd;

static error()
{
  if (ttyfd) ioctl(ttyfd, TIOCSETP, &saved_sgttyb);
  exit(1);
}

main(int argc, char **argv)
{
  struct sgttyb sgttyb;
  struct winsize win;
  char termcap[1024], msg[80], bp[1024], area[1024], *ap = area;
  int code, n;

  if (argc == 1) code = 0;
  else if ((argc == 2) && !strcmp(argv[1], "-sh")) code = SH;
  else if ((argc == 2) && !strcmp(argv[1], "-csh")) code = CSH;
  else {
    fprintf(stderr, "usage: mac2term [ -sh | -csh ]\n");
    exit(1);
  }
  if ((ttyfd = open("/dev/tty", 2)) < 0) exit(1);
  ioctl(ttyfd, TIOCGETP, &saved_sgttyb);
  ioctl(ttyfd, TIOCGETP, &sgttyb);
  sgttyb.sg_flags &= ~ECHO;
  ioctl(ttyfd, TIOCSETP, &sgttyb);
  termcap[0] = 'F' - '@';
  signal(SIGALRM, error);
  alarm(TIMEOUT);
  write(ttyfd, termcap, 1);
  n = read(ttyfd, termcap, sizeof(termcap) - 1);
  ioctl(ttyfd, TIOCSETP, &saved_sgttyb);
  if (n <= 0) error();
  termcap[n] = 0;
  setenv("TERMCAP", termcap, 1);
  if (tgetent(bp, "mac2") != 1) error();
  if (tgetstr("kb", &ap)) {
    saved_sgttyb.sg_erase = *(char *)tgetstr("kb", &ap);
    ioctl(ttyfd, TIOCSETP, &saved_sgttyb);
  }
  ioctl(ttyfd, TIOCGWINSZ, &win);
  win.ws_row = tgetnum("li");
  win.ws_col = tgetnum("co");
  ioctl(ttyfd, TIOCSWINSZ, &win);
  sprintf(msg, "%cmac2term: %d x %d%c", 1, win.ws_row, win.ws_col, 2);
  write(ttyfd, msg, strlen(msg));
  close(ttyfd);
  if (code == SH)
    printf("TERM=\"mac2\"; TERMCAP=\"%s\"; export TERM TERMCAP\n", termcap);
  else if (code == CSH)
    printf("setenv TERM \"mac2\"; setenv TERMCAP \"%s\"\n", termcap);
  else puts(termcap);
  exit(0);
}
