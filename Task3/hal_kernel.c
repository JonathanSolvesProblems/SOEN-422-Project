#include "bsl_vm.h"
#include "hal_kernel.h"
#include "hal_task.h"

#define INVALID -1
#define ADD 0
#define ALSO 1
#define AND 2
#define ASSIGN 3
#define BLANK 4
#define COBEGIN 5
#define CONSTANT 6
#define CONSTRUCT 7
//  #define DIFFERENCE 8
#define DIVIDE 9
#define DO 10
#define ELSE 11
#define ENDCODE 12
#define ENDIF 13
#define ENDLIB 14
#define ENDPROC 15
#define ENDWHEN 16
#define EQUAL 17
#define ERROR 18
#define FIELD 19
//  #define FUNCVAL 20
#define GOTO 21
#define GREATER 22
//  #define IN 23
#define INDEX 24
#define INSTANCE 25
//  #define INTERSECTION 26
#define LESS 27
#define LIBPROC 28
#define MINUS 29
#define MODULO 30
#define MULTIPLY 31
#define NEWLINE 32
#define NOT 33
#define NOTEQUAL 34
#define NOTGREATER 35
#define NOTLESS 36
#define OR 37
#define PARAMARG 38
#define PARAMCALL 39
#define PARAMETER 40
#define PROCARG 41
#define PROCCALL 42
#define PROCEDURE 43
#define PROCESS 44
#define SUBTRACT 45
//  #define UNION 46
#define VALSPACE 47
#define VALUE 48
#define VARIABLE 49
#define WAIT 50
#define WHEN 51
#define WHILE 52
#define ADDR 53
#define HALT 54
#define OBTAIN 55
#define PLACE 56
#define SENSE 57
/** Extra */
#define ELEMASSIGN 58
#define ELEMVALUE 59
#define LOCALCASE 60
#define LOCALSET 61
#define LOCALVALUE 62
#define LOCALVAR 63
#define OUTERCALL 64
#define OUTERCASE 65
#define OUTERPARAM 66
#define OUTERSET 67
#define OUTERVALUE 68
#define OUTERVAR 69
#define SETCONST 70
#define SINGLETON 71
#define STRINGCONST 72

#define PUTI 73
#define PUTC 74
#define PUTB 75
#define PUTN 76

#define MAX_KERNEL_STACK_SIZE 100
#define MAX_QUEUE 10
#define MIN_ADDRESS 0
#define MAX_ADDRESS 20000
#define SPACE (int)(' ')
#define int INSTR_TABLE 400
#define SET_LENGTH 0x8
#define SET_LIMIT 127

typedef struct KernelDesc {
    
    // temporary fields for stack evaluation
    uint8_t v1, v2;

    // kernel
    // const int MAX_KERNEL_STACK_SIZE = 100;
    uint8_t *itsKernelStack; // stack
    uint8_t itsKernelSP;     //  sp

    // variable stack
    uint8_t bp; //  base pointer
    uint8_t sp; //  stack pointer

    // program
    uint8_t ip; //  instruction pointer
    uint8_t pe; //  program end

    // task
    // const int MAX_QUEUE = 10;

    Task **taskQueue;  // Task taskQueue[];
    uint8_t taskCurrent;   // this
    uint8_t numberOfTasks; // tasks

    uint8_t taskStackTop; // stackTop
    uint8_t taskProgTop;  // progTop

    // const int MIN_ADDRESS = 0;
    // const int MAX_ADDRESS = 20000;
    // const int SPACE = (int)(' ');
    // const int INSTR_TABLE = 400;
    // const int SET_LENGTH = 0x8;
    // const int SET_LIMIT = 127;

    uint8_t *memory;
    uint8_t lineNo;
} KernelDesc;


Kernel Kernel_New(uint8_t size) { // TODO: Init to clean up.
    Kernel kInstance = (Kernel)malloc(sizeof(KernelDesc) * size);
    kInstance->v1 = kInstance->v2 = 0;
    kInstance->ip = 0;
    kInstance->taskStackTop = 0;
    kInstance->taskProgTop = 0;

    kInstance->taskCurrent = 0;
    kInstance->numberOfTasks = 1;
    kInstance->taskQueue = Task_New(MAX_QUEUE); // new Task *[MAX_QUEUE];
    for (uint8_t i = 0; i < MAX_QUEUE; i++)
        kInstance->taskQueue[i] = Task_New(MAX_QUEUE);
        // taskQueue[i] = new Task();

    kInstance->itsKernelStack = new int[MAX_KERNEL_STACK_SIZE];
    kInstance->itsKernelSP = 0;

    kInstance->memory = new int[MAX_ADDRESS];
    for (int i = 0; i < MAX_ADDRESS; i++) // Reset all memory locations.
        kInstance->memory[i] = 0;

    kInstance->pe = 1024;
    kInstance->bp = 0;
    kInstance->sp = 4;
    kInstance->memory[kInstance->sp] = 0; // no return
    kInstance->lineNo = 0;

    return kInstance;
}