# strace
Strace clone project (42)

string printer werkt niet voor execve
return values kloppen niet


-PTRACE_SYSCALL
	Restart the stopped tracee as for PTRACE_CONT, but arrange
    for the tracee to be stopped at the next entry to or exit
    from a system call, or after execution of a single
    instruction, respectively.  (The tracee will also, as
    usual, be stopped upon receipt of a signal.)  From the
    tracer's perspective, the tracee will appear to have been
    stopped by receipt of a SIGTRAP.  So, for PTRACE_SYSCALL,
    for example, the idea is to inspect the arguments to the
    system call at the first stop, then do another
    PTRACE_SYSCALL and inspect the return value of the system
    call at the second stop.  The data argument is treated as
    for PTRACE_CONT.  (addr is ignored.)
-PTRACE_GETREGSET
	Read the tracee's registers.  addr specifies, in an
	architecture-dependent way, the type of registers to be
	read.  NT_PRSTATUS (with numerical value 1) usually
	results in reading of general-purpose registers.  If the
	CPU has, for example, floating-point and/or vector
	registers, they can be retrieved by setting addr to the
	corresponding NT_foo constant.  data points to a struct
	iovec, which describes the destination buffer's location
	and length.  On return, the kernel modifies iov.len to
	indicate the actual number of bytes returned.
-PTRACE_SETOPTIONS
-PTRACE_GETSIGINFO
    Retrieve information about the signal that caused the
    stop.  Copy a siginfo_t structure (see sigaction(2)) from
    the tracee to the address data in the tracer.  (addr is
    ignored.)
-PTRACE_SEIZE
	Attach to the process specified in pid, making it a tracee
	of the calling process.  Unlike PTRACE_ATTACH,
	PTRACE_SEIZE does not stop the process.  Group-stops are
	reported as PTRACE_EVENT_STOP and WSTOPSIG(status) returns
	the stop signal.  Automatically attached children stop
	with PTRACE_EVENT_STOP and WSTOPSIG(status) returns
	SIGTRAP instead of having SIGSTOP signal delivered to
	them.  execve(2) does not deliver an extra SIGTRAP.  Only
	a PTRACE_SEIZEd process can accept PTRACE_INTERRUPT and
	PTRACE_LISTEN commands.  The "seized" behavior just
	described is inherited by children that are automatically
	attached using PTRACE_O_TRACEFORK, PTRACE_O_TRACEVFORK,
	and PTRACE_O_TRACECLONE.  addr must be zero.  data
	contains a bit mask of ptrace options to activate
	immediately.	
	Permission to perform a PTRACE_SEIZE is governed by a
	ptrace access mode PTRACE_MODE_ATTACH_REALCREDS check; see
	below.
-PTRACE_INTERRUPT
	Stop a tracee.  If the tracee is running or sleeping in
	kernel space and PTRACE_SYSCALL is in effect, the system
	call is interrupted and syscall-exit-stop is reported.
	(The interrupted system call is restarted when the tracee
	is restarted.)  If the tracee was already stopped by a
	signal and PTRACE_LISTEN was sent to it, the tracee stops
	with PTRACE_EVENT_STOP and WSTOPSIG(status) returns the
	stop signal.  If any other ptrace-stop is generated at the
	same time (for example, if a signal is sent to the
	tracee), this ptrace-stop happens.  If none of the above
	applies (for example, if the tracee is running in user
	space), it stops with PTRACE_EVENT_STOP with
	WSTOPSIG(status) == SIGTRAP.  PTRACE_INTERRUPT only works
	on tracees attached by PTRACE_SEIZE.
-PTRACE_LISTEN
	Restart the stopped tracee, but prevent it from executing.
	The resulting state of the tracee is similar to a process
	which has been stopped by a SIGSTOP (or other stopping
	signal).  See the "group-stop" subsection for additional
	information.  PTRACE_LISTEN works only on tracees attached
	by PTRACE_SEIZE.
