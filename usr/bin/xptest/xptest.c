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
 * $Log:	xptest.c,v $
 * Revision 2.5  91/08/30  15:41:46  rpd
 * 	Added memory_object_supply_completed, memory_object_data_return,
 * 	memory_object_change_completed.
 * 	[91/08/30            rpd]
 * 
 * Revision 2.4  91/03/27  17:31:37  mrt
 * 	Changed mach.h include
 * 
 * Revision 2.3  91/03/19  12:55:37  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  90/11/05  23:34:31  rpd
 * 	Created.
 * 	[90/10/30            rpd]
 * 
 */

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <strings.h>
#include <mach.h>
#include <mach/message.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <cthreads.h>

#define	VM_PROT_UNAVAILABLE	(VM_PROT_ALL+1)

#define streql(a, b)	(strcmp((a), (b)) == 0)

static jmp_buf savepoint;
static int sigbus();
static int sigsegv();
static int readint();
static int writeint();

static boolean_t saw_sigsegv = FALSE;

static boolean_t TestRandom = FALSE;
static void test_random();
static void add_operation();
static void dump_buffer();

#define MAX_OPS		100
enum op {
    MapObject, UnmapObject,
    ProtectMap, ProtectPage,
    ReadSucceeded, ReadFailed,
    WriteSucceeded, WriteFailed,
    MungeProt
};
static int total_ops = 0;
static int next_slot = 0;
static struct {
    enum op op;
    vm_prot_t prot;
} buffer[MAX_OPS];

static void test_read_after_map();
static void test_write_after_map();
static void test_write_after_read_after_map();
static void test_read_after_read();
static void test_write_after_read();
static void test_read_after_write();
static void test_write_after_write();
static void test_read_after_zero_after_read();
static void test_write_after_zero_after_read();
static void test_read_after_zero_after_write();
static void test_write_after_zero_after_write();
static void test_read_after_lock_protect_after_read();
static void test_write_after_lock_protect_after_read();
static void test_read_after_lock_protect_after_write();
static void test_write_after_lock_protect_after_write();
static void test_read_after_protect_lock_after_read();
static void test_write_after_protect_lock_after_read();
static void test_read_after_protect_lock_after_write();
static void test_write_after_protect_lock_after_write();

static void try_all_prots();
static void try_all_prots2();
static void try_all_protsm();

static void flushserver();
static void server();

static struct mutex printf_lock;

static vm_prot_t map_prot;
static vm_prot_t page_prot;
static vm_prot_t page_access_mask;
static boolean_t page_written;
static boolean_t page_cleaned;
static mach_port_t object_control;

static void
usage()
{
    quit(1, "usage: xptest [-random seed]\n");
}

main(argc, argv)
    int argc;
    char *argv[];
{
    mach_port_t object;
    kern_return_t kr;
    int i;

    for (i = 1; i < argc; i++)
	if ((streql(argv[i], "-r") || streql(argv[i], "-random")) &&
	    (i < argc-1)) {
	    TestRandom = TRUE;
	    srandom(atoi(argv[++i]));
	} else
	    usage();

    printf("Testing VM protection...\n");

    (void) signal(SIGBUS, sigbus);
    (void) signal(SIGSEGV, sigsegv);
    mutex_init(&printf_lock);

    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE,
			    &object);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: mach_port_allocate: %s\n", mach_error_string(kr));

    kr = mach_port_insert_right(mach_task_self(), object,
				object, MACH_MSG_TYPE_MAKE_SEND);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: mach_port_insert_right: %s\n", mach_error_string(kr));

    cthread_detach(cthread_fork((cthread_fn_t) server, (any_t) object));

    if (TestRandom)
	test_random(object);
    else {
	try_all_protsm(object, test_read_after_map);
	try_all_protsm(object, test_write_after_map);
	try_all_protsm(object, test_write_after_read_after_map);
	try_all_prots(object, test_read_after_read);
	try_all_prots(object, test_write_after_read);
	try_all_prots(object, test_read_after_write);
	try_all_prots(object, test_write_after_write);
	try_all_prots(object, test_read_after_zero_after_read);
	try_all_prots(object, test_write_after_zero_after_read);
	try_all_prots(object, test_read_after_zero_after_write);
	try_all_prots(object, test_write_after_zero_after_write);
	try_all_prots2(object, test_read_after_lock_protect_after_read);
	try_all_prots2(object, test_write_after_lock_protect_after_read);
	try_all_prots2(object, test_read_after_lock_protect_after_write);
	try_all_prots2(object, test_write_after_lock_protect_after_write);
	try_all_prots2(object, test_read_after_protect_lock_after_read);
	try_all_prots2(object, test_write_after_protect_lock_after_read);
	try_all_prots2(object, test_read_after_protect_lock_after_write);
	try_all_prots2(object, test_write_after_protect_lock_after_write);
    }

    if (saw_sigsegv) {
	mutex_lock(&printf_lock);
	printf("\n");
	printf("    **** Caught a SIGSEGV.\n");
	mutex_unlock(&printf_lock);
    }

    exit(0);
}

