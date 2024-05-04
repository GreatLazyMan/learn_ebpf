#include <uapi/linux/ptrace.h>

struct syscall_enter_args {
	unsigned long long common_tp_fields;
	long		   syscall_nr;
	unsigned long	   args[6];
};

//int trace_bpf_prog_load(void ctx) {
int trace_bpf_prog_load(struct syscall_enter_args *args)
  char comm[16];
  bpf_get_current_comm(&comm, sizeof(comm));

  bpf_trace_printk("%s is loading a BPF program", comm);
  return 0;
}
