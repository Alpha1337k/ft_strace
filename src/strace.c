#include "strace.h"

extern const t_header SYSCALLS_x64[1024];

unsigned long long get_register_idx(size_t idx, struct user_regs_struct regs)
{
	switch (idx)
	{
	case 0:
		return regs.rdi;
	case 1:
		return regs.rsi;
	case 2:
		return regs.rdx;
	case 3:
		return regs.r10;
	case 4:
		return regs.r8;
	case 5:
		return regs.r9;
	}
	return 0;
}

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
		int status;

		waitpid(pid, &status, 0);
		ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL);
		

		for (size_t i = 0; i < 100; i++)
		{
			ptrace(PTRACE_SYSCALL, pid, 0, 0);
			waitpid(pid, &status, 0);

			struct user_regs_struct regs;
			ptrace(PTRACE_GETREGS, pid, 0, &regs);
			long syscall = regs.orig_rax;

			printf("\033[31;1m%s\033[0m(", SYSCALLS_x64[syscall].name);
			for (size_t i = 0; i < 6; i++)
			{
				if (SYSCALLS_x64[syscall].funcs[i] != NULL) 
				{
					char *str = SYSCALLS_x64[syscall].funcs[i]((void *)get_register_idx(i, regs), pid, regs);

					printf("%s", str);
					free(str);
					if (i + 1 != 6 && SYSCALLS_x64[syscall].funcs[i + 1] != 0)
						printf(",");
				} else {
					break;
				}
			}

			ptrace(PTRACE_SYSCALL, pid, 0, 0);
			waitpid(pid, &status, 0);

			ptrace(PTRACE_GETREGS, pid, 0, &regs);
			printf(") = %p\n", (void *)regs.rax);

			if (WIFEXITED(status))
			{
				printf("--exited--\n");
				return 0;
			}

			// fprintf(stderr, "%ld(%ld, %ld, %ld, %ld, %ld, %ld) = %lx\n",
			// 		syscall,
			// 		(long)regs.rdi, (long)regs.rsi, (long)regs.rdx,
			// 		(long)regs.r10, (long)regs.r8,  (long)regs.r9, (long)regs.rax);
		}
		

	}

	return (0);
}