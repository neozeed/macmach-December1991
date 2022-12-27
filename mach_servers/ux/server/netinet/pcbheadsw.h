/* 
 **********************************************************************
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 **********************************************************************
 * HISTORY
 * $Log:	pcbheadsw.h,v $
 * Revision 2.1  89/08/04  14:23:15  rwd
 * Created.
 * 
 **********************************************************************
 */ 

/*
 * pcbheadsw.h
 */


struct pcbheadsw {
    short protocol;
    struct inpcb *head;
};

