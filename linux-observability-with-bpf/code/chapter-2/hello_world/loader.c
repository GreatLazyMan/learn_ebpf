#include <linux/types.h>
//#include <bpf/bpf_helpers.h>
#include <bpf/libbpf.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {

/*
 //在高版本的Linux内核中，load_bpf_file 函数已经不再使用，取而代之的是更现代的API，如 libbpf 库提供的函数。
  if (load_bpf_file("bpf_program.o") != 0) {
    printf("The kernel didn't load the BPF program\n");
    return -1;
  }
*/
  struct bpf_program *prog;
  struct bpf_link *link;
  int prog_fd;
  int err;
  struct bpf_object *obj;
  int ret;
  
  printf("333333333\n");
  obj = bpf_object__open("./bpf_program.o");
  printf("222222222\n");
  if (libbpf_get_error(obj))
      return libbpf_get_error(obj);
  
  ret = bpf_object__load(obj);
  if (ret)
      return ret;
  printf("111111111\n");

  // 找到你的 BPF 程序,前面load之后
  prog = bpf_object__find_program_by_name(obj, "bpf_prog");
  if (!prog) {
      fprintf(stderr, "Failed to find BPF program\n");
      return 1;
  }
  // 获取程序的文件描述符
  prog_fd = bpf_program__fd(prog);
  if (prog_fd < 0) {
      fprintf(stderr, "Failed to get BPF program FD\n");
      return 1;
  }
  // 附加 BPF 程序到 kprobe
  link = bpf_program__attach_kprobe(prog, false, "do_sys_openat2");
  if (libbpf_get_error(link)) {
      fprintf(stderr, "Failed to attach BPF program to kprobe\n");
      return 1;
  }
  int i = 10;
  while (i > 0) {
    i--;
    sleep(1);
  }
  /*
   
https://github.com/bpftools/linux-observability-with-bpf/issues/50
I saw the source code in /kernel-src/samples/bpf/bpf_load.c, but I didn't find the function read_trace_pipe. Then I found a commit samples, bpf: Move read_trace_pipe to trace_helpers, from kernel-5.7 the function read_trace_pipe has been moved to file /kernel-src/tools/testing/selftests/bpf/trace_helpers.c.

To solve this problem:

    add a header #include "trace_helpers.h" in loader.c;
    add BPFTEST = /kernel-src/tools/testing/selftests/bpf/trace_helpers.c in Makefile;
    then modify Makefile target bpfload into:
    //bpf_printk()：kernel 5.2+
    //对于 5.2 以下的内核，打印日志可以用 bpf_trace_printk()，它比 bpf_printk() 要麻烦一点

   * */
  //bpf_printk("tcp_v4_connect latency_us");
  //    // 清理资源
  bpf_link__destroy(link);
  bpf_object__close(obj);

  return 0;


  return 0;
}
