#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
//#include "vmlinux.h"
//#include <bpf/bpf_helpers.h>

//#define SEC(NAME) __attribute__((section(NAME), used))
//
//static int (*bpf_trace_printk)(const char *fmt, int fmt_size,
//                               ...) = (void *)BPF_FUNC_trace_printk;

SEC("kprobe/do_sys_openat2")
int bpf_prog(void *ctx) {
  bpf_printk("Hello, BPF World!!!");
  return 0;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";
