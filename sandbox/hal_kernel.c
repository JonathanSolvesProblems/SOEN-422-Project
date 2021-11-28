#include <stdint.h>
#include <stdlib.h> 
#include "hal_task.h"
#include "hal_kernel.h"
#include "../Task1/COutForAUnit.h"
#include "private_kernel_variables.h"
#include "../Shared/shared.h"
#include <string.h>

typedef struct KernelDesc {
    int8_t v1, v2;

    // kernel
    int8_t MAX_KERNEL_STACK_SIZE;
    int8_t *itsKernelStack; // stack
    int8_t itsKernelSP;     //  sp

    // variable stack
    uint16_t bp; //  base pointer
    uint16_t sp; //  stack pointer

    // program
    uint16_t ip; //  instruction pointer
    uint16_t pe; //  program end

    // task
    int8_t MAX_QUEUE;

    Task *taskQueue;     // Task taskQueue[];
    int8_t taskCurrent;   // this
    int8_t numberOfTasks; // tasks

    int8_t taskStackTop; // stackTop
    int8_t taskProgTop;  // progTop

    uint8_t MIN_ADDRESS;
    uint16_t MAX_ADDRESS;
    uint8_t SPACE;
    uint16_t INSTR_TABLE;
    uint16_t SET_LENGTH;
    uint8_t SET_LIMIT;

    int16_t *memory;
    int8_t lineNo;

} KernelDesc, *Kernel;

void Kernel_Init(Kernel k) {
    k->MAX_KERNEL_STACK_SIZE = 100;
    k->MAX_QUEUE = 10;
    k->MIN_ADDRESS = 0;
#ifndef HOST
    k->MAX_ADDRESS = 100;
#else
    k->MAX_ADDRESS = 20000;
#endif
    k->SPACE = (int8_t)(' ');
    k->INSTR_TABLE = 400;
    k->SET_LENGTH = 0x8;
    k->SET_LIMIT = 127;


    k->v1 = 0;
    k->v2 = 0;
    k->ip = 0;
    k->taskStackTop = 0;
    k->taskProgTop = 0;

    k->taskCurrent = 0;
    k->numberOfTasks = 1;
    k->taskQueue = (Task*) malloc(k->MAX_QUEUE*sizeof(Task));
    for (int8_t i = 0; i < k->MAX_QUEUE; i++)
      k->taskQueue[i] = createTask(10);

    k->itsKernelStack = (int8_t*) malloc(k->MAX_KERNEL_STACK_SIZE*sizeof(int8_t));
    k->itsKernelSP = 0;

    k->memory = (int16_t*) malloc(k->MAX_ADDRESS*sizeof(int16_t));
    for (int16_t i = 0; i < k->MAX_ADDRESS; i++) // Reset all memory locations.
        k->memory[i] = 0;

    k->pe = 1024;
    k->bp = 0;
    k->sp = 4;
    k->memory[k->sp] = 0; // no return
    k->lineNo = 0;
}

Kernel createKernel() {
    Kernel k = (Kernel) malloc(sizeof(KernelDesc)); // Allocate space to the queue
    Kernel_Init(k);
    return k;
}

#if defined(Host)
    void load(Kernel k, FILE *input)
    {
        uint16_t i = k->ip = k->pe;
        char line[10];
        int16_t code;
        while (fgets(line, 10, input) != NULL)
        {
            code = atoi(line);
            k->memory[i++] = code;
        }
        fclose(input);
    }
#else

void load(Kernel k, int16_t *input) {
    uint16_t i = k->ip = k->pe;
    do {
        k->memory[i++] = *input++;
    } while (*input != -1);
}

#endif

void loadInMemory(Kernel kInstance, uint8_t* memory) {
    uint16_t i = kInstance->ip = kInstance->pe;
    int8_t lineSize = 10;
    char* line = (char*)malloc(sizeof(char) * lineSize);
    int16_t code;
    uint8_t count = 0;
    while (*memory) {
        if (*memory == 0x0A) {
            code = atoi(line);
            kInstance->memory[i++] = code;
            memset(line, 0, lineSize);
            count = 0;
        }
        else if (*memory == 0x2D) {
            kInstance->memory[i++] = -1;
            break;
        } else {
            line[count++] = *memory;
        }
        memory++;
    }
    free(line);
}

void runError(const char *msg)
{
    PutS(msg);
    PutN();
    exit(1);
}

