#include "hal_kernel.h"
#include "hal_task.h"
#include <stdbool.h>
#include "../Task1/COutForAUnit.h"
#include <stdlib.h>
#include "private_kernel_variables.h"

typedef struct KernelDesc {
    
    // temporary fields for stack evaluation
    uint8_t v1, v2;

    // kernel
    // const int MAX_KERNEL_STACK_SIZE = 100;
    uint8_t *itsKernelStack; // stack
    uint8_t itsKernelSP;     //  sp

    // variable stack
    uint16_t bp; //  base pointer
    uint16_t sp; //  stack pointer

    // program
    uint16_t ip; //  instruction pointer
    uint16_t pe; //  program end

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

    uint16_t *memory;
    uint8_t lineNo;
} KernelDesc;

// PRIVATE

void runError(const char *msg, Kernel kInstance)
{
    PutS(msg);
    PutN();
    exit(1);
}

void preempt(Kernel kInstance)
{
    SetBp(*kInstance->taskQueue[kInstance->taskCurrent], kInstance->bp);
    SetSp(*kInstance->taskQueue[kInstance->taskCurrent], kInstance->sp);
    SetPe(*kInstance->taskQueue[kInstance->taskCurrent], kInstance->pe);
    SetIp(*kInstance->taskQueue[kInstance->taskCurrent], kInstance->ip);
}

void resume(Kernel kInstance)
{
    kInstance->bp = GetBp(*kInstance->taskQueue[kInstance->taskCurrent]);
    kInstance->sp = GetSp(*kInstance->taskQueue[kInstance->taskCurrent]);
    kInstance->pe = GetPe(*kInstance->taskQueue[kInstance->taskCurrent]);
    kInstance->ip = GetIp(*kInstance->taskQueue[kInstance->taskCurrent]);
    //t     printf("Resume: ip = %u\n", ip);
}

/** goto: op displacement */
void Goto(Kernel kInstance)
{
    //t     printf("Goto\n");
    int8_t displacement = kInstance->memory[kInstance->ip];
    kInstance->ip = kInstance->ip + displacement - 1;
}

/** proc: op   paramLength, varLength, tempLength, lineNo */
void Procedure(Kernel kInstance)
{
    
    uint8_t paramLength = kInstance->memory[kInstance->ip];
    uint8_t varLength = kInstance->memory[kInstance->ip + 1];
    uint8_t tempLength = kInstance->memory[kInstance->ip + 2];
    kInstance->lineNo = kInstance->memory[kInstance->ip + 3];

    //t     printf("Procedure: sp = %u\n", sp);
    kInstance->memory[kInstance->bp + 2] = kInstance->bp - paramLength - 1;
    kInstance->sp = kInstance->sp + varLength;
    if ((kInstance->sp + tempLength) > kInstance->pe)
    {
        runError("Stack overflows: sp", kInstance);
    }
    kInstance->ip = kInstance->ip + 4;
}

void EndProc(Kernel kInstance)
{
    //tt        System.out.println("EndProc");
    //tt        printStack();

    if (kInstance->memory[kInstance->bp + 4] != 0)
    {
        kInstance->ip = kInstance->memory[kInstance->bp + 4];
        kInstance->pe = kInstance->memory[kInstance->bp + 3];
        kInstance->sp = kInstance->memory[kInstance->bp + 2];
        kInstance->bp = kInstance->memory[kInstance->bp + 1];
    }
    //t        printStack();
}
/** instance: op  steps
    *
    *           Variable stack (before)
    *           ----------------------
    *   A       |   X                | <-- sp
    *           ----------------------
    *   A+1     |                    |
    *           ----------------------
    *
    *           Variable stack (after)
    *           ----------------------
    *   A       |   X                |
    *           ----------------------
    *   A+1     |   link             | <-- sp
    *           ----------------------
    */
void Instance(Kernel kInstance)
{
    int8_t steps = kInstance->memory[kInstance->ip];
    int8_t link = kInstance->bp;
    for (int i = steps; i > 0; i--)
    {
        link = kInstance->memory[link];
    }
    kInstance->sp = kInstance->sp + 1;
    kInstance->memory[kInstance->sp] = link;
    kInstance->ip = kInstance->ip + 1;
}
/** var: op Displacement
    *
    *           Variable stack (before)
    *           ---------------------
    *   A       | Link              | <-- sp
    *           ---------------------
    *   A+1     |                   |
    *           ---------------------
    *
    *           Variable stack (after)
    *           ---------------------
    *   A       | Link + disp       | <-- sp
    *           ---------------------
    *   A+1     |                   |
    *           ---------------------
    */
void Variable(Kernel kInstance)
{
    int8_t displ = kInstance->memory[kInstance->ip];
    kInstance->memory[kInstance->sp] = kInstance->memory[kInstance->sp] + displ;
    kInstance->ip = kInstance->ip + 1;
}

