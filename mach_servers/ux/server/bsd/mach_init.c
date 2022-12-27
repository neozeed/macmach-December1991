/* 
 * Mach Operating System
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach_init.c,v $
 * Revision 2.14  91/08/12  22:37:00  rvb
 * 	Removed calls to vm_pageable.
 * 	[91/08/06            rpd]
 * 
 * Revision 2.13  90/11/05  15:33:44  rpd
 * 	Use 48 buffers, instead of calculating nbuf from memory_size.
 * 	[90/10/28            rpd]
 * 
 * Revision 2.12  90/09/09  14:12:50  rpd
 * 	Convert to use only one object for shared uares.
 * 	[90/08/24            rwd]
 * 
 * Revision 2.11  90/06/19  23:07:50  rpd
 * 	Initialize p_sigref in system_setup.
 * 	[90/06/12            rpd]
 * 
 * Revision 2.10  90/06/02  15:22:00  rpd
 * 	Converted debugger thread code to new IPC.
 * 	[90/06/01            rpd]
 * 
 * 	Fixed shared memory mapping to avoid execute permission.
 * 	[90/05/31            rpd]
 * 	In system_setup, register/deregister as a server thread for proc 0.
 * 	[90/05/10            rpd]
 * 	Get unprivileged host port.
 * 	[90/04/27            rpd]
 * 
 * 	Lookup the default processor-set name/control ports.
 * 	[90/04/23            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  19:37:58  rpd]
 * 
 * Revision 2.9  90/05/29  20:23:16  rwd
 * 	In system_setup, register/deregister as a server thread for proc 0.
 * 	[90/05/10            rpd]
 * 	Added debugger thread option so we can always get into server
 * 	context from kdb.
 * 	[90/05/11            rwd]
 * 	Remove reference to lastfile in bogus uarea.
 * 	[90/04/11            rwd]
 * 
 * Revision 2.8  90/03/14  21:26:12  rwd
 * 	We now have macros for uu_ cases.
 * 	[90/02/22            rwd]
 * 	Get default pager port before creating init proc.
 * 	[90/01/22            rwd]
 * 
 * Revision 2.7  90/01/19  14:34:51  rwd
 * 	Kill first thread since its stack won't be right for new uarea
 * 	opt.
 * 	[89/12/15            rwd]
 * 
 * Revision 2.6  89/12/08  20:19:02  rwd
 * 	nbuf code from af
 * 	[89/12/06            rwd]
 * 
 * Revision 2.4.2.2  89/11/26  15:27:11  rwd
 * 	Set master_mutex special before grabbing it for the first time.
 * 	[89/11/26            rwd]
 * 
 * Revision 2.4.2.1  89/11/24  23:56:09  rwd
 * 	Wire down initial thread until it becomes a server thread.
 * 	[89/11/16            rwd]
 * 
 * Revision 2.5  89/11/29  15:27:26  af
 * 	If loading the emulator fails, say why.
 * 	[89/11/16  17:02:17  af]
 * 
 * Revision 2.4  89/10/17  11:25:39  rwd
 * 	Remove call to signal_handler_setup.
 * 	[89/09/21            dbg]
 * 
 * Revision 2.3  89/09/15  15:28:30  rwd
 * 	Printout version of server
 * 	[89/09/14            rwd]
 * 
 * Revision 2.2  89/08/09  14:35:33  rwd
 * 	Added call to init_mappable_time.
 * 	[89/08/06            rwd]
 * 
 *
 */
/*
 * Initialization for out-of-kernel BSD UX kernel.
 */
#include <cmucs.h>
#include <cmucs_rfs.h>
#include <quota.h>
#include <vice.h>
#include <map_uarea.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/proc.h>
#include <sys/namei.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/file.h>

#include <sys/inode.h>
#include <sys/fs.h>
#include <sys/buf.h>
#include <sys/clist.h>
#if	CMUCS_RFS
#include <sys/rfs.h>
#endif	CMUCS_RFS
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/reboot.h>
#include <sys/parallel.h>

/*
 *	For arptab:
 */
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#include <machine/vmparam.h>

#include <uxkern/import_mach.h>

#include <uxkern/proc_to_task.h>
#include <uxkern/device_utils.h>


#include <sys/zalloc.h>

