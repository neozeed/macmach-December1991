/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	thstate.c,v $
 * Revision 2.5  91/08/29  15:49:31  rpd
 * 	Moved machid include files into the standard include directory.
 * 	[91/08/29            rpd]
 * 
 * Revision 2.4  91/03/27  17:27:51  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:32:37  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/09/12  16:33:04  rpd
 * 	Enhanced to do the right thing with tasks and multiple ids.
 * 	[90/09/12            rpd]
 * 
 * 	Created.
 * 	[90/06/18            rpd]
 * 
 */

#include <stdio.h>
#include <mach.h>
#include <mach/message.h>
#include <mach_error.h>
#include <servers/machid.h>

#define streql(a, b)	(strcmp((a), (b)) == 0)

static mach_port_t server;
static mach_port_t auth;

static void machine_thread_state();
static void mips_thread_state();
static void sun3_thread_state();
static void vax_thread_state();
static void i386_thread_state();

static void
usage()
{
    quit(1, "usage: thstate [-host machine] ids...\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    char *hostname = "";
    int i, j;
    kern_return_t kr;

    for (i = 1; i < argc; i++)
	if (streql(argv[i], "-host") && (i < argc-1))
	    hostname = argv[++i];
	else if (streql(argv[i], "--")) {
	    i++;
	    break;
	} else if (argv[i][0] == '-')
	    usage();
	else
	    break;

    argc -= i;
    argv += i;

    kr = netname_look_up(name_server_port, hostname, "MachID", &server);
    if (kr != KERN_SUCCESS)
	quit(1, "thstate: netname_lookup_up(MachID): %s\n",
	     mach_error_string(kr));

    auth = mach_host_priv_self();
    if (auth == MACH_PORT_NULL)
	auth = mach_task_self();

    for (i = 0; i < argc; i++) {
	mach_id_t id = atoi(argv[i]), origid = id;
	mach_type_t type;
	mthread_t *threads;
	unsigned int threadCnt;

	kr = machid_mach_type(server, auth, id, &type);
	if (kr != KERN_SUCCESS)
	    quit(1, "thstate: machid_mach_type: %s\n", mach_error_string(kr));

	switch (type) {
	  case MACH_TYPE_NONE:
	    continue;

	  default:
	    kr = machid_mach_lookup(server, auth, id,
				    MACH_TYPE_THREAD, &id);
	    if (kr != KERN_SUCCESS)
		quit(1, "ms: machid_mach_lookup: %s\n", mach_error_string(kr));

	    if (id == 0)
		goto badtype;
	    /* fall-through */

	  case MACH_TYPE_THREAD:
	    machine_thread_state(id);
	    continue;

	  case MACH_TYPE_TASK:
	    kr = machid_task_threads(server, auth, id,
				     &threads, &threadCnt);
	    break;

	  case MACH_TYPE_PROCESSOR_SET_NAME:
	    kr = machid_mach_lookup(server, auth, id,
				    MACH_TYPE_PROCESSOR_SET, &id);
	    if (kr != KERN_SUCCESS)
		quit(1, "ms: machid_mach_lookup: %s\n", mach_error_string(kr));

	    if (id == 0)
		goto badtype;
	    /* fall-through */

	  case MACH_TYPE_PROCESSOR_SET:
	    kr = machid_processor_set_threads(server, auth, id,
					      &threads, &threadCnt);
	    break;

	  case MACH_TYPE_HOST:
	    kr = machid_mach_lookup(server, auth, id,
				    MACH_TYPE_HOST_PRIV, &id);
	    if (kr != KERN_SUCCESS)
		quit(1, "ms: machid_mach_lookup: %s\n", mach_error_string(kr));

	    if (id == 0)
		goto badtype;
	    /* fall-through */

	  case MACH_TYPE_HOST_PRIV:
	    kr = machid_host_threads(server,auth, id,
				     &threads, &threadCnt);
	    break;

	  badtype:
	    fprintf(stderr, "ms: %u has type %s\n",
		    origid, mach_type_string(type));
	    continue;
	}
	if (kr != KERN_SUCCESS)
	    continue;

	for (j = 0; j < threadCnt; j++)
	    machine_thread_state(threads[j]);

	kr = vm_deallocate(mach_task_self(), (vm_offset_t) threads,
			   (vm_size_t) (threadCnt * sizeof *threads));
	if (kr != KERN_SUCCESS)
	    quit(1, "ms: vm_deallocate: %s\n", mach_error_string(kr));
    }

    exit(0);
}

static void
machine_thread_state(thread)
    mthread_t thread;
{
#ifdef	mips
    mips_thread_state(thread);
#endif	mips
#ifdef	sun3
    sun3_thread_state(thread);
#endif	sun3
#ifdef	vax
    vax_thread_state(thread);
#endif	vax
#ifdef	i386
    i386_thread_state(thread);
#endif	i386
}

#ifdef	mips
static void
mips_thread_state(thread)
    mthread_t thread;
{
    mips_thread_state_t state;
    kern_return_t kr;

    kr = machid_mips_thread_state(server, auth, thread, &state);
    if (kr != KERN_SUCCESS)
	return;

    printf("Thread %d:\n", thread);
    printf("at = %08x\tv0 = %08x\tv1 = %08x\ta0 = %08x\n",
	   state.r1, state.r2, state.r3, state.r4);
    printf("a1 = %08x\ta2 = %08x\ta3 = %08x\tt0 = %08x\n",
	   state.r5, state.r6, state.r7, state.r8);
    printf("t1 = %08x\tt2 = %08x\tt3 = %08x\tt4 = %08x\n",
	   state.r9, state.r10, state.r11, state.r12);
    printf("t5 = %08x\tt6 = %08x\tt7 = %08x\ts0 = %08x\n",
	   state.r13, state.r14, state.r15, state.r16);
    printf("s1 = %08x\ts2 = %08x\ts3 = %08x\ts4 = %08x\n",
	   state.r17, state.r18, state.r19, state.r20);
    printf("s5 = %08x\ts6 = %08x\ts7 = %08x\tt8 = %08x\n",
	   state.r21, state.r22, state.r23, state.r24);
    printf("t9 = %08x\tk0 = %08x\tk1 = %08x\tgp = %08x\n",
	   state.r25, state.r26, state.r27, state.r28);
    printf("sp = %08x\tfp = %08x\tra = %08x\tlo = %08x\n",
	   state.r29, state.r30, state.r31, state.mdlo);
    printf("hi = %08x\tpc = %08x\n", state.mdhi, state.pc);
}
#endif	mips

#ifdef	sun3
static void
sun3_thread_state(thread)
    mthread_t thread;
{
    sun3_thread_state_t state;
    kern_return_t kr;

    kr = machid_sun3_thread_state(server, auth, thread, &state);
    if (kr != KERN_SUCCESS)
	return;

    printf("Thread %d:\n", thread);
    printf("d0 = %08x\td1 = %08x\td2 = %08x\td3 = %08x\n",
	   state.d0, state.d1, state.d2, state.d3);
    printf("d4 = %08x\td5 = %08x\td6 = %08x\td7 = %08x\n",
	   state.d4, state.d5, state.d6, state.d7);
    printf("a0 = %08x\ta1 = %08x\ta2 = %08x\ta3 = %08x\n",
	   state.a0, state.a1, state.a2, state.a3);
    printf("a4 = %08x\ta5 = %08x\ta6 = %08x\tsp = %08x\n",
	   state.a4, state.a5, state.a6, state.sp);
    printf("pc = %08x\tsr = %08x\n",
	   state.pc, state.sr);
}
#endif	sun3

#ifdef	vax
static void
vax_thread_state(thread)
    mthread_t thread;
{
    vax_thread_state_t state;
    kern_return_t kr;

    kr = machid_vax_thread_state(server, auth, thread, &state);
    if (kr != KERN_SUCCESS)
	return;

    printf("Thread %d:\n", thread);
    printf("r0  = %08x\tr1  = %08x\tr2  = %08x\tr3  = %08x\n",
	   state.r0, state.r1, state.r2, state.r3);
    printf("r4  = %08x\tr5  = %08x\tr6  = %08x\tr7  = %08x\n",
	   state.r4, state.r5, state.r6, state.r7);
    printf("r8  = %08x\tr9  = %08x\tr10 = %08x\tr11 = %08x\n",
	   state.r8, state.r9, state.r10, state.r11);
    printf("ap  = %08x\tfp  = %08x\tsp  = %08x\tpc  = %08x\n",
	   state.ap, state.fp, state.sp, state.pc);
    printf("ps  = %08x\n", state.ps);
}
#endif	vax

#ifdef	i386
static void
i386_thread_state(thread)
    mthread_t thread;
{
    i386_thread_state_t state;
    kern_return_t kr;

    kr = machid_i386_thread_state(server, auth, thread, &state);
    if (kr != KERN_SUCCESS)
	return;

    printf("Thread %d:\n", thread);
    printf("gs   = %08x\tfs   = %08x\tes   = %08x\tds   = %08x\n",
	   state.gs, state.fs, state.es, state.ds);
    printf("edi  = %08x\tesi  = %08x\tebp  = %08x\tesp  = %08x\n",
	   state.edi, state.esi, state.ebp, state.esp);
    printf("ebx  = %08x\tedx  = %08x\tecx  = %08x\teax  = %08x\n",
	   state.ebx, state.edx, state.ecx, state.eax);
    printf("eip  = %08x\tcs   = %08x\tefl  = %08x\tuesp = %08x\n",
	   state.eip, state.cs, state.efl, state.uesp);
    printf("ss   = %08x\n", state.ss);
}
#endif	i386
