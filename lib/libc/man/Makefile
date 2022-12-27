# Makefile to install library man pages

# These need to be moved to the library source directories !!!

MANDIR=	${DESTDIR}/usr/man/man3

FILES=	abort.3 abs.3 abspath.3 accept.3 access.3 acct.3 \
	adjtime.3 alarm.3 asinh.3 assert.3 atof.3 atoh.3 \
	atot.3 bind.3 bindresvport.3 boolarg.3 bstring.3 \
	byteorder.3 chdir.3 chmod.3 chown.3 chrarg.3 ci.3 \
	close.3 compat-sys5.3 concat.3 connect.3 \
	convert_ts_to_tv.3 creat.3 crypt.3 ctime.3 ctype.3 \
	curses.3 dbm.3 del.3 directory.3 doublearg.3 dup.3 \
	ecvt.3 editor.3 end.3 env_conn.3 env_list.3 env_port.3 \
	env_string.3 erf.3 execl.3 execve.3 exit.3 exp.3 \
	expand.3 fclose.3 fcntl.3 fdate.3 ferror.3 fgetpass.3 \
	fgets.3 filecopy.3 flock.3 floor.3 fmap.3 fold.3 \
	fopen.3 fork.3 fread.3 frexp.3 fseek.3 fsync.3 \
	getacent.3 getbool.3 getc.3 getchr.3 getdirentries.3 \
	getdiskbyname.3 getdomainname.3 getdouble.3 getdtablesize.3 \
	getenv.3 getfsent.3 getgid.3 getgrent.3 getgroups.3 \
	gethostbyname.3 gethostid.3 gethostname.3 getint.3 \
	getitimer.3 getlogin.3 getmntent.3 getname.3 getnetent.3 \
	getoct.3 getopt.3 getpagesize.3 getpass.3 getpeername.3 \
	getpgrp.3 getpid.3 getpriority.3 getprotoent.3 getpw.3 \
	getpwent.3 getrlimit.3 getrpcent.3 getrpcport.3 \
	getrusage.3 gets.3 getservent.3 getsockname.3 getsockopt.3 \
	getstr.3 gettimeofday.3 getttyent.3 getuid.3 \
	getusershell.3 getwd.3 gtime.3 hardcrypt.3 hypot.3 \
	ieee.3 inet.3 infnan.3 initgroups.3 insque.3 intarg.3 \
	intro.3 ioctl.3 ipcx.3 j0.3 kill.3 killpg.3 lgamma.3 \
	lib2648.3 link.3 listen.3 lseek.3 mach_error.3 \
	mach_exception.3 mach_init.3 makepath.3 malloc.3 mkdir.3 \
	mknod.3 mkstemp.3 mktemp.3 monitor.3 mount.3 movefile.3 \
	mp.3 ndbm.3 netname.3 nice.3 nlist.3 ns.3 nxtarg.3 \
	octarg.3 oklogin.3 okpassword.3 open.3 openp.3 \
	parsedate.3 path.3 pathof.3 pause.3 perror.3 pipe.3 \
	plot.3 popen.3 printf.3 profil.3 prstab.3 psignal.3 \
	ptrace.3 putc.3 puts.3 qsort.3 quit.3 quota.3 rand.3 \
	random.3 rcmd.3 regex.3 regexp.3 resolver.3 rexec.3 \
	run.3 salloc.3 scandir.3 scanf.3 service_checkin.3 \
	setbuf.3 setjmp.3 setuid.3 siginterrupt.3 signal.3 \
	sin.3 sindex.3 sinh.3 skipto.3 sleep.3 slot_name.3 \
	sqrt.3 srchscore.3 stablk.3 stabsearch.3 stdio.3 \
	stlmatch.3 strarg.3 string.3 stty.3 swab.3 syslog.3 \
	system.3 termcap.3 threads.3 time.3 times.3 ttyname.3 \
	ualarm.3 ungetc.3 usleep.3 utime.3 valloc.3 varargs.3 \
	vlimit.3 vtimes.3 wantread.3 wantwrite.3 warnlogin.3 xdr.3

all:	FRC

install: FRC

