#ifndef FT_STRACE_H
#define FT_STRACE_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <signal.h>


#ifndef COLORS_DISABLED
	#define COLOR_RESET "\033[0m"
	#define COLOR_CYAN "\033[36;1m"
	#define COLOR_YELLOW "\033[0;33m"
	#define COLOR_RED "\033[0;31m"
	#define COLOR_RED_BOLD "\033[31;1m"
	#define COLOR_GRAY "\033[38;5;248m"
#else
	#define COLOR_RESET 
	#define COLOR_CYAN
	#define COLOR_YELLOW
	#define COLOR_RED
	#define COLOR_RED_BOLD 
	#define COLOR_GRAY
#endif

#define _POSIX_C_SOURCE  200809L

typedef struct regs_s
{
	unsigned syscall;
	unsigned long rval;
	unsigned long args[6];
	u_int8_t is64bit;
} regs_t;

struct i386_user_regs_struct {
	u_int32_t ebx;
	u_int32_t ecx;
	u_int32_t edx;
	u_int32_t esi;
	u_int32_t edi;
	u_int32_t ebp;
	u_int32_t eax;
	u_int32_t xds;
	u_int32_t xes;
	u_int32_t xfs;
	u_int32_t xgs;
	u_int32_t orig_eax;
	u_int32_t eip;
	u_int32_t xcs;
	u_int32_t eflags;
	u_int32_t esp;
	u_int32_t xss;
};



typedef struct s_header
{
	char name[30];
	char *(*funcs[6])(void *, pid_t, regs_t);
	char *(*rax_resolver)(void *, pid_t, regs_t);
} t_header;

#define SIGTRAP	5

char	*parse_char_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_char_ptr_rsi(void *data, pid_t pid, regs_t regs);
char	*parse_char_ptr_rdx(void *data, pid_t pid, regs_t regs);
char	*parse_char_ptr_r10(void *data, pid_t pid, regs_t regs);

char	*handle_signal(siginfo_t *info);

char	*parse_struct_mq_attr_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_unsigned_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_sembuf_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_u64(void *data, pid_t pid, regs_t regs);
char	*parse_struct_siginfo_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_void_ptr___ptr(void *data, pid_t pid, regs_t regs);
char	*parse_loff_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_user_msghdr_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_uid_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct___kernel_itimerspec_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_umode_t(void *data, pid_t pid, regs_t regs);
char	*parse_int_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_unsigned(void *data, pid_t pid, regs_t regs);
char	*parse_char_ptr__ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct___kernel_timespec_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_gid_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_statfs_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_key_t(void *data, pid_t pid, regs_t regs);
char	*parse_struct_mmsghdr_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_pid_t(void *data, pid_t pid, regs_t regs);
char	*parse_struct_old_utsname_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_sched_param_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_ustat_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_cap_user_header_t(void *data, pid_t pid, regs_t regs);
char	*parse_u32(void *data, pid_t pid, regs_t regs);
char	*parse_unknown(void *data, pid_t pid, regs_t regs);
char	*parse_fd_set_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_long(void *data, pid_t pid, regs_t regs);
char	*parse_struct_rusage_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_int(void *data, pid_t pid, regs_t regs);
char	*parse_struct_robust_list_head_ptr(void *data, pid_t pid, regs_t regs);
char	*parse___s32(void *data, pid_t pid, regs_t regs);
char	*parse_struct_stat_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_sigset_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_timeval_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_cap_user_data_t(void *data, pid_t pid, regs_t regs);
char	*parse_struct_sockaddr_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_iocb_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_gid_t(void *data, pid_t pid, regs_t regs);
char	*parse_timer_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_sched_attr_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_msqid_ds_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_epoll_event_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_size_t(void *data, pid_t pid, regs_t regs);
char	*parse_struct_rlimit_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_sigaltstack_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_uid_t(void *data, pid_t pid, regs_t regs);
char	*parse_siginfo_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_linux_dirent64_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_unsigned_long_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_key_serial_t(void *data, pid_t pid, regs_t regs);
char	*parse_rwf_t(void *data, pid_t pid, regs_t regs);
char	*parse_u32_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_time_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_sysinfo_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_iovec_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_aio_context_t(void *data, pid_t pid, regs_t regs);
char	*parse_off_t(void *data, pid_t pid, regs_t regs);
char	*parse_struct_timezone_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_sigevent_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_off_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_timer_t(void *data, pid_t pid, regs_t regs);
char	*parse_struct_rlimit64_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_unsigned_long(void *data, pid_t pid, regs_t regs);
char	*parse_qid_t(void *data, pid_t pid, regs_t regs);
char	*parse_unsigned_int_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_perf_event_attr_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct___old_kernel_stat_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_void_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_io_event_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_shmid_ds_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_aio_context_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_mqd_t(void *data, pid_t pid, regs_t regs);
char	*parse_struct_msgbuf_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_iocb_ptr___ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_itimerval_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_unsigned_int(void *data, pid_t pid, regs_t regs);
char	*parse_struct___kernel_timex_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_file_handle_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_getcpu_cache_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_pollfd_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_clockid_t(void *data, pid_t pid, regs_t regs);
char	*parse_loff_t(void *data, pid_t pid, regs_t regs);
char	*parse_struct_sigaction_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_size_t_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_unsigned_char_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_robust_list_head_ptr___ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_utimbuf_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_union_bpf_attr_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_statx_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_kexec_segment_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_linux_dirent_ptr(void *data, pid_t pid, regs_t regs);
char	*parse_struct_tms_ptr(void *data, pid_t pid, regs_t regs);


char *ft_strjoin(char *s1, char *s2);

#endif