static void
try_all_prots(object, test)
    mach_port_t object;
    void (*test)();
{
    vm_prot_t prot;

    for (prot = VM_PROT_NONE; prot <= VM_PROT_ALL; prot++)
	(*test)(object, prot);
}

static void
try_all_prots2(object, test)
    mach_port_t object;
    void (*test)();
{
    vm_prot_t prot1, prot2;

    for (prot1 = VM_PROT_NONE; prot1 <= VM_PROT_ALL; prot1++)
	for (prot2 = VM_PROT_NONE; prot2 <= VM_PROT_ALL; prot2++)
	    (*test)(object, prot1, prot2);
}

static void
try_all_protsm(object, test)
    mach_port_t object;
    void (*test)();
{
    vm_prot_t prot1, prot2;

    for (prot1 = VM_PROT_NONE; prot1 <= VM_PROT_ALL; prot1++)
	for (prot2 = VM_PROT_NONE; prot2 <= VM_PROT_UNAVAILABLE; prot2++)
	    (*test)(object, prot1, prot2);
}

static int
sigbus()
{
    longjmp(savepoint, 1);
}

static int
sigsegv()
{
    saw_sigsegv = TRUE;
    longjmp(savepoint, 1);
}

static int
readint(addr)
    vm_offset_t addr;
{
    if (setjmp(savepoint))
	return 0xdeadbeef;
    else
	return * (int *) addr;
}

static int
writeint(addr, val)
    vm_offset_t addr;
    int val;
{
    if (setjmp(savepoint))
	return 0xdeadbeef;
    else
	return * (int *) addr = val;
}

static char *prot_name[] =
	{ "zero", "R", "W", "RW", "X", "RX", "WX", "RWX", "none" };

static vm_offset_t
mapobject(object, prot)
    mach_port_t object;
    vm_prot_t prot;
{
    vm_offset_t addr = 0;
    kern_return_t kr;

    add_operation(MapObject, prot);

    mutex_lock(&printf_lock);
    printf("\nMapping with %s perm.\n", prot_name[prot]);
    mutex_unlock(&printf_lock);

    map_prot = prot;
    page_prot = VM_PROT_NONE;
    page_access_mask = VM_PROT_NONE;
    page_written = FALSE;
    page_cleaned = FALSE;
    object_control = MACH_PORT_NULL;

    kr = vm_map(mach_task_self(), &addr, vm_page_size, 0, TRUE,
		object, 0, FALSE, prot, VM_PROT_ALL,
		VM_INHERIT_DEFAULT);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: vm_map: %s\n", mach_error_string(kr));

    flushserver(object);
    return addr;
}

static void
protectmap(addr, prot)
    vm_offset_t addr;
    vm_prot_t prot;
{
    kern_return_t kr;

    add_operation(ProtectMap, prot);

    mutex_lock(&printf_lock);
    printf("Changing map to %s perm.\n", prot_name[prot]);
    mutex_unlock(&printf_lock);

    kr = vm_protect(mach_task_self(), addr, vm_page_size, FALSE, prot);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: vm_protect: %s\n", mach_error_string(kr));

    map_prot = prot;
}

static void
protectpage(addr, prot)
    vm_offset_t addr;
    vm_prot_t prot;
{
    kern_return_t kr;

    add_operation(ProtectPage, prot);

    mutex_lock(&printf_lock);
    printf("Changing lock to %s perm.\n", prot_name[prot]);
    mutex_unlock(&printf_lock);

    kr = memory_object_lock_request(object_control, 0, vm_page_size,
				    FALSE, FALSE,
				    VM_PROT_ALL &~ prot,
				    MACH_PORT_NULL);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: memory_object_lock_request: %s\n",
	     mach_error_string(kr));

    page_prot = prot;
}