void preempt(Kernel kInstance)
{
    // Task current = kInstance->taskQueue[kInstance->taskCurrent];
    SetBp(kInstance->taskQueue[kInstance->taskCurrent], kInstance->bp);
    SetSp(kInstance->taskQueue[kInstance->taskCurrent], kInstance->sp);
    SetPe(kInstance->taskQueue[kInstance->taskCurrent], kInstance->pe);
    SetIp(kInstance->taskQueue[kInstance->taskCurrent], kInstance->ip);
}

void resume(Kernel k)
{
    Task current = k->taskQueue[k->taskCurrent];
    k->bp = GetBp(current);
    k->sp = GetSp(current);
    k->pe = GetPe(current);
    k->ip = GetIp(current);
}

/** goto: op displacement */
void Goto(Kernel k)
{
    // t     PutS("Goto\n");
    int8_t displacement = k->memory[k->ip];
    k->ip = k->ip + displacement - 1;
}

/** proc: op   paramLength, varLength, tempLength, lineNo */
void Procedure(Kernel k)
{
    int8_t paramLength = k->memory[k->ip];
    int8_t varLength = k->memory[k->ip + 1];
    int8_t tempLength = k->memory[k->ip + 2];
    k->lineNo = k->memory[k->ip + 3];

    // t     PutS("Procedure: sp = %u\n", sp);
    k->memory[k->bp + 2] = k->bp - paramLength - 1;
    k->sp = k->sp + varLength;
    if ((k->sp + tempLength) > k->pe)
    {
        runError("Stack overflows: sp");
    }
    k->ip = k->ip + 4;
}