/** construct: op  number, lineNo
    *
    *           Variable stack (before)
    *           ---------------------
    *   A       |   X0              | <-- sp
    *           ---------------------
    *   A+1     |   X1              |
    *           ---------------------
    *                ..........
    *           ---------------------
    *   A+n     |   Xn              |
    *           ---------------------
    *
    *           Variable stack (after)
    *           ---------------------
    *   A       |   S0              |
    *           ---------------------
    *   A+1     |   S2              |
    *           ---------------------
    *                ..........
    *           ---------------------
    *   A+m     |   Sm              | <-- sp
    *           ---------------------
    *             n = number - 1
    *             m = SET_LENGTH - 1
    */
void Construct(Kernel kInstance)
{
    int8_t number = kInstance->memory[kInstance->ip];
    int8_t elem, index, i, val;
    int8_t *tempSet = (int8_t*)malloc(sizeof(int8_t) * SET_LENGTH);

    //t     printf("Set number = %d\n", number);
    for (i = 0; i < SET_LENGTH; i++)
        tempSet[i] = 0;

    for (i = 0; i < number; i++)
    {
        elem = kInstance->memory[kInstance->sp - i];
        if ((elem >= 0) && (elem <= SET_LIMIT))
        {
            index = elem & (SET_LENGTH - 1);
            val = elem >> 3;

            tempSet[index] |= (1 << val);
        }
        else
            // printf("Error: element has the value outside set range\n");
            PutS("Error: element has the value outside set range\n");
    }
    kInstance->sp = kInstance->sp - number + 1;
    for (i = 0; i < SET_LENGTH; i++)
    {
        kInstance->memory[kInstance->sp + i] = tempSet[i];
    }
    kInstance->sp = kInstance->sp + SET_LENGTH - 1;
    kInstance->ip = kInstance->ip + 2;
}

/** constant: op value
    *
    *           Variable stack (before)
    *           ---------------------
    *   A       | X                 | <-- sp
    *           ---------------------
    *   A+1     |                   |
    *           ---------------------
    *
    *           Variable stack (after)
    *           ---------------------
    *   A       | X                 |
    *           ---------------------
    *   A+1     | value             | <-- sp
    *           ---------------------
    */
void Constant(Kernel kInstance)
{
    int value = kInstance->memory[kInstance->ip];
    kInstance->sp = kInstance->sp + 1;
    kInstance->memory[kInstance->sp] = value;
    kInstance->ip = kInstance->ip + 1;
}

/** value: op length
    *
    *           Variable stack (before)
    *           ---------------------
    *   A       | link + displ      | <-- sp
    *           ---------------------
    *   A+1     |                   |
    *           ---------------------
    *
    *           Variable stack (after)
    *           ---------------------
    *   A       | value             |
    *           ---------------------
    *   A+1     | value1            |
    *           ---------------------
    *               ............
    *           ---------------------
    *   A+n     | valuen            | <-- sp
    *           ---------------------
    *             n = length-1
    */
void Value(Kernel kInstance)
{
    int length = kInstance->memory[kInstance->ip];
    int varAdd = kInstance->memory[kInstance->sp];

    for (int i = 0; i < length; i++)
    {
        kInstance->memory[kInstance->sp + i] = kInstance->memory[varAdd + i];
    }
    kInstance->sp = kInstance->sp + length - 1;
    kInstance->ip = kInstance->ip + 1;
}

/** valSpace: op length
    *
    *           Variable stack (before)
    *           ---------------------
    *   A       | X                 | <-- sp
    *           ---------------------
    *   A+1     |                               |
    *           ---------------------
    *
    *           Variable stack (after)
    *           ---------------------
    *   A       | X                 |
    *           ---------------------
    *   A+1     | ?                 |
    *           ---------------------
    *                ............
    *           ---------------------
    *   A+n     | ?                 | <-- sp
    *           ---------------------
    *               n = length
    */
void ValSpace(Kernel kInstance)
{
    int length = kInstance->memory[kInstance->ip];
    kInstance->sp = kInstance->sp + length;
    kInstance->ip = kInstance->ip + 1;
}

// Not: r = ~v    before: [v, ...    after:  [r, ...
void Not(Kernel kInstance)
{
    kInstance->memory[kInstance->sp] = ~kInstance->memory[kInstance->sp];
}

// Mul: r = v1 * v2    before: [v1, v2, ...    after:  [r, ...
void Mul(Kernel kInstance)
{
    kInstance->v2 = kInstance-> memory[kInstance->sp--];
    kInstance->memory[kInstance->sp] = kInstance->memory[kInstance->sp] * kInstance->v2;

    kInstance->lineNo = kInstance->memory[kInstance->ip++];
}
// Div: r = v1 / v2    before: [v1, v2, ...    after:  [r, ...
void Div(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    if (kInstance->v2 == 0)
        runError("division by zero", kInstance);
    kInstance->v1 = kInstance->memory[kInstance->sp];
    kInstance->memory[kInstance->sp] = kInstance->v1 / kInstance->v2;

    kInstance->lineNo = kInstance->memory[kInstance->ip++];
}

// Mod: r = v1 % v2    before: [v1, v2, ...    after:  [r, ...
void Mod(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    if (kInstance->v2 == 0)
        runError("modulus by zero", kInstance);
    kInstance->v1 = kInstance->memory[kInstance->sp];
    kInstance->memory[kInstance->sp] = kInstance->v1 % kInstance->v2;

    kInstance->lineNo = kInstance->memory[kInstance->ip++];
}