static void
unmap(object, addr)
    mach_port_t object;
    vm_offset_t addr;
{
    boolean_t problem = FALSE;
    kern_return_t kr;

    add_operation(UnmapObject, VM_PROT_NONE);

    mutex_lock(&printf_lock);
    printf("Removing mapping.\n");
    mutex_unlock(&printf_lock);

    kr = vm_deallocate(mach_task_self(), addr, vm_page_size);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: vm_deallocate: %s\n", mach_error_string(kr));

    flushserver(object);

    if (page_written && !page_cleaned) {
	mutex_lock(&printf_lock);
	printf("    **** Page wasn't written back.\n");
	mutex_unlock(&printf_lock);
	problem = TRUE;
    }

    if (!page_written && page_cleaned) {
	mutex_lock(&printf_lock);
	printf("    **** Page was written back.\n");
	mutex_unlock(&printf_lock);
	problem = TRUE;
    }

    if (problem && TestRandom)
	dump_buffer();
}

static boolean_t
checkaccess(required, succeeded)
    vm_prot_t required;
    boolean_t succeeded;
{
    boolean_t correct = TRUE;

    if (succeeded) {
	if ((map_prot & required) != required) {
	    printf("    **** Map protection is %s.\n", prot_name[map_prot]);
	    correct = FALSE;
	}
	if ((page_prot & required) != required) {
	    printf("    **** Page protection is %s.\n", prot_name[page_prot]);
	    correct = FALSE;
	}
    } else {
	if ((map_prot & required) == required) {
	    printf("    **** Map protection is %s.\n", prot_name[map_prot]);
	    correct = FALSE;
	}
    }

    return correct;
}

static void
tryread(addr)
    vm_offset_t addr;
{
    int value;
    boolean_t correct;

    value = readint(addr);
    add_operation(value != 0xdeadbeef ? ReadSucceeded : ReadFailed,
		  VM_PROT_NONE);

    mutex_lock(&printf_lock);
    if (value == 0xdeadbeef)
	printf("Read failed.\n");
    else
	printf("Read succeeded.\n");
    correct = checkaccess(VM_PROT_READ, value != 0xdeadbeef);
    mutex_unlock(&printf_lock);

    if (TestRandom && !correct)
	dump_buffer();
}

static void
trywrite(addr)
    vm_offset_t addr;
{
    int value;
    boolean_t correct;

    value = writeint(addr, 1);
    if (value != 0xdeadbeef)
	page_written = TRUE;
    add_operation(value != 0xdeadbeef ? WriteSucceeded : WriteFailed,
		  VM_PROT_NONE);

    mutex_lock(&printf_lock);
    if (value == 0xdeadbeef)
	printf("Write failed.\n");
    else
	printf("Write succeeded.\n");
    correct = checkaccess(VM_PROT_READ|VM_PROT_WRITE, value != 0xdeadbeef);
    mutex_unlock(&printf_lock);

    if (TestRandom && !correct)
	dump_buffer();
}

static void
setaccessmask(prot)
    vm_prot_t prot;
{
    page_access_mask = prot;
    add_operation(MungeProt, prot);

    mutex_lock(&printf_lock);
    printf("Setting access mask to %s.\n", prot_name[prot]);
    mutex_unlock(&printf_lock);
}

static void
add_operation(op, prot)
    enum op op;
    vm_prot_t prot;
{
    if (next_slot == MAX_OPS)
	next_slot = 0;

    buffer[next_slot].op = op;
    buffer[next_slot].prot = prot;

    total_ops++;
    next_slot++;
}

static void
print_operation(i)
    int i;
{
    switch (buffer[i].op) {
      case MapObject:
	fprintf(stderr, "%3d: MapObject(%s)\n",
		i, prot_name[buffer[i].prot]);
	break;
      case UnmapObject:
	fprintf(stderr, "%3d: UnmapObject\n", i);
	break;
      case ProtectMap:
	fprintf(stderr, "%3d: ProtectMap(%s)\n",
		i, prot_name[buffer[i].prot]);
	break;
      case ProtectPage:
	fprintf(stderr, "%3d: ProtectPage(%s)\n",
		i, prot_name[buffer[i].prot]);
	break;
      case ReadSucceeded:
	fprintf(stderr, "%3d: ReadSucceeded\n", i);
	break;
      case ReadFailed:
	fprintf(stderr, "%3d: ReadFailed\n", i);
	break;
      case WriteSucceeded:
	fprintf(stderr, "%3d: WriteSucceeded\n", i);
	break;
      case WriteFailed:
	fprintf(stderr, "%3d: WriteFailed\n", i);
	break;
      case MungeProt:
	fprintf(stderr, "%3d: MungeProt(%s)\n",
		i, prot_name[buffer[i].prot]);
	break;
      default:
	fprintf(stderr, "%3d: %d/%s\n",
		i, buffer[i].op, prot_name[buffer[i].prot]);
    }
}

