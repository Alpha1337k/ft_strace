#include "strace.h"

extern const t_header SYSCALLS_x64[1024];
#define EXIT_STATE_UNSET 0xFFFF
#define IS_EXIT_STATE(status) status & 0xFF == status
#define PTRACE_EVENT_STATE 0x8000
#define PTRACE_EVENT(status) status >> 16

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

void	handle_ptrace_event(pid_t pid, int status)
{
	if (status>>8 == (SIGTRAP | (PTRACE_EVENT_EXIT<<8)))
	{
		unsigned long res = 0;
		if (ptrace(PTRACE_GETEVENTMSG, pid, 0, &res) == -1) {
			perror("TRACEEXIT");
			exit(1);
		}

		printf(") = ?\n+++ exited with %lu +++\n", res);

		exit(0);
	}

	assert(0);
}

struct user_regs_struct get_next_syscall_regs(pid_t pid, int *exit_state, int *status)
{
	struct user_regs_struct registries;

	if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1) {
		perror("syscall1");
		exit(1);
	}
	if (waitpid(pid, status, 0) == -1) {
		perror("waitpid");
		exit(1);
	}

	if (WIFEXITED(*status)) {
		exit_state[0] = *status;
		return registries;
	}

	if (PTRACE_EVENT(*status)) {
		exit_state[0] = PTRACE_EVENT_STATE;
		return registries;
	}

	if (ptrace(PTRACE_GETREGS, pid, 0, &registries) == -1) {
		perror("getregs");
		exit(1);
	}

	return registries;
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
		// ptrace(PTRACE_TRACEME, 0, 0, 0);
		execvp(argv[1], &argv[1]);
		printf("FATAL: execve error\n");
	} else if (pid == -1) {
		perror("fork");
		exit(1);
	} else
	{
		int status;

		if (ptrace(PTRACE_SEIZE, pid, 0, PTRACE_O_TRACESYSGOOD | PTRACE_O_EXITKILL | PTRACE_O_TRACEEXIT) == -1) {
			perror("ATTACH");
			exit(1);
		}

		int interrupt = ptrace(PTRACE_INTERRUPT, pid, NULL, NULL);
		int pid = waitpid(pid, &status, 0);
		
		while(1)
		{
			int exit_state = EXIT_STATE_UNSET;
			struct user_regs_struct regs = get_next_syscall_regs(pid, &exit_state, &status);

			if (exit_state == PTRACE_EVENT_STATE) {
				handle_ptrace_event(pid, status);
				return 0;
			}

			if (IS_EXIT_STATE(exit_state)) {
				unsigned long res = 0;

				printf("+++ exited with %u +++\n", status);
				return 0;
			}

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

			regs = get_next_syscall_regs(pid, &exit_state, &status);

			if (exit_state == PTRACE_EVENT_STATE) {
				handle_ptrace_event(pid, status);
				return 0;
			}
			if (IS_EXIT_STATE(exit_state)) {

				printf("+++ exited with %u +++\n", status);
				return 0;
			}

			printf(") = %s\n", SYSCALLS_x64[syscall].rax_resolver((void *)regs.rax, pid, regs));

			// fprintf(stderr, "%ld(%ld, %ld, %ld, %ld, %ld, %ld) = %lx\n",
			// 		syscall,
			// 		(long)regs.rdi, (long)regs.rsi, (long)regs.rdx,
			// 		(long)regs.r10, (long)regs.r8,  (long)regs.r9, (long)regs.rax);
		}
	}

	return (0);
}