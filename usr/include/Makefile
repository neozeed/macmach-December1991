# Makefile for /usr/include, Mach 3.0 version.

DESTDIR=

MIGFLAGS= -I${DESTDIR}/usr/include

STDDIRS=arpa protocols cmu servers
STDFILES= arpa/telnet.h arpa/bootp.h arpa/inet.h arpa/ftp.h arpa/tftp.h \
	arpa/nameser.h protocols/rwhod.h protocols/timed.h protocols/talkd.h \
	protocols/routed.h protocols/dumprestore.h cmu/c.h cmu/del.h \
	cmu/access.h cmu/acc.h cmu/ci.h cmu/libc.h cmu/ttyloc.h fstab.h \
	memory.h regexp.h struct.h ar.h stab.h grp.h pwd.h dbm.h mp.h pcc.h \
	vfont.h resolv.h fcntl.h ttyent.h nlist.h ndbm.h sgtty.h a.out.h \
	utmp.h varargs.h math.h time.h stdio.h ctype.h netdb.h setjmp.h \
	lastlog.h curses.h mntent.h assert.h tzfile.h disktab.h \
	servers/errorlib.h servers/machid.defs servers/machid_debug.defs \
	servers/machid_types.defs servers/machid_types.h \
	servers/netname.defs servers/netname_defs.h servers/service.defs \
	sysexits.h ranlib.h mtab.h string.h strings.h mach_tahoe_fs.h

UX=../../mach_servers/ux/server
UX29DIRS=sys net netimp netinet netns nfs rpc
UX29FILES= sys/signal_macros.h sys/acct.h sys/assert.h sys/bkmac.h sys/synch.h \
	sys/ux_exception.h sys/mbuf.h sys/clist.h sys/types.h sys/cmupty.h \
	sys/conf.h sys/dir.h sys/disklabel.h sys/dk.h sys/dkbad.h \
	sys/domain.h sys/enet.h sys/errno.h sys/error.h sys/kernel.h \
	sys/exec.h sys/fe.h sys/file.h sys/filsys.h sys/fs.h sys/gprof.h \
	sys/ild.h sys/inode.h sys/ioctl.h sys/jioctl.h sys/proc.h sys/map.h \
	sys/zalloc.h sys/varargs.h sys/systm.h sys/parallel.h sys/queue.h \
	sys/macro_help.h sys/tty.h sys/mfs.h sys/mman.h sys/mount.h \
	sys/msgbuf.h sys/mtio.h sys/namei.h sys/param.h sys/protosw.h \
	sys/ptrace.h sys/quota.h sys/reboot.h sys/resource.h sys/signal.h \
	sys/socket.h sys/socketvar.h sys/stat.h \
	sys/syscall.h sys/syslog.h sys/table.h sys/syscall_emulation.h \
	sys/tablet.h sys/time.h sys/timeb.h sys/times.h sys/trace.h \
	sys/ttychars.h sys/ttydev.h sys/ttyloc.h sys/uio.h sys/un.h \
	sys/unpcb.h sys/user.h sys/vadvise.h sys/vcmd.h sys/version.h \
	sys/viceioctl.h sys/vicermt.h sys/vlimit.h sys/vsio.h sys/vtimes.h \
	sys/wait.h sys/xpr.h sys/shared_lock.h sys/coff.h sys/buf.h \
	sys/sysnames.h sys/ushared.h sys/file_info.h sys/kalloc.h net/af.h \
	net/dli_var.h net/if.h net/if_arp.h net/netisr.h net/raw_cb.h \
	net/route.h netimp/if_imp.h netimp/if_imphost.h netinet/agent_var.h \
	netinet/bbn.h netinet/esp.h netinet/esp_cache.h netinet/esp_ip.h \
	netinet/esp_key.h netinet/esp_probe.h netinet/esp_var.h \
	netinet/group.h netinet/icmp_var.h netinet/igmp.h netinet/in.h \
	netinet/if_ether.h netinet/if_lan.h netinet/igmp_user.h \
	netinet/igmp_var.h netinet/in_pcb.h netinet/in_systm.h \
	netinet/in_var.h netinet/ip.h netinet/ip_icmp.h netinet/ip_var.h \
	netinet/pcbheadsw.h netinet/tcp.h netinet/tcp_debug.h \
	netinet/tcp_fsm.h netinet/tcp_seq.h netinet/tcp_timer.h \
	netinet/tcp_var.h netinet/tcpip.h netinet/udp.h netinet/vmtp_cksum.h \
	netinet/udp_var.h netinet/vmtp.h netinet/vmtp_buf.h \
	netinet/vmtp_ether.h netinet/vmtp_group.h netinet/vmtp_ip.h \
	netinet/vmtp_kdc.h netinet/vmtp_send.h netinet/vmtp_manager.h \
	netinet/vmtp_so.h netinet/vmtp_sys.h netinet/vmtp_var.h \
	netinet/ip_mroute.h netns/idp.h netns/idp_var.h netns/ns.h \
	netns/ns_error.h netns/ns_if.h netns/ns_pcb.h netns/sp.h \
	netns/spidp.h netns/spp_debug.h netns/spp_var.h netns/spp_timer.h \
	nfs/nfs.h nfs/nfs_clnt.h nfs/nfs_mount.h nfs/rnode.h rpc/auth.h \
	rpc/auth_unix.h rpc/clnt.h rpc/pmap_clnt.h rpc/pmap_prot.h rpc/rpc.h \
	rpc/rpc_msg.h rpc/svc.h rpc/svc_auth.h rpc/types.h rpc/xdr.h

