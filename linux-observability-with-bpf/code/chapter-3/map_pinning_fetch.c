#include <errno.h>
#include <linux/bpf.h>
#include <stdio.h>
#include <string.h>
#include <bpf/bpf.h>

static const char *file_path = "/sys/fs/bpf/my_map";

int main(int argc, char **argv) {
  int fd, key, value, result;

  fd = bpf_obj_get(file_path);
  if (fd < 0) {
    printf("Failed to fetch the map: %d (%s)\n", fd, strerror(errno));
    return -1;
  }

  key = 1;

  result = bpf_map_lookup_elem(fd, &key, &value);
  if (result < 0) {
    printf("Failed to read value from the map: %d (%s)\n", result,
           strerror(errno));
    return -1;
  }

  printf("Value read from the map: '%d'\n", value);
  int i = 0;
  for (i = 2; i < 6; i++) {
    int new_value = 1234 + i;
    bpf_map_update_elem(fd,&i,&new_value,BPF_NOEXIST);
  }
  
  int lookup_key = -1;
  int next_key;
  while(bpf_map_get_next_key(fd,&lookup_key,&next_key)==0){
    //请注意，使用 BPF_F_LOCK 标志时，你需要确保映射元素确实包含自旋锁，
    //result = bpf_map_lookup_elem_flags(fd, &next_key, &value, BPF_F_LOCK);
    result = bpf_map_lookup_elem(fd, &next_key, &value);
    if (result < 0) {
      printf("Failed to read value from the map: %d (%s)\n", result,
             strerror(errno));
      return -1;
    }
    printf("the next key is: %d,value: %d\n", next_key,value);
    lookup_key = next_key;
  }

  /*
  //在BPF中，BPF_MAP_TYPE_ARRAY类型的map不支持单独删除元素的操作。这是因为BPF_MAP_TYPE_ARRAY是一个固定大小的数组，其中的每个元素都是预先分配的，所以不能像哈希表那样单独删除。
  result = bpf_map_delete_elem(fd, &key);
  if (result != 0) {
    printf("Failed to delete value from the map: %d (%s)\n", result,
           strerror(errno));
    return -1;
  }
  */
  return 0;
}
