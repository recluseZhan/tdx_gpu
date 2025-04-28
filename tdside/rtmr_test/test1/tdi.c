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
    //printk("pa0=0x%lx\n",pa);
    unsigned long ret=2;
    unsigned long opcode = 1;
    unsigned long size = 4096;
    asm volatile(
        "movq %1,%%rax\n\t"
        //"movq %2,%%rcx\n\t"
        "tdcall\n\t"
        "movq %%rcx,%0\n\t"
        :"=r"(ret):"r"(opcode):
    );
    printk("GPAW=%d\n",ret);
    //printk("ret=0x%lx\n",ret);
    printk("pa=0x%lx\n",pa);
    
    opcode = 23;
    asm volatile(
        "movq %1,%%rax\n\t"
        "movq %2,%%rcx\n\t"
        "tdcall\n\t"
        "movq %%rcx,%0\n\t"
        :"=r"(ret):"r"(opcode),"r"(pa):	
    );
    printk("GPA_MAPPING=0x%llx\n",ret);

    printk(KERN_INFO "loading tdi\n");
    return 0;
}
static void __exit tdi_exit(void){
    printk(KERN_INFO "unloading tdi\n");
}
module_init(tdi_init);
module_exit(tdi_exit);
