```
go build -o hello-bpf main.go
nm hello-bpf | grep main # nm用于列出目标文件中的符号
```
