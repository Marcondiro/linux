// SPDX-License-Identifier: GPL-2.0

#include <linux/entry-common.h>

#define CREATE_TRACE_POINTS
#include <trace/events/syscalls.h>

/* Out of line to prevent tracepoint code duplication */

long trace_syscall_enter(struct pt_regs *regs, long syscall)
{
	struct pt_regs scratch = *regs;
	unsigned long args[6];

	trace_sys_enter(&scratch, syscall);
	/*
	 * Probes or BPF hooks in the tracepoint may have changed the
	 * system call number as well as the arguments. Write back
	 * the potentially modified arguments.
	 */
	syscall = syscall_get_nr(current, &scratch);
	syscall_set_nr(current, regs, syscall);
	if (syscall != -1) {
		syscall_get_arguments(current, &scratch, args);
		syscall_set_arguments(current, regs, args);
	}

	return syscall;
}

void trace_syscall_exit(struct pt_regs *regs, long ret)
{
	trace_sys_exit(regs, ret);
}