static void
dump_buffer()
{
    int i;

    fprintf(stderr, "%d operations:\n", total_ops);
    if (total_ops > MAX_OPS)
	for (i = next_slot; i < MAX_OPS; i++)
	    print_operation(i);
    for (i = 0; i < next_slot; i++)
	print_operation(i);

    exit(1);
}

static vm_prot_t
random_prot()
{
    return random() & VM_PROT_ALL;
}

static void
test_random(object)
    mach_port_t object;
{
    vm_offset_t addr;

    goto start;
    for (;;) switch (random() & 0xf) {
      case 0x0:
	unmap(object, addr);
      start:
	addr = mapobject(object, random_prot());
	if (random() & 1)
	    setaccessmask(VM_PROT_UNAVAILABLE);
	else
	    setaccessmask(random_prot());
	break;

      case 0x1: case 0x2: case 0x3:
	protectmap(addr, random_prot());
	break;

      case 0x4: case 0x5: case 0x6:
	protectpage(addr, random_prot());
	break;

      case 0x7: case 0x8: case 0x9:
	tryread(addr);
	break;

      case 0xa: case 0xb: case 0xc:
	trywrite(addr);
	break;
    }
}

static void
test_read_after_map(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, prot1);
    setaccessmask(prot2);
    tryread(addr);
    unmap(object, addr);
}

static void
test_write_after_map(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, prot1);
    setaccessmask(prot2);
    trywrite(addr);
    unmap(object, addr);
}

static void
test_write_after_read_after_map(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, prot1);
    setaccessmask(prot2);
    tryread(addr);
    trywrite(addr);
    unmap(object, addr);
}

static void
test_read_after_read(object, prot)
    mach_port_t object;
    vm_prot_t prot;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ);
    tryread(addr);
    protectmap(addr, prot);
    tryread(addr);
    unmap(object, addr);
}

static void
test_write_after_read(object, prot)
    mach_port_t object;
    vm_prot_t prot;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ);
    tryread(addr);
    protectmap(addr, prot);
    trywrite(addr);
    unmap(object, addr);
}

static void
test_read_after_write(object, prot)
    mach_port_t object;
    vm_prot_t prot;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ|VM_PROT_WRITE);
    trywrite(addr);
    protectmap(addr, prot);
    tryread(addr);
    unmap(object, addr);
}

static void
test_write_after_write(object, prot)
    mach_port_t object;
    vm_prot_t prot;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ|VM_PROT_WRITE);
    trywrite(addr);
    protectmap(addr, prot);
    trywrite(addr);
    unmap(object, addr);
}

static void
test_read_after_zero_after_read(object, prot)
    mach_port_t object;
    vm_prot_t prot;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ);
    tryread(addr);
    protectmap(addr, VM_PROT_NONE);
    protectmap(addr, prot);
    tryread(addr);
    unmap(object, addr);
}

static void
test_write_after_zero_after_read(object, prot)
    mach_port_t object;
    vm_prot_t prot;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ);
    tryread(addr);
    protectmap(addr, VM_PROT_NONE);
    protectmap(addr, prot);
    trywrite(addr);
    unmap(object, addr);
}

static void
test_read_after_zero_after_write(object, prot)
    mach_port_t object;
    vm_prot_t prot;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ|VM_PROT_WRITE);
    trywrite(addr);
    protectmap(addr, VM_PROT_NONE);
    protectmap(addr, prot);
    tryread(addr);
    unmap(object, addr);
}

static void
test_write_after_zero_after_write(object, prot)
    mach_port_t object;
    vm_prot_t prot;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ|VM_PROT_WRITE);
    trywrite(addr);
    protectmap(addr, VM_PROT_NONE);
    protectmap(addr, prot);
    trywrite(addr);
    unmap(object, addr);
}

static void
test_read_after_lock_protect_after_read(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ);
    tryread(addr);
    protectmap(addr, prot1);
    protectpage(addr, prot2);
    tryread(addr);
    unmap(object, addr);
}

static void
test_write_after_lock_protect_after_read(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ);
    tryread(addr);
    protectmap(addr, prot1);
    protectpage(addr, prot2);
    trywrite(addr);
    unmap(object, addr);
}

