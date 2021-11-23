#include "hal_kernel.h"
#include <string.h>
#include "../Task1/bsl_Uart.h"

#define KERNEL_SIZE 100

static void usage()
{
    // TODO, CHANGE WITH PUT FUNCTIONS
    printf("Small Edison VM for Arduino Nano v1.0\n");
    printf("Usage: se <file>.pic\n");
    exit(1);
}

#if defined(Host)
int main(int argc, char **args)
{
    
    if (argc != 2)
        usage();
    
    char filename[32];
    strcpy(filename, args[1]); // Save name and extension.
                               //tt    printf("Filename: '%s'\n", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("'%s' does not exist.\n", filename);
        return -1;
    }

    Kernel kernel = createKernel(KERNEL_SIZE);
    load(kernel, file);
    run(kernel);

    return 0;
}
#else
void test(int16_t input[]) {
    for(int i = 0; i < 40; i++){
        if (input[i] == -1) 
             break;
        // PutC(code);
        PutX16(input[i]);
        PutN();
    }
}
int main()
{
    bsl_Uart_Init();
    int16_t memory[] = {
    443, 1, 1, 0, 0, 406, 84, 474, 406, 101, 474, 406, 115, 474, 406, 116, 474, 406, 32,
    474, 406, 102, 474, 406, 48, 474, 476, 406, 48, 474, 476, 406, 0, 473, 476, 415, 412, -1
    };
    int16_t memory2[] = {
    0x01BB, 0x0001, 0x0001, 0x0000, 0x0000, 0x0196, 0x0054, 0x01DA, 0x0196, 0x0065, 0x01DA, 0x0196, 0x0073, 0x01DA, 0x0196, 
    0x0074, 0x01DA, 0x0196, 0x0020,
    0x01DA, 0x0196, 0x0066, 0x01DA, 0x0196, 0x0030, 0x01DA, 0x01DC, 0x0196, 0x0030, 0x01DA, 0x01DC, 0x0196, 
    0x0000, 0x01D9, 0x01DC, 0x019F, 0x019C, 0xFFFF
    };

    // int16_t memory2[] = {
    // 1, 2, 3, 4, 5, 6, 7, 300, -1, 
    // };

    

    Kernel kernel = createKernel(KERNEL_SIZE);
    load(kernel, memory2);
    run(kernel);

    return 0;
}
#endif
