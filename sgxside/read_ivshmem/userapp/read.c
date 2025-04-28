#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

#define IVSHMEM_BAR0_ADDRESS 0x387000000000UL
#define IVSHMEM_BAR0_SIZE (1 * 1024 * 1024)

volatile uint64_t *ivshmem_base = NULL;  // 64位指针

// 写入 ivshmem（64位）
void write_ivshmem64(int offset, uint64_t value) {
    if (offset < IVSHMEM_BAR0_SIZE) {
        *(ivshmem_base + (offset / sizeof(uint64_t))) = value;
        printf("Wrote 0x%lx to offset 0x%x\n", value, offset);
    } else {
        fprintf(stderr, "Offset out of bounds: 0x%x\n", offset);
    }
}

// 读取 ivshmem（64位）
uint64_t read_ivshmem64(int offset) {
    uint64_t value;

    if (offset < IVSHMEM_BAR0_SIZE) {
        value = *(ivshmem_base + (offset / sizeof(uint64_t)));
        printf("Read 0x%lx from offset 0x%x\n", value, offset);
        return value;
    } else {
        fprintf(stderr, "Offset out of bounds: 0x%x\n", offset);
        return 0;
    }
}

int main() {
    int fd;

    // 打开 /dev/mem
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open /dev/mem");
        return -1;
    }

    // mmap 物理地址
    ivshmem_base = mmap(NULL, IVSHMEM_BAR0_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, IVSHMEM_BAR0_ADDRESS);
    if (ivshmem_base == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    printf("ivshmem memory mapped at address: %p\n", ivshmem_base);

    // 测试读写
    // write_ivshmem64(0, 0x1122334455667788UL);  // 可以取消注释测试写
    uint64_t val = read_ivshmem64(0);   // 读取偏移0
    printf("Value read: 0x%lx\n", val);

    // 清理
    munmap((void *)ivshmem_base, IVSHMEM_BAR0_SIZE);
    close(fd);

    return 0;
}

