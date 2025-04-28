#include<linux/init.h>
#include<linux/module.h>
#include<linux/string.h>
#include<linux/kernel.h>
#include<linux/export.h>
#include<linux/scatterlist.h>
#include<linux/crypto.h>
#include <crypto/sha256_base.h>
#include <linux/err.h>
#include<crypto/skcipher.h>
#include<asm/desc.h>
#include<linux/interrupt.h>
#include<asm/irq_vectors.h>
#include<asm/io.h>


#include <linux/gfp.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <crypto/akcipher.h>
#include <linux/random.h>
#include <linux/delay.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

static int __init tdi_init(void){
    unsigned long *va = kmalloc(4096,GFP_KERNEL);
    unsigned long pa = virt_to_phys(va);
    memset(va,2,4096);
    unsigned long ret=2;
    asm volatile(
        "mov $2,%%rax\n\t"
        "mov %1,%%rcx\n\t"
        "mov $2,%%rdx\n\t"
        "tdcall\n\t"
        "mov %%rax,%0\n\t"
        :"=r"(ret):"r"(pa):
    );
    printk("ret=%lx\n",ret);

    unsigned long *report_va = kmalloc(4096,GFP_KERNEL);
    unsigned long report_pa = virt_to_phys(report_va);
    memset(report_va,0,4096);
    asm volatile(
        "mov $4,%%rax\n\t"
	"mov %1,%%rcx\n\t"
	"mov %2,%%rdx\n\t"
	"mov $0,%%r8\n\t"
	"tdcall\n\t"
	"mov %%rax,%0\n\t"
	:"=r"(ret):"r"(report_pa),"r"(pa):
    );
    printk("ret2=%lx\n",ret);
    printk("report[]=%lx\n",report_va[0]);
    printk(KERN_INFO "loading tdi\n");
    return 0;
}
static void __exit tdi_exit(void){
    printk(KERN_INFO "unloading tdi\n");
}
module_init(tdi_init);
module_exit(tdi_exit);
