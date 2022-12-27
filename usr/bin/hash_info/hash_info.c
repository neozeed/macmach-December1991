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
 * $Log:	hash_info.c,v $
 * Revision 2.3  91/03/27  17:18:00  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.3  91/03/19  11:33:33  mrt
 * 	Changed to new copyright
 * 
 * Revision 2.2  91/01/08  14:57:23  rpd
 * 	Created.
 * 	[91/01/08            rpd]
 * 
 */

#include <mach.h>
#include <mach_debug/mach_debug.h>
#include <mach_error.h>
#include <mach/message.h>
#include <stdio.h>
#include <strings.h>

extern double expected();

static void print_bucket_info();

static void
usage()
{
    quit(1, "usage: hash_info\n");
}

void
main(argc, argv)
    int argc;
    char *argv[];
{
    mach_port_t host = mach_host_self();
    hash_info_bucket_t *hinfo, *mainfo, *vpinfo;
    mach_msg_type_number_t hcount, macount, vpcount;
    unsigned int max_requests;
    kern_return_t kr;

    if (argc != 1)
	usage();

    kr = host_ipc_hash_info(host, &hinfo, &hcount);
    if (kr != KERN_SUCCESS)
	quit(1, "hash_info: host_ipc_hash_info: %s\n",
	     mach_error_string(kr));

    kr = host_ipc_marequest_info(host, &max_requests, &mainfo, &macount);
    if (kr != KERN_SUCCESS)
	quit(1, "hash_info: host_ipc_marequest_info: %s\n",
	     mach_error_string(kr));

    kr = host_virtual_physical_table_info(host, &vpinfo, &vpcount);
    if (kr != KERN_SUCCESS)
	quit(1, "hash_info: host_virtual_physical_table_info: %s\n",
	     mach_error_string(kr));

    printf("Global reverse hash table:\n");
    print_bucket_info(hinfo, hcount);
    printf("\n");
    printf("Msg-accepted request hash table:\n");
    printf("max requests:     %u\n", max_requests);
    print_bucket_info(mainfo, macount);
    printf("\n");
    printf("Virtual-Physical table:\n");
    print_bucket_info(vpinfo, vpcount);
    exit(0);
}

static unsigned int
total_count(info, num)
    hash_info_bucket_t *info;
    mach_msg_type_number_t num;
{
    unsigned int sum = 0;
    unsigned int i;

    for (i = 0; i < num; i++)
	sum += info[i].hib_count;

    return sum;
}

static unsigned int
max_count(info, num)
    hash_info_bucket_t *info;
    mach_msg_type_number_t num;
{
    unsigned int max = 0;
    unsigned int i;

    for (i = 0; i < num; i++)
	if (info[i].hib_count > max)
	    max = info[i].hib_count;

    return max;
}

static unsigned int
num_count(info, num, count)
    hash_info_bucket_t *info;
    mach_msg_type_number_t num;
    unsigned int count;
{
    unsigned int total = 0;
    unsigned i;

    for (i = 0; i < num; i++)
	if (info[i].hib_count == count)
	    total++;

    return total;
}

static void
print_bucket_info(info, num)
    hash_info_bucket_t *info;
    mach_msg_type_number_t num;
{
    unsigned int sum = total_count(info, num);
    unsigned int max =  max_count(info, num);
    unsigned int nonempty = num - num_count(info, num, 0);
    double mean1, mean2;
    int i;

    if (nonempty == 0)
	mean1 = 0.0;
    else
	mean1 = sum/(double)nonempty;
    mean2 = sum/(double)num;

    printf("size:             %u\n", num);
    printf("total:            %u\n", sum);
    printf("non-empty:        %u (%d%%)\n",
	   nonempty, 100*nonempty/num);
    printf("mean count:       %4.2f (%4.2f)\n", mean1, mean2);
    printf("max count:        %u\n", max);

    for (i = 0; i <= max; i++)
	printf("%1d-count number: %3u (%4.2f)\n",
	       i, num_count(info, num, i),
	       expected((double) num, (double) sum, (double) i));
}
