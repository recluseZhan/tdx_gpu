#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/pci.h>

#define IVSHMEM_BAR0_ADDRESS 0x383800000000
#define IVSHMEM_BAR0_SIZE (1 * 1024 * 1024)

void __iomem *ivshmem_base;  // 保存映射的基地址


// 写入 ivshmem 的函数
static void write_ivshmem(int offset, uint32_t value) {
    if (offset < IVSHMEM_BAR0_SIZE) {
        writel(value, ivshmem_base + offset);  // 写入指定偏移的值
        pr_info("Wrote value 0x%x to offset 0x%x\n", value, offset);
    } else {
        pr_err("Offset out of bounds: 0x%x\n", offset);
    }
}

// 读取 ivshmem 的函数
static uint32_t read_ivshmem(int offset) {
    uint32_t value;

    if (offset < IVSHMEM_BAR0_SIZE) {
        value = readl(ivshmem_base + offset);  // 从指定偏移读取值
        pr_info("Read value 0x%x from offset 0x%x\n", value, offset);
        return value;
    } else {
        pr_err("Offset out of bounds: 0x%x\n", offset);
        return 0;
    }
}
// 初始化函数：映射共享内存
static int __init ivshmem_init(void) {
    uint32_t read_value;

    pr_info("Initializing ivshmem module...\n");

    // 映射 ivshmem 的 BAR 0 内存区域
    ivshmem_base = ioremap(IVSHMEM_BAR0_ADDRESS, IVSHMEM_BAR0_SIZE);
    if (!ivshmem_base) {
        pr_err("Could not map ivshmem memory\n");
        return -EIO;
    }

    pr_info("ivshmem memory mapped at address: %p\n", ivshmem_base);

    write_ivshmem(0, 0x12345678);  // 将 0x12345678 写入偏移 0
    //read_value = read_ivshmem(0);  // 读取偏移 0 的数据
    //pr_info("Read value 0x%x from offset 0x0\n", read_value);

    return 0;
}

// 退出函数：解除映射
static void __exit ivshmem_exit(void) {
    if (ivshmem_base) {
        iounmap(ivshmem_base);
        pr_info("ivshmem memory unmapped\n");
    }

    pr_info("Exiting ivshmem module\n");
}


module_init(ivshmem_init);
module_exit(ivshmem_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("IVSHMEM Kernel Module for Reading and Writing to Shared Memory");