#if	MAP_UAREA
#include <mach/default_pager_object.h>
#endif	MAP_UAREA

long		avenrun[3] = {0, 0, 0};	/* XXX */

mach_port_t	privileged_host_port;
mach_port_t	host_port;
mach_port_t	device_server_port;
mach_port_t	default_processor_set;
mach_port_t	default_processor_set_name;
#if	MAP_UAREA
mach_port_t	default_pager_port = MACH_PORT_NULL;
mach_port_t	shared_memory_port = MACH_PORT_NULL;
#endif MAP_UAREA

int	cmask = CMASK;
#if	CMUCS
#include <sys/table.h>
int	umodes = UMODE_NONICE;	/* default mode for all processes */
int	maxuprc = MAXUPRC;	/* default maximum process per user */
#endif	CMUCS

struct rlimit vm_initial_limit_stack = { DFLSSIZ, MAXSSIZ };
struct rlimit vm_initial_limit_data  = { DFLDSIZ, MAXDSIZ };
struct rlimit vm_initial_limit_core  = { RLIM_INFINITY, RLIM_INFINITY };

extern struct proc *newproc();

extern any_t	ux_create_thread();

cthread_fn_t	system_setup();	/* forward */

struct condition kill_main = CONDITION_INITIALIZER;
struct mutex kill_lock = MUTEX_INITIALIZER;
int debugger_thread = 0;

main(argc, argv)
	int	argc;
	char	**argv;
{
	/*
	 * Wire down this thread until it becomes a server thread
	 */

	cthread_wire();

	/*
	 * Get initial ports and arguments.
	 */
	get_config_info(argc, argv);

	/*
	 * Get a port to talk to the world.
	 */
	console_init();

	/*
	 * Setup mappable time
	 */
	init_mapped_time();

	/*
	 * Allocate tables - from wired-down memory
	 */
	alloc_tables();
	zone_init();

	/*
	 * Initialize mock wait queues
	 */
	rqinit();

	/*
	 * Initialize SPL emulator
	 */
	spl_init();

	/*
	 * Start callout thread
	 */
	callout_init();

	/*
	 * Start device reply server
	 */
	dev_utils_init();
	device_reply_hdlr();

	ux_server_init();
	/*
	 * Turn into a U*X thread, to do the rest of the
	 * initialization
	 */
	(void) ux_create_thread(system_setup);

	/*
	 * Unwire now
	 */
	cthread_unwire();

	/*
	 * This should never return from this condition wait
	 */
	if (debugger_thread) {
	    mach_port_t		bogus_port;
	    mach_msg_header_t	bogus_msg;
	    int			limit;

	    cthread_wire();
	    limit = cthread_kernel_limit();
	    if (limit != 0)
		cthread_set_kernel_limit(limit + 1);
	    (void) mach_port_allocate(mach_task_self(),
				      MACH_PORT_RIGHT_RECEIVE, &bogus_port);
	    while (1) {
		(void) mach_msg(&bogus_msg, MACH_RCV_MSG|MACH_RCV_TIMEOUT,
				0, 0, bogus_port, 5000, MACH_PORT_NULL);
	    }
	} else {
	    mutex_lock(&kill_lock);
	    condition_wait(&kill_main,&kill_lock);
	}
}

