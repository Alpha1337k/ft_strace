#include "strace.h"

extern const t_header SYSCALLS_x64[1024];
extern const t_header SYSCALLS_x86[1024];

#define EXIT_STATE_UNSET 0xFFFF
#define IS_EXIT_STATE(status) (status & 0xFF) == status
#define PTRACE_EVENT_STATE 0x8000
#define PTRACE_EVENT(status) status >> 16
#define IS_SIGNAL_STATE(status) ((status >> 8) & 0x7f) != 5
#define NT_PRSTATUS 1

pid_t pid_ref = 0;

void	stop_tracing()
{
	if (pid_ref != 0) {
		dprintf(STDERR_FILENO, "ft_strace: Process %d detached\n<detached ...>\n\n", pid_ref);
		ptrace(PTRACE_DETACH, pid_ref, 0, SIGCONT);
	}

	exit(0);
}

t_header get_syscall_header(regs_t registers)
{
	if (registers.is64bit) {
		return SYSCALLS_x64[registers.syscall];
	}
	return SYSCALLS_x86[registers.syscall];
}

regs_t get_next_syscall_regs(pid_t pid, int *exit_state, int *status)
{
	struct i386_user_regs_struct i386_regs;
	struct user_regs_struct x64_regs;
	regs_t rv = {};

	if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1) {
		perror("syscall1");
		exit(1);
	}
	if (waitpid(pid, status, 0) == -1) {
		perror("waitpid");
		exit(1);
	}

	if (WIFEXITED(*status)) {
		exit_state[0] = 0xFF;
		return rv;
	}

	u_int8_t regs[144];
	struct iovec iov;

	iov.iov_base = &regs;
	iov.iov_len = sizeof(regs);

	if (ptrace(PTRACE_GETREGSET, pid, (void *)NT_PRSTATUS, &iov) == -1) {
		perror("getregset");
		exit(1);
	}

	if (iov.iov_len == 144) {
		memcpy(&x64_regs, regs, 144);
		rv.is64bit = 1;

		rv.rval = x64_regs.rax;
		rv.syscall = x64_regs.orig_rax;
		rv.args[0] = x64_regs.rdi;
		rv.args[1] = x64_regs.rsi;
		rv.args[2] = x64_regs.rdx;
		rv.args[3] = x64_regs.r10;
		rv.args[4] = x64_regs.r8;
		rv.args[5] = x64_regs.r9;
	} else if (iov.iov_len == 68) {
		memcpy(&i386_regs, regs, 68);
		rv.is64bit = 0;

		rv.rval = i386_regs.eax;
		rv.syscall = i386_regs.orig_eax;
		rv.args[0] = i386_regs.ebx;
		rv.args[1] = i386_regs.ecx;
		rv.args[2] = i386_regs.edx;
		rv.args[3] = i386_regs.esi;
		rv.args[4] = i386_regs.edi;
		rv.args[5] = i386_regs.ebp;
	} else
		assert(0);

	return rv;
}

int	trace(pid_t pid)
{
	int status;
	int exited = 0;
	int is_64bit = 1;

	unsigned int flags = PTRACE_O_TRACESYSGOOD;

	if (pid_ref == 0) {
		// Exit on kill if not in -p.
		flags |= PTRACE_O_EXITKILL;
	}

	if (ptrace(PTRACE_SEIZE, pid, 0, flags) == -1) {
		perror("ATTACH");
		exit(1);
	}

	int interrupt = ptrace(PTRACE_INTERRUPT, pid, NULL, NULL);
	if (interrupt == -1) {
		perror("PTRACE_INTERRUPT");
		exit(1);
	}

	pid = waitpid(pid, &status, 0);
	
	while(1)
	{
		int exit_state = EXIT_STATE_UNSET;
		regs_t regs = get_next_syscall_regs(pid, &exit_state, &status);

		if (is_64bit == 1 && regs.is64bit == 0) {
			dprintf(STDERR_FILENO, COLOR_GRAY"strace: [ Process PID=%d runs in 32 bit mode. ]\n"COLOR_RESET, pid);
			is_64bit = 0;
		} else if (is_64bit == 0 && regs.is64bit == 1) {
			dprintf(STDERR_FILENO, COLOR_GRAY"strace: [ Process PID=%d runs in 64 bit mode. ]\n"COLOR_RESET, pid);
			is_64bit = 1;
		}

		if (IS_SIGNAL_STATE(status)) {
			siginfo_t info;

			if (ptrace(PTRACE_GETSIGINFO, pid, 0, &info) == -1) {
				perror("siginfo");			
				exit(1);
			}

			dprintf(STDERR_FILENO, "%s", handle_signal(&info));

			if (regs.syscall == (unsigned int)-1) {
				dprintf(STDERR_FILENO, "+++ killed +++\n");
				exit(0);
			}
			continue;
		}

		if (IS_EXIT_STATE(exit_state)) {
			dprintf(STDERR_FILENO,"+++ exited with %u +++\n", WEXITSTATUS(status));
			return 0;
		}


		t_header header = get_syscall_header(regs);
		dprintf(STDERR_FILENO, COLOR_RED_BOLD"%s"COLOR_RESET"(", header.name);

		for (size_t i = 0; i < 6; i++)
		{
			if (header.funcs[i] != NULL) 
			{
				char *str = header.funcs[i]((void *)regs.args[i], pid, regs);

				dprintf(STDERR_FILENO, "%s%c", str, i + 1 != 6 && header.funcs[i + 1] != 0 ? ',' : ')');
				free(str);
			} else {
				break;
			}
		}

		regs = get_next_syscall_regs(pid, &exit_state, &status);

		if (IS_EXIT_STATE(exit_state)) {
			dprintf(STDERR_FILENO, " = ?\n+++ exited with %u +++\n", WEXITSTATUS(status));
			return 0;
		}

		char *rax_res = exited ? strdup("?") : header.rax_resolver((void *)regs.rval, pid, regs);

		dprintf(STDERR_FILENO, " = %s\n", rax_res);

		free(rax_res);

	}
}

int main(int argc, char **argv)
{
	pid_t pid;

	if (argc < 2)
	{
		dprintf(STDERR_FILENO, "ft_strace: must have PROG [ARGS]\n");
		return 1;
	}
	if (argc == 3 && strcmp("-p", argv[1]) == 0 && atoi(argv[2]) != 0)
	{
		pid = atoi(argv[2]);
		pid_ref = pid;

		signal(SIGINT, stop_tracing);
	} else {
		pid = fork();
		if (pid == -1) {
			dprintf(STDERR_FILENO, "failed to open pipe");
			return 1;
		} else if (pid == 0)
		{
			execvp(argv[1], &argv[1]);
			dprintf(STDERR_FILENO, "FATAL: execve error\n");
			exit(1);

		} else if (pid == -1) {
			perror("fork");
			exit(1);
		}
	}
	return trace(pid);
}