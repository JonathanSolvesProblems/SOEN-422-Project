#include "../Task3/hal_kernel.h"
#include "../Task1/bsl_Uart.h"

#define KERNEL_SIZE (100)

int main()
{
    bsl_Uart_Init();
    int16_t memory[] = {
    443, 1, 1, 0, 0, 406, 84, 474, 406, 101, 474, 406, 115, 474, 406, 116, 474, 406, 32,
    474, 406, 102, 474, 406, 48, 474, 476, 406, 48, 474, 476, 406, 0, 473, 476, 415, 412, -1
    };

    Kernel kernel = createKernel(KERNEL_SIZE);
    load(kernel, memory);
    run(kernel);

    return 0;
}