UXDIRS=sys net netimp netinet netns
UXFILES= net/af.h net/dli_var.h net/if.h net/if_arp.h net/netisr.h \
	net/raw_cb.h net/route.h \
	netimp/if_imp.h netimp/if_imphost.h \
	netinet/agent_var.h netinet/bbn.h netinet/esp.h netinet/esp_cache.h \
	netinet/esp_ip.h netinet/esp_key.h netinet/esp_probe.h \
	netinet/esp_var.h netinet/group.h netinet/icmp_var.h \
	netinet/if_ether.h netinet/if_lan.h netinet/igmp.h \
	netinet/igmp_user.h netinet/igmp_var.h netinet/in.h netinet/in_pcb.h \
	netinet/in_systm.h netinet/in_var.h netinet/ip.h netinet/ip_icmp.h \
	netinet/ip_var.h netinet/pcbheadsw.h netinet/tcp.h \
	netinet/tcp_debug.h netinet/tcp_fsm.h netinet/tcp_seq.h \
	netinet/tcp_timer.h netinet/tcp_var.h netinet/tcpip.h netinet/udp.h \
	netinet/udp_var.h netinet/vmtp.h netinet/vmtp_buf.h \
	netinet/vmtp_cksum.h netinet/vmtp_ether.h netinet/vmtp_group.h \
	netinet/vmtp_ip.h netinet/vmtp_kdc.h netinet/vmtp_manager.h \
	netinet/vmtp_send.h netinet/vmtp_so.h netinet/vmtp_sys.h \
	netinet/vmtp_var.h \
	netns/idp.h netns/idp_var.h netns/ns.h netns/ns_error.h \
	netns/ns_if.h netns/ns_pcb.h netns/sp.h netns/spidp.h \
	netns/spp_debug.h netns/spp_var.h \
	sys/acct.h sys/assert.h sys/bkmac.h sys/buf.h sys/clist.h \
	sys/cmupty.h sys/coff.h sys/conf.h sys/dir.h sys/disklabel.h \
	sys/dk.h sys/dkbad.h sys/domain.h sys/enet.h sys/errno.h \
	sys/error.h sys/exec.h sys/fe.h sys/features.h sys/file.h \
	sys/file_info.h sys/filsys.h sys/fpsconf.h sys/fpsreg.h sys/fs.h \
	sys/gprof.h sys/ild.h sys/inode.h sys/ioctl.h sys/jioctl.h \
	sys/kernel.h sys/macro_help.h sys/map.h sys/mbuf.h sys/mfs.h \
	sys/mman.h sys/mount.h sys/msgbuf.h sys/mtio.h sys/namei.h \
	sys/parallel.h sys/param.h sys/proc.h sys/protosw.h sys/ptrace.h \
	sys/queue.h sys/quota.h sys/reboot.h sys/resource.h sys/rfs.h \
	sys/shared_lock.h sys/signal.h sys/signal_macros.h sys/socket.h \
	sys/socketvar.h sys/stat.h sys/synch.h sys/syscall.h \
	sys/syscall_emulation.h sys/syslog.h sys/sysnames.h sys/systm.h \
	sys/table.h sys/tablet.h sys/time.h sys/timeb.h sys/times.h \
	sys/trace.h sys/tty.h sys/ttychars.h sys/ttydev.h sys/ttyloc.h \
	sys/types.h sys/uio.h sys/un.h sys/unpcb.h sys/user.h sys/ushared.h \
	sys/ux_exception.h sys/vadvise.h sys/varargs.h sys/vcmd.h \
	sys/version.h sys/viceioctl.h sys/vicermt.h sys/vlimit.h sys/vsio.h \
	sys/vtimes.h sys/wait.h sys/xpr.h sys/zalloc.h

