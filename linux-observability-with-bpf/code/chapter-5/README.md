```
apt install jq graphviz
cat /proc/sys/net/core/bpf_jit_enable 
sudo bpftool prog show
sudo bpftool prog show --json id 31 | jq
sudo bpftool prog dump xlated id 31
sudo bpftool prog dump xlated id 7 visual &> output.out
dot -Tpng output.out -o visula-graph.png
bpftool map create /sys/fs/bpf/counter type array key 4 value 4 entries 5 name counter
bpftool map update id 27 key 3 0 0 0 value 1 0 0 0
bpftool map dump id 27
# 查看特定类型
bpftool perf show
bpftool net show
bpftool cgroup tree
bpftool cgroup show /sys/fs/cgroup/system.slice/systemd-udevd.service
# 批处理
cat > batch.txt <<\EOF
map create /sys/fs/bpf/counter type array key 4 value 4 entries 5 name counter
map update id 27 key 3 0 0 0 value 1 0 0 0
map dump id 27
EOF
bpftool batch batch.txt
# btf
bpftool btf dump id 7
```