// And: r = v1 & v2    before: [v1, v2, ...    after:  [r, ...
void And(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    kInstance->memory[kInstance->sp] = kInstance->memory[kInstance->sp] & kInstance->v2;
}

// Neg: r = -v         before: [v, ...         after:  [r, ...
void Neg(Kernel kInstance)
{
    kInstance->memory[kInstance->sp] = -kInstance->memory[kInstance->sp];
    kInstance->ip = kInstance->ip + 1;
}

// Add: r = v1 + v2    before: [v1, v2, ...    after:  [r, ...
void Add(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    kInstance->memory[kInstance->sp] += kInstance->v2;

    kInstance->lineNo = kInstance->memory[kInstance->ip++];
}

// Sub: r = v1 - v2    before: [v1, v2, ...    after:  [r, ...
void Subtract(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    kInstance->memory[kInstance->sp] -= kInstance->v2;

    kInstance->lineNo = kInstance->memory[kInstance->ip++];
}

// Or: r = v1 | v2     before: [v1, v2, ...    after:  [r, ...
void Or(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    kInstance->memory[kInstance->sp] |= kInstance->v2;
}
/** equal: op length
    *
    *           Variable stack (before)
    *           --------------------------
    *   A       | X                      |
    *           --------------------------
    *   A+1     | X1                     |
    *           --------------------------
    *               ..................
    *           --------------------------
    *   A+n     |   Xn                   |
    *           --------------------------
    *   A+n     |   Y                    |
    *    +1     --------------------------
    *   A+n     |   Y+1                  |
    *    +2     --------------------------
    *                .................
    *           --------------------------
    *   A+2n    |   Y+n                  | <-- sp
    *    +1     --------------------------
    *
    *           Variable stack (after)
    *           --------------------------
    *   A       | 1 if(X=Y) - 0 if (X!=Y)| <-- sp
    *           --------------------------
    *   A+1     |   X1                   |
    *           --------------------------
    *               ..................
    *           --------------------------
    *   A+n     |   Xn                   |
    *           --------------------------
    *   A+n     |   Y                    |
    *    +1     --------------------------
    *   A+n     |   Y+1                  |
    *    +2     --------------------------
    *                 .................
    *           --------------------------
    *   A+2n    |   Y+n                  |
    *    +1     --------------------------
    *
    *           n = length - 1
    */
void Equal(Kernel kInstance)
{
    int length = kInstance->memory[kInstance->ip];
    int y = kInstance->sp - length + 1;
    int x = y - length;
    kInstance->sp = x;
    bool equal = true;
    for (int i = 0; i < length; i++)
    {
        if (kInstance->memory[x + i] != kInstance->memory[y + i])
        {
            equal = false;
            break;
        }
    }
    kInstance->ip = kInstance->ip + 1;
    if (equal)
        kInstance->memory[kInstance->sp] = 1;
    else
        kInstance->memory[kInstance->sp] = 0;
}
/** not equal: op length
    *
    *           Variable stack (before)
    *           --------------------------
    *   A       | X                      |
    *           --------------------------
    *   A+1     | X1                     |
    *           --------------------------
    *               ..................
    *           --------------------------
    *   A+n     |   Xn                   |
    *           --------------------------
    *   A+n     |   Y                    |
    *    +1     --------------------------
    *   A+n     |   Y+1                  |
    *    +2     --------------------------
    *               .................
    *           --------------------------
    *   A+2n    |   Y+n                  | <-- sp
    *    +1     --------------------------
    *
    *           Variable stack (after)
    *           --------------------------
    *   A       | 0 if(X=Y) - 1 if (X!=Y)| <-- sp
    *           --------------------------
    *   A+1     |   X1                   |
    *           --------------------------
    *               ..................
    *           --------------------------
    *   A+n     |   Xn                   |
    *           --------------------------
    *   A+n     |   Y                    |
    *    +1     --------------------------
    *   A+n     |   Y+1                  |
    *    +2     --------------------------
    *                 .................
    *           --------------------------
    *   A+2n    |   Y+n                  | <-- sp
    *    +1     --------------------------
    *
    *           n = length - 1
    */
void NotEqual(Kernel kInstance)
{
    int length = kInstance->memory[kInstance->ip];
    int y = kInstance->sp - length + 1;
    int x = kInstance->sp - length;
    kInstance->sp = x;

    kInstance->memory[kInstance->sp] = 0;
    for (int i = 0; i < length; i++)
    {
        if (kInstance->memory[x + i] != kInstance->memory[y + i])
        {
            kInstance->memory[kInstance->sp] = 1;
            break;
        }
    }
    kInstance->ip = kInstance->ip + 1;
}

// TestForLessThan: r = v1 < v2    before: [v1, v2, ...    after:  [r, ...
void TestForLessThan(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    kInstance->memory[kInstance->sp] = (kInstance->memory[kInstance->sp] < kInstance->v2) ? 1 : 0;
}

