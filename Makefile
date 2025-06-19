
all: heap addrspace arch32 arch64 mem-layout memaddr memLayout virt2phys

heap:	heap.c
	cc -o heap heap.c

addrspace:	addrspace.c
	cc -o addrspace addrspace.c

arch32:	arch32-64.c
	cc -m32 -o arch32 arch32-64.c

arch64:	arch32-64.c
	cc -m64 -o arch64 arch32-64.c

mem-layout:	mem-layout.c
	cc -o mem-layout mem-layout.c

memaddr:	memaddr.c
	cc -o memaddr memaddr.c

memLayout:	memLayout.c
	cc -o memLayout  memLayout.c

virt2phys:	virt2phys.c
	cc -o virt2phys virt2phys.c