void EndProc(Kernel k)
{
    // tt        System.out.println("EndProc");
    // tt        printStack();

    if (k->memory[k->bp + 4] != 0)
    {
        k->ip = k->memory[k->bp + 4];
        k->pe = k->memory[k->bp + 3];
        k->sp = k->memory[k->bp + 2];
        k->bp = k->memory[k->bp + 1];
    }
    // t        printStack();
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
void Instance(Kernel k)
{
    int8_t steps = k->memory[k->ip];
    int8_t link = k->bp;
    for (int8_t i = steps; i > 0; i--)
    {
        link = k->memory[link];
    }
    k->sp = k->sp + 1;
    k->memory[k->sp] = link;
    k->ip = k->ip + 1;
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
void Variable(Kernel k)
{
    int8_t displ = k->memory[k->ip];
    k->memory[k->sp] = k->memory[k->sp] + displ;
    k->ip = k->ip + 1;
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
void Construct(Kernel k)
{
    int8_t number = k->memory[k->ip];
    uint16_t elem, index, i, val;
    uint16_t *tempSet = (uint16_t*) malloc(k->SET_LENGTH*sizeof(int16_t));
    // t     PutS("Set number = %d\n", number);
    for (i = 0; i < k->SET_LENGTH; i++)
        tempSet[i] = 0;

    for (i = 0; i < number; i++)
    {
        elem = k->memory[k->sp - i];
        if ((elem >= 0) && (elem <= k->SET_LIMIT))
        {
            index = elem & (k->SET_LENGTH - 1);
            val = elem >> 3;

            tempSet[index] |= (1 << val);
        }
        else
            PutS("Error: element has the value outside set range\n");
    }
    k->sp = k->sp - number + 1;
    for (i = 0; i < k->SET_LENGTH; i++)
    {
      k->memory[k->sp + i] = tempSet[i];
    }
    k->sp = k->sp + k->SET_LENGTH - 1;
    k->ip = k->ip + 2;
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
void Constant(Kernel k)
{
    int8_t value = k->memory[k->ip];
    k->sp = k->sp + 1;
    k->memory[k->sp] = value;
    k->ip = k->ip + 1;
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
void Value(Kernel k)
{
    int8_t length = k->memory[k->ip];
    int8_t varAdd = k->memory[k->sp];

    for (int8_t i = 0; i < length; i++)
    {
        k->memory[k->sp + i] = k->memory[varAdd + i];
    }
    k->sp = k->sp + length - 1;
    k->ip = k->ip + 1;
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
void ValSpace(Kernel k)
{
    int8_t length = k->memory[k->ip];
    k->sp = k->sp + length;
    k->ip = k->ip + 1;
}

// Not: r = ~v    before: [v, ...    after:  [r, ...
void Not(Kernel k)
{
    k->memory[k->sp] = ~k->memory[k->sp];
}

// Mul: r = v1 * v2    before: [v1, v2, ...    after:  [r, ...
void Mul(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    k->memory[k->sp] = k->memory[k->sp] * k->v2;

    k->lineNo = k->memory[k->ip++];
}
// Div: r = v1 / v2    before: [v1, v2, ...    after:  [r, ...
void Div(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    if (k->v2 == 0)
        runError("division by zero");
    k->v1 = k->memory[k->sp];
    k->memory[k->sp] = k->v1 / k->v2;

    k->lineNo = k->memory[k->ip++];
}

// Mod: r = v1 % v2    before: [v1, v2, ...    after:  [r, ...
void Mod(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    if (k->v2 == 0)
        runError("modulus by zero");
    k->v1 = k->memory[k->sp];
    k->memory[k->sp] = k->v1 % k->v2;

    k->lineNo = k->memory[k->ip++];
}

// And: r = v1 & v2    before: [v1, v2, ...    after:  [r, ...
void And(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    k->memory[k->sp] = k->memory[k->sp] & k->v2;
}

// Neg: r = -v         before: [v, ...         after:  [r, ...
void Neg(Kernel k)
{
    k->memory[k->sp] = -k->memory[k->sp];
    k->ip = k->ip + 1;
}

// Add: r = v1 + v2    before: [v1, v2, ...    after:  [r, ...
void Add(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    k->memory[k->sp] += k->v2;

    k->lineNo = k->memory[k->ip++];
}

// Sub: r = v1 - v2    before: [v1, v2, ...    after:  [r, ...
void Subtract(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    k->memory[k->sp] -= k->v2;

    k->lineNo = k->memory[k->ip++];
}

// Or: r = v1 | v2     before: [v1, v2, ...    after:  [r, ...
void Or(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    k->memory[k->sp] |= k->v2;
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
void Equal(Kernel k)
{
    int8_t length = k->memory[k->ip];
    int8_t y = k->sp - length + 1;
    int8_t x = y - length;
    k->sp = x;
    bool equal = true;
    for (int8_t i = 0; i < length; i++)
    {
        if (k->memory[x + i] != k->memory[y + i])
        {
            equal = false;
            break;
        }
    }
    k->ip = k->ip + 1;
    if (equal)
       k->memory[k->sp] = 1;
    else
        k->memory[k->sp] = 0;
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
void NotEqual(Kernel k)
{
    int8_t length = k->memory[k->ip];
    int8_t y = k->sp - length + 1;
    int8_t x = k->sp - length;
    k->sp = x;

    k->memory[k->sp] = 0;
    for (int8_t i = 0; i < length; i++)
    {
        if (k->memory[x + i] != k->memory[y + i])
        {
            k->memory[k->sp] = 1;
            break;
        }
    }
    k->ip = k->ip + 1;
}

// TestForLessThan: r = v1 < v2    before: [v1, v2, ...    after:  [r, ...
void TestForLessThan(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    k->memory[k->sp] = (k->memory[k->sp] < k->v2) ? 1 : 0;
}

// TestForGreaterOrEqual: r = v1 >= v2    before: [v1, v2, ...    after:  [r, ...
void TestForGreaterOrEqual(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    k->memory[k->sp] = (k->memory[k->sp] >= k->v2) ? 1 : 0;
}

// TestForGreater: r = v1 >= v2    before: [v1, v2, ...    after:  [r, ...
void TestForGreater(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    k->memory[k->sp] = (k->memory[k->sp] > k->v2) ? 1 : 0;
}

// TestForGreaterOrEqual: r = v1 <= v2    before: [v1, v2, ...    after:  [r, ...
void TestForLessOrEqual(Kernel k)
{
    k->v2 = k->memory[k->sp--];
    k->memory[k->sp] = (k->memory[k->sp] <= k->v2) ? 1 : 0;
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
void Assign(Kernel k)
{
    int8_t length = k->memory[k->ip];
    k->sp = k->sp - length - 1;
    int8_t x = k->memory[k->sp + 1];
    int8_t y = k->sp + 2;
    for (int8_t i = 0; i < length; i++)
    {
        k->memory[x + i] = k->memory[y + i];
    }
    k->ip = k->ip + 1;
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

void ProcCall(Kernel k)
{
    int8_t displacement = k->memory[k->ip];
    // t         printStack();

    k->memory[k->sp + 1] = k->bp;
    k->memory[k->sp + 3] = k->pe;
    k->memory[k->sp + 4] = k->ip + 1; // next instruction
    k->bp = k->sp;
    k->sp = k->sp + 4;
    k->ip = k->ip + displacement - 1;
    // t         printStack();
}

void ProcArg(Kernel k)
{
    int8_t displacement = k->memory[k->ip++];
    // t        printStack();
    k->memory[++k->sp] = displacement;
    // t        printStack();
}

void printStack(Kernel k)
{
    PutS("ip=");
    uint16_t n = k->ip - 1024;
    PutX16(n);
    PutS(" [");
    // PutS("ip=%02x [", ip - 1024);
    for (int8_t n = 0; n < 8; n++)
    {
        PutX16(k->memory[k->sp + n]);
        PutS(", ");
        // PutS("%02x, ", memory[sp + n]);
    }
    PutS("...\n");
}
void ParamCall(Kernel k)
{
    // t        printStack();
    int8_t displacement = k->memory[k->ip];
    int8_t addr = k->memory[k->sp] + displacement;
    int8_t dest = k->memory[addr + 1];

    // t        printStack();

    k->memory[k->sp] = k->memory[addr];
    k->memory[k->sp + 1] = k->bp;
    k->memory[k->sp + 3] = k->pe;
    k->memory[k->sp + 4] = k->ip + 1; // next instruction
    k->bp = k->sp;
    k->sp = k->sp + 4;
    k->ip = dest;
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
void Do(Kernel k)
{
    int8_t displacement = k->memory[k->ip];
    if (k->memory[k->sp] == 1)
        k->ip++;
    else
        k->ip = k->ip + displacement - 1;
    k->sp--;
}

/**
 *           Program
 *           --------------------------
 *           |   ElseOpcode           |
 *           --------------------------
 *           |   displacement         | <-- ip
 *           --------------------------
 */
void Else(Kernel k)
{
    int8_t displacement = k->memory[k->ip];
    k->ip = k->ip + displacement - 1;
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
void When(Kernel k)
{
    // do nothing
}

/**
 *           Program
 *           --------------------------
 *           |   WaitOpcode           |
 *           --------------------------
 *           |   displacement         | <-- ip
 *           --------------------------
 */
void Wait(Kernel k)
{
    int8_t displacement = k->memory[k->ip];
    k->ip = k->ip + displacement - 1;
    preempt(k);
    if (k->taskCurrent >= (k->numberOfTasks - 1))
        k->taskCurrent = 0;
    else
        k->taskCurrent++;
    resume(k);
}

/**
 *           Program
 *           --------------------------
 *           |   EndWhenOpcode        |
 *           --------------------------
 *           |                        | <-- ip
 *           --------------------------
 */
void EndWhen(Kernel k)
{
    // do nothing
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
void Process(Kernel k)
{
    int8_t tempLength = k->memory[k->ip];
    k->lineNo = k->memory[k->ip + 1];

    if ((k->sp + tempLength) > k->pe)
    {
        runError("Not enough memory to run a process sp");
    }
    k->ip += 2;
}

/**
 *           Program
 *           --------------------------
 *           |   AlsoOpcode           |
 *           --------------------------
 *           |   displacement         | <-- ip
 *           --------------------------
 */
void Also(Kernel k)
{
    int8_t displacement = k->memory[k->ip];
    if (k->numberOfTasks > 1)
    {
        while (k->taskCurrent < (k->numberOfTasks - 1))
        {
            k->taskQueue[k->taskCurrent] = k->taskQueue[k->taskCurrent + 1];
            k->taskCurrent++;
        }
        k->numberOfTasks = k->numberOfTasks - 1;
        k->taskCurrent = 0;
        resume(k);
    }
    else
    { // numberOfTasks = 1
        k->sp = k->taskStackTop;
        k->pe = k->taskProgTop;
        k->ip = k->ip + displacement - 1;
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
void Cobegin(Kernel k)
{
    int8_t numOfTask = k->memory[k->ip];
    k->lineNo = k->memory[k->ip + 1];

    k->numberOfTasks = numOfTask;
    if (k->numberOfTasks > k->MAX_QUEUE)
        runError("Exceeds maximum number of processes");
    k->taskStackTop = k->sp;
    k->taskProgTop = k->pe;

    int8_t length = (k->pe - k->sp) / numOfTask;
    for (int8_t i = 0; i < numOfTask; i++)
    {
        k->pe = k->sp + length;
        k->taskQueue[i] = createTask(10);
        Task current = k->taskQueue[i];
        SetBp(current, k->bp);
        SetSp(current, k->sp);
        SetPe(current, k->pe);
        SetIp(current, k->ip + k->memory[k->ip + i * 2 + 3] - 1);
        k->sp = k->pe;
    }
    k->taskCurrent = 0;
    resume(k);
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
void Index(Kernel k)
{
    int8_t index;

    int8_t lower = k->memory[k->ip];
    int8_t upper = k->memory[k->ip + 1];
    int8_t length = k->memory[k->ip + 2];

    k->lineNo = k->memory[k->ip + 3];

    index = k->memory[k->sp];
    k->sp = k->sp - 1;

    if ((index < lower) || (index > upper))
        runError("index exceeds array dimension");

    k->memory[k->sp] = k->memory[k->sp] + (index - lower) * length;
    k->ip += 4;
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
void Field(Kernel k)
{
    int8_t displ = k->memory[k->ip];
    k->memory[k->sp] = k->memory[k->sp] + displ;
    k->ip = k->ip + 1;
}
//------------------------------------------- Extras
void PutInteger(Kernel k)
{
    // PutC(k->memory[k->sp]);
    PutC(int16ToChar(k->memory[k->sp]));
    k->sp = k->sp - 1;
}
void PutCharacter(Kernel k)
{
    PutC((char)k->memory[k->sp]);
    k->sp = k->sp - 1;
}
void PutBoolean(Kernel k)
{
    PutS((k->memory[k->sp] == 0) ? "false" : "true");
    k->sp = k->sp - 1;
}
void PutLine(Kernel k)
{
    PutS("\n");
}
void EndCode(Kernel k)
{
    PutS(Equals() ? ".\n" : "F\n");
    exit(0);
}

void run(Kernel k)
{
    int16_t opcode = 0;
    while (true)
    {
        // PutS("ip=%02x opcode=%d", (ip-1024), opcode);
        switch ((opcode = k->memory[k->ip++]) - k->INSTR_TABLE)
        {
        case ENDPROC:
            EndProc(k);
            break;
        case PROCEDURE:
            Procedure(k);
            break;
        case INDEX:
            Index(k);
            break;
        case COBEGIN:
            Cobegin(k);
            break;
            //          case LIBPROC:           Libproc();     break;
        case PARAMCALL:
            ParamCall(k);
            break;
        case PROCCALL:
            ProcCall(k);
            break;
        case ALSO:
            Also(k);
            break;
        case ELSE:
            Else(k);
            break;
            //          case ENDLIB:            Endlib();      break;
            //          case FUNCVAL:           Funcval();     break;
            //          case PARAMARG:          Paramarg();    break;
            //          case PARAMETER:         Parameter();   break;
        case PROCARG:
            ProcArg(k);
            break;
        case PROCESS:
            Process(k);
            break;
        case VARIABLE:
            Variable(k);
            break;
        case ASSIGN:
            Assign(k);
            break;
            //          case BLANK:             Blank();       break;
        case CONSTANT:
            Constant(k);
            break;
        case CONSTRUCT:
            Construct(k);
            break;
        case DO:
            Do(k);
            break;
            //          case ENDIF:             EndIf();       break;
        case ENDWHEN:
            EndWhen(k);
            break;
        case EQUAL:
            Equal(k);
            break;
        case FIELD:
            Field(k);
            break;
        case GOTO:
            Goto(k);
            break;
        case NOTEQUAL:
            NotEqual(k);
            break;
        case VALSPACE:
            ValSpace(k);
            break;
        case VALUE:
            Value(k);
            break;
        case WAIT:
            Wait(k);
            break;
        case WHEN:
            When(k);
            break;
            //          case ERROR:             Error();       break;
        case ADD:
            Add(k);
            break;
        case AND:
            And(k);
            break;
        case DIVIDE:
            Div(k);
            break;
        case ENDCODE:
            EndCode(k);
            break;
        case GREATER:
            TestForGreater(k);
            break;
        case LESS:
            TestForLessThan(k);
            break;
        case MINUS:
            Neg(k);
            break;
        case MODULO:
            Mod(k);
            break;
        case MULTIPLY:
            Mul(k);
            break;
        case NOT:
            Not(k);
            break;
        case NOTGREATER:
            TestForLessOrEqual(k);
            break;
        case NOTLESS:
            TestForGreaterOrEqual(k);
            break;
        case OR:
            Or(k);
            break;
        case SUBTRACT:
            Subtract(k);
            break;
            //          case ADDR:              Addr();        break;
            //          case HALT:              Halt();        break;
            //          case OBTAIN:            Obtain();      break;
            //          case PLACE:             Place();       break;
            //          case SENSE:             Sense();       break;
        case INSTANCE:
            Instance(k);
            break;
        case PUTI:
            PutInteger(k);
            break;
        case PUTC:
            PutCharacter(k);
            break;
        case PUTB:
            PutBoolean(k);
            break;
        case PUTN:
            PutLine(k);
            break;
        default:
            PutS("Unknown opcode=");
            PutX16(opcode);
            PutS(" ip=");
            PutX16(k->ip);
            PutN();
            exit(1);
        }
    }
}

void kernelClearMemory(Kernel kInstance) {
    free(kInstance->taskQueue);
    free(kInstance->itsKernelStack);
    free(kInstance->memory);
    free(kInstance);
}
