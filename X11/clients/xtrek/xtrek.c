/*
 * Copyright 1989 Jon Bennett, Mike Bolotski, David Gagne, Dan Lovinger
 * Copyright 1986 Chris Gutherie
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <X11/Xos.h>

#include <stdio.h>
#include <pwd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <netdb.h>
#include <signal.h>

#ifndef FD_SET
#define	FD_SET(n, s)	(((s)->fds_bits[0]) |= (1 << n))
#define	FD_CLR(n, s)	(((s)->fds_bits[0]) &= ~(1 << n))
#define	FD_ZERO(s)	bzero((char *)(s), sizeof (*(s)))
#define	FD_ISSET(n, s)	(((s)->fds_bits[0]) & (1 << n))
#endif

struct sockaddr_in sin;
struct sockaddr_in udp_sin;
struct servent *sp;
jmp_buf         sdied;

done()
{
  fprintf(stderr, "broken pipe\n");
  longjmp(sdied);
}

main(ac, av)
  register int    ac;
  register char **av;
{
  register struct hostent *host = (struct hostent *) NULL;
  struct passwd  *pwent;
  int             s;
  char           *h, *p;
  char            hostname[80];		/* The xtrek-server-machine */
  char            dname[80];
  char           *display;
  extern struct servent *getservbyname();
  extern char    *getenv();

  p = rindex(av[0], '/');
  if (p == (char *) NULL)
    p = av[0];
  if (ac != 2)
    usage(p);

  pwent = getpwuid(getuid());
  if (pwent == (struct passwd *) NULL) {
    printf("Who are you?\n");
    exit(2);
  }
  display = getenv("DISPLAY");
  if (strncmp(display, "unix", 4) == 0) {
    gethostname(dname, sizeof dname);
    strcat(dname, rindex(display, ':'));
    display = dname;
  }
  if (display == (char *) NULL) {
    printf("Your DISPLAY environment variable is not set.\n");
    exit(3);
  }
  h = av[1];
  sin.sin_addr.s_addr = inet_addr(h);
  if (sin.sin_addr.s_addr != -1) {
    sin.sin_family = AF_INET;
    (void) strcpy(hostname, h);
  }
  else {
    host = gethostbyname(h);
    if (host) {
      sin.sin_family = host->h_addrtype;
      bcopy(host->h_addr, (caddr_t) & sin.sin_addr,
	    host->h_length);
      strncpy(hostname, host->h_name, strlen(host->h_name));
    }
    else {
      printf("%s: unknown host\n", h);
      exit(1);
    }
  }

  sin.sin_port = htons(5701);
  signal(SIGPIPE, done);
  s = socket(AF_INET, SOCK_STREAM, 0);

  if (s < 0) {
    perror("socket");
    exit(1);
  }
  if (connect(s, (struct sockaddr *) & sin, sizeof (sin)) < 0) {
    perror("connect");
    (void) close(s);
    exit(1);
  }
  if (setjmp(sdied) == 0)
    startup(s, display, pwent->pw_name);

  exit(0);
}

startup(s, d, l)
  register int    s;
  register char  *d, *l;
{
  char            buf[80];
  int             n = 1;

  sprintf(buf, "Display: %s Login: %s", d, l);
  write(s, buf, strlen(buf));
  write(s, "\015\012", 2);
  fflush(stdout);
  while ((n = read(s, buf, sizeof buf)) >= 0) {
    if (n <= 0) {
      write(s, "DONE\n", 5);		/* Does this cause SIGPIPE? */
      /* No, I think it causes read() above to return -1 */
      sleep(1);
    }
    else
      write(fileno(stdout), buf, n);
  }
  return 1;
}

usage(p)
  register char  *p;
{
  printf("Usage: %s xtrek-server-machine\n", p);
  exit(1);
}
