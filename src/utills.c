#include "strace.h"

char *ft_strjoin(char *s1, char *s2)
{
	size_t lengths[2] = {
		strlen(s1),
		strlen(s2)
	};
	size_t new_len = lengths[0] + lengths[1] + 1;

	char *rv = malloc(new_len);
	if (!rv) {
		return 0;
	}

	rv[new_len - 1] = 0;

	memcpy(rv, s1, lengths[0]);
	memcpy(rv + lengths[0], s2, lengths[1]);

	return rv;
}