/*
 * Stubs for missing routines.
 */
#include <uxkern/import_mach.h>

boolean_t
task_secure(task)
	task_t	task;
{
	return (TRUE);
}

gets(name)
	char	name[];
{
	panic("gets missing");
}

mach_error(str, err)
	char	str[];
	int	err;
{
	printf("Mach Error %s (%d)\n", str, err);
	panic("mach_error");
}

