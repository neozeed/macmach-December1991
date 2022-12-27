/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
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
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	cfb_misc.c,v $
 * Revision 2.7  91/08/24  11:51:34  af
 * 	CharRows is gone, pm_init_screen_params() is different.
 * 	[91/08/02  02:00:39  af]
 * 
 * Revision 2.6  91/06/19  11:47:19  rvb
 * 	File moved here from mips/PMAX since it tries to be generic.
 * 	[91/06/04            rvb]
 * 
 * Revision 2.5  91/05/14  17:20:10  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/02/05  17:40:08  mrt
 * 	Added author notices
 * 	[91/02/04  11:12:28  mrt]
 * 
 * 	Changed to use new Mach copyright
 * 	[91/02/02  12:10:16  mrt]
 * 
 * Revision 2.3  90/12/05  23:30:37  af
 * 	Cleaned up.
 * 	[90/12/03  23:12:16  af]
 * 
 * Revision 2.1.1.1  90/11/01  03:42:15  af
 * 	Created, from the DEC specs:
 * 	"PMAG-BA TURBOchannel Color Frame Buffer Functional Specification"
 * 	Workstation Systems Engineering, Palo Alto, CA. Aug 27, 1990
 * 	[90/09/03            af]
 */
/*
 *	File: cfb_misc.c
 * 	Author: Alessandro Forin, Carnegie Mellon University
 *	Date:	9/90
 *
 *	Driver for the PMAG-BA simple color framebuffer
 *
 */

#include <cfb.h>
#if	(NCFB > 0)

/*
 * NOTE: This driver relies heavily on the pm one.
 */

#include <device/device_types.h>

#include <chips/screen_defs.h>

#include <chips/pm_defs.h>
typedef pm_softc_t	cfb_softc_t;

#include <mips/PMAX/pmag_ba.h>
#include <chips/bt459.h>
#define	bt459		cursor_registers


/*
 * Initialize color map, for kernel use
 */
cfb_init_colormap(sc)
	screen_softc_t	sc;
{
	cfb_softc_t	*cfb = (cfb_softc_t*)sc->hw_state;
	user_info_t	*up = sc->up;
	color_map_t	Bg_Fg[2];
	register int	i;

	bt459_init_colormap( cfb->bt459 );

	/* init bg/fg colors */
	for (i = 0; i < 3; i++) {
		up->dev_dep_2.pm.Bg_color[i] = 0x00;
		up->dev_dep_2.pm.Fg_color[i] = 0xff;
	}

	Bg_Fg[0].red = Bg_Fg[0].green = Bg_Fg[0].blue = 0x00;
	Bg_Fg[1].red = Bg_Fg[1].green = Bg_Fg[1].blue = 0xff;
	bt459_cursor_color( cfb->bt459, Bg_Fg);
}

/*
 * Large viz small cursor
 */
cfb_small_cursor_to_large(up, cursor)
	user_info_t	*up;
	cursor_sprite_t	cursor;
{
	unsigned short new_cursor[32];
	unsigned char  *curbytes, fg, fbg;
	int             i, j, k;
	char           *sprite;

	/* Clear out old cursor */
	bzero(	up->dev_dep_2.pm.cursor_sprite,
		sizeof(up->dev_dep_2.pm.cursor_sprite));

	/* small cursor is 32x2 bytes, fg first */
	curbytes = (unsigned char *) cursor;

	/* use the upper left corner of the large cursor
	 * as a 64x1 cursor, fg&bg alternated */
	for (i = 0; i < 32; i++) {
		fg = curbytes[i];
		fbg = fg | curbytes[i + 32];
		new_cursor[i] = 0;
		for (j = 0, k = 15; j < 8; j++, k -= 2) {
			new_cursor[i] |= ((fbg >> j) & 0x1) << (k);
			new_cursor[i] |= ((fg >> j) & 0x1) << (k - 1);
		}
	}

	/* Now stick it in the proper place */

	curbytes = (unsigned char *) new_cursor;
	sprite = up->dev_dep_2.pm.cursor_sprite;
	for (i = 0; i < 64; i += 4) {
		/* butterfly as we go */
		*sprite++ = curbytes[i + 1];
		*sprite++ = curbytes[i + 0];
		*sprite++ = curbytes[i + 3];
		*sprite++ = curbytes[i + 2];
		sprite += 12; /* skip rest of the line */
	}
}


/*
 * Device-specific set status
 */
cfb_set_status(sc, flavor, status, status_count)
	screen_softc_t	sc;
	int		flavor;
	dev_status_t	status;
	unsigned int	status_count;
{
	cfb_softc_t		*cfb = (cfb_softc_t*) sc->hw_state;

	switch (flavor) {

	case SCREEN_ADJ_MAPPED_INFO:
		return pm_set_status(sc, flavor, status, status_count);

	case SCREEN_LOAD_CURSOR:

		if (status_count < sizeof(cursor_sprite_t)/sizeof(int))
			return D_INVALID_SIZE;
		cfb_small_cursor_to_large(sc->up, (cursor_sprite_t*) status);

		/* Fall through */

	case SCREEN_LOAD_CURSOR_LONG: /* 3max only */
		bt459_cursor_sprite(cfb->bt459, sc->up->dev_dep_2.pm.cursor_sprite);
		break;
	     
	case SCREEN_SET_CURSOR_COLOR:
		bt459_cursor_color (cfb->bt459, (color_map_t*) status);
		break;

	case SCREEN_SET_CMAP_ENTRY: {
		color_map_entry_t	*e = (color_map_entry_t*) status;

		if (e->index < 256)
			bt459_load_colormap_entry( cfb->bt459, e->index, &e->value);
		break;
	}

	default:
		return D_INVALID_OPERATION;
	}
	return D_SUCCESS;
}

/*
 * Hardware initialization
 */
cfb_init_screen(cfb)
	cfb_softc_t *cfb;
{
	bt459_init( cfb->bt459,
		    cfb->bt459 + (CFB_OFFSET_RESET - CFB_OFFSET_BT459));
}

/*
 * Do what's needed when X exits
 */
cfb_soft_reset(sc)
	screen_softc_t	sc;
{
	cfb_softc_t	*cfb = (cfb_softc_t*) sc->hw_state;
	user_info_t	*up =  sc->up;
	extern cursor_sprite_t	dc503_default_cursor;

	/*
	 * Restore params in mapped structure
	 */
	pm_init_screen_params(sc,up);
	up->row = MinCharRows - 1;

	up->dev_dep_2.pm.x26 = 2; /* you do not want to know */
	up->dev_dep_1.pm.x18 = (short*)2;

	/*
	 * Restore RAMDAC chip to default state
	 */
	cfb_init_screen(cfb);

	/*
	 * Load kernel's cursor sprite: just use the same pmax one
	 */
	cfb_small_cursor_to_large(up, dc503_default_cursor);
	bt459_cursor_sprite(cfb->bt459, up->dev_dep_2.pm.cursor_sprite);

	/*
	 * Color map and cursor color
	 */
	cfb_init_colormap(sc);
}




#endif	(NCFB > 0)