/*ARGSUSED*/
cthread_fn_t
system_setup(arg)
	any_t	arg;
{
	register struct proc *p;
	register struct utask *utask;
	int	i, s;
	struct fs *fs;

	struct proc *init_proc;
	mach_port_t ux_exception_port;
	kern_return_t kr;

	extern vm_size_t memory_size;
	extern int	 nbuf;


#if	MAP_UAREA
	kern_return_t	result;
	int		cproc = 0;
	vm_address_t	shared_address = 0;

	/*
	 * Get default_pager port and shared memory port
	 */

	result = vm_set_default_memory_manager(privileged_host_port,
					       &default_pager_port);

	if (result != KERN_SUCCESS)
		panic("getting default pager port");

	result = default_pager_object_create(default_pager_port,
						 &shared_memory_port,
						 3*vm_page_size*nproc);

	if (result != KERN_SUCCESS)
		panic("getting shared_memory port");

	result = vm_map(mach_task_self(),
			&shared_address, 3*nproc*vm_page_size,
			0, 1, shared_memory_port, 0, 0,
			VM_PROT_READ|VM_PROT_WRITE, VM_PROT_ALL,
			VM_INHERIT_NONE);
	if (result != KERN_SUCCESS)
		panic("system_setup:vm_map shared_memory %d\n",result);

	for(p = proc; p != procNPROC; p++,cproc++) {
	    p->p_shared_off = 3*vm_page_size*cproc;
	    p->p_readwrite = (char *)shared_address;
	    p->p_shared_rw = (struct ushared_rw *)
				(shared_address + vm_page_size);
	    p->p_shared_ro = (struct ushared_ro *)
				(shared_address + 2*vm_page_size);
	    shared_address += 3*vm_page_size;
	}

#endif MAP_UAREA
	/*
	 * Set up system process 0 (dummy)
	 */
	p = &proc[0];
	p->p_stat = SRUN;
	p->p_flag = SLOAD|SSYS;
	p->p_nice = NZERO;

	p->p_task = mach_task_self();
	p->p_thread = mach_thread_self();

	mutex_init(&p->p_lock);
	p->p_ref_count = 1;
	queue_init(&p->p_servers);
	p->p_sigref = 0;

	server_thread_register(&u, p);
	uarea_init(&u, p);

	/*
	 * Grab the master lock to satisfy sleep.
	 */
	unix_master();

	utask = &p->p_utask;

#if	MAP_UAREA
#else	MAP_UAREA
	utask->uu_lastfile = -1;
#endif	MAP_UAREA
	utask->uu_procp = p;
	utask->uu_cmask = cmask;
	for (i = 1; i < NGROUPS; i++)
	    utask->uu_groups[i] = NOGROUP;
	for (i = 0; i < sizeof(u.u_rlimit)/sizeof(u.u_rlimit[0]); i++) {
	    utask->uu_rlimit[i].rlim_cur = RLIM_INFINITY;
	    utask->uu_rlimit[i].rlim_max = RLIM_INFINITY;
	}
	utask->uu_rlimit[RLIMIT_STACK] = vm_initial_limit_stack;
	utask->uu_rlimit[RLIMIT_DATA]  = vm_initial_limit_data;
	utask->uu_rlimit[RLIMIT_CORE]  = vm_initial_limit_core;
#if	CMUCS
	utask->uu_modes = umodes;
	utask->uu_maxuprc = maxuprc;
#endif	CMUCS

#if	QUOTA
	qtinit();
	p->p_quota = uth->uu_quota = getquota(0, 0, Q_NDQ);
#endif	QUOTA

#if	CMUCS_RFS
	/*
	 *  RFS initialization part 1.
 	 *
	 *  Initialize all data structures which could not be handled at
	 *  compile-time.  This includes, in particular, the parallel process
	 *  table queue headers which must be valid before the newproc() call
	 *  below.
	 */
	rfs_init();
	mbinit();			/* needs SPL */
#endif	CMUCS_RFS
	cinit();
#include <sl.h>
#if NSL > 0
	slattach();			/* XXX */
#endif
#include <loop.h>
#if NLOOP > 0
	loattach();			/* XXX */
#endif
	/*
	 * Start network server thread.
	 */
	netisr_init();

	/*
	 * Block reception of incoming packets
	 * until protocols have been initialized.
	 */
	s = splimp();
	ifinit();
	domaininit();
	splx(s);

	/*
	 * Initialize process table.
	 */
	pqinit();
	task_to_proc_init();

	/*
	 * Initialize block IO system.
	 */
	ihinit();
	nchinit();
	bio_init();

	/*
	 * Open the console.
	 */
	(void) cons_open(makedev(0,0), FREAD|FWRITE);

	printf("%s\n",version);
	printf("Available memory = %d.%d megabytes\n",
			memory_size / (1024*1024),
			((memory_size % (1024*1024)) * 100) / (1024*1024));
	printf("Using up to %d buffers\n", nbuf);

	/*
	 * inode_pager_bootstrap initializes the fs swapping preferences
	 * table; it must be called before mounting any file systems.
	 */
	inode_pager_bootstrap();

	fs = mountfs(rootdev, 0, (struct inode *)0, TRUE);
	if (fs == 0)
	    panic("iinit");
	bcopy("/", fs->fs_fsmnt, 2);

	inittodr(fs->fs_time);
	/*
	 * Set up the root file system.
	 */
	rootdir = iget(rootdev, fs, (ino_t)ROOTINO);
	iunlock(rootdir);
	utask->uu_cdir = iget(rootdev, fs, (ino_t)ROOTINO);
	iunlock(utask->uu_cdir);
	utask->uu_rdir = NULL;

#if	CMUCS_RFS
	/*
	 *  RFS initialization part 2.
	 *
	 *  Switch over to the local root.
	 */
	rfs_initroot();
#endif	CMUCS_RFS
#if	VICE
	utask->uu_rmtWd.dev = NODEV;
#endif	VICE
#if	CMUCS
	/*
	 *  Default to pausing process on these errors.
	 */
	utask->uu_rpause = (URPS_AGAIN|URPS_NOMEM|URPS_NFILE|URPS_NOSPC);
#endif	CMUCS

	/*
	 * Make init process - it MUST have pid 1 and be &proc[1]
	 */
	init_proc = newproc(TRUE);
/* lots of MACH initialization */

	/* inode pager */
	inode_pager();

	/* exception handler */
	ux_exception_port = ux_handler_setup();
	kr = task_set_exception_port(init_proc->p_task, ux_exception_port);
	if (kr != KERN_SUCCESS)
	    panic("system_setup: can't set exception port");

	/* LOAD THE EMULATION LIBRARY */

	/*
	 * Load the emulator library
	 */
	load_emulator(init_proc);
	if (u.u_error)
	    panic("load_emulator x%x", u.u_error);

	/*
	 * Start the first task!
	 */
	(void) thread_resume(init_proc->p_thread);

	/*
	 * Release the master lock...
	 */
	unix_release();

	/*
	 * Let ps know what we are
	 */
	strncpy(utask->uu_comm, "BSD4.3 server", sizeof(utask->uu_comm));

	/*
	 * Become the first server thread
	 */
	server_thread_deregister(&u, p);
	ux_server_loop();
	/*NOTREACHED*/
}