install.man:	FRC
	install -c -o bin -g bin -m 444 ${FILES} ${MANDIR}/
	rm -f ${MANDIR}/acosh.3; \
	  ln -s asinh.3 ${MANDIR}/acosh.3
	rm -f ${MANDIR}/atanh.3; \
	  ln -s asinh.3 ${MANDIR}/atanh.3
	rm -f ${MANDIR}/atoi.3; \
	  ln -s atof.3 ${MANDIR}/atoi.3
	rm -f ${MANDIR}/atol.3; \
	  ln -s atof.3 ${MANDIR}/atol.3
	rm -f ${MANDIR}/bcmp.3; \
	  ln -s bstring.3 ${MANDIR}/bcmp.3
	rm -f ${MANDIR}/bcopy.3; \
	  ln -s bstring.3 ${MANDIR}/bcopy.3
	rm -f ${MANDIR}/bzero.3; \
	  ln -s bstring.3 ${MANDIR}/bzero.3
	rm -f ${MANDIR}/ffs.3; \
	  ln -s bstring.3 ${MANDIR}/ffs.3
	rm -f ${MANDIR}/htonl.3; \
	  ln -s byteorder.3 ${MANDIR}/htonl.3
	rm -f ${MANDIR}/htons.3; \
	  ln -s byteorder.3 ${MANDIR}/htons.3
	rm -f ${MANDIR}/ntohl.3; \
	  ln -s byteorder.3 ${MANDIR}/ntohl.3
	rm -f ${MANDIR}/ntohs.3; \
	  ln -s byteorder.3 ${MANDIR}/ntohs.3
	rm -f ${MANDIR}/memccpy.3; \
	  ln -s compat-sys5.3 ${MANDIR}/memccpy.3
	rm -f ${MANDIR}/memchr.3; \
	  ln -s compat-sys5.3 ${MANDIR}/memchr.3
	rm -f ${MANDIR}/memcmp.3; \
	  ln -s compat-sys5.3 ${MANDIR}/memcmp.3
	rm -f ${MANDIR}/memcpy.3; \
	  ln -s compat-sys5.3 ${MANDIR}/memcpy.3
	rm -f ${MANDIR}/memset.3; \
	  ln -s compat-sys5.3 ${MANDIR}/memset.3
	rm -f ${MANDIR}/strchr.3; \
	  ln -s compat-sys5.3 ${MANDIR}/strchr.3
	rm -f ${MANDIR}/strcspn.3; \
	  ln -s compat-sys5.3 ${MANDIR}/strcspn.3
	rm -f ${MANDIR}/strrchr.3; \
	  ln -s compat-sys5.3 ${MANDIR}/strrchr.3
	rm -f ${MANDIR}/strspn.3; \
	  ln -s compat-sys5.3 ${MANDIR}/strspn.3
	rm -f ${MANDIR}/strtok.3; \
	  ln -s compat-sys5.3 ${MANDIR}/strtok.3
	rm -f ${MANDIR}/tempnam.3; \
	  ln -s compat-sys5.3 ${MANDIR}/tempnam.3
	rm -f ${MANDIR}/tmpfile.3; \
	  ln -s compat-sys5.3 ${MANDIR}/tmpfile.3
	rm -f ${MANDIR}/tmpnam.3; \
	  ln -s compat-sys5.3 ${MANDIR}/tmpnam.3
	rm -f ${MANDIR}/encrypt.3; \
	  ln -s crypt.3 ${MANDIR}/encrypt.3
	rm -f ${MANDIR}/setkey.3; \
	  ln -s crypt.3 ${MANDIR}/setkey.3
	rm -f ${MANDIR}/asctime.3; \
	  ln -s ctime.3 ${MANDIR}/asctime.3
	rm -f ${MANDIR}/gmtime.3; \
	  ln -s ctime.3 ${MANDIR}/gmtime.3
	rm -f ${MANDIR}/localtime.3; \
	  ln -s ctime.3 ${MANDIR}/localtime.3
	rm -f ${MANDIR}/timezone.3; \
	  ln -s ctime.3 ${MANDIR}/timezone.3
	rm -f ${MANDIR}/tzset.3; \
	  ln -s ctime.3 ${MANDIR}/tzset.3
	rm -f ${MANDIR}/isalnum.3; \
	  ln -s ctype.3 ${MANDIR}/isalnum.3
	rm -f ${MANDIR}/isalpha.3; \
	  ln -s ctype.3 ${MANDIR}/isalpha.3
	rm -f ${MANDIR}/isascii.3; \
	  ln -s ctype.3 ${MANDIR}/isascii.3
	rm -f ${MANDIR}/iscntrl.3; \
	  ln -s ctype.3 ${MANDIR}/iscntrl.3
	rm -f ${MANDIR}/isdigit.3; \
	  ln -s ctype.3 ${MANDIR}/isdigit.3
	rm -f ${MANDIR}/islower.3; \
	  ln -s ctype.3 ${MANDIR}/islower.3
	rm -f ${MANDIR}/isprint.3; \
	  ln -s ctype.3 ${MANDIR}/isprint.3
	rm -f ${MANDIR}/ispunct.3; \
	  ln -s ctype.3 ${MANDIR}/ispunct.3
	rm -f ${MANDIR}/isspace.3; \
	  ln -s ctype.3 ${MANDIR}/isspace.3
	rm -f ${MANDIR}/isupper.3; \
	  ln -s ctype.3 ${MANDIR}/isupper.3
	rm -f ${MANDIR}/isxdigit.3; \
	  ln -s ctype.3 ${MANDIR}/isxdigit.3
	rm -f ${MANDIR}/toascii.3; \
	  ln -s ctype.3 ${MANDIR}/toascii.3
	rm -f ${MANDIR}/tolower.3; \
	  ln -s ctype.3 ${MANDIR}/tolower.3
	rm -f ${MANDIR}/toupper.3; \
	  ln -s ctype.3 ${MANDIR}/toupper.3
	rm -f ${MANDIR}/dbminit.3; \
	  ln -s dbm.3 ${MANDIR}/dbminit.3
	rm -f ${MANDIR}/delete.3; \
	  ln -s dbm.3 ${MANDIR}/delete.3
	rm -f ${MANDIR}/fetch.3; \
	  ln -s dbm.3 ${MANDIR}/fetch.3
	rm -f ${MANDIR}/firstkey.3; \
	  ln -s dbm.3 ${MANDIR}/firstkey.3
	rm -f ${MANDIR}/nextkey.3; \
	  ln -s dbm.3 ${MANDIR}/nextkey.3
	rm -f ${MANDIR}/store.3; \
	  ln -s dbm.3 ${MANDIR}/store.3
	rm -f ${MANDIR}/closedir.3; \
	  ln -s directory.3 ${MANDIR}/closedir.3
	rm -f ${MANDIR}/dirfd.3; \
	  ln -s directory.3 ${MANDIR}/dirfd.3
	rm -f ${MANDIR}/opendir.3; \
	  ln -s directory.3 ${MANDIR}/opendir.3
	rm -f ${MANDIR}/readdir.3; \
	  ln -s directory.3 ${MANDIR}/readdir.3
	rm -f ${MANDIR}/rewinddir.3; \
	  ln -s directory.3 ${MANDIR}/rewinddir.3
	rm -f ${MANDIR}/seekdir.3; \
	  ln -s directory.3 ${MANDIR}/seekdir.3
	rm -f ${MANDIR}/telldir.3; \
	  ln -s directory.3 ${MANDIR}/telldir.3
	rm -f ${MANDIR}/fcvt.3; \
	  ln -s ecvt.3 ${MANDIR}/fcvt.3
	rm -f ${MANDIR}/gcvt.3; \
	  ln -s ecvt.3 ${MANDIR}/gcvt.3
	rm -f ${MANDIR}/edata.3; \
	  ln -s end.3 ${MANDIR}/edata.3
	rm -f ${MANDIR}/etext.3; \
	  ln -s end.3 ${MANDIR}/etext.3
	rm -f ${MANDIR}/erfc.3; \
	  ln -s erf.3 ${MANDIR}/erfc.3
	rm -f ${MANDIR}/exec.3; \
	  ln -s execl.3 ${MANDIR}/exec.3
	rm -f ${MANDIR}/execle.3; \
	  ln -s execl.3 ${MANDIR}/execle.3
	rm -f ${MANDIR}/execlp.3; \
	  ln -s execl.3 ${MANDIR}/execlp.3
	rm -f ${MANDIR}/exect.3; \
	  ln -s execl.3 ${MANDIR}/exect.3
	rm -f ${MANDIR}/execv.3; \
	  ln -s execl.3 ${MANDIR}/execv.3
	rm -f ${MANDIR}/execvp.3; \
	  ln -s execl.3 ${MANDIR}/execvp.3
	rm -f ${MANDIR}/brk.3; \
	  ln -s exp.3 ${MANDIR}/brk.3
	rm -f ${MANDIR}/expm1.3; \
	  ln -s exp.3 ${MANDIR}/expm1.3
	rm -f ${MANDIR}/log.3; \
	  ln -s exp.3 ${MANDIR}/log.3
	rm -f ${MANDIR}/log10.3; \
	  ln -s exp.3 ${MANDIR}/log10.3
	rm -f ${MANDIR}/log1p.3; \
	  ln -s exp.3 ${MANDIR}/log1p.3
	rm -f ${MANDIR}/pow.3; \
	  ln -s exp.3 ${MANDIR}/pow.3
	rm -f ${MANDIR}/fflush.3; \
	  ln -s fclose.3 ${MANDIR}/fflush.3
	rm -f ${MANDIR}/clearerr.3; \
	  ln -s ferror.3 ${MANDIR}/clearerr.3
	rm -f ${MANDIR}/feof.3; \
	  ln -s ferror.3 ${MANDIR}/feof.3
	rm -f ${MANDIR}/fileno.3; \
	  ln -s ferror.3 ${MANDIR}/fileno.3
	rm -f ${MANDIR}/ceil.3; \
	  ln -s floor.3 ${MANDIR}/ceil.3
	rm -f ${MANDIR}/fabs.3; \
	  ln -s floor.3 ${MANDIR}/fabs.3
	rm -f ${MANDIR}/rint.3; \
	  ln -s floor.3 ${MANDIR}/rint.3
	rm -f ${MANDIR}/fdopen.3; \
	  ln -s fopen.3 ${MANDIR}/fdopen.3
	rm -f ${MANDIR}/freopen.3; \
	  ln -s fopen.3 ${MANDIR}/freopen.3
	rm -f ${MANDIR}/fwrite.3; \
	  ln -s fread.3 ${MANDIR}/fwrite.3
	rm -f ${MANDIR}/ldexp.3; \
	  ln -s frexp.3 ${MANDIR}/ldexp.3
	rm -f ${MANDIR}/modf.3; \
	  ln -s frexp.3 ${MANDIR}/modf.3
	rm -f ${MANDIR}/ftell.3; \
	  ln -s fseek.3 ${MANDIR}/ftell.3
	rm -f ${MANDIR}/rewind.3; \
	  ln -s fseek.3 ${MANDIR}/rewind.3
	rm -f ${MANDIR}/fgetc.3; \
	  ln -s getc.3 ${MANDIR}/fgetc.3
	rm -f ${MANDIR}/getchar.3; \
	  ln -s getc.3 ${MANDIR}/getchar.3
	rm -f ${MANDIR}/getw.3; \
	  ln -s getc.3 ${MANDIR}/getw.3
	rm -f ${MANDIR}/setenv.3; \
	  ln -s getenv.3 ${MANDIR}/setenv.3
	rm -f ${MANDIR}/unsetenv.3; \
	  ln -s getenv.3 ${MANDIR}/unsetenv.3
	rm -f ${MANDIR}/endfsent.3; \
	  ln -s getfsent.3 ${MANDIR}/endfsent.3
	rm -f ${MANDIR}/getfsfile.3; \
	  ln -s getfsent.3 ${MANDIR}/getfsfile.3
	rm -f ${MANDIR}/getfsspec.3; \
	  ln -s getfsent.3 ${MANDIR}/getfsspec.3
	rm -f ${MANDIR}/getfstype.3; \
	  ln -s getfsent.3 ${MANDIR}/getfstype.3
	rm -f ${MANDIR}/setfsent.3; \
	  ln -s getfsent.3 ${MANDIR}/setfsent.3
	rm -f ${MANDIR}/endgrent.3; \
	  ln -s getgrent.3 ${MANDIR}/endgrent.3
	rm -f ${MANDIR}/getgrgid.3; \
	  ln -s getgrent.3 ${MANDIR}/getgrgid.3
	rm -f ${MANDIR}/getgrnam.3; \
	  ln -s getgrent.3 ${MANDIR}/getgrnam.3
	rm -f ${MANDIR}/setgrent.3; \
	  ln -s getgrent.3 ${MANDIR}/setgrent.3
	rm -f ${MANDIR}/setgrfile.3; \
	  ln -s getgrent.3 ${MANDIR}/setgrfile.3
	rm -f ${MANDIR}/endhostent.3; \
	  ln -s gethostbyname.3 ${MANDIR}/endhostent.3
	rm -f ${MANDIR}/gethostbyaddr.3; \
	  ln -s gethostbyname.3 ${MANDIR}/gethostbyaddr.3
	rm -f ${MANDIR}/gethostent.3; \
	  ln -s gethostbyname.3 ${MANDIR}/gethostent.3
	rm -f ${MANDIR}/sethostent.3; \
	  ln -s gethostbyname.3 ${MANDIR}/sethostent.3
	rm -f ${MANDIR}/sethostfile.3; \
	  ln -s gethostbyname.3 ${MANDIR}/sethostfile.3
	rm -f ${MANDIR}/endnetent.3; \
	  ln -s getnetent.3 ${MANDIR}/endnetent.3
	rm -f ${MANDIR}/getnetbyaddr.3; \
	  ln -s getnetent.3 ${MANDIR}/getnetbyaddr.3
	rm -f ${MANDIR}/getnetbyname.3; \
	  ln -s getnetent.3 ${MANDIR}/getnetbyname.3
	rm -f ${MANDIR}/setnetent.3; \
	  ln -s getnetent.3 ${MANDIR}/setnetent.3
	rm -f ${MANDIR}/endprotoent.3; \
	  ln -s getprotoent.3 ${MANDIR}/endprotoent.3
	rm -f ${MANDIR}/getprotobyname.3; \
	  ln -s getprotoent.3 ${MANDIR}/getprotobyname.3
	rm -f ${MANDIR}/getprotobynumber.3; \
	  ln -s getprotoent.3 ${MANDIR}/getprotobynumber.3
	rm -f ${MANDIR}/setprotoent.3; \
	  ln -s getprotoent.3 ${MANDIR}/setprotoent.3
	rm -f ${MANDIR}/endpwent.3; \
	  ln -s getpwent.3 ${MANDIR}/endpwent.3
	rm -f ${MANDIR}/getpwnam.3; \
	  ln -s getpwent.3 ${MANDIR}/getpwnam.3
	rm -f ${MANDIR}/getpwuid.3; \
	  ln -s getpwent.3 ${MANDIR}/getpwuid.3
	rm -f ${MANDIR}/setpwent.3; \
	  ln -s getpwent.3 ${MANDIR}/setpwent.3
	rm -f ${MANDIR}/setpwfile.3; \
	  ln -s getpwent.3 ${MANDIR}/setpwfile.3
	rm -f ${MANDIR}/endservent.3; \
	  ln -s getservent.3 ${MANDIR}/endservent.3
	rm -f ${MANDIR}/getservbyname.3; \
	  ln -s getservent.3 ${MANDIR}/getservbyname.3
	rm -f ${MANDIR}/getservbyport.3; \
	  ln -s getservent.3 ${MANDIR}/getservbyport.3
	rm -f ${MANDIR}/setservent.3; \
	  ln -s getservent.3 ${MANDIR}/setservent.3
	rm -f ${MANDIR}/endttyent.3; \
	  ln -s getttyent.3 ${MANDIR}/endttyent.3
	rm -f ${MANDIR}/getttynam.3; \
	  ln -s getttyent.3 ${MANDIR}/getttynam.3
	rm -f ${MANDIR}/setttyent.3; \
	  ln -s getttyent.3 ${MANDIR}/setttyent.3
	rm -f ${MANDIR}/endusershell.3; \
	  ln -s getusershell.3 ${MANDIR}/endusershell.3
	rm -f ${MANDIR}/setusershell.3; \
	  ln -s getusershell.3 ${MANDIR}/setusershell.3
	rm -f ${MANDIR}/cabs.3; \
	  ln -s hypot.3 ${MANDIR}/cabs.3
	rm -f ${MANDIR}/copysign.3; \
	  ln -s ieee.3 ${MANDIR}/copysign.3
	rm -f ${MANDIR}/drem.3; \
	  ln -s ieee.3 ${MANDIR}/drem.3
	rm -f ${MANDIR}/finite.3; \
	  ln -s ieee.3 ${MANDIR}/finite.3
	rm -f ${MANDIR}/logb.3; \
	  ln -s ieee.3 ${MANDIR}/logb.3
	rm -f ${MANDIR}/scalb.3; \
	  ln -s ieee.3 ${MANDIR}/scalb.3
	rm -f ${MANDIR}/addr.3; \
	  ln -s inet.3 ${MANDIR}/addr.3
	rm -f ${MANDIR}/inet_addr.3; \
	  ln -s inet.3 ${MANDIR}/inet_addr.3
	rm -f ${MANDIR}/inet_lnaof.3; \
	  ln -s inet.3 ${MANDIR}/inet_lnaof.3
	rm -f ${MANDIR}/inet_makeaddr.3; \
	  ln -s inet.3 ${MANDIR}/inet_makeaddr.3
	rm -f ${MANDIR}/inet_netof.3; \
	  ln -s inet.3 ${MANDIR}/inet_netof.3
	rm -f ${MANDIR}/inet_network.3; \
	  ln -s inet.3 ${MANDIR}/inet_network.3
	rm -f ${MANDIR}/inet_ntoa.3; \
	  ln -s inet.3 ${MANDIR}/inet_ntoa.3
	rm -f ${MANDIR}/network.3; \
	  ln -s inet.3 ${MANDIR}/network.3
	rm -f ${MANDIR}/ntoa.3; \
	  ln -s inet.3 ${MANDIR}/ntoa.3
	rm -f ${MANDIR}/remque.3; \
	  ln -s insque.3 ${MANDIR}/remque.3
	rm -f ${MANDIR}/getpwwho.3; \
	  ln -s intarg.3 ${MANDIR}/getpwwho.3
	rm -f ${MANDIR}/j1.3; \
	  ln -s j0.3 ${MANDIR}/j1.3
	rm -f ${MANDIR}/jn.3; \
	  ln -s j0.3 ${MANDIR}/jn.3
	rm -f ${MANDIR}/y0.3; \
	  ln -s j0.3 ${MANDIR}/y0.3
	rm -f ${MANDIR}/y1.3; \
	  ln -s j0.3 ${MANDIR}/y1.3
	rm -f ${MANDIR}/yn.3; \
	  ln -s j0.3 ${MANDIR}/yn.3
	rm -f ${MANDIR}/gamma.3; \
	  ln -s lgamma.3 ${MANDIR}/gamma.3
	rm -f ${MANDIR}/alloca.3; \
	  ln -s malloc.3 ${MANDIR}/alloca.3
	rm -f ${MANDIR}/calloc.3; \
	  ln -s malloc.3 ${MANDIR}/calloc.3
	rm -f ${MANDIR}/free.3; \
	  ln -s malloc.3 ${MANDIR}/free.3
	rm -f ${MANDIR}/realloc.3; \
	  ln -s malloc.3 ${MANDIR}/realloc.3
	rm -f ${MANDIR}/moncontrol.3; \
	  ln -s monitor.3 ${MANDIR}/moncontrol.3
	rm -f ${MANDIR}/monstartup.3; \
	  ln -s monitor.3 ${MANDIR}/monstartup.3
	rm -f ${MANDIR}/dbm_clearerr.3; \
	  ln -s ndbm.3 ${MANDIR}/dbm_clearerr.3
	rm -f ${MANDIR}/dbm_close.3; \
	  ln -s ndbm.3 ${MANDIR}/dbm_close.3
	rm -f ${MANDIR}/dbm_delete.3; \
	  ln -s ndbm.3 ${MANDIR}/dbm_delete.3
	rm -f ${MANDIR}/dbm_error.3; \
	  ln -s ndbm.3 ${MANDIR}/dbm_error.3
	rm -f ${MANDIR}/dbm_fetch.3; \
	  ln -s ndbm.3 ${MANDIR}/dbm_fetch.3
	rm -f ${MANDIR}/dbm_firstkey.3; \
	  ln -s ndbm.3 ${MANDIR}/dbm_firstkey.3
	rm -f ${MANDIR}/dbm_nextkey.3; \
	  ln -s ndbm.3 ${MANDIR}/dbm_nextkey.3
	rm -f ${MANDIR}/dbm_open.3; \
	  ln -s ndbm.3 ${MANDIR}/dbm_open.3
	rm -f ${MANDIR}/dbm_store.3; \
	  ln -s ndbm.3 ${MANDIR}/dbm_store.3
	rm -f ${MANDIR}/ns_addr.3; \
	  ln -s ns.3 ${MANDIR}/ns_addr.3
	rm -f ${MANDIR}/ns_ntoa.3; \
	  ln -s ns.3 ${MANDIR}/ns_ntoa.3
	rm -f ${MANDIR}/errlist.3; \
	  ln -s perror.3 ${MANDIR}/errlist.3
	rm -f ${MANDIR}/sys.3; \
	  ln -s perror.3 ${MANDIR}/sys.3
	rm -f ${MANDIR}/sys_errlist.3; \
	  ln -s perror.3 ${MANDIR}/sys_errlist.3
	rm -f ${MANDIR}/sys_nerr.3; \
	  ln -s perror.3 ${MANDIR}/sys_nerr.3
	rm -f ${MANDIR}/arc.3; \
	  ln -s plot.3 ${MANDIR}/arc.3
	rm -f ${MANDIR}/circle.3; \
	  ln -s plot.3 ${MANDIR}/circle.3
	rm -f ${MANDIR}/cont.3; \
	  ln -s plot.3 ${MANDIR}/cont.3
	rm -f ${MANDIR}/erase.3; \
	  ln -s plot.3 ${MANDIR}/erase.3
	rm -f ${MANDIR}/label.3; \
	  ln -s plot.3 ${MANDIR}/label.3
	rm -f ${MANDIR}/line.3; \
	  ln -s plot.3 ${MANDIR}/line.3
	rm -f ${MANDIR}/linemod.3; \
	  ln -s plot.3 ${MANDIR}/linemod.3
	rm -f ${MANDIR}/move.3; \
	  ln -s plot.3 ${MANDIR}/move.3
	rm -f ${MANDIR}/openpl.3; \
	  ln -s plot.3 ${MANDIR}/openpl.3
	rm -f ${MANDIR}/point.3; \
	  ln -s plot.3 ${MANDIR}/point.3
	rm -f ${MANDIR}/space.3; \
	  ln -s plot.3 ${MANDIR}/space.3
	rm -f ${MANDIR}/pclose.3; \
	  ln -s popen.3 ${MANDIR}/pclose.3
	rm -f ${MANDIR}/fprintf.3; \
	  ln -s printf.3 ${MANDIR}/fprintf.3
	rm -f ${MANDIR}/sprintf.3; \
	  ln -s printf.3 ${MANDIR}/sprintf.3
	rm -f ${MANDIR}/sys_siglist.3; \
	  ln -s psignal.3 ${MANDIR}/sys_siglist.3
	rm -f ${MANDIR}/fputc.3; \
	  ln -s putc.3 ${MANDIR}/fputc.3
	rm -f ${MANDIR}/putchar.3; \
	  ln -s putc.3 ${MANDIR}/putchar.3
	rm -f ${MANDIR}/putw.3; \
	  ln -s putc.3 ${MANDIR}/putw.3
	rm -f ${MANDIR}/fputs.3; \
	  ln -s puts.3 ${MANDIR}/fputs.3
	rm -f ${MANDIR}/srand.3; \
	  ln -s rand.3 ${MANDIR}/srand.3
	rm -f ${MANDIR}/initstate.3; \
	  ln -s random.3 ${MANDIR}/initstate.3
	rm -f ${MANDIR}/setstate.3; \
	  ln -s random.3 ${MANDIR}/setstate.3
	rm -f ${MANDIR}/srandom.3; \
	  ln -s random.3 ${MANDIR}/srandom.3
	rm -f ${MANDIR}/rresvport.3; \
	  ln -s rcmd.3 ${MANDIR}/rresvport.3
	rm -f ${MANDIR}/ruserok.3; \
	  ln -s rcmd.3 ${MANDIR}/ruserok.3
	rm -f ${MANDIR}/re_comp.3; \
	  ln -s regex.3 ${MANDIR}/re_comp.3
	rm -f ${MANDIR}/re_exec.3; \
	  ln -s regex.3 ${MANDIR}/re_exec.3
	rm -f ${MANDIR}/regcomp.3; \
	  ln -s regexp.3 ${MANDIR}/regcomp.3
	rm -f ${MANDIR}/regerror.3; \
	  ln -s regexp.3 ${MANDIR}/regerror.3
	rm -f ${MANDIR}/regexec.3; \
	  ln -s regexp.3 ${MANDIR}/regexec.3
	rm -f ${MANDIR}/regsub.3; \
	  ln -s regexp.3 ${MANDIR}/regsub.3
	rm -f ${MANDIR}/alphasort.3; \
	  ln -s scandir.3 ${MANDIR}/alphasort.3
	rm -f ${MANDIR}/fscanf.3; \
	  ln -s scanf.3 ${MANDIR}/fscanf.3
	rm -f ${MANDIR}/sscanf.3; \
	  ln -s scanf.3 ${MANDIR}/sscanf.3
	rm -f ${MANDIR}/setbuffer.3; \
	  ln -s setbuf.3 ${MANDIR}/setbuffer.3
	rm -f ${MANDIR}/setlinebuf.3; \
	  ln -s setbuf.3 ${MANDIR}/setlinebuf.3
	rm -f ${MANDIR}/_longjmp.3; \
	  ln -s setjmp.3 ${MANDIR}/_longjmp.3
	rm -f ${MANDIR}/_setjmp.3; \
	  ln -s setjmp.3 ${MANDIR}/_setjmp.3
	rm -f ${MANDIR}/longjmp.3; \
	  ln -s setjmp.3 ${MANDIR}/longjmp.3
	rm -f ${MANDIR}/setegid.3; \
	  ln -s setuid.3 ${MANDIR}/setegid.3
	rm -f ${MANDIR}/seteuid.3; \
	  ln -s setuid.3 ${MANDIR}/seteuid.3
	rm -f ${MANDIR}/setgid.3; \
	  ln -s setuid.3 ${MANDIR}/setgid.3
	rm -f ${MANDIR}/setrgid.3; \
	  ln -s setuid.3 ${MANDIR}/setrgid.3
	rm -f ${MANDIR}/setruid.3; \
	  ln -s setuid.3 ${MANDIR}/setruid.3
	rm -f ${MANDIR}/acos.3; \
	  ln -s sin.3 ${MANDIR}/acos.3
	rm -f ${MANDIR}/asin.3; \
	  ln -s sin.3 ${MANDIR}/asin.3
	rm -f ${MANDIR}/atan.3; \
	  ln -s sin.3 ${MANDIR}/atan.3
	rm -f ${MANDIR}/atan2.3; \
	  ln -s sin.3 ${MANDIR}/atan2.3
	rm -f ${MANDIR}/cos.3; \
	  ln -s sin.3 ${MANDIR}/cos.3
	rm -f ${MANDIR}/tan.3; \
	  ln -s sin.3 ${MANDIR}/tan.3
	rm -f ${MANDIR}/cosh.3; \
	  ln -s sinh.3 ${MANDIR}/cosh.3
	rm -f ${MANDIR}/tanh.3; \
	  ln -s sinh.3 ${MANDIR}/tanh.3
	rm -f ${MANDIR}/cbrt.3; \
	  ln -s sqrt.3 ${MANDIR}/cbrt.3
	rm -f ${MANDIR}/index.3; \
	  ln -s string.3 ${MANDIR}/index.3
	rm -f ${MANDIR}/rindex.3; \
	  ln -s string.3 ${MANDIR}/rindex.3
	rm -f ${MANDIR}/searchp.3; \
	  ln -s string.3 ${MANDIR}/searchp.3
	rm -f ${MANDIR}/strcasecmp.3; \
	  ln -s string.3 ${MANDIR}/strcasecmp.3
	rm -f ${MANDIR}/strcat.3; \
	  ln -s string.3 ${MANDIR}/strcat.3
	rm -f ${MANDIR}/strcmp.3; \
	  ln -s string.3 ${MANDIR}/strcmp.3
	rm -f ${MANDIR}/strcpy.3; \
	  ln -s string.3 ${MANDIR}/strcpy.3
	rm -f ${MANDIR}/strlen.3; \
	  ln -s string.3 ${MANDIR}/strlen.3
	rm -f ${MANDIR}/strncasecmp.3; \
	  ln -s string.3 ${MANDIR}/strncasecmp.3
	rm -f ${MANDIR}/strncat.3; \
	  ln -s string.3 ${MANDIR}/strncat.3
	rm -f ${MANDIR}/strncmp.3; \
	  ln -s string.3 ${MANDIR}/strncmp.3
	rm -f ${MANDIR}/strncpy.3; \
	  ln -s string.3 ${MANDIR}/strncpy.3
	rm -f ${MANDIR}/gtty.3; \
	  ln -s stty.3 ${MANDIR}/gtty.3
	rm -f ${MANDIR}/closelog.3; \
	  ln -s syslog.3 ${MANDIR}/closelog.3
	rm -f ${MANDIR}/openlog.3; \
	  ln -s syslog.3 ${MANDIR}/openlog.3
	rm -f ${MANDIR}/setlogmask.3; \
	  ln -s syslog.3 ${MANDIR}/setlogmask.3
	rm -f ${MANDIR}/tgetent.3; \
	  ln -s termcap.3 ${MANDIR}/tgetent.3
	rm -f ${MANDIR}/tgetflag.3; \
	  ln -s termcap.3 ${MANDIR}/tgetflag.3
	rm -f ${MANDIR}/tgetnum.3; \
	  ln -s termcap.3 ${MANDIR}/tgetnum.3
	rm -f ${MANDIR}/tgetstr.3; \
	  ln -s termcap.3 ${MANDIR}/tgetstr.3
	rm -f ${MANDIR}/tgoto.3; \
	  ln -s termcap.3 ${MANDIR}/tgoto.3
	rm -f ${MANDIR}/tputs.3; \
	  ln -s termcap.3 ${MANDIR}/tputs.3
	rm -f ${MANDIR}/ftime.3; \
	  ln -s time.3 ${MANDIR}/ftime.3
	rm -f ${MANDIR}/isatty.3; \
	  ln -s ttyname.3 ${MANDIR}/isatty.3
	rm -f ${MANDIR}/ttyslot.3; \
	  ln -s ttyname.3 ${MANDIR}/ttyslot.3

clean: FRC

FRC:
