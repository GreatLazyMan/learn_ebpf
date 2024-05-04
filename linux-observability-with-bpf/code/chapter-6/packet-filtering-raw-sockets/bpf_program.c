#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/string.h>
#include <linux/tcp.h>
#include <linux/types.h>
#include <linux/udp.h>
#include <stdio.h>

/*
struct bpf_map_def {
  unsigned int type;
  unsigned int key_size;
  unsigned int value_size;
  unsigned int max_entries;
  unsigned int map_flags;
};
*/

unsigned long long load_byte(void *skb,
                             unsigned long long off) asm("llvm.bpf.load.byte");

/*
// https://github.com/libbpf/libbpf/wiki/Libbpf:-the-road-to-v1.0#drop-support-for-legacy-bpf-map-declaration-syntax
// https://github.com/eunomia-bpf/bpf-developer-tutorial/issues/75
struct bpf_map_def SEC("maps") countmap = {
    .type = BPF_MAP_TYPE_ARRAY,
    .key_size = sizeof(int),
    .value_size = sizeof(int),
    .max_entries = 256,
};
*/
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 4);
    __type(key, int);
    __type(value, int);
} countmap SEC(".maps");


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 255);
    __type(key, int);
    __type(value, __u32);
} countmap_ip SEC(".maps");

SEC("socket")
int socket_prog(struct __sk_buff *skb) {
  //bpf_printk("%d,%d",skb->remote_ip4,skb->local_ip4);
  //bpf_trace_printk("PROTO %xn",skb->remote_ip4);
  int proto = load_byte(skb, ETH_HLEN + offsetof(struct iphdr, protocol));
  int one = 1;
  
  //char str[22];
  // __u64 data_values[] = {(__u64)skb->local_ip4,(__u64)skb->remote_ip4};
  //bpf_snprintf(str, sizeof(str), "Message: %s, Data: %llu %llu",data_values,2);
  
  int *el = bpf_map_lookup_elem(&countmap, &proto);
  if (el) {
    (*el)++;
  } else {
    el = &one;
  }

  bpf_map_update_elem(&countmap, &proto, el, BPF_ANY);
  return 0;
}
char LICENSE[] SEC("license") = "Dual BSD/GPL";
