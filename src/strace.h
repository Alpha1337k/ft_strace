#ifndef FT_STRACE_H
#define FT_STRACE_H

#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>


typedef struct s_header
{
	char name[30];
	int (*funcs[6])();
} t_header;

int parse_unsigned_int_fd();
int parse_char_ptr_buf();
int parse_size_t();
int parse_char_ptr();
int parse_char_ptr_filename();
int parse_int_flags();
int parse_umode_t_mode();

#endif