/*
 * Initialize clist by freeing all character blocks, then count
 * number of character devices. (Once-only routine)
 */
cinit()
{
	register int ccp;
	register struct cblock *cp;

	ccp = (int)cfree;
	ccp = (ccp+CROUND) & ~CROUND;
	for(cp=(struct cblock *)ccp; cp < &cfree[nclist-1]; cp++) {
		cp->c_next = cfreelist;
		cfreelist = cp;
		cfreecount += CBSIZE;
	}
}

int	nbuf = 0;
vm_size_t	memory_size;

alloc_tables()
{
	register vm_offset_t firstaddr, v;
	vm_offset_t	alloc_addr;
	vm_size_t	size;

	{
	    vm_statistics_data_t vm_stat;
	    vm_statistics(mach_task_self(), &vm_stat);
	    /*
	     *	Consider wired down memory not available
	     */
	    memory_size = vm_stat.pagesize * (vm_stat.free_count +
					      vm_stat.active_count +
					      vm_stat.inactive_count);
	}

	if (nbuf == 0)
#ifdef mac2 /* set nbuf to 128 */
	    nbuf = 128;
#else
	    nbuf = 48;
#endif

#define	valloc(name, type, num) \
	    (name) = (type *)v; v = (vm_offset_t)((name)+(num))
#define	valloclim(name, type, num, lim) \
	    (name) = (type *)v; v = (vm_offset_t)((lim) = ((name)+(num)))

	/*
	 *	We make two passes over the table allocation code.
	 *	The first pass merely calculates the size needed
	 *	for the various data structures.  The second pass
	 *	really allocates memory and then sets the actual
	 *	addresses.  The code must not change any of the
	 *	allocated sizes between the two passes.
	 */
	firstaddr = 0;
	for (;;) {
	    v = firstaddr;	    
#if	CMUCS_RFS
	    valloc(rfsProcessTable, struct rfsProcessEntry, nproc);
#endif	CMUCS_RFS
	    valloclim(inode, struct inode, ninode, inodeNINODE);
	    valloclim(file, struct file, nfile, fileNFILE);
	    valloclim(proc, struct proc, nproc, procNPROC);
	    valloc(cfree, struct cblock, nclist);
#if	!MACH_DIRECTORY
	    valloc(namecache, struct namecache, nchsize);
#endif	!MACH_DIRECTORY
#if	QUOTA
	    valloclim(quota, struct quota, nquota, quotaNQUOTA);
	    valloclim(dquot, struct dquot, ndquot, dquotNDQUOT);
#endif	QUOTA
	    {
		extern struct arptab *arptab;
		extern int arptab_size, arptab_bsiz, arptab_nb;

		arptab_size = arptab_bsiz * arptab_nb;
		valloc(arptab, struct arptab, arptab_size);
	    }

	    valloc(buf, struct buf, nbuf);

	    if (firstaddr == 0) {
		/*
		 *	Size has been calculated; allocate memory.
		 */
		size = (vm_size_t)(v - firstaddr);
		if (vm_allocate(mach_task_self(),
				&alloc_addr,
				size,
				TRUE)
			!= KERN_SUCCESS)
		    panic("startup: no room for tables");
#if	0
		(void) vm_pageable(mach_task_self(),
				   alloc_addr,
				   size,
				   VM_PROT_READ|VM_PROT_WRITE);
#endif
		firstaddr = alloc_addr;
	    }
	    else {
		/*
		 *	Memory has been allocated.  Make sure that
		 *	table size has not changed.
		 */
		if ((vm_size_t)(v - firstaddr) != size)
		    panic("startup: table size inconsistent");
		break;
	    }
	}
}

