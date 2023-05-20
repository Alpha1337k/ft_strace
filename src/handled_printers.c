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

// char	*convert_undisplayable_chars(char *str, size_t len)
// {
// 	size_t i = 0;
// 	size_t idx = 0;
// 	char *new_str = malloc(1024);
// 	while (i < len)
// 	{
// 		if (str[i] < 32 || str[i] > 126) {
// 			idx += sprintf(&new_str[idx], "\\%x", str[i]);
// 		}
// 		i++;
// 	}
// 	return new_str;	
// }

char	*replace_special_chars(char s) {
	switch (s)
	{
	case '\n':
		return "\\n";
	case '\t':
		return "\\t";
	default:
		return 0;
	}
}

char	*parse_char_ptr(void *data, pid_t pid, struct user_regs_struct regs)
{
	size_t idx = 0;
	size_t str_idx = 0;
	char *s = malloc(1024);
	while (1)
	{
		long addy = ptrace(PTRACE_PEEKTEXT, pid, data + idx);
		for (size_t i = 0; i < 8; i++)
		{
			if (replace_special_chars(((char *)&addy)[i]) != 0)
				str_idx += sprintf(s + str_idx, "%s", replace_special_chars(((char *)&addy)[i]));
			else
				str_idx += sprintf(s + str_idx, "%c", ((char *)&addy)[i]);
		}

		if (memchr((void *)&addy, 0, 8)) {
			break;
		}
		idx += 8;
	}
	char *rv = malloc(1024);
	sprintf(rv, "\"\033[0;31m%s\033[0m\"", s);
	
	return rv;
}