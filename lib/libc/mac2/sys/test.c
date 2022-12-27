#include <fcntl.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <setjmp.h>

extern int printf();
extern void exit();
extern void _exit();
extern int vfork();
extern int fork();
extern int wait();

test_argv(argc, argv)
int argc;
char **argv;
{

  int i;
  int n;
  int flag;

  (void)printf("argv: argc=%d\n", argc);
  if (argv == 0) {
    (void)printf("argv: null argv\n");
    goto error;
  }
  for (flag = i = n = 0; i < argc; i++) {
    (void)printf("argv: argv[%d]=0x%X=\"%s\"", n, i, argv[i], argv[i]);
    if (i == 2) {
      if (argv[2] == 0) goto error;
      if (atoi(argv[2]) > 0) flag = n = argc - 2;
    }
    if ((i > 1) && (n > 0)) (void)printf(", should be \"%d\"", n);
    (void)printf("\n");
    if (n > 0) if (atoi(argv[i]) != n--) goto error;
  }
  (void)printf("argv: argv[%d]=0x%X\n", argc, argv[argc]);
  if (!flag) {
    (void)printf("argv: no test sequence\n");
    goto error;
  }
  return;

error:
  printf("argv: failed\n");
  exit(1);

}

test_setjmp()
{
  jmp_buf there;

  printf("setjmp: sizeof(jmp_buf) = %d\n", sizeof(jmp_buf));
  if (setjmp(there)) {
    printf("setjmp: there\n");
    return;
  }
  else {
    printf("setjmp: here\n");
    longjmp(there);
  }
  exit(1);
}

test_envp(envp)
char **envp;
{

  int i;
  int n;
  int flag;

  if (envp == 0) {
    (void)printf("envp: null envp\n");
    goto error;
  }
  for (flag = i = n = 0; envp[i] != 0; i++) {
    (void)printf("envp[%d]=0x%X=\"%s\"", i, envp[i], envp[i]);
    if (i == 0) flag = n = atoi(envp[0]);
    if ((n > 0)) (void)printf(", should be \"%d\"", n);
    (void)printf("\n");
    if (n > 0) if (atoi(envp[i]) != n--) goto error;
  }
  (void)printf("envp[%d]=0x0", i);
  if (n > 0) (void)printf(", should be \"%d\"", n);
  (void)printf("\n");
  if (n > 0) goto error;
  if (i == 0) {
    (void)printf("envp: no environment\n");
    goto error;
  }
  if (!flag) {
    (void)printf("envp: no test sequence\n");
    goto error;
  }
  return;

error:
  printf("envp: failed\n");
  exit(1);

}