extern dev_t	parse_root_device();

get_config_info(argc, argv)
	int	argc;
	char	**argv;
{
	mach_port_t	bootstrap_port;
	mach_port_t	reply_port;
	kern_return_t	result;

	struct imsg {
	    mach_msg_header_t	hdr;
	    mach_msg_type_t	port_desc_1;
	    mach_port_t		port_1;
	    mach_msg_type_t	port_desc_2;
	    mach_port_t		port_2;
	} imsg;

	/*
	 * Get our bootstrap port
	 */
	result = task_get_bootstrap_port(mach_task_self(), &bootstrap_port);
	if (result != KERN_SUCCESS)
	    panic("get bootstrap port %d", result);

	/*
	 * Allocate a reply port
	 */
	reply_port = mach_reply_port();
	if (reply_port == MACH_PORT_NULL)
	    panic("allocate reply port");

	/*
	 * Send a message to it, asking for the host and device ports
	 */
	imsg.hdr.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND,
					    MACH_MSG_TYPE_MAKE_SEND_ONCE);
	imsg.hdr.msgh_size = 0;
	imsg.hdr.msgh_remote_port = bootstrap_port;
	imsg.hdr.msgh_local_port = reply_port;
	imsg.hdr.msgh_kind = MACH_MSGH_KIND_NORMAL;
	imsg.hdr.msgh_id = 999999;

	result = mach_msg(&imsg.hdr, MACH_SEND_MSG|MACH_RCV_MSG,
			  sizeof imsg.hdr, sizeof imsg, reply_port,
			  MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	if (result != MACH_MSG_SUCCESS)
	    panic("mach_msg");

	privileged_host_port = imsg.port_1;
	device_server_port = imsg.port_2;

	host_port = mach_host_self();

	/*
	 * Lookup our default processor-set name/control ports.
	 */

	(void) processor_set_default(mach_host_self(),
				     &default_processor_set_name);
	(void) host_processor_set_priv(privileged_host_port,
				       default_processor_set_name,
				       &default_processor_set);

	/*
	 * Parse the arguments.
	 */

	/*
	 * Arg 0 is program name
	 */
	argv++, argc--;

	/*
	 * Arg 1 should be flags
	 */
	if (argc == 0)
	    return;

	if (argv[0][0] == '-') {
	    register char *cp = argv[0];
	    register char c;

	    while ((c = *cp++) != '\0') {
		switch (c) {
		    case 'a':
			boothowto |= RB_ASKNAME;
			break;
		    case 's':
			boothowto |= RB_SINGLE;
			break;
		    case 'd':
			boothowto |= RB_KDB;
			break;
		    case 'n':
			boothowto |= RB_INITNAME;
			break;
		}
	    }
	    argv++, argc--;
	}
	/*
	 * Arg 2 should be root name
	 */
	if (argc == 0)
	    return;

	rootdev = parse_root_device(argv[0]);
}
