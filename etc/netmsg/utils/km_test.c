/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	km_test.c,v $
 * Revision 2.1  90/10/27  20:47:12  dpj
 * Moving under MACH3 source control
 * 
 * Revision 1.7  89/05/02  11:20:11  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 14-Oct-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Make multi-threaded.  Removed kds_new_key_for_host and HOST_TO_STRING.
 *
 *  2-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Get km_port from mach_ports_lookup.
 *	Removed km_set_crypt_algorithm call.
 *
 * 26-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */

/*
 * km_test.c
 *
 *
 */

#ifndef	lint
static char     rcsid[] = "$Header: /afs/cs.cmu.edu/project/mach-2/rcs/pkg/netmsg/utils/km_test.c,v 2.1 90/10/27 20:47:12 dpj Exp $";
#endif not lint

/*
 * Test program for Network Server Key Management.
 */


#define MACH_INIT_SLOTS		1

#include <cthreads.h>
#include <mach.h>
#include <mach_init.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <mach/message.h>
#include <sys/time.h>
#include <sys/types.h>

#include "key_defs.h"
#include "km.h"
#include "multperm.h"
#include "nm_defs.h"
#include "sm_init_defs.h"

static port_t	km_port = PORT_NULL;
static port_t	rec_port = PORT_NULL;
static port_t	network_server_port = PORT_NULL;
static char	in_buff[128];
static char	rep_buff[64];

#define NETNAME_NAME	"NEW_NETWORK_NAME_SERVER"
#define KM_SERVICE_NAME	"NETWORK_SERVER_KEY_MANAGER"

extern char	*gets();

/*
 * String and macro for printing keys.
 */
static char		key_string[48];

#define KEY_TO_STRING(key) (						\
    (void)(sprintf(key_string, "%d.%d.%d.%d", (key).key_longs[0],	\
	    (key).key_longs[1], (key).key_longs[2],			\
	    (key).key_longs[3])),					\
	key_string)


/*
 * host_address
 *	Return a host address for a given host name.
 *
 */
long host_address(host_name)
char *host_name;
{
    register struct hostent *hp = gethostbyname(host_name);
    if (hp == 0) return 0;
    else return *(long *)(hp->h_addr);
}



/*
 * get_key
 *	Creates a new key.
 *
 */
void get_key(key_ptr)
key_t	*key_ptr;
{
    int 		i;
    unsigned int	temp;
    char		input_line[16];

    for (i = 0; i < 4; i++) {
	printf("Input a long: ");
	do {
	    (void)gets(input_line);
	    (void)sscanf((char *)input_line, "%d", &temp);
	    if ((minverse(temp)) == 0) {
		printf("%d is not a valid multperm key, try again: ", temp);
		temp = 0;
	    }
/*
	    else printf("Multiplicative inverse of %d is %d.\n", temp, minverse(temp));
*/
	} while (temp == 0);
	key_ptr->key_longs[i] = temp;
    }
    printf("New key = %s.\n", KEY_TO_STRING(*key_ptr));

}



/*
 * get_net_address
 *	Prompts for and returns a network address.
 *
 */
netaddr_t get_net_address()
{
    int		num_items;
    ip_addr_t	input_address;
    int		net_owner, net_node_type, host_high, host_low;
    char	input_line[40];

    num_items = 0;
    while (num_items < 4) {
	(void)gets(input_line);
	num_items = sscanf((char *)input_line, "%d%d%d%d",
				&net_owner, &net_node_type, &host_high, &host_low);
	if (num_items < 4) fprintf(stderr, "Need four integers for network address.\n");
	else {
	    input_address.ia_bytes.ia_net_owner = net_owner;
	    input_address.ia_bytes.ia_net_node_type = net_node_type;
	    input_address.ia_bytes.ia_host_high = host_high;
	    input_address.ia_bytes.ia_host_low = host_low;
	}
    }
    return input_address.ia_netaddr;

}


/*
 * km_test_input
 *	If there is input, processes it.
 *	Calls km_use_key_for_host.
 *
 */
