#include "strace.h"

char	*parse_void_ptr(void *data, pid_t pid, regs_t regs)
{
	(void)pid;
	(void)regs;

	char *s = malloc(40);
	if (!s) return 0;

	if (data == 0)
		sprintf(s, COLOR_CYAN"NULL"COLOR_RESET);
	else
		sprintf(s, COLOR_CYAN"%p"COLOR_RESET, data);
	return s;
}

char	*parse_unsigned_int(void *data, pid_t pid, regs_t regs)
{
	(void)pid;
	(void)regs;

	char *s = malloc(40);
	if (!s) return 0;

	sprintf(s, COLOR_YELLOW"%ld"COLOR_RESET, (unsigned long)data);
	return s;
}

char	*parse_size_t(void *data, pid_t pid, regs_t regs)
{
	(void)pid;
	(void)regs;

	char *s = malloc(40);
	if (!s) return 0;

	sprintf(s, COLOR_YELLOW"%ld"COLOR_RESET, (size_t)data);
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

char	*__parse_char_ptr(void *data, pid_t pid, regs_t regs, int len)
{
	(void)regs;

	size_t idx = 0;
	size_t str_idx = 0;
	char is_eol = 0;
	char *s = calloc(1, 1024);
	if (!s) return 0;

	if (len < 0) len = __INT_MAX__;

	if (data == NULL) {
		char *rv = malloc(1024);
		sprintf(rv, COLOR_CYAN"NULL"COLOR_RESET);
		free(s);
		return rv;
	}

	while (idx < 32 && (int)idx < len)
	{
		if (errno != 0) {
			perror("PTB4");
			exit(1);
		}

		long addy = ptrace(PTRACE_PEEKTEXT, pid, data + idx, NULL);
		if (errno != 0) {
			perror("peektext");
			exit(1);
		}

		for (size_t i = 0; i < 8; i++)
		{
			char val = ((char *)&addy)[i];
			// printf("V:%x %d\n", (unsigned)val, val);
			if (replace_special_chars(val) != 0)
				str_idx += sprintf(s + str_idx, "%s", replace_special_chars(val));
			else if ((val < 31 || val > 126))
				str_idx += sprintf(s + str_idx, "\\%u", val & 0xff);
			else
				str_idx += sprintf(s + str_idx, "%c", val);
		}

		if (memchr((void *)&addy, 0, 8) && len == -1) {
			is_eol = 1;
			break;
		}
		idx += 8;
	}
	char *rv = malloc(1024);
	if (is_eol)
		sprintf(rv, "\""COLOR_RED"%s"COLOR_RESET"\"", s);
	else 
		sprintf(rv, "\""COLOR_RED"%s"COLOR_RESET"\"...", s);
	
	free(s);

	return rv;
}

char	*parse_char_ptr(void *data, pid_t pid, regs_t regs)
{
	return __parse_char_ptr(data, pid, regs, -1);
}

char	*parse_char_ptr_rsi(void *data, pid_t pid, regs_t regs)
{
	return __parse_char_ptr(data, pid, regs, regs.args[1]);
}

char	*parse_char_ptr_rdx(void *data, pid_t pid, regs_t regs)
{
	return __parse_char_ptr(data, pid, regs, regs.args[2]);
}

char	*parse_char_ptr_r10(void *data, pid_t pid, regs_t regs)
{
	return __parse_char_ptr(data, pid, regs, regs.args[3]);
}