// TestForGreaterOrEqual: r = v1 >= v2    before: [v1, v2, ...    after:  [r, ...
void TestForGreaterOrEqual(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    kInstance->memory[kInstance->sp] = (kInstance->memory[kInstance->sp] >= kInstance->v2) ? 1 : 0;
}

// TestForGreater: r = v1 >= v2    before: [v1, v2, ...    after:  [r, ...
void TestForGreater(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    kInstance->memory[kInstance->sp] = (kInstance->memory[kInstance->sp] > kInstance->v2) ? 1 : 0;
}

// TestForGreaterOrEqual: r = v1 <= v2    before: [v1, v2, ...    after:  [r, ...
void TestForLessOrEqual(Kernel kInstance)
{
    kInstance->v2 = kInstance->memory[kInstance->sp--];
    kInstance->memory[kInstance->sp] = (kInstance->memory[kInstance->sp] <= kInstance->v2) ? 1 : 0;
}

/** *========================================
    *   Assigment_statement: Variable_symbol Expression ASSIGN
    *========================================
    *
    *           Program
    *           --------------------------
    *           |   AssignOpcode         |
    *           --------------------------
    *           |   length               | <-- ip
    *           --------------------------
    *
    *           Variable stack (before)
    *           --------------------------
    *   A-1     |   ?                    |
    *           --------------------------
    *   A       |   X                    |
    *           --------------------------
    *   A+1     |   Y0                   |
    *           --------------------------
    *   A+2     |   Y1                   |
    *           --------------------------
    *               .................
    *           --------------------------
    *   A+n     |   Yn                   | <-- sp
    *           --------------------------
    *
    *           Variable stack (after)
    *           --------------------------
    *   A-1     |   ?                    | <-- sp
    *           --------------------------
    *   A       |   X                    |
    *           --------------------------
    *   A+1     |   Y0                   |
    *           --------------------------
    *   A+2     |   Y1                   |
    *           --------------------------
    *                .................
    *           --------------------------
    *   A+n     |   Yn                   |
    *           --------------------------
    *               n = length
    */
void Assign(Kernel kInstance)
{
    int8_t length = kInstance->memory[kInstance->ip];
    kInstance->sp = kInstance->sp - length - 1;
    int8_t x = kInstance->memory[kInstance->sp + 1];
    int8_t y = kInstance->sp + 2;
    for (int i = 0; i < length; i++)
    {
        kInstance->memory[x + i] = kInstance->memory[y + i];
    }
    kInstance->ip = kInstance->ip + 1;
}

/** ====================================================
    *   Argument      = Expression | Variable_symbol | Procedure_argument
    *   Argument_list = Argument {Argument}
    *   Procedure_call
    *       Standard_call |
    *       [Argument_list] Instance ProcCall
    *       [Argument_list] Instance ParamCall
    *=====================================================
    *           Program
    *           --------------------------
    *           |   ProcCallOpcode       |
    *           --------------------------
    *           |   displacement         | <-- ip
    *           --------------------------
    *               ip = ip + displ
    *
    *           Variable stack (before)
    *           --------------------------
    *   A       |   link                 | <-- sp
    *           --------------------------
    *   A+1     |                        |
    *           --------------------------
    *   A+2     |                        |
    *           --------------------------
    *   A+3     |                        |
    *           --------------------------
    *   A+4     |                        |
    *           --------------------------
    *
    *           Variable stack (after)
    *           --------------------------
    *   A       |   link                 | <-- bp
    *           --------------------------
    *   A+1     |   bp                   |
    *           --------------------------
    *   A+2     |                        |
    *           --------------------------
    *   A+3     |   pe                   |
    *           --------------------------
    *   A+4     |   ip+2(next inst)      | <-- sp
    *           --------------------------
    */

void ProcCall(Kernel kInstance)
{
    int8_t displacement = kInstance->memory[kInstance->ip];
    //t         printStack();

    kInstance->memory[kInstance->sp + 1] = kInstance->bp;
    kInstance->memory[kInstance->sp + 3] = kInstance->pe;
    kInstance->memory[kInstance->sp + 4] = kInstance->ip + 1; // next instruction
    kInstance->bp = kInstance->sp;
    kInstance->sp = kInstance->sp + 4;
    kInstance->ip = kInstance->ip + displacement - 1;
    //t         printStack();
}

void ProcArg(Kernel kInstance)
{
    int displacement = kInstance->memory[kInstance->ip++];
    //t        printStack();
    kInstance->memory[++kInstance->sp] = displacement;
    //t        printStack();
}

