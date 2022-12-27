#include <mach.h>
#include <cthreads.h>

#define NTHREADS 6

int nthreads;

int loop_delta[NTHREADS];

int loop_last[NTHREADS];

int *loop_count;

any_t loop(n)
register n;
{
    for (;;)
	loop_count[n]++;
}

main(argc, argv)
int argc, **argv;
{
    int n;

    argc--;
    if (argc != 1)
	nthreads = NTHREADS;
    else {
	nthreads = atoi(argv[1]);
	if (nthreads < 0 || nthreads > NTHREADS)
	    nthreads = NTHREADS;
    }

    if (vm_allocate(task_self(),
		    (vm_address_t *)&loop_count,
		    (vm_size_t)(nthreads * sizeof (*loop_count)),
		    TRUE) != KERN_SUCCESS) {
	printf("vm_allocate\n");
	exit(0);
    }

    if (vm_inherit(task_self(),
		   (vm_address_t)loop_count,
		   (vm_size_t)(nthreads * sizeof (*loop_count)),
		   VM_INHERIT_SHARE) != KERN_SUCCESS) {
	printf("vm_inherit\n");
	exit(0);
    }

    if (fork()) {
	for (;;) {
	    for (n = 0; n < nthreads; n++) {
		loop_delta[n] = loop_count[n] - loop_last[n];
		loop_last[n] = loop_count[n];
	    }	
	    for (n = 0; n < nthreads; n++)
		printf("%d ", loop_delta[n]);
	    printf("\n");
	    sleep(5);
	}
    }

    cthread_init();

    for (n = 0; n < nthreads; n++)
	cthread_fork(loop, (any_t)n);

    thread_suspend(thread_self());
}
