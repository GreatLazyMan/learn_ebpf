#KERNEL_SRCTREE=$1
#LIBBPF=${KERNEL_SRCTREE}/tools/lib/bpf/libbpf.a
#LIBBPF=/usr/lib/x86_64-linux-gnu/libbpf.a
#clang -o loader-bin -I${KERNEL_SRCTREE}/tools/lib/bpf/ \
#  -I${KERNEL_SRCTREE}/tools/lib -I${KERNEL_SRCTREE}/tools/include \
#  -I${KERNEL_SRCTREE}/tools/perf -I${KERNEL_SRCTREE}/samples \
#  loader.c "${LIBBPF}" -lelf
CFLAGS=$(grep -q "define HAVE_ATTR_TEST 1"  ../../../../linux/tools/perf/perf-sys.h \
                  && echo "-DHAVE_ATTR_TEST=0")


#clang  -o loader-bin  \
# -I/usr/src/linux-headers-$(uname -r)/samples/bpf \
# -I/usr/src/linux-headers-$(uname -r)/tools/lib  \
# -I/usr/src/linux-headers-$(uname -r)/tools/perf  \
# -I/usr/src/linux-headers-$(uname -r)/tools/include \
# -L/usr/local/lib64  -lelf loader.c

clang  -o loader-bin -lelf \
                           -L/usr/local/lib64 -lbpf loader.c
#                  -I/usr/src/linux-headers-$(uname -r)/samples/bpf \
#                  -I/usr/src/linux-headers-$(uname -r)/tools/lib  \
#                  -I/usr/src/linux-headers-$(uname -r)/tools/perf  \
#                  -I/usr/src/linux-headers-$(uname -r)/tools/include \
#                           -L/usr/local/lib64 -lbpf loader.c