void printStack(Kernel kInstance)
{
    // printf("ip=%02x [", kInstance->ip - 1024);
    // PutS("ip=");
    // PutS((char)(kInstance->ip - 1024));
    // PutS(" [");
    for (uint8_t n = 0; n < 8; n++)
    {
        // printf("%02x, ", kInstance->memory[kInstance->sp + n]);
        // PutS((char)(kInstance->memory[kInstance->sp + n]));
        // PutS(", ");
    }
    // PutS("...\n");
    // printf("...\n");
}
void ParamCall(Kernel kInstance)
{
    //t        printStack();
    int8_t displacement = kInstance->memory[kInstance->ip];
    int addr = kInstance->memory[kInstance->sp] + displacement;
    int dest = kInstance->memory[addr + 1];

    //t        printStack();

    kInstance->memory[kInstance->sp] = kInstance->memory[addr];
    kInstance->memory[kInstance->sp + 1] = kInstance->bp;
    kInstance->memory[kInstance->sp + 3] = kInstance->pe;
    kInstance->memory[kInstance->sp + 4] = kInstance->ip + 1; // next instruction
    kInstance->bp = kInstance->sp;
    kInstance->sp = kInstance->sp + 4;
    kInstance->ip = dest;
    //        printStack();
}

/** =====================================================
    *   Conditional_statement = [Expression Do] Statement_list [Else]
    *   Conditional_statement_list = Conditional_statement {Conditinal_statement}
    * =====================================================
    *           Program
    *           --------------------------
    *           |   DoOpcode             |
    *           --------------------------
    *           |   displacement         | <-- ip
    *           --------------------------
    *
    *           Variable stack (before)
    *           --------------------------
    *   A       |   X                    |
    *           --------------------------
    *   A+1     |   (0,1)                | <-- sp
    *           --------------------------
    *
    *           Variable stack (after)
    *           --------------------------
    *   A       |   X                    | <-- sp
    *           --------------------------
    *   A+1     |   (0,1)                |
    *           --------------------------
    */
void Do(Kernel kInstance)
{
    int8_t displacement = kInstance->memory[kInstance->ip];
    if (kInstance->memory[kInstance->sp] == 1)
        kInstance->ip++;
    else
        kInstance->ip = kInstance->ip + displacement - 1;
    kInstance->sp--;
}

/**
    *           Program
    *           --------------------------
    *           |   ElseOpcode           |
    *           --------------------------
    *           |   displacement         | <-- ip
    *           --------------------------
    */
void Else(Kernel kInstance)
{
    int8_t displacement = kInstance->memory[kInstance->ip];
    kInstance->ip = kInstance->ip + displacement - 1;
}

/** =====================================================
    *   if_statement = conditional_statement_list
    *   while_statement
    *           conditional_statement_list
    *   when_statement =
    *           When conditional_statement_list Wait Endwhen
    * =====================================================
    *           Program
    *           --------------------------
    *           |   WhenOpcode           |
    *           --------------------------
    *           |                        | <-- ip
    *           --------------------------
    */
void When()
{
    //do nothing
}

/**
    *           Program
    *           --------------------------
    *           |   WaitOpcode           |
    *           --------------------------
    *           |   displacement         | <-- ip
    *           --------------------------
    */
void Wait(Kernel kInstance)
{
    int8_t displacement = kInstance->memory[kInstance->ip];
    kInstance->ip = kInstance->ip + displacement - 1;
    preempt(kInstance);
    if (kInstance->taskCurrent >= (kInstance->numberOfTasks - 1))
        kInstance->taskCurrent = 0;
    else
        kInstance->taskCurrent++;
    resume(kInstance);
}

/**
    *           Program
    *           --------------------------
    *           |   EndWhenOpcode        |
    *           --------------------------
    *           |                        | <-- ip
    *           --------------------------
    */
void EndWhen()
{
    //do nothing
}

/** =====================================================
    *   Process_statement = Process statement_list Also
    * =====================================================
    *           Program
    *           --------------------------
    *           |   ProcessOpcode        |
    *           --------------------------
    *           |   tempLength           | <-- ip
    *           --------------------------
    *           |   lineNo               |
    *           --------------------------
    */
void Process(Kernel kInstance)
{
    int tempLength = kInstance->memory[kInstance->ip];
    kInstance->lineNo = kInstance->memory[kInstance->ip + 1];

    if ((kInstance->sp + tempLength) > kInstance->pe)
    {
        runError("Not enough memory to run a process sp", kInstance);
    }
    kInstance->ip += 2;
}

/**
    *           Program
    *           --------------------------
    *           |   AlsoOpcode           |
    *           --------------------------
    *           |   displacement         | <-- ip
    *           --------------------------
    */
void Also(Kernel kInstance)
{
    int8_t displacement = kInstance->memory[kInstance->ip];
    if (kInstance->numberOfTasks > 1)
    {
        while (kInstance->taskCurrent < (kInstance->numberOfTasks - 1))
        {
            kInstance->taskQueue[kInstance->taskCurrent] = kInstance->taskQueue[kInstance->taskCurrent + 1];
            kInstance->taskCurrent++;
        }
        kInstance->numberOfTasks = kInstance->numberOfTasks - 1;
        kInstance->taskCurrent = 0;
        resume(kInstance);
    }
    else
    { // numberOfTasks = 1
        kInstance->sp = kInstance->taskStackTop;
        kInstance->pe = kInstance->taskProgTop;
        kInstance->ip = kInstance->ip + displacement - 1;
    }
}

