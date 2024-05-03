
```
make
sudo ./bpf_fs/save 
bpftool map show 
bpftool map dump id XX # XX 根据show的情况填写
sudo ./fetch 
bpftool map dump id XX # XX 根据show的情况填写
rm -f /sys/fs/bpf/my_map
bpftool map show 
```