static void
test_read_after_lock_protect_after_write(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ|VM_PROT_WRITE);
    trywrite(addr);
    protectmap(addr, prot1);
    protectpage(addr, prot2);
    tryread(addr);
    unmap(object, addr);
}

static void
test_write_after_lock_protect_after_write(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ|VM_PROT_WRITE);
    trywrite(addr);
    protectmap(addr, prot1);
    protectpage(addr, prot2);
    trywrite(addr);
    unmap(object, addr);
}

static void
test_read_after_protect_lock_after_read(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ);
    tryread(addr);
    protectpage(addr, prot2);
    protectmap(addr, prot1);
    tryread(addr);
    unmap(object, addr);
}

static void
test_write_after_protect_lock_after_read(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ);
    tryread(addr);
    protectpage(addr, prot2);
    protectmap(addr, prot1);
    trywrite(addr);
    unmap(object, addr);
}

static void
test_read_after_protect_lock_after_write(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ|VM_PROT_WRITE);
    trywrite(addr);
    protectpage(addr, prot2);
    protectmap(addr, prot1);
    tryread(addr);
    unmap(object, addr);
}

static void
test_write_after_protect_lock_after_write(object, prot1, prot2)
    mach_port_t object;
    vm_prot_t prot1, prot2;
{
    vm_offset_t addr;

    addr = mapobject(object, VM_PROT_READ|VM_PROT_WRITE);
    trywrite(addr);
    protectpage(addr, prot2);
    protectmap(addr, prot1);
    trywrite(addr);
    unmap(object, addr);
}

static void
flushserver(object)
    mach_port_t object;
{
    extern mach_port_t mig_get_reply_port();
    mig_reply_header_t msg;
    mach_msg_return_t mr;

    msg.Head.msgh_bits =
	MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, MACH_MSG_TYPE_MAKE_SEND_ONCE);
    msg.Head.msgh_size = 0;
    msg.Head.msgh_remote_port = object;
    msg.Head.msgh_local_port = mig_get_reply_port();
    msg.Head.msgh_kind = 0;
    msg.Head.msgh_id = 0;

    mr = mach_msg(&msg.Head, MACH_SEND_MSG|MACH_RCV_MSG,
		  sizeof msg.Head, sizeof msg, msg.Head.msgh_local_port,
		  MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (mr != MACH_MSG_SUCCESS)
	quit(1, "xptest: mach_msg: %s\n", mach_error_string(mr));
}

static void
server(object)
    mach_port_t object;
{
    extern kern_return_t memory_object_server();
    kern_return_t kr;

    kr = mach_msg_server(memory_object_server, 8192, object);
    quit(1, "xptest: mach_msg_server: %s\n", mach_error_string(kr));
}

kern_return_t
memory_object_init(object, control, name, page_size)
    mach_port_t object;
    mach_port_t control;
    mach_port_t name;
    vm_size_t page_size;
{
    kern_return_t kr;

    mutex_lock(&printf_lock);
    printf("memory_object_init(%x, %x, %x, %x)\n",
	   object, control, name, page_size);
    mutex_unlock(&printf_lock);

    kr = memory_object_set_attributes(control, TRUE, FALSE,
				      MEMORY_OBJECT_COPY_NONE);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: memory_object_set_attributes: %s\n",
	     mach_error_string(kr));

    object_control = control;
    return KERN_SUCCESS;
}

kern_return_t
memory_object_terminate(object, control, name)
    mach_port_t object;
    mach_port_t control;
    mach_port_t name;
{
    kern_return_t kr;

    mutex_lock(&printf_lock);
    printf("memory_object_terminate(%x, %x, %x)\n",
	   object, control, name);
    mutex_unlock(&printf_lock);

    kr = mach_port_destroy(mach_task_self(), control);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: mach_port_destroy: %s\n", mach_error_string(kr));

    kr = mach_port_destroy(mach_task_self(), name);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: mach_port_destroy: %s\n", mach_error_string(kr));

    return KERN_SUCCESS;
}

kern_return_t
memory_object_copy(object, control, offset, length, new_object)
    mach_port_t object;
    mach_port_t control;
    vm_offset_t offset;
    vm_size_t length;
    mach_port_t new_object;
{
    quit(1, "xptest: memory_object_copy\n");
}