/** =====================================================
    *   Process_statement_list =  Process_statement {Process_statement}
    *   Concurent_statement    =  Goto Process_statement_list Cobegin
    * =====================================================
    *           Program
    *           --------------------------
    *           |   CoBeginOpcode        |
    *           --------------------------
    *           |   numOfTask            | <-- ip
    *           --------------------------
    *           |   lineNo               |
    *           --------------------------
    *           |   process constant     |
    *           --------------------------
    *           |   displacement         |
    *           --------------------------
    *           |   process constant     |
    *           --------------------------
    *           |   displacement         |
    *           --------------------------
    *           |   lineNo               |
    *           --------------------------
    *               .................
    *           --------------------------
    *           |   lineNo               |
    *           --------------------------
    *           |   displacement         |
    *           --------------------------
    */
void Cobegin(Kernel kInstance)
{
    int8_t numOfTask = kInstance->memory[kInstance->ip];
    kInstance->lineNo = kInstance->memory[kInstance->ip + 1];

    kInstance->numberOfTasks = numOfTask;
    if (kInstance->numberOfTasks > MAX_QUEUE)
        runError("Exceeds maximum number of processes", kInstance);
    kInstance->taskStackTop = kInstance->sp;
    kInstance->taskProgTop = kInstance->pe;

    int8_t length = (kInstance->pe - kInstance->sp) / numOfTask;
    for (uint8_t i = 0; i < numOfTask; i++)
    {
        kInstance->pe = kInstance->sp + length;
        *kInstance->taskQueue[i] = createTask(MAX_QUEUE);

        SetBp(*kInstance->taskQueue[i], kInstance->bp);
        SetSp(*kInstance->taskQueue[i], kInstance->sp);
        SetPe(*kInstance->taskQueue[i], kInstance->pe);
        SetIp(*kInstance->taskQueue[i], kInstance->ip + kInstance->memory[kInstance->ip + i * 2 + 3] - 1);

        kInstance->sp = kInstance->pe;
    }
    kInstance->taskCurrent = 0;
    resume(kInstance);
}

/**         Program
    *           -----------------
    *           |   IndexOpcode |
    *           -----------------
    *           |   lower       | <-- ip
    *           -----------------
    *           | upper         |
    *           -----------------
    *           |   length      |
    *           -----------------
    *           |   lineNo      |
    *           -----------------
    *
    *           Variable stack (before)
    *           ------------------
    *   A       | VariableAddress|
    *           ------------------
    *   A+1     |   Index        | <-- VI
    *           ------------------
    *
    *           Variable stack (after)
    *           ------------------
    *   A       | IndexAddress   | <-- VI
    *           ------------------
    *   A+1     |                |
    *           ------------------
    */
void Index(Kernel kInstance)
{
    uint8_t index;

    uint8_t lower = kInstance->memory[kInstance->ip];
    uint8_t upper = kInstance->memory[kInstance->ip + 1];
    uint8_t length = kInstance->memory[kInstance->ip + 2];

    kInstance->lineNo = kInstance->memory[kInstance->ip + 3];

    index = kInstance->memory[kInstance->sp];
    kInstance->sp = kInstance->sp - 1;

    if ((index < lower) || (index > upper))
        runError("index exceeds array dimension", kInstance);

    kInstance->memory[kInstance->sp] = kInstance->memory[kInstance->sp] + (index - lower) * length;
    kInstance->ip += 4;
}

/**
    *           Program
    *           -----------------
    *           |   FieldOpcode |
    *           -----------------
    *           |   Displacement| <-- ip
    *           -----------------
    *
    *           Variable stack (before)
    *           ------------------
    *   A       | Link           | <-- VI
    *           ------------------
    *   A+1     |                |
    *           ------------------
    *
    *           Variable stack (after)
    *           ------------------
    *   A       | Link + disp    | <-- VI
    *           ------------------
    *   A+1     |                |
    *           ------------------
    *
    */
void Field(Kernel kInstance)
{
    uint8_t displ = kInstance->memory[kInstance->ip];
    kInstance->memory[kInstance->sp] = kInstance->memory[kInstance->sp] + displ;
    kInstance->ip = kInstance->ip + 1;
}
//------------------------------------------- Extras
void PutInteger(Kernel kInstance)
{
    #if defined(Host)
        printf("%d", kInstance->memory[kInstance->sp]);
    #else
        // PutS("PutInteger");
        // PutC(intToChar(kInstance->memory[kInstance->sp]));
    #endif
    
    kInstance->sp = kInstance->sp - 1;
}
void PutCharacter(Kernel kInstance)
{
    #if defined(Host)
        printf("%c", (char)kInstance->memory[kInstance->sp]);
    #else
        
        // PutS("PutCharacter");
        // PutN();
        // PutC(intToChar(kInstance->memory[kInstance->sp]));
    #endif
         
    kInstance->sp = kInstance->sp - 1;
}
void PutBoolean(Kernel kInstance){
    PutS((kInstance->memory[kInstance->sp] == 0) ? "false" : "true");
    kInstance->sp = kInstance->sp - 1;
}
void PutLine(){
    PutN();
}
void EndCode(){
    //t     printf("The program terminates.\n");
    exit(0);
}

// PUBLIC