void km_test_input()
{
    char            input_string[32];
    char            host_name[32];
    netaddr_t       host_id;
    key_t           key;
    kern_return_t   kr;
    int             new_algorithm;

    while (1) {
	printf("\nHit return to call km_use_key_for_host.\n");
	(void)gets(input_string);
	host_id = 0;
	while (host_id == 0) {
	    printf("km_test_input - input host name: \n");
	    (void) gets(input_string);
	    (void) sscanf(input_string, "%s", host_name);
	    if (strcmp(host_name, "quit") == 0) exit(0);
	    if ((host_id = host_address(host_name)) == 0) {
		fprintf(stderr, "Host '%s' not found.\n", host_name);
	    } else {
		fprintf(stderr, "host_id = %s.\n", inet_ntoa(host_id));
	    }
	}
	printf("km_test_input - input new key: \n");
	get_key(&key);
	NTOH_KEY(key);
	if ((kr = km_use_key_for_host(km_port, host_id, key)) != KERN_SUCCESS) {
	    fprintf(stderr, "km_test_input.km_do_key_exchange fails, kr = %d.\n", kr);
	}

    }
}



/*
 * kds_do_key_exchange
 *
 */
void _kds_do_key_exchange(ServPort, host_id)
port_t		ServPort;
netaddr_t	host_id;
{

    printf("kds_do_key_exchange called: ServPort = %d, host_id = %s.\n", ServPort, inet_ntoa(host_id));

}



/*
 * km_test_receive
 *	tries to receive a message on the km_port and to process it.
 *
 */
void km_test_receive()
{
    kern_return_t	kr;
    msg_header_t	*in_msg_ptr, *rep_msg_ptr;

    in_msg_ptr = (msg_header_t *)in_buff;
    rep_msg_ptr = (msg_header_t *)rep_buff;

    while (1) {
	in_msg_ptr->msg_size = 128;
	in_msg_ptr->msg_local_port = rec_port;
	if ((kr = msg_receive(in_msg_ptr, RCV_TIMEOUT, 2000)) == RCV_SUCCESS) {
	    if (!kds_server((caddr_t)in_msg_ptr, (caddr_t)rep_msg_ptr)) {
		fprintf(stderr, "km_test_receive.km_server fails, msg_id = %d.\n", in_msg_ptr->msg_id);
	    }
	}
	else if (kr != RCV_TIMED_OUT) {
	    fprintf(stderr, "km_test_receive.msg_receive fails, kr = %d.\n", kr);
	}
    }
}



/*
 * km_test_init
 *	looks up the network server key management port.
 *	Allocates a port and registers it with the network server.
 *
 */
void km_test_init()
{
    kern_return_t	kr;

#if	1
    port_array_t	ports;
    unsigned int	ports_cnt;

    /*
     * Get hold of our private port to the network server via mach_ports_lookup.
     * Create the ns_lkds_port and connect to the network server.
     */
    if ((kr = mach_ports_lookup(task_self(), &ports, &ports_cnt)) != KERN_SUCCESS) {
	fprintf(stderr, "lkds_procs_init.mach_ports_lookup fails, kr = %d.\n", kr);
	exit(-1);
    }
    km_port = ports[KM_SERVICE_SLOT];
    printf("km_test_init: km_port = %x.\n", km_port);
#else
    /*
     * Look up the new network server.
     */
    if ((kr = netname_look_up(name_server_port, "", NETNAME_NAME, &network_server_port)) != KERN_SUCCESS) {
	fprintf(stderr, "Look up of new network server fails, kr = %d.\n", kr);
	exit(-1);
    }

    do {
	if ((kr = netname_look_up(network_server_port, "", KM_SERVICE_NAME, &km_port)) != KERN_SUCCESS) {
	    fprintf(stderr, "km_test_init.netname_look_up of km_port fails - trying again.\n");
	}
    } while (kr != KERN_SUCCESS);
#endif

    if ((kr = port_allocate(task_self(), &rec_port)) != KERN_SUCCESS) {
	fprintf(stderr, "km_test_init.port_allocate fails, kr = %d.\n", kr);
	exit(-1);
    }
    kr = port_disable(task_self(), rec_port);

    if ((kr = km_kds_connect(km_port, rec_port)) != KERN_SUCCESS) {
	fprintf(stderr, "km_test_init.km_kds_connect fails, kr = %d.\n");
	exit(-1);
    }

}



/*
 * main
 *	Initialise.
 *	Call km_test_receive and km_test_input.
 *
 */
main()
{
    cthread_t	new_thread;

    km_test_init();

    new_thread = cthread_fork(km_test_receive, 0);
    cthread_detach(new_thread);

    printf("KM test initialised.\n");

    km_test_input();
}
