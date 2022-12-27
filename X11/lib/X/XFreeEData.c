#include "copyright.h"

/* $XConsortium: XFreeEData.c,v 11.12 88/09/06 16:07:30 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include "Xlibint.h"

_XFreeExtData (extension)
     XExtData *extension;
{
	XExtData *temp;
	while (extension) {
		if (extension->free_private) 
		    (*extension->free_private)(extension);
		else Xfree ((char *)extension->private_data);
		temp = extension->next;
		Xfree ((char *)extension);
		extension = temp;
	}
	return;
}
