#include "strace.h"

extern const t_header SYSCALLS_x64[1024];
#define EXIT_STATE_UNSET 0xFFFF
#define IS_EXIT_STATE(status) status & 0xFF == status
#define PTRACE_EVENT_STATE 0x8000
#define PTRACE_EVENT(status) status >> 16
#define IS_SIGNAL_STATE(status) ((status >> 8) & 0x7f) != 5

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

		printf(" = ?\n+++ exited with %lu +++\n", res >> 8 & 0xFF);

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

int	trace(pid_t pid)
{
	int status;
	int exited = 0;

	if (ptrace(PTRACE_SEIZE, pid, 0, PTRACE_O_TRACESYSGOOD | PTRACE_O_EXITKILL | PTRACE_O_TRACEEXIT) == -1) {
		perror("ATTACH");
		exit(1);
	}

	int interrupt = ptrace(PTRACE_INTERRUPT, pid, NULL, NULL);
	pid = waitpid(pid, &status, 0);
	
	while(1)
	{
		int exit_state = EXIT_STATE_UNSET;
		struct user_regs_struct regs = get_next_syscall_regs(pid, &exit_state, &status);

		if (exit_state == PTRACE_EVENT_STATE) {
			handle_ptrace_event(pid, status);
			return 0;
		}

		if (IS_SIGNAL_STATE(status)) {
			siginfo_t info;

			if (ptrace(PTRACE_GETSIGINFO, pid, 0, &info) == -1) {
				perror("siginfo");			
				exit(1);
			}

			printf("%s", handle_signal(&info));

			if (regs.orig_rax == -1) {
				printf("+++ killed +++\n");
				exit(0);
			}
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

				printf("%s%c", str, i + 1 != 6 && SYSCALLS_x64[syscall].funcs[i + 1] != 0 ? ',' : ')');
				free(str);
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

		char *rax_res = exited ? strdup("?") : SYSCALLS_x64[syscall].rax_resolver((void *)regs.rax, pid, regs);

		printf(" = %s %p\n", rax_res, rax_res);

		free(rax_res);

	}
}

int main(int argc, char **argv)
{
	pid_t pid;

	if (argc < 2)
	{
		printf("ft_strace: must have PROG [ARGS]\n");
		return 1;
	}
	if (argc == 3 && strcmp("-p", argv[1]) == 0 && atoi(argv[2]) != 0)
	{
		pid = atoi(argv[2]);
		printf("%d\n", pid);
	} else {
		pid = fork();
		if (pid == -1) {
			printf("failed to open pipe");
			return 1;
		} else if (pid == 0)
		{
			execvp(argv[1], &argv[1]);
			printf("FATAL: execve error\n");
		} else if (pid == -1) {
			perror("fork");
			exit(1);
		}
	}
	return trace(pid);
}