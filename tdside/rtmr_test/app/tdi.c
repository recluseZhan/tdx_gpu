#include <stdio.h>

void main(){
    unsigned long pa = 0x100000;
    unsigned long ret=2;
    asm volatile(
        "mov $2,%%rax\n\t"
        "mov %1,%%rcx\n\t"
        "mov $2,%%rdx\n\t"
        "tdcall\n\t"
        "mov %%rax,%0\n\t"
        :"=r"(ret):"r"(pa):
    );
    printf("ret=%lx\n",ret);

    unsigned long report_pa = 0x100000;
    asm volatile(
        "mov $4,%%rax\n\t"
	"mov %1,%%rcx\n\t"
	"mov %2,%%rdx\n\t"
	"mov $0,%%r8\n\t"
	"tdcall\n\t"
	"mov %%rax,%0\n\t"
	:"=r"(ret):"r"(report_pa),"r"(pa):
    );
    printf("ret2=%lx\n",ret);
}
