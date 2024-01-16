#include <unistd.h>


int main() {
	char *s = 0;

	write(STDOUT_FILENO, (char *)s[10], 10);

	return 0;
}