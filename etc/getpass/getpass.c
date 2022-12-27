main(argc, argv)
int argc;
char **argv;
{
  extern char *getpass();
  puts(getpass(argc > 1 ? argv[1] : "Password:"));
}

