/*
 * xtotroff
 *
 * convert X font metrics into troff font metrics
 */

# include	<X11/Xlib.h>
# include	<stdio.h>
# include	<ctype.h>
# include	"libXdvi/XFontName.h"
# include	"libXdvi/DviChar.h"

# define charWidth(fi,c)	((fi)->per_char[(c) - (fi)->min_char_or_byte2].width)
# define fontWidth(width,count,c)	(count[c] ? width[c] / count[c] : 0)

Display		*dpy;

MapFont (font_name, troff_name, internname, scale)
	char	*font_name;
	char	*troff_name;
	int	internname;
	int	scale;
{
	XFontStruct	*fi;
	int		count;
	char		**names;
	char		**sizeNames;
	int		sizeCount;
	FILE		*out;
	int		c;
	unsigned int	attributes;
	XFontName	parsed;
	XFontNameString	otherstring;
	XFontName	sizeFont;
	int		widthTable[256];
	int		countTable[256];
	int		j, k;
	DviCharNameMap	*char_map;
	char		encoding[256];
	char		*s;

	names = XListFonts (dpy, font_name, 100000, &count);
	if (count < 1) {
		fprintf (stderr, "bad font name: %s\n", font_name);
		return 0;
	}
	if (count > 1) {
		fprintf (stderr, "ambiguous font name: %s\n", font_name);
		return 0;
	}
	if (!XParseFontName (names[0], &parsed, &attributes)) {
		fprintf (stderr, "not a standard name: %s\n", names[0]);
		return 0;
	}
	sprintf (encoding, "%s-%s", parsed.CharSetRegistry,
				    parsed.CharSetEncoding);
	for (s = encoding; *s; s++)
		if (isupper (*s))
			*s = tolower (*s);
	char_map = DviFindMap (encoding);
	if (!char_map) {
		fprintf (stderr, "not a standard encoding: %s\n", encoding);
		return 0;
	}
	XFormatFontName (&parsed,
			attributes & (FontNameRegistry|
				      FontNameFoundry|
				      FontNameFamilyName|
				      FontNameWeightName|
				      FontNameSlant|
				      FontNameSetwidthName|
				      FontNameAddStyleName|
				      FontNameCharSetRegistry|
				      FontNameCharSetEncoding),
			otherstring);
	sizeNames = XListFonts (dpy, otherstring, 100000, &sizeCount);
	for (j = 0; j < 256; j++) {
		widthTable[j] = 0;
		countTable[j] = 0;
	}
	for (j = 0; j < sizeCount ; j++) {
		XParseFontName (sizeNames[j], &sizeFont, &attributes);
		
		fi = XLoadQueryFont (dpy, sizeNames[j]);
		for (c = fi->min_char_or_byte2; c <= fi->max_char_or_byte2; c++) {
			widthTable[c] += charWidth (fi, c) *
			    scale / (sizeFont.PointSize / 10);
			countTable[c]++;
		}
		XUnloadFont (dpy, fi->fid);
	}
	out = fopen (troff_name, "w");
	fprintf (out, "name %s\n", troff_name);
	fprintf (out, "internanname %d\n", internname);
	if (!strcmp (char_map->encoding, "adobe-fontspecific"))
	    fprintf (out, "special\n");
	fprintf (out, "spacewidth %d\n",
		fontWidth(widthTable,countTable,' '));
	fprintf (out, "charset\n");
	for (c = 32; c < 256; c++) {
		for (k = 0; DviCharName(char_map,c,k); k++) {
			if (k == 0)
				fprintf (out, "%s\t%d\t0\t0%o\n",
						DviCharName(char_map,c,k),
		 				fontWidth(widthTable,countTable,c),
		 				c);
			else
				fprintf (out, "%s\t\"\n",
						DviCharName(char_map,c,k));
		}
	}
	fclose (out);
	return 1;
}

main (argc, argv)
	char	**argv;
{
	char	troff_name[1024];
	char	font_name[1024];
	char	line[1024];
	char	*a, *b, c;
	int	scale;
	int	position;

	if (argc != 2) {
		fprintf (stderr, "usage: %s scale\n", argv[0]);
		exit (1);
	}
	scale = atoi(argv[1]);
	dpy = XOpenDisplay (0);
	if (!dpy)
		exit (1);
        position = 1;
	while (fgets (line, sizeof (line), stdin)) {
		for (a=line,b=troff_name; *a; a++,b++) {
			c = (*b = *a);
			if (c == ' ' || c == '\t')
				break;
		}
		*b = '\0';
		while (*a && (*a == ' ' || *a == '\t'))
			++a;
		for (b=font_name; *a; a++,b++)
			if ((*b = *a) == '\n')
				break;
		*b = '\0';
		printf ("%s -> %s\n", font_name, troff_name);
		if (!MapFont (font_name, troff_name, position, scale))
			exit (1);
		++position;
	}
	exit (0);
}

