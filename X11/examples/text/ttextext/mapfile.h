/*
 * mapfilein returns an array of (struct mapfile) in the maplist variable as 
 * well as a count of the number of elements in the array.
 */

struct mapfile {
    int len;
    char *line;
};

extern int mapfilein();
extern void mapfilefree();
