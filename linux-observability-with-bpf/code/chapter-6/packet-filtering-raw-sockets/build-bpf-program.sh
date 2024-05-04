clang -O2 -g -target bpf -c bpf_program.c -o bpf_program.o # 这里的 -g 选项是必须的，因为它会生成 BTF 信息。

sudo bpftool gen skeleton bpf_program.o  | tee bpf_program.skeleton.h
sed -i 's/bpf_program__load/bpf_program__load2/g' bpf_program.skeleton.h 
sed -i 's/bpf_program__attach/bpf_program__attach2/g' bpf_program.skeleton.h 