Kernel createKernel(uint16_t size) { // TODO: Init to clean up.
    Kernel kInstance = (Kernel)malloc(sizeof(KernelDesc) * size);
    kInstance->v1 = kInstance->v2 = 0;
    kInstance->ip = 0;
    kInstance->taskStackTop = 0;
    kInstance->taskProgTop = 0;

    kInstance->taskCurrent = 0;
    kInstance->numberOfTasks = 1;
    
    
    kInstance->taskQueue = createDoubleArrayTask(MAX_QUEUE);
    
    for (uint8_t i = 0; i < MAX_QUEUE; i++) 
        kInstance->taskQueue[i] = (Task*)createTask(MAX_QUEUE);
    

    kInstance->itsKernelStack = (uint8_t*)malloc(sizeof(uint8_t) * MAX_KERNEL_STACK_SIZE);
    kInstance->itsKernelSP = 0;

    
    
    kInstance->memory = (uint16_t*)malloc(sizeof(uint16_t) * MAX_ADDRESS);
    for (uint16_t i = 0; i < MAX_ADDRESS; i++) // Reset all memory locations.
        kInstance->memory[i] = 0;

    
    kInstance->pe = 1024;
    kInstance->bp = 0;
    kInstance->sp = 4;
    kInstance->memory[kInstance->sp] = 0; // no return
    kInstance->lineNo = 0;
    
    return kInstance;
}

#if defined(Host)
void load(Kernel kInstance, FILE *input)
{
    
    uint16_t i = kInstance->ip = kInstance->pe;
    char line[10];
    int16_t code;

    while (fgets(line, sizeof(line), input) != NULL) {
        code = atoi(line);
        if (code == -1) 
             break;
        kInstance->memory[i] = code;
        i++;
    }

    // Use a factory to isolate the configuration. In Lecture 6.

    fclose(input);
    //t        printf("\n%d words loaded.\n", i - pe);
    // PutC((char)(i - kInstance->pe));
    // PutS(" words loaded.\n");
}
#else

#define loadLength (50)
void load(Kernel kInstance, int16_t input[]){
    uint16_t i = kInstance->ip = kInstance->pe;
    int16_t code;
    // int16_t combine = sizeof(input) * sizeof(int16_t);
    // PutC(intToChar(combine));
    // PutN();

    // for(int j = 0; j < 40; j++){
    //     if (input[i] == -1) 
    //          break;
    //     // PutC(code);
    //     PutX16(input[j]);
    //     PutN();

    //     kInstance->memory[j] = input[j];
    // }

    for(i = 0; i < loadLength; i++){
        if (input[i] == -1) 
             break;
        code = input[i];
        // PutC(code);
        PutX16(code);
        PutN();
        kInstance->memory[i] = code;
    }

    // Use a factory to isolate the configuration. In Lecture 6.

    //t        printf("\n%d words loaded.\n", i - pe);
    // PutC(intToChar(i - kInstance->pe));
    // PutS(" words loaded.");
    // PutN();
}

#endif