kern_return_t
memory_object_data_request(object, control, offset, length, desired_access)
    mach_port_t object;
    mach_port_t control;
    vm_offset_t offset;
    vm_size_t length;
    vm_prot_t desired_access;
{
    kern_return_t kr;

    mutex_lock(&printf_lock);
    printf("memory_object_data_request(%x, %x, %x, %x, %s)\n",
	   object, control, offset, length, prot_name[desired_access]);
    mutex_unlock(&printf_lock);

    if (page_access_mask == VM_PROT_UNAVAILABLE) {
	page_prot = VM_PROT_ALL;

	mutex_lock(&printf_lock);
	printf("memory_object_data_unavailable(%x, %x, %x, %x)\n",
	       object, control, offset, length);
	mutex_unlock(&printf_lock);

	kr = memory_object_data_unavailable(control, offset, length);
	if (kr != KERN_SUCCESS)
	    quit(1, "xptest: memory_object_data_unavailable: %s\n",
		 mach_error_string(kr));
    } else {
	vm_offset_t data;

	desired_access ^= page_access_mask;
	page_prot = desired_access;

	mutex_lock(&printf_lock);
	printf("memory_object_data_provided(%x, %x, %x, %x, %s)\n",
	       object, control, offset, length, prot_name[desired_access]);
	mutex_unlock(&printf_lock);

	kr = vm_allocate(mach_task_self(), &data, length, TRUE);
	if (kr != KERN_SUCCESS)
	    quit(1, "xptest: vm_allocate: %s\n", mach_error_string(kr));

	kr = memory_object_data_provided(control, offset,
					 data, length,
					 VM_PROT_ALL &~ desired_access);
	if (kr != KERN_SUCCESS)
	    quit(1, "xptest: memory_object_data_provided: %s\n",
		 mach_error_string(kr));

	kr = vm_deallocate(mach_task_self(), data, length);
	if (kr != KERN_SUCCESS)
	    quit(1, "xptest: vm_deallocate: %s\n", mach_error_string(kr));
    }

    return KERN_SUCCESS;
}

kern_return_t
memory_object_data_unlock(object, control, offset, length, desired_access)
    mach_port_t object;
    mach_port_t control;
    vm_offset_t offset;
    vm_size_t length;
    vm_prot_t desired_access;
{
    kern_return_t kr;

    mutex_lock(&printf_lock);
    printf("memory_object_data_unlock(%x, %x, %x, %x, %s)\n",
	   object, control, offset, length, prot_name[desired_access]);
    mutex_unlock(&printf_lock);

    page_prot = desired_access;

    kr = memory_object_lock_request(control, offset, length,
				    FALSE, FALSE,
				    VM_PROT_ALL &~ desired_access,
				    MACH_PORT_NULL);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: memory_object_lock_request: %s\n",
	     mach_error_string(kr));

    return KERN_SUCCESS;
}

kern_return_t
memory_object_data_write(object, control, offset, data, data_size)
    mach_port_t object;
    mach_port_t control;
    vm_offset_t offset;
    vm_offset_t data;
    vm_size_t data_size;
{
    kern_return_t kr;

    mutex_lock(&printf_lock);
    printf("memory_object_data_write(%x, %x, %x, %x, %x)\n",
	   object, control, offset, data, data_size);
    mutex_unlock(&printf_lock);

    kr = vm_deallocate(mach_task_self(), data, data_size);
    if (kr != KERN_SUCCESS)
	quit(1, "xptest: vm_deallocate: %s\n", mach_error_string(kr));

    page_cleaned = TRUE;
    return KERN_SUCCESS;
}

kern_return_t
memory_object_lock_completed(object, control, offset, length)
    mach_port_t object;
    mach_port_t control;
    vm_offset_t offset;
    vm_size_t length;
{
    quit(1, "xptest: memory_object_lock_completed\n");
}

kern_return_t
memory_object_supply_completed(object, control, offset, length,
			       result, error_offset)
    mach_port_t object;
    mach_port_t control;
    vm_offset_t offset;
    vm_size_t length;
    kern_return_t result;
    vm_offset_t error_offset;
{
    quit(1, "xptest: memory_object_supply_completed\n");
}

kern_return_t
memory_object_data_return(object, control, offset,
			  data, size, dirty, kernel_copy)
    mach_port_t object;
    mach_port_t control;
    vm_offset_t offset;
    vm_offset_t data;
    vm_size_t size;
    boolean_t dirty;
    boolean_t kernel_copy;
{
    quit(1, "xptest: memory_object_data_return\n");
}

kern_return_t
memory_object_change_completed(object, may_cache, copy_strategy)
    mach_port_t object;
    boolean_t may_cache;
    memory_object_copy_strategy_t copy_strategy;
{
    quit(1, "xptest: memory_object_change_completed\n");
}
