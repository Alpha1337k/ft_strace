#include "strace.h"


char *handle_signal(siginfo_t *info)
{
	char *rv = calloc(1, 1024);
	if (!rv) return 0;

	switch (info->si_signo)
	{
	case SIGHUP:
		snprintf(rv, 1024, "--- SIGHUP {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGINT:
		snprintf(rv, 1024, "--- SIGINT {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGQUIT:
		snprintf(rv, 1024, "--- SIGQUIT {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGILL:
		snprintf(rv, 1024, "--- SIGILL {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGTRAP:
		snprintf(rv, 1024, "--- SIGTRAP {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGABRT:
		snprintf(rv, 1024, "--- SIGABRT {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGBUS:
		snprintf(rv, 1024, "--- SIGBUS {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGFPE:
		snprintf(rv, 1024, "--- SIGFPE {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGKILL:
		snprintf(rv, 1024, "--- SIGKILL {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGUSR1:
		snprintf(rv, 1024, "--- SIGUSR1 {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGSEGV:
		snprintf(rv, 1024, "--- SIGSEGV {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGUSR2:
		snprintf(rv, 1024, "--- SIGUSR2 {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGPIPE:
		snprintf(rv, 1024, "--- SIGPIPE {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGALRM:
		snprintf(rv, 1024, "--- SIGALRM {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGTERM:
		snprintf(rv, 1024, "--- SIGTERM {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGCHLD:
		snprintf(rv, 1024, "--- SIGCHLD {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGCONT:
		snprintf(rv, 1024, "--- SIGCONT {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGSTOP:
		snprintf(rv, 1024, "--- SIGSTOP {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGTSTP:
		snprintf(rv, 1024, "--- SIGTSTP {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGTTIN:
		snprintf(rv, 1024, "--- SIGTTIN {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGTTOU:
		snprintf(rv, 1024, "--- SIGTTOU {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGURG:
		snprintf(rv, 1024, "--- SIGURG {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGXCPU:
		snprintf(rv, 1024, "--- SIGXCPU {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGXFSZ:
		snprintf(rv, 1024, "--- SIGXFSZ {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGVTALRM:
		snprintf(rv, 1024, "--- SIGVTALRM {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGPROF:
		snprintf(rv, 1024, "--- SIGPROF {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGWINCH:
		snprintf(rv, 1024, "--- SIGWINCH {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGIO:
		snprintf(rv, 1024, "--- SIGIO {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGPWR:
		snprintf(rv, 1024, "--- SIGPWR {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;
	case SIGSYS:
		snprintf(rv, 1024, "--- SIGSYS {si_signo=%d, si_code=%d, si_errno=%d} ---\n", info->si_signo, info->si_code, info->si_errno);
		break;

	default:
		snprintf(rv, 1024, "--- SIGNAL_%d ??? ---\n", info->si_signo);
		break;
	}

	return rv;
}