void run(Kernel kInstance) {
    int16_t opcode = 0;
    while (1)
    {
        // printf("%d\n", kInstance->memory[kInstance->ip++] - INSTR_TABLE);
        //t            printf("ip=%02x opcode=%d", (ip-1024), opcode));
        // printf("MEOW %d", kInstance->memory[kInstance->ip++] - INSTR_TABLE);
        
        opcode = (kInstance->memory[kInstance->ip] - INSTR_TABLE);
        kInstance->ip = kInstance->ip + 1;

        #if defined(Host)
            printf("OPCODE: %d\n", opcode);
        #else
            PutS("Check Op Code");
            PutX16(opcode);
            PutN();
        #endif

        switch (opcode)
        {
        case ENDPROC:
            // printf("ENDPROC\n");
            PutS("ENDPROC");
            EndProc(kInstance);
            break;
        case PROCEDURE:
            // printf("PROCEDURE\n");
            PutS("PROCEDURE\n");
            Procedure(kInstance);
            break;
        case INDEX:
            // printf("INDEX\n");
            PutS("INDEX\n");
            Index(kInstance);
            break;
        case COBEGIN:
            // printf("COBEGIN\n");
            PutS("COBEGIN\n");
            Cobegin(kInstance);
            break;
            //          case LIBPROC:           Libproc();     break;
        case PARAMCALL:
            // printf("PARAMCALL\n");    
            PutS("PARAMCALL\n");  
            ParamCall(kInstance);
            break;
        case PROCCALL:
            // printf("PROCCALL\n");  
            ProcCall(kInstance);
            break;
        case ALSO:
            // printf("ENDPROC\n");  
            PutS("ENDPROC\n");  
            Also(kInstance);
            break;
        case ELSE:
            // printf("ELSE\n");  
            PutS("ELSE\n");
            Else(kInstance);
            break;
            //          case ENDLIB:            Endlib();      break;
            //          case FUNCVAL:           Funcval();     break;
            //          case PARAMARG:          Paramarg();    break;
            //          case PARAMETER:         Parameter();   break;
        case PROCARG:
            // printf("PROCARG\n");  
            PutS("PROCARG\n");  
            ProcArg(kInstance);
            break;
        case PROCESS:
            // printf("PROCESS\n");  
            PutS("PROCESS\n");  
            Process(kInstance);
            break;
        case VARIABLE:
            // printf("VARIABLE\n");  
            PutS("VARIABLE\n"); 
            Variable(kInstance);
            break;
        case ASSIGN:
            // printf("ASSIGN\n");    
            PutS("ASSIGN\n");    
            Assign(kInstance);
            break;
            //          case BLANK:             Blank();       break;
        case CONSTANT:
            // printf("CONSTANT\n");  
            PutS("CONSTANT\n");  
            Constant(kInstance);
            break;
        case CONSTRUCT:
            // printf("CONSTRUCT\n");  
            PutS("CONSTRUCT\n"); 
            Construct(kInstance);
            break;
        case DO:
            // printf("DO\n");  
            PutS("DO\n"); 
            Do(kInstance);
            break;
            //          case ENDIF:             EndIf();       break;
        case ENDWHEN:
            // printf("ENDWHEN\n"); 
            PutS("ENDWHEN\n");  
            EndWhen(kInstance);
            break;
        case EQUAL:
            // printf("EQUAL\n");  
            PutS("EQUAL\n"); 
            Equal(kInstance);
            break;
        case FIELD:
            // printf("FIELD\n"); 
            PutS("FIELD\n");   
            Field(kInstance);
            break;
        case GOTO:
            // printf("GOTO\n");
            PutS("GOTO\n");  
            Goto(kInstance);
            break;
        case NOTEQUAL:
            // printf("ENDPROC\n"); 
            PutS("ENDPROC\n");  
            NotEqual(kInstance);
            break;
        case VALSPACE:
            // printf("VALSPACE\n"); 
            PutS("VALSPACE\n");   
            ValSpace(kInstance);
            break;
        case VALUE:
            // printf("VALUE\n");  
            PutS("VALUE\n"); 
            Value(kInstance);
            break;
        case WAIT:
            // printf("WAIT\n");
            PutS("WAIT\n");  
            Wait(kInstance);
            break;
        case WHEN:
            // printf("WHEN\n");  
            PutS("WHEN\n");
            When(kInstance);
            break;
            //          case ERROR:             Error();       break;
        case ADD:
            // printf("ADD\n");  
            PutS("ADD\n");
            Add(kInstance);
            break;
        case AND:
            // printf("AND\n"); 
            PutS("AND\n"); 
            And(kInstance);
            break;
        case DIVIDE:
            // printf("DIVIDE\n");  
            PutS("DIVIDE\n");  
            Div(kInstance);
            break;
        case ENDCODE:
            // printf("ENDCODE\n");  
            PutS("ENDCODE\n");  
            EndCode(kInstance);
            break;
        case GREATER:
            // printf("GREATER\n");  
            TestForGreater(kInstance);
            break;
        case LESS:
            // printf("LESS\n");
            PutS("LESS\n");
            TestForLessThan(kInstance);
            break;
        case MINUS:
            // printf("MINUS\n");
            PutS("MINUS\n");
            Neg(kInstance);
            break;
        case MODULO:
            // printf("MODULO\n");
            PutS("MODULO\n");
            Mod(kInstance);
            break;
        case MULTIPLY:
            // printf("MULTIPLY\n");
            PutS("MULTIPLY\n");
            Mul(kInstance);
            break;
        case NOT:
            // printf("NOT\n");
            PutS("NOT\n");
            Not(kInstance);
            break;
        case NOTGREATER:
            // printf("NOTGREATER\n");
            PutS("NOTGREATER\n");
            TestForLessOrEqual(kInstance);
            break;
        case NOTLESS:
            // printf("NOTLESS\n");
            PutS("NOTLESS\n");
            TestForGreaterOrEqual(kInstance);
            break;
        case OR:
            // printf("OR\n");
            PutS("OR\n");
            Or(kInstance);
            break;
        case SUBTRACT:
            // printf("SUBTRACT\n");
            PutS("SUBTRACT\n");
            Subtract(kInstance);
            break;
            //          case ADDR:              Addr();        break;
            //          case HALT:              Halt();        break;
            //          case OBTAIN:            Obtain();      break;
            //          case PLACE:             Place();       break;
            //          case SENSE:             Sense();       break;
        case INSTANCE:
            // printf("INSTANCE\n");
            PutS("INSTANCE\n");
            Instance(kInstance);
            break;
        case PUTI:
            // printf("PUTI\n");
            PutS("PUTI\n");
            PutInteger(kInstance);
            break;
        case PUTC:
            // printf("PUTC\n");
            PutS("Here");
            PutCharacter(kInstance);
            break;
        case PUTB:
            // printf("PUTB\n");
            PutS("PUTB\n");
            PutBoolean(kInstance);
            break;
        case PUTN:
            // printf("PUTN\n");
            PutS("PUTN\n");
            PutLine(kInstance);
            break;
        default:
            // printf("Unknown opcode=%d ip=%u\n", opcode, kInstance->ip);
            // printf("Can't say gamer without GayYEEEE!!!!");
            PutS("Default\n");
            exit(1);
        }
    }
}

