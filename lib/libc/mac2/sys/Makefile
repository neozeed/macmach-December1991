# Makefile for mac2 libc sys.

DEFS=	-MD

# standard system calls, like Tahoe BSD 4.3 release
SRC1=	_exit.S accept.S access.S acct.S adjtime.S bind.S brk.S cerror.S \
	chdir.S chmod.S chown.S chroot.S close.S connect.S creat.S \
	dup.S dup2.S execl.S execle.S execv.S execve.S exect.S fchmod.S \
	fchown.S fcntl.S flock.S fork.S fstat.S fsync.S ftruncate.S \
	getdtablesize.S getegid.S geteuid.S getgid.S getgroups.S \
	gethostid.S gethostname.S getitimer.S getpagesize.S \
	getpeername.S getpgrp.S getpid.S getppid.S getpriority.S \
	getrlimit.S getrusage.S getsockname.S getsockopt.S \
	gettimeofday.S getuid.S ioctl.S kill.S killpg.S link.S \
	listen.S lseek.S lstat.S mkdir.S mknod.S mount.S open.S \
	pipe.S profil.S ptrace.S quota.S read.S readlink.S readv.S \
	reboot.S recv.S recvfrom.S recvmsg.S rename.S rmdir.S sbrk.S \
	select.S send.S sendmsg.S sendto.S setgroups.S sethostid.S \
	sethostname.S setitimer.S setpgrp.S setpriority.S setquota.S \
	setregid.S setreuid.S setrlimit.S setsockopt.S settimeofday.S \
	shutdown.S sigblock.S sigpause.S sigreturn.S sigsetmask.S \
	sigstack.S sigvec.S socket.S socketpair.S stat.S swapon.S \
	symlink.S sync.S syscall.S truncate.S umask.S umount.S \
	unlink.S utimes.S vfork.S vhangup.S wait.S wait3.S \
	write.S writev.S

OBJ1=	_exit.o accept.o access.o acct.o adjtime.o bind.o brk.o cerror.o \
	chdir.o chmod.o chown.o chroot.o close.o connect.o creat.o \
	dup.o dup2.o execl.o execle.o execv.o execve.o exect.o fchmod.o \
	fchown.o fcntl.o flock.o fork.o fstat.o fsync.o ftruncate.o \
	getdtablesize.o getegid.o geteuid.o getgid.o getgroups.o \
	gethostid.o gethostname.o getitimer.o getpagesize.o \
	getpeername.o getpgrp.o getpid.o getppid.o getpriority.o \
	getrlimit.o getrusage.o getsockname.o getsockopt.o \
	gettimeofday.o getuid.o ioctl.o kill.o killpg.o link.o \
	listen.o lseek.o lstat.o mkdir.o mknod.o mount.o open.o \
	pipe.o profil.o ptrace.o quota.o read.o readlink.o readv.o \
	reboot.o recv.o recvfrom.o recvmsg.o rename.o rmdir.o sbrk.o \
	select.o send.o sendmsg.o sendto.o setgroups.o sethostid.o \
	sethostname.o setitimer.o setpgrp.o setpriority.o setquota.o \
	setregid.o setreuid.o setrlimit.o setsockopt.o settimeofday.o \
	shutdown.o sigblock.o sigpause.o sigreturn.o sigsetmask.o \
	sigstack.o sigvec.o socket.o socketpair.o stat.o swapon.o \
	symlink.o sync.o syscall.o truncate.o umask.o umount.o \
	unlink.o utimes.o vfork.o vhangup.o wait.o wait3.o \
	write.o writev.o

# other system calls for MACH and CMU additions
SRC2=	afs_call.S async_daemon.S exportfs.S fstatfs.S getaid.S \
	getdirentries.S getdomainname.S getdopt.S getmodes.S icreate.S \
	idec.S iinc.S iopen.S iread.S iwrite.S madvise.S mincore.S \
	mprotect.S mremap.S munmap.S nfs_getfh.S nfs_svc.S pioctl.S mmap.S \
	quotactl.S rpause.S setaid.S setdomainname.S setdopt.S setmodes.S \
	setpag.S sstk.S statfs.S table.S unmount.S vfsmount.S xutimes.S

OBJ2=	afs_call.o async_daemon.o exportfs.o fstatfs.o getaid.o \
	getdirentries.o getdomainname.o getdopt.o getmodes.o icreate.o \
	idec.o iinc.o iopen.o iread.o iwrite.o madvise.o mincore.o mmap.o \
	mprotect.o mremap.o munmap.o nfs_getfh.o nfs_svc.o pioctl.o \
	quotactl.o rpause.o setaid.o setdomainname.o setdopt.o setmodes.o \
	setpag.o sstk.o statfs.o table.o unmount.o vfsmount.o xutimes.o

SRCS=	${SRC1} ${SRC2}

OBJS=	${OBJ1} ${OBJ2}

TAGSFILE=tags

all:	${OBJS} .depend

link:	all
	(cd ../../library; rm -f ${OBJS})
	(cd ../../profiled; rm -f ${OBJS})
	cp ${OBJS} ../../library
	(cd profiled; cp ${OBJS} ../../../profiled)

.S.o:
	@cc -p ${DEFS} -c $*.S
	@-ld -X -o profiled/$*.o -r $*.o
	cc ${DEFS} -c $*.S
	@-ld -x -r $*.o
	@mv a.out $*.o

install.man: FRC
	for i in *.man; do \
	  install -c -o bin -g bin -m 444 $$i \
	    ${DESTDIR}/usr/man/man2/`basename $$i .man`.2; \
	done

clean:	FRC
	rm -f .depend *.o profiled/* a.out core ${TAGSFILE}

tags:	FRC
	(P=`pwd`; \
	egrep "^ENTRY(.*)|^SYSCALL(.*)" ${SRCS} | /bin/sed \
"s;\([^:]*\):\([^(]*\)(\([^, )]*\)\(.*\);\3 $$P/\1 /^\2(\3\4$$/;" \
	>> ${TAGSFILE})

.depend: ${SRCS}
	md -d -f -u .depend *.d 2>/dev/null

-include .depend

FRC:
