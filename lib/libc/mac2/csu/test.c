/* test program for mon/gmon */

void odd();
void even();

void odd(f, x)
register int x;
void (*f)();
{
  register int i;
  for (i=0; i<x; i++) (*f)(even, i);
}

void even(f, x)
register int x;
void (*f)();
{
  register int i;
  for (i=0; i<x; i++) (*f)(odd, i);
}

main(argc, argv)
int argc;
char **argv;
{
  odd(even, argc > 1 ? atoi(argv[1]) : 0);
}
