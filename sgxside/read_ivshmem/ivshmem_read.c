#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/pci.h>

#define IVSHMEM_BAR0_ADDRESS 0x387000000000UL
#define IVSHMEM_BAR0_SIZE (1 * 1024 * 1024)

void __iomem *ivshmem_base;  // 保存映射的基地址

// 写入 ivshmem（64位）
static void write_ivshmem64(int offset, uint64_t value) {
    if (offset < IVSHMEM_BAR0_SIZE) {
        writeq(value, ivshmem_base + offset);  // 64位写
        pr_info("Wrote 64-bit value 0x%llx to offset 0x%x\n", value, offset);
    } else {
        pr_err("Offset out of bounds: 0x%x\n", offset);
    }
}

// 读取 ivshmem（64位）
static uint64_t read_ivshmem64(int offset) {
    uint64_t value;

    if (offset < IVSHMEM_BAR0_SIZE) {
        value = readq(ivshmem_base + offset);  // 64位读
        pr_info("Read 64-bit value 0x%llx from offset 0x%x\n", value, offset);
        return value;
    } else {
        pr_err("Offset out of bounds: 0x%x\n", offset);
        return 0;
    }
}

// 初始化函数：映射共享内存
static int __init ivshmem_init(void) {
    uint64_t read_value;

    pr_info("Initializing ivshmem module (64-bit version)...\n");

    // 映射 ivshmem 的 BAR0
    ivshmem_base = ioremap(IVSHMEM_BAR0_ADDRESS, IVSHMEM_BAR0_SIZE);
    if (!ivshmem_base) {
        pr_err("Could not map ivshmem memory\n");
        return -EIO;
    }
    pr_info("ivshmem memory mapped at address: %p\n", ivshmem_base);

    // 测试读取
    // write_ivshmem64(0, 0x1122334455667788ULL);  // 可选测试写入
    read_value = read_ivshmem64(0);  // 读取偏移0
    pr_info("Read 64-bit value 0x%llx from offset 0x0\n", read_value);

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
MODULE_DESCRIPTION("IVSHMEM Kernel Module for 64-bit Reading and Writing");

