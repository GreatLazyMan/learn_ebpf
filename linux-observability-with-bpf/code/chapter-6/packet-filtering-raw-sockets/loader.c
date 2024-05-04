#include "bpf_program.skeleton.h"
#include <linux/types.h>
#include <bpf/bpf.h>
//#include <bpf/libbpf.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include <argp.h>
#include <arpa/inet.h>
#include <bpf/libbpf.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <net/if.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <unistd.h>


static int open_raw_sock(const char *name)
{
	struct sockaddr_ll sll;
	int sock;

	sock = socket(PF_PACKET, SOCK_RAW | SOCK_NONBLOCK | SOCK_CLOEXEC, htons(ETH_P_ALL));
	if (sock < 0) {
		fprintf(stderr, "Failed to create raw socket\n");
		return -1;
	}

	memset(&sll, 0, sizeof(sll));
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = if_nametoindex(name);
	sll.sll_protocol = htons(ETH_P_ALL);
	if (bind(sock, (struct sockaddr *)&sll, sizeof(sll)) < 0) {
		fprintf(stderr, "Failed to bind to %s: %s\n", name, strerror(errno));
		close(sock);
		return -1;
	}

	return sock;
}

int main(int argc, char **argv) {
  int sock = -1, i, key;
  int tcp_cnt, udp_cnt, icmp_cnt;

  
  struct bpf_object_skeleton *skeleton = NULL;
  struct bpf_program *prog;
  struct bpf_link *link;
  int prog_fd;
  int err;
  struct bpf_object *obj;
  int ret;
  
  printf("#########1\n");
  libbpf_set_strict_mode(LIBBPF_STRICT_ALL);
  prog = bpf_program__open_and_load();
  if (!prog) {
      fprintf(stderr, "Failed to find BPF program\n");
      return 1;
  }
  printf("#########2\n");
  if (prog != NULL) {
    printf("########\n");
  }
  prog_fd = bpf_program__fd(prog->progs.socket_prog);
  if (prog_fd < 0) {
      fprintf(stderr, "Failed to get file descriptor for program\n");
      return 1;
  }
  sock = open_raw_sock("lo");


  /*
  //sock = open_raw_sock("lo");
  int sockfd;
  struct sockaddr_in sa;
  // 创建原始套接字
  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  if (sockfd < 0) {
      perror("无法创建套接字");
      return 1;
  }

  int optval = 1;
if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval)) < 0) {
    perror("setsockopt");
    close(sockfd);
    exit(EXIT_FAILURE);
}

  struct sockaddr_in local_addr;
local_addr.sin_family = AF_INET;
local_addr.sin_port = htons(0); // 0 表示由系统选择端口
local_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
    perror("bind");
    close(sockfd);
    exit(EXIT_FAILURE);
}

  //// 将套接字绑定到lo接口
  //memset(&sa, 0, sizeof(sa));
  //sa.sin_family = AF_INET;
  //sa.sin_addr.s_addr = inet_addr("127.0.0.1"); // lo接口的IP地址

  //if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
  //    perror("绑定失败");
  //    close(sockfd);
  //    return 1;
  //}
  */

  // 将 BPF 程序附加到 socket
  err = setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, &prog_fd, 
                   sizeof(prog_fd));
  if (err != 0){
    perror("setsockopt err:");
    return -1;
  }else {
    printf("setsockopt ok\n");
  }


  int map_fd;
  //// 加载 BTF 信息
  //err = bpf_object__load(prog->obj);
  //if (err) {
  //    fprintf(stderr, "Failed to load BPF object: %d\n", err);
  //    return 1;
  //}

  /*
  // 将 map pin 到文件系统
  err = bpf_object__pin_maps(prog->obj, "/sys/fs/bpf/");
  if (err) {
      fprintf(stderr, "Failed to pin maps: %d\n", err);
  }
  //map_fd = bpf_obj_get("/sys/fs/bpf/countmap"); // 获取map文件描述符
*/
  map_fd = bpf_map__fd(prog->maps.countmap);

  for (i = 0; i < 30; i++) {
    sleep(1);
    key = IPPROTO_TCP;
    //assert(bpf_map_lookup_elem(map_fd[0], &key, &tcp_cnt) == 0);
    //assert(bpf_map_lookup_elem(map_fd , &key, &tcp_cnt) == 0);
    bpf_map_lookup_elem(map_fd , &key, &tcp_cnt); 

    key = IPPROTO_UDP;
    //assert(bpf_map_lookup_elem(map_fd, &key, &udp_cnt) == 0);
    bpf_map_lookup_elem(map_fd, &key, &udp_cnt);

    key = IPPROTO_ICMP;
    //assert(bpf_map_lookup_elem(map_fd, &key, &icmp_cnt) == 0);
    bpf_map_lookup_elem(map_fd, &key, &icmp_cnt);

    printf("TCP %d UDP %d ICMP %d packets\n", tcp_cnt, udp_cnt, icmp_cnt);
  }
}
