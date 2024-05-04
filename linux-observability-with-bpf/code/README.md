# 前置条件

操作系统版本：ubuntu22.04
内核版本：6.5.0-18-generic

```
apt install clang  linux-tools-$(uname -r) #libbpf-dev 
apt install gcc-multilib
apt install linux-headers-`uname -r`
apt install linux-tools-6.5.0-18-generic bpfcc-tools

sudo ln -s /usr/include/x86_64-linux-gnu/asm /usr/include/asm
sudo ln -s /usr/src/linux-headers-$(uname -r)/tools/bpf/resolve_btfids/libbpf/include/bpf /usr/include/bpf

#bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
#echo 1 > /sys/kernel/debug/tracing/events/syscalls/sys_enter_openat/enable
#cat /sys/kernel/debug/tracing/trace_pipe
```