MK=../../mach_kernel/kernel
MKDIRS=device mach mach_debug mach/mac2
MKFILES= device/cirbuf.h device/conf.h device/dev_forward.defs \
	device/dev_hdr.h device/dev_master.h device/device.defs \
	device/device_port.h device/device_reply.defs \
	device/device_request.defs device/device_types.defs \
	device/device_types.h device/device_types_kernel.h \
	device/disk_status.h device/dk.h device/ds_routines.h \
	device/errno.h device/if_ether.h device/if_hdr.h device/io_req.h \
	device/map.h device/net_io.h device/net_status.h device/param.h \
	device/tape_status.h device/tty.h device/tty_status.h \
	mach/boolean.h mach/default_pager_object.defs mach/error.h \
	mach/exc.defs mach/exception.h mach/host_info.h mach/kern_return.h \
	mach/mach.defs mach/mach_host.defs mach/mach_param.h \
	mach/mach_port.defs mach/mach_traps.h mach/mach_types.defs \
	mach/mach_types.h mach/machine.h mach/memory_object.defs \
	mach/memory_object.h mach/memory_object_default.defs mach/message.h \
	mach/mig_errors.h mach/msg_type.h mach/notify.defs mach/notify.h \
	mach/policy.h mach/port.h mach/processor_info.h mach/std_types.defs \
	mach/std_types.h mach/syscall_sw.h mach/task_info.h \
	mach/task_special_ports.h mach/thread_info.h \
	mach/thread_special_ports.h mach/thread_status.h \
	mach/thread_switch.h mach/time_value.h mach/vm_attributes.h \
	mach/vm_inherit.h mach/vm_param.h mach/vm_prot.h mach/vm_statistics.h \
	mach_debug/hash_info.h mach_debug/ipc_info.h \
	mach_debug/mach_debug.defs mach_debug/mach_debug_types.defs \
	mach_debug/mach_debug_types.h mach_debug/page_info.h \
	mach_debug/vm_info.h mach_debug/zone_info.h \
	mach/mac2/boolean.h mach/mac2/exception.h mach/mac2/frame.h \
	mach/mac2/kern_return.h mach/mac2/reg.h mach/mac2/syscall_sw.h \
	mach/mac2/thread_status.h mach/mac2/vm_param.h mach/mac2/vm_types.h

LIBTHREADS=../lib/libthreads
LIBTHREADSFILES= cthreads.h ${target_machine}/cthreads.h

LIBMACH=../lib/libmach
LIBMACHFILES= mach.h mach_error.h mach_init.h mach_privileged_ports.h 

MIG_SERVERS= service.h netname.h machid.h machid_debug.h
MIG_MACH_DEBUG= mach_debug.h
MIG_DEVICE= device.h device_reply.h device_request.h
MIG_MACH= memory_object_default.h memory_object_user.h default_pager_object.h \
	exc.h notify_user.h mach_host.h mach_port.h mach_interface.h
MIGSTUFF= ${MIG_SERVERS} ${MIG_MACH_DEBUG} ${MIG_DEVICE} ${MIG_MACH}

MAC2DIRS=mac2 mac2dev mac2os
MAC2MKFILES= mac2/act.h mac2/asm.h mac2/ast.h mac2/call_frame.h mac2/clock.h \
	mac2/cpu.h mac2/endian.h mac2/exec.h mac2/mach_param.h mac2/pmap.h \
	mac2/pmap_kmap.h mac2/psl.h mac2/slots.h mac2/thread.h \
	mac2/time_stamp.h mac2/trap.h mac2/vm_tuning.h mac2/xpr.h \
	mac2dev/adb.h mac2dev/mscsi.h mac2dev/video.h mac2dev/sdisk.h \
	mac2dev/sony.h mac2os/Types.h mac2os/Video.h
MAC2UXFILES= mac2/pcb.h mac2/queue.h mac2/reg.h mac2/vmparam.h \
	mac2/machparam.h mac2/DEFAULT.h mac2/SPECIAL.h

all:	FRC

install: std mach mig ${target_machine} lib links
	chown -R bin.bin ${DESTDIR}/usr/include
	chmod -R a-w ${DESTDIR}/usr/include

