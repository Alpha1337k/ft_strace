#include "strace.h"

char	*parse_void_ptr(void *data, pid_t pid, struct user_regs_struct regs)
{
	char *s = malloc(40);
	if (data == 0)
		sprintf(s, "\033[36;1mNULL\033[0m");
	else
		sprintf(s, "\033[36;1m%p\033[0m", data);
	return s;
}

char	*parse_unsigned_int(void *data, pid_t pid, struct user_regs_struct regs)
{
	char *s = malloc(40);
	sprintf(s, "\033[0;33m%ld\033[0m", (unsigned long)data);
	return s;
}

char	*parse_size_t(void *data, pid_t pid, struct user_regs_struct regs)
{
	char *s = malloc(40);
	sprintf(s, "\033[0;33m%ld\033[0m", (size_t)data);
	return s;
}

char	*parse_char_ptr(void *data, pid_t pid, struct user_regs_struct regs)
{
	void *addy = (void *)ptrace(PTRACE_PEEKTEXT, pid, data);

	char *s = malloc(1024);
	sprintf(s, "\033[0;33m%s\033[0m", (char *)&addy);
	return s;
}