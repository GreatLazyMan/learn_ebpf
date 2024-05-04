```
#https://github.com/scylladb/seastar/issues/223
apt-get install systemtap-sdt-dev
gcc -o hello_usdt hello_usdt.c

readelf -n hello_usdt
tplist-bpfcc -l  ./hello_usdt

```