test_vfork()
{

  char *n;
  int f;
  int i;
  int x;
  int pid, pid2;

  n="/tmp/test_vfork";

  x=0;
  (void)printf("vfork: x=%d\n", x);

  if ((f = open(n, O_WRONLY | O_CREAT, 0600)) < 0) goto bug;
  i = 1;
  if (write(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  if (close(f) != 0) goto bug;
  (void)printf("vfork: wrote %d\n", i);

  pid = vfork();
  (void)printf("vfork: pid=%d\n", pid);

  if (pid == 0) {

    (void)printf("vfork: about to sleep(5) (child)\n");
    sleep(5);

    if ((f = open(n, O_WRONLY | O_APPEND)) < 0) goto bug;
    i = 2;
    if (write(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
    if (close(f) != 0) goto bug;
    (void)printf("vfork: wrote %d (child)\n", i);

    x = 1;
    (void)printf("vfork: x=%d (child)\n", x);

    (void)printf("vfork: about to _exit(0) (child)\n");
    _exit(0);

  }

  if ((f = open(n, O_WRONLY | O_APPEND)) < 0) goto bug;
  i = 3;
  if (write(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  if (close(f) != 0) goto bug;
  (void)printf("vfork: wrote %d (parent)\n", i);

  pid2 = wait(0);
  printf("vfork: wait()=%d, should be %d\n", pid2, pid);
  if (pid2 != pid) goto error;

  if ((f = open(n, O_RDONLY)) < 0) goto bug;
  (void)printf("vfork: f=%d, should be 3\n", f);
  if (f != 3) goto error;

  if (read(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  (void)printf("vfork: read %d, should be 1\n", i);
  if (i != 1) goto error;

  if (read(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  (void)printf("vfork: read %d, should be 2\n", i);
  if (i != 2) goto error;

  if (read(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  (void)printf("vfork: read %d, should be 3\n", i);
  if (i != 3) goto error;

  if (close(f) != 0) goto bug;

  if (unlink(n) != 0) goto bug;

  (void)printf("vfork: x=%d, should be 1\n", x);
  if (x != 1) goto error;

  return;

error:
  (void)printf("vfork: error\n");
  exit(1);

bug:
  perror("vfork bug");
  exit(1);

}

test_fork()
{

  char *n;
  int f;
  int i;
  int x;
  int pid, pid2;

  n="/tmp/test_fork";

  x=0;
  (void)printf("fork: x=%d\n", x);

  if ((f = open(n, O_WRONLY | O_CREAT, 0600)) < 0) goto bug;
  i = 1;
  if (write(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  if (close(f) != 0) goto bug;
  (void)printf("fork: wrote %d\n", i);

  pid = fork();
  (void)printf("fork: pid=%d\n", pid);

  if (pid == 0) {

    (void)printf("fork: about to sleep(5) (child)\n");
    sleep(5);

    if ((f = open(n, O_WRONLY | O_APPEND)) < 0) goto bug;
    i = 2;
    if (write(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
    if (close(f) != 0) goto bug;
    (void)printf("fork: wrote %d (child)\n", i);

    x = 1;
    (void)printf("fork: x=%d (child)\n", x);

    (void)printf("fork: about to _exit(0) (child)\n");
    _exit(0);

  }

  if ((f = open(n, O_WRONLY | O_APPEND)) < 0) goto bug;
  i = 3;
  if (write(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  if (close(f) != 0) goto bug;
  (void)printf("fork: wrote %d (parent)\n", i);

  pid2 = wait(0);
  printf("fork: wait()=%d, should be %d\n", pid2, pid);
  if (pid2 != pid) goto error;

  if ((f = open(n, O_RDONLY)) < 0) goto bug;
  (void)printf("fork: f=%d, should be 3\n", f);
  if (f != 3) goto error;

  if (read(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  (void)printf("fork: read %d, should be 1\n", i);
  if (i != 1) goto error;

  if (read(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  (void)printf("fork: read %d, should be 3\n", i);
  if (i != 3) goto error;

  if (read(f, (char *)&i, sizeof(i)) != sizeof(i)) goto bug;
  (void)printf("fork: read %d, should be 2\n", i);
  if (i != 2) goto error;

  if (close(f) != 0) goto bug;

  if (unlink(n) != 0) goto bug;

  (void)printf("fork: x=%d, should be 0\n", x);
  if (x != 0) goto error;

  return;

error:
  (void)printf("fork: error\n");
  exit(1);

bug:
  perror("fork bug");
  exit(1);

}

test_exit(argc, argv)
int argc;
char **argv;
{
  int i;

  if (argc > 2) i = atoi(argv[2]);
  else i = 1;

  (void)printf("exit: about to exit(0x%X)\n", i);
  exit(i);

}

test_wait()
{
  int pid, pid1, pid2, pid3;
  union wait status;
  int i;

  if ((pid = wait(0)) != -1) {
    (void)printf("wait: no children, wait returned 0x%X\n", pid);
    goto error;
  }

  if ((pid1 = fork()) == 0) {
    sleep(1);
    (void)printf("wait: about to exit(0)\n");
    exit(0);
  }
  (void)printf("wait: started pid = %d\n", pid1);
  sleep(5);
  (void)printf("wait: about to wait(0)\n");
  pid = wait(0);
  (void)printf("wait: pid = %d, should be %d\n", pid, pid1);
  if (pid != pid1) goto error;

  if ((pid1 = fork()) == 0) {
    sleep(5);
    (void)printf("wait: about to exit(%d)\n", 0xF0);
    exit(0xF0);
  }
  (void)printf("wait: started pid = %d\n", pid1);

  if ((pid2 = fork()) == 0) {
    sleep(10);
    (void)printf("wait: about to exit(%d)\n", 074);
    exit(074);
  }
  (void)printf("wait: started pid = %d\n", pid2);

  if ((pid3 = fork()) == 0) {
    sleep(15);
    (void)printf("wait: about to exit(%d)\n", 0x0F);
    exit(0x0F);
  }
  (void)printf("wait: started pid = %d\n", pid3);

  pid = wait(&status);
  (void)printf("wait: pid = %d, should be %d\n", pid, pid1);
  if (pid != pid1) goto error;
  (void)printf("wait: status = 0x%X, should be 0xF000\n", status.w_status);
  if (status.w_status != 0xF000) goto error;
  (void)printf("wait: retcode = %d\n", status.w_retcode);

  pid = wait(&status);
  (void)printf("wait: pid = %d, should be %d\n", pid, pid2);
  if (pid != pid2) goto error;
  (void)printf("wait: status = 0x%X, should be 0x3C00\n", status.w_status);
  if (status.w_status != 0x3C00) goto error;
  (void)printf("wait: retcode = %d\n", status.w_retcode);

  pid = wait(&status);
  (void)printf("wait: pid = %d, should be %d\n", pid, pid3);
  if (pid != pid3) goto error;
  (void)printf("wait: status = 0x%X, should be 0xF00\n", status.w_status);
  if (status.w_status != 0xF00) goto error;
  (void)printf("wait: retcode = %d\n", status.w_retcode);

  if ((pid1 = fork()) == 0) {
    sleep(1);
    (void)printf("wait: waiting for signal\n");
    while (1) sleep(1);
  }
  (void)printf("wait: started pid = %d\n", pid1);
  sleep(5);
  (void)printf("wait: sending signal %d to pid %d\n", SIGKILL, pid1);
  if (kill(pid1, SIGKILL) == -1) {
    perror(wait);
    exit(1);
  }
  pid = wait(&status);
  (void)printf("wait: pid = %d, should be %d\n", pid, pid1);
  if (pid != pid1) goto error;
  (void)printf("wait: status = 0x%X, should be 0x%X\n",
    status.w_status, SIGKILL);
  if (status.w_status != SIGKILL) goto error;
  (void)printf("wait: termsig = %d\n", status.w_termsig);

  return;

error:
  (void)printf("wait: failed\n");
  exit(1);

}

/*
execl(name, arg0, arg1, ..., argn, 0)
char *name, *arg0, *arg1, ..., *argn;
execle(name, arg0, arg1, ..., argn, 0, envp)"
char *name, *arg0, *arg1, ..., *argn, *envp[];"
exect(name, argv, envp)
char *name, *argv[], *envp[];
extern char **environ;
*/

test_exect(prog)
char *prog;
{

  char *argv[10];
  char *envp[10];
  int i;
  union wait status;

  argv[0] = prog;
  argv[1] = "argv";
  argv[2] = "3";
  argv[3] = "2";
  argv[4] = "1";
  argv[5] = 0;
  envp[0] = 0;

  (void)printf("exect: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("exect: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  (void)printf("exect: about to call exect...\n");
  if (fork() == 0) exect(prog, argv, envp);
  wait(&status);
  (void)printf("exect: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  argv[0] = prog;
  argv[1] = "envp";
  argv[2] = 0;
  envp[0] = "3";
  envp[1] = "2";
  envp[2] = "1";
  envp[3] = 0;
  (void)printf("exect: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("exect: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  printf("exect: about to call exect...\n");
  if (fork() == 0) exect(prog, argv, envp);
  wait(&status);
  (void)printf("exect: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  return;

error:
  (void)printf("exect: failed\n");
  exit(1);

}

test_execve(prog)
char *prog;
{

  char *argv[10];
  char *envp[10];
  int i;
  union wait status;

  argv[0] = prog;
  argv[1] = "argv";
  argv[2] = "3";
  argv[3] = "2";
  argv[4] = "1";
  argv[5] = 0;
  envp[0] = 0;

  (void)printf("execve: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("execve: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  (void)printf("execve: about to call execve...\n");
  if (fork() == 0) execve(prog, argv, envp);
  wait(&status);
  (void)printf("execve: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  argv[0] = prog;
  argv[1] = "envp";
  argv[2] = 0;
  envp[0] = "3";
  envp[1] = "2";
  envp[2] = "1";
  envp[3] = 0;
  (void)printf("execve: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("execve: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  printf("execve: about to call execve...\n");
  if (fork() == 0) execve(prog, argv, envp);
  wait(&status);
  (void)printf("execve: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  argv[0] = prog;
  argv[1] = "envp";
  argv[2] = 0;
  (void)printf("execve: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("execve: envp = NULL\n");
  printf("execve: about to call execve...\n");
  if (fork() == 0) execve(prog, argv, 0);
  wait(&status);
  (void)printf("execve: retcode = %d, should be 1\n", status.w_retcode);
  if (status.w_retcode != 1) goto error;

  return;

error:
  (void)printf("execve: failed\n");
  exit(1);

}

test_execv(prog)
char *prog;
{

  extern char **environ;
  char *argv[10];
  char *envp[10];
  int i;
  union wait status;

  argv[0] = prog;
  argv[1] = "argv";
  argv[2] = "3";
  argv[3] = "2";
  argv[4] = "1";
  argv[5] = 0;
  envp[0] = 0;

  (void)printf("execv: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("execv: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  (void)printf("execv: about to call execv...\n");
  if (fork() == 0) {
    environ = envp;
    execv(prog, argv);
  }
  wait(&status);
  (void)printf("execv: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  argv[0] = prog;
  argv[1] = "envp";
  argv[2] = 0;
  envp[0] = "3";
  envp[1] = "2";
  envp[2] = "1";
  envp[3] = 0;
  (void)printf("execv: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("execv: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  printf("execv: about to call execv...\n");
  if (fork() == 0) {
    environ = envp;
    execv(prog, argv);
  }
  wait(&status);
  (void)printf("execv: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  return;

error:
  (void)printf("execv: failed\n");
  exit(1);

}

test_execle(prog)
char *prog;
{

  char *argv[10];
  char *envp[10];
  int i;
  union wait status;

  argv[0] = prog;
  argv[1] = "argv";
  argv[2] = "3";
  argv[3] = "2";
  argv[4] = "1";
  argv[5] = 0;
  envp[0] = 0;

  (void)printf("execle: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("execle: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  (void)printf("execle: about to call execle...\n");
  if (fork() == 0)
    execle(prog, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], envp);
  wait(&status);
  (void)printf("execle: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  argv[0] = prog;
  argv[1] = "envp";
  argv[2] = 0;
  envp[0] = "3";
  envp[1] = "2";
  envp[2] = "1";
  envp[3] = 0;
  (void)printf("execle: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("execle: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  printf("execle: about to call execle...\n");
  if (fork() == 0)
    execle(prog, argv[0], argv[1], argv[2], envp);
  wait(&status);
  (void)printf("execle: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  return;

error:
  (void)printf("execle: failed\n");
  exit(1);

}

test_execl(prog)
char *prog;
{

  extern char **environ;
  char *argv[10];
  char *envp[10];
  int i;
  union wait status;

  argv[0] = prog;
  argv[1] = "argv";
  argv[2] = "3";
  argv[3] = "2";
  argv[4] = "1";
  argv[5] = 0;
  envp[0] = 0;

  (void)printf("execl: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("execl: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  (void)printf("execl: about to call execl...\n");
  if (fork() == 0) {
    environ = envp;
    execl(prog, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
  }
  wait(&status);
  (void)printf("execl: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  argv[0] = prog;
  argv[1] = "envp";
  argv[2] = 0;
  envp[0] = "3";
  envp[1] = "2";
  envp[2] = "1";
  envp[3] = 0;
  (void)printf("execl: argv =");
  for (i = 0; argv[i]; i++) (void)printf(" \"%s\"", argv[i]);
  (void)printf("\n");
  (void)printf("execl: envp =");
  for (i = 0; envp[i]; i++) (void)printf(" \"%s\"", envp[i]);
  (void)printf("\n");
  printf("execl: about to call execl...\n");
  if (fork() == 0) {
    environ = envp;
    execl(prog, argv[0], argv[1], argv[2]);
  }
  wait(&status);
  (void)printf("execl: retcode = %d, should be 0\n", status.w_retcode);
  if (status.w_retcode != 0) goto error;

  return;

error:
  (void)printf("execl: failed\n");
  exit(1);

}

int test(prog, func)
char *prog;
char *func;
{
  char *argv[3];
  union wait status;

  argv[0] = prog;
  argv[1] = func;
  argv[2] = 0;

  if (fork() == 0) execv(prog, argv);
  wait(&status);
  return status.w_retcode;

}

main(argc,argv,envp)
int argc;
char **argv;
char **envp;
{
  int error;

  error = 0;
  if (argc == 1) error++;
  else if (!strcmp(argv[1], "argv")) test_argv(argc, argv);
  else if (!strcmp(argv[1], "envp")) test_envp(envp);
  else if (!strcmp(argv[1], "fork")) test_fork();
  else if (!strcmp(argv[1], "vfork")) test_vfork();
  else if (!strcmp(argv[1], "exect")) test_exect(argv[0]);
  else if (!strcmp(argv[1], "execv")) test_execv(argv[0]);
  else if (!strcmp(argv[1], "execve")) test_execve(argv[0]);
  else if (!strcmp(argv[1], "execl")) test_execl(argv[0]);
  else if (!strcmp(argv[1], "execle")) test_execle(argv[0]);
  else if (!strcmp(argv[1], "exit")) test_exit(argc, argv);
  else if (!strcmp(argv[1], "wait")) test_wait();
  else if (!strcmp(argv[1], "setjmp")) test_setjmp();
  else if (!strcmp(argv[1], "all")) {
    if (!test(argv[0], "argv")) exit(1);
    if (!test(argv[0], "envp")) exit(1);
    if (test(argv[0], "wait")) exit(1);
    if (test(argv[0], "fork")) exit(1);
    if (test(argv[0], "execve")) exit(1);
    if (test(argv[0], "execv")) exit(1);
    if (test(argv[0], "execle")) exit(1);
    if (test(argv[0], "execl")) exit(1);
    if (test(argv[0], "vfork")) /*exit(1)*/;
  }
  else error++;
  if (error) {
   (void)printf("usage: %s <function> <options>\n", argv[0]);
    exit(1);
  }
  exit(0);
}

/*
syscalls yet to test...
accept
access
acct
adjtime
bind
brk
cerror
chdir
chmod
chown
chroot
close
connect
creat
dup
dup2
fchmod
fchown
fcntl
flock
fstat
fsync
ftruncate
getdtablesize
getegid
geteuid
getgid
getgroups
gethostid
gethostname
getitimer
getpagesize
getpeername
getpgrp
getpid
getppid
getpriority
getrlimit
getrusage
getsockname
getsockopt
gettimeofday
getuid
ioctl
kill
killpg
link
listen
lseek
lstat
mkdir
mknod
mount
open
pipe
profil
ptrace
quota
read
readlink
readv
reboot
recv
recvfrom
recvmsg
rename
rmdir
sbrk
select
send
sendmsg
sendto
setgroups
sethostid
sethostname
setitimer
setpgrp
setpriority
setquota
setregid
setreuid
setrlimit
setsockopt
settimeofday
shutdown
sigblock
sigpause
sigreturn
sigsetmask
sigstack
sigvec
socket
socketpair
stat
swapon
symlink
sync
syscall
truncate
umask
umount
unlink
utimes
vadvise
vhangup
wait3
write
writev
*/
