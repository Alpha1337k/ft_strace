#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main() {
	int pid = fork();

	if (pid == -1) {
		exit(2);		
	}
	else if (pid != 0) {
		int pid2 = fork();

		if (pid2 == -1) {
			exit(3);
		}
		else if (pid2 != 0) {
			write(STDOUT_FILENO, "\nFORKED!2\n", 10);
			exit(4);
		}
		else {
			int status = 0;
			waitpid(pid, &status, 0);
			write(STDOUT_FILENO, "\nFORKED!!\n", 10);
			exit(5);
		}
	}
	else {
		int status = 0;
		waitpid(pid, &status, 0);

		return status;
	}

}