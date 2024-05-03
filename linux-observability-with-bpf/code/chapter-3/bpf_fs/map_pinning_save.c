#include <errno.h>
#include <linux/bpf.h>
#include <stdio.h>
#include <string.h>
#include <bpf/bpf.h>
#include <unistd.h>

static const char *file_path = "/sys/fs/bpf/my_map";

int main(int argc, char **argv) {
  int key, value, fd, added, pinned;

  fd = bpf_create_map(BPF_MAP_TYPE_HASH, sizeof(int), sizeof(int), 100, 0);
  if (fd < 0) {
    printf("Failed to create map: %d (%s)\n", fd, strerror(errno));
    return -1;
  }

  key = 1, value = 1234;
  //BPF_ANY 0,存在则更新，不存在则创建
  //BPF_NOEXIST 1，不存在则闯将
  //BPF_EXIST 2，存在则更新
  added = bpf_map_update_elem(fd, &key, &value, BPF_ANY);
  if (added < 0) {
    printf("Failed to update map: %d (%s)\n", added, strerror(errno));
    return -1;
  }

  pinned = bpf_obj_pin(fd, file_path);
  if (pinned < 0) {
    printf("Failed to pin map to the file system: %d (%s)\n", pinned,
           strerror(errno));
    return -1;
  }
  close(fd);

  return 0;
}