links:	FRC
	@rm -f ${DESTDIR}/usr/include/machine
	ln -s ${target_machine} ${DESTDIR}/usr/include/machine
	@rm -f ${DESTDIR}/usr/include/mach/machine
	ln -s ${target_machine} ${DESTDIR}/usr/include/mach/machine
	@rm -f ${DESTDIR}/usr/include/mach/mach.h
	ln -s ../mach.h ${DESTDIR}/usr/include/mach/mach.h
	@rm -f ${DESTDIR}/usr/include/sys/message.h
	ln -s ../mach/message.h ${DESTDIR}/usr/include/sys/message.h
	@rm -f ${DESTDIR}/usr/include/sys/kern_return.h
	ln -s ../mach/kern_return.h ${DESTDIR}/usr/include/sys/kern_return.h
	@rm -f ${DESTDIR}/usr/include/errno.h;
	ln -s sys/errno.h ${DESTDIR}/usr/include/errno.h
	@rm -f ${DESTDIR}/usr/include/signal.h
	ln -s sys/signal.h ${DESTDIR}/usr/include/signal.h
	@rm -f ${DESTDIR}/usr/include/syslog.h
	ln -s sys/syslog.h ${DESTDIR}/usr/include/syslog.h

std:	${STDFILES}
	-@[ -d ${DESTDIR}/usr/include ] || mkdir ${DESTDIR}/usr/include
	-@for i in ${STDDIRS}; do \
	  [ -d ${DESTDIR}/usr/include/$$i ] || \
	    mkdir ${DESTDIR}/usr/include/$$i; \
	done
	-@for i in ${STDFILES}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/$$i; }; \
	done

mach:	FRC
	-@for i in ${MKDIRS}; do \
	  [ -d ${DESTDIR}/usr/include/$$i ] || \
	    mkdir ${DESTDIR}/usr/include/$$i; \
	done
	-@cd ${MK}; for i in ${MKFILES}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/$$i; }; \
	done
	-@for i in ${UXDIRS}; do \
	  [ -d ${DESTDIR}/usr/include/$$i ] || \
	    mkdir ${DESTDIR}/usr/include/$$i; \
	done
	-@cd ${UX}; for i in ${UXFILES}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/$$i; }; \
	done

mig:	${MIGSTUFF}
	-@for i in ${MIG_SERVERS}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/servers/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/servers/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/servers/$$i; }; \
	done
	-@for i in ${MIG_MACH_DEBUG}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/mach_debug/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/mach_debug/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/mach_debug/$$i; }; \
	done
	-@for i in ${MIG_DEVICE}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/device/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/device/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/device/$$i; }; \
	done
	-@for i in ${MIG_MACH}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/mach/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/mach/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/mach/$$i; }; \
	done

lib:	FRC
	-@cd ${LIBMACH}; for i in ${LIBMACHFILES}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/$$i; }; \
	done
	-@cd ${LIBTHREADS}; for i in ${LIBTHREADSFILES}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/$$i; }; \
	done

mac2:	FRC
	-@for i in ${MAC2DIRS}; do \
	  [ -d ${DESTDIR}/usr/include/$$i ] || mkdir ${DESTDIR}/usr/include/$$i; \
	done
	-@cd ${MK}; for i in ${MAC2MKFILES}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/$$i; }; \
	done
	-@cd ${UX}; for i in ${MAC2UXFILES}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/$$i || { \
	    echo "installing ${DESTDIR}/usr/include/$$i"; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/$$i; }; \
	done

machid.h: servers/machid.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

machid_debug.h: servers/machid_debug.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

service.h: servers/service.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

netname.h: servers/netname.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

mach_debug.h: ${MK}/mach_debug/mach_debug.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

memory_object_default.h: ${MK}/mach/memory_object_default.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

memory_object_user.h: ${MK}/mach/memory_object.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

default_pager_object.h: ${MK}/mach/default_pager_object.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

exc.h:	${MK}/mach/exc.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

notify_user.h: ${MK}/mach/notify.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

mach_host.h: ${MK}/mach/mach_host.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

mach_port.h: ${MK}/mach/mach_port.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

mach_interface.h: ${MK}/mach/mach.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

device.h: ${MK}/device/device.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

device_reply.h: ${MK}/device/device_reply.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

device_request.h: ${MK}/device/device_request.defs
	mig ${MIGFLAGS} $? -header $@ -server /dev/null -user /dev/null

install.man: FRC

clean:	FRC
	rm -f ${MIGSTUFF}

FRC:
