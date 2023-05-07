#include "strace.h"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("ft_strace: must have PROG [ARGS]\n");
		return 1;
	}

	pid_t pid = fork();
	if (pid == -1) {
		printf("failed to open pipe");
		return 1;
	} else if (pid == 0)
	{
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		execvp(argv[1], &argv[1]);
		printf("FATAL: execve error\n");
	} else
	{
		waitpid(pid, 0, 0);
		ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL);
		

		for (size_t i = 0; i < 1000; i++)
		{
			ptrace(PTRACE_SYSCALL, pid, 0, 0);
			waitpid(pid, 0, 0);

			struct user_regs_struct regs;
			ptrace(PTRACE_GETREGS, pid, 0, &regs);
			long syscall = regs.orig_rax;

			fprintf(stderr, "%ld(%ld, %ld, %ld, %ld, %ld, %ld) = %lx\n",
					syscall,
					(long)regs.rdi, (long)regs.rsi, (long)regs.rdx,
					(long)regs.r10, (long)regs.r8,  (long)regs.r9, (long)regs.rax);
		}
		

	}

	return (0);
}