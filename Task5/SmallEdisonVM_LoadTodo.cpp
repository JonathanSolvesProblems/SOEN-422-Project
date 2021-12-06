// SmallEdisonVM.cpp - Implementation of the Small Edison Virtual Machine.
// Nov 9, 2021 - Michel de Champlain

#include <stdlib.h> // exit, EXIT_FAILURE
#include <stdio.h>
#include <stdint.h>
#include <string.h>   // string
#include "../Task1/COutForAUnit.h"

#define MaxCharactersInLine 256
#define MaxCharPerLine 9

class Task
{
public:
    int bp;
    int sp;
    int pe;
    int ip;

    Task()
    {
        bp = sp = pe = ip = 0;
    }
};

class Kernel
{
public:
    static const int INVALID = -1;
    static const int ADD = 0;
    static const int ALSO = 1;
    static const int AND = 2;
    static const int ASSIGN = 3;
    static const int BLANK = 4;
    static const int COBEGIN = 5;
    static const int CONSTANT = 6;
    static const int CONSTRUCT = 7;
    //  static const int DIFFERENCE              = 8;
    static const int DIVIDE = 9;
    static const int DO = 10;
    static const int ELSE = 11;
    static const int ENDCODE = 12;
    static const int ENDIF = 13;
    static const int ENDLIB = 14;
    static const int ENDPROC = 15;
    static const int ENDWHEN = 16;
    static const int EQUAL = 17;
    static const int ERROR = 18;
    static const int FIELD = 19;
    //  static const int FUNCVAL                 = 20;
    static const int GOTO = 21;
    static const int GREATER = 22;
    //  static const int IN                      = 23;
    static const int INDEX = 24;
    static const int INSTANCE = 25;
    //  static const int INTERSECTION            = 26;
    static const int LESS = 27;
    static const int LIBPROC = 28;
    static const int MINUS = 29;
    static const int MODULO = 30;
    static const int MULTIPLY = 31;
    static const int NEWLINE = 32;
    static const int NOT = 33;
    static const int NOTEQUAL = 34;
    static const int NOTGREATER = 35;
    static const int NOTLESS = 36;
    static const int OR = 37;
    static const int PARAMARG = 38;
    static const int PARAMCALL = 39;
    static const int PARAMETER = 40;
    static const int PROCARG = 41;
    static const int PROCCALL = 42;
    static const int PROCEDURE = 43;
    static const int PROCESS = 44;
    static const int SUBTRACT = 45;
    //  static const int UNION                   = 46;
    static const int VALSPACE = 47;
    static const int VALUE = 48;
    static const int VARIABLE = 49;
    static const int WAIT = 50;
    static const int WHEN = 51;
    static const int WHILE = 52;
    static const int ADDR = 53;
    static const int HALT = 54;
    static const int OBTAIN = 55;
    static const int PLACE = 56;
    static const int SENSE = 57;
    /** Extra */
    static const int ELEMASSIGN = 58;
    static const int ELEMVALUE = 59;
    static const int LOCALCASE = 60;
    static const int LOCALSET = 61;
    static const int LOCALVALUE = 62;
    static const int LOCALVAR = 63;
    static const int OUTERCALL = 64;
    static const int OUTERCASE = 65;
    static const int OUTERPARAM = 66;
    static const int OUTERSET = 67;
    static const int OUTERVALUE = 68;
    static const int OUTERVAR = 69;
    static const int SETCONST = 70;
    static const int SINGLETON = 71;
    static const int STRINGCONST = 72;

    static const int PUTI = 73;
    static const int PUTC = 74;
    static const int PUTB = 75;
    static const int PUTN = 76;

private:
    // temporary fields for stack evaluation
    int v1, v2;

    // kernel
    const int MAX_KERNEL_STACK_SIZE = 100;
    int *itsKernelStack; // stack
    int itsKernelSP;     //  sp

    // variable stack
    int bp; //  base pointer
    int sp; //  stack pointer

    // program
    int ip; //  instruction pointer
    int pe; //  program end

    // task
    const int MAX_QUEUE = 10;

    Task **taskQueue;  // Task taskQueue[];
    int taskCurrent;   // this
    int numberOfTasks; // tasks

    int taskStackTop; // stackTop
    int taskProgTop;  // progTop

    const int MIN_ADDRESS = 0;
    const int MAX_ADDRESS = 20000;
    const int SPACE = (int)(' ');
    const int INSTR_TABLE = 400;
    const int SET_LENGTH = 0x8;
    const int SET_LIMIT = 127;

    int *memory;
    int lineNo;

public:
    Kernel()
    {
        v1 = v2 = 0;
        ip = 0;
        taskStackTop = 0;
        taskProgTop = 0;

        taskCurrent = 0;
        numberOfTasks = 1;
        taskQueue = new Task *[MAX_QUEUE];
        for (int i = 0; i < MAX_QUEUE; i++)
            taskQueue[i] = new Task();

        itsKernelStack = new int[MAX_KERNEL_STACK_SIZE];
        itsKernelSP = 0;

        memory = new int[MAX_ADDRESS];
        for (int i = 0; i < MAX_ADDRESS; i++) // Reset all memory locations.
            memory[i] = 0;

        pe = 1024;
        bp = 0;
        sp = 4;
        memory[sp] = 0; // no return
        lineNo = 0;
    }

    void load(FILE *input)
    {
        int i = ip = pe;
        char line[10];
        int code;

        // TODO: Your code...

        while (fgets(line, sizeof(line), input) != NULL) {
            code = atoi(line);
            if (code == -1)
                break;
            // printf("%d\n", code);
            memory[i] = code;
            i++;
        }

        // Use a factory to isolate the configuration. In Lecture 6.

        fclose(input);
        //t        printf("\n%d words loaded.\n", i - pe);
        PutC((i - pe) + '0');
        PutS("words loaded.");
        PutC('\n');
    }

    void run()
    {
        int opcode = 0;
        while (true)
        {
            //t            printf("ip=%02x opcode=%d", (ip-1024), opcode));
            switch ((opcode = memory[ip++]) - INSTR_TABLE)
            {
            case ENDPROC:
                EndProc();
                break;
            case PROCEDURE:
                Procedure();
                break;
            case INDEX:
                Index();
                break;
            case COBEGIN:
                Cobegin();
                break;
                //          case LIBPROC:           Libproc();     break;
            case PARAMCALL:
                ParamCall();
                break;
            case PROCCALL:
                ProcCall();
                break;
            case ALSO:
                Also();
                break;
            case ELSE:
                Else();
                break;
                //          case ENDLIB:            Endlib();      break;
                //          case FUNCVAL:           Funcval();     break;
                //          case PARAMARG:          Paramarg();    break;
                //          case PARAMETER:         Parameter();   break;
            case PROCARG:
                ProcArg();
                break;
            case PROCESS:
                Process();
                break;
            case VARIABLE:
                Variable();
                break;
            case ASSIGN:
                Assign();
                break;
                //          case BLANK:             Blank();       break;
            case CONSTANT:
                Constant();
                break;
            case CONSTRUCT:
                Construct();
                break;
            case DO:
                Do();
                break;
                //          case ENDIF:             EndIf();       break;
            case ENDWHEN:
                EndWhen();
                break;
            case EQUAL:
                Equal();
                break;
            case FIELD:
                Field();
                break;
            case GOTO:
                Goto();
                break;
            case NOTEQUAL:
                NotEqual();
                break;
            case VALSPACE:
                ValSpace();
                break;
            case VALUE:
                Value();
                break;
            case WAIT:
                Wait();
                break;
            case WHEN:
                When();
                break;
                //          case ERROR:             Error();       break;
            case ADD:
                Add();
                break;
            case AND:
                And();
                break;
            case DIVIDE:
                Div();
                break;
            case ENDCODE:
                EndCode();
                break;
            case GREATER:
                TestForGreater();
                break;
            case LESS:
                TestForLessThan();
                break;
            case MINUS:
                Neg();
                break;
            case MODULO:
                Mod();
                break;
            case MULTIPLY:
                Mul();
                break;
            case NOT:
                Not();
                break;
            case NOTGREATER:
                TestForLessOrEqual();
                break;
            case NOTLESS:
                TestForGreaterOrEqual();
                break;
            case OR:
                Or();
                break;
            case SUBTRACT:
                Subtract();
                break;
                //          case ADDR:              Addr();        break;
                //          case HALT:              Halt();        break;
                //          case OBTAIN:            Obtain();      break;
                //          case PLACE:             Place();       break;
                //          case SENSE:             Sense();       break;
            case INSTANCE:
                Instance();
                break;
            case PUTI:
                PutInteger();
                break;
            case PUTC:
                PutCharacter();
                break;
            case PUTB:
                PutBoolean();
                break;
            case PUTN:
                PutLine();
                break;
            default:
                printf("Unknown opcode=%d ip=%u\n", opcode, ip);
                exit(1);
            }
        }
    }

private:
    void runError(const char *msg)
    {
        printf("%s\n", msg);
        exit(1);
    }
    void preempt()
    {
        taskQueue[taskCurrent]->bp = bp;
        taskQueue[taskCurrent]->sp = sp;
        taskQueue[taskCurrent]->pe = pe;
        taskQueue[taskCurrent]->ip = ip;
    }
    void resume()
    {
        bp = taskQueue[taskCurrent]->bp;
        sp = taskQueue[taskCurrent]->sp;
        pe = taskQueue[taskCurrent]->pe;
        ip = taskQueue[taskCurrent]->ip;
        //t     printf("Resume: ip = %u\n", ip);
    }

    /** goto: op displacement */
    void Goto()
    {
        //t     printf("Goto\n");
        int displacement = memory[ip];
        ip = ip + displacement - 1;
    }

    /** proc: op   paramLength, varLength, tempLength, lineNo */
    void Procedure()
    {
        int paramLength = memory[ip];
        int varLength = memory[ip + 1];
        int tempLength = memory[ip + 2];
        lineNo = memory[ip + 3];

        //t     printf("Procedure: sp = %u\n", sp);
        memory[bp + 2] = bp - paramLength - 1;
        sp = sp + varLength;
        if ((sp + tempLength) > pe)
        {
            runError("Stack overflows: sp");
        }
        ip = ip + 4;
    }

    void EndProc()
    {
        //tt        System.out.println("EndProc");
        //tt        printStack();

        if (memory[bp + 4] != 0)
        {
            ip = memory[bp + 4];
            pe = memory[bp + 3];
            sp = memory[bp + 2];
            bp = memory[bp + 1];
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
    void Instance()
    {
        int steps = memory[ip];
        int link = bp;
        for (int i = steps; i > 0; i--)
        {
            link = memory[link];
        }
        sp = sp + 1;
        memory[sp] = link;
        ip = ip + 1;
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
    void Variable()
    {
        int displ = memory[ip];
        memory[sp] = memory[sp] + displ;
        ip = ip + 1;
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
    void Construct()
    {
        int number = memory[ip];
        int elem, index, i, val;
        int *tempSet = new int[SET_LENGTH];

        //t     printf("Set number = %d\n", number);
        for (i = 0; i < SET_LENGTH; i++)
            tempSet[i] = 0;

        for (i = 0; i < number; i++)
        {
            elem = memory[sp - i];
            if ((elem >= 0) && (elem <= SET_LIMIT))
            {
                index = elem & (SET_LENGTH - 1);
                val = elem >> 3;

                tempSet[index] |= (1 << val);
            }
            else
                printf("Error: element has the value outside set range\n");
        }
        sp = sp - number + 1;
        for (i = 0; i < SET_LENGTH; i++)
        {
            memory[sp + i] = tempSet[i];
        }
        sp = sp + SET_LENGTH - 1;
        ip = ip + 2;
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
    void Constant()
    {
        int value = memory[ip];
        sp = sp + 1;
        memory[sp] = value;
        ip = ip + 1;
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
    void Value()
    {
        int length = memory[ip];
        int varAdd = memory[sp];

        for (int i = 0; i < length; i++)
        {
            memory[sp + i] = memory[varAdd + i];
        }
        sp = sp + length - 1;
        ip = ip + 1;
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
    void ValSpace()
    {
        int length = memory[ip];
        sp = sp + length;
        ip = ip + 1;
    }

    // Not: r = ~v    before: [v, ...    after:  [r, ...
    void Not()
    {
        memory[sp] = ~memory[sp];
    }

    // Mul: r = v1 * v2    before: [v1, v2, ...    after:  [r, ...
    void Mul()
    {
        v2 = memory[sp--];
        memory[sp] = memory[sp] * v2;

        lineNo = memory[ip++];
    }
    // Div: r = v1 / v2    before: [v1, v2, ...    after:  [r, ...
    void Div()
    {
        v2 = memory[sp--];
        if (v2 == 0)
            runError("division by zero");
        v1 = memory[sp];
        memory[sp] = v1 / v2;

        lineNo = memory[ip++];
    }

    // Mod: r = v1 % v2    before: [v1, v2, ...    after:  [r, ...
    void Mod()
    {
        v2 = memory[sp--];
        if (v2 == 0)
            runError("modulus by zero");
        v1 = memory[sp];
        memory[sp] = v1 % v2;

        lineNo = memory[ip++];
    }

    // And: r = v1 & v2    before: [v1, v2, ...    after:  [r, ...
    void And()
    {
        v2 = memory[sp--];
        memory[sp] = memory[sp] & v2;
    }

    // Neg: r = -v         before: [v, ...         after:  [r, ...
    void Neg()
    {
        memory[sp] = -memory[sp];
        ip = ip + 1;
    }

    // Add: r = v1 + v2    before: [v1, v2, ...    after:  [r, ...
    void Add()
    {
        v2 = memory[sp--];
        memory[sp] += v2;

        lineNo = memory[ip++];
    }

    // Sub: r = v1 - v2    before: [v1, v2, ...    after:  [r, ...
    void Subtract()
    {
        v2 = memory[sp--];
        memory[sp] -= v2;

        lineNo = memory[ip++];
    }

    // Or: r = v1 | v2     before: [v1, v2, ...    after:  [r, ...
    void Or()
    {
        v2 = memory[sp--];
        memory[sp] |= v2;
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
    void Equal()
    {
        int length = memory[ip];
        int y = sp - length + 1;
        int x = y - length;
        sp = x;
        bool equal = true;
        for (int i = 0; i < length; i++)
        {
            if (memory[x + i] != memory[y + i])
            {
                equal = false;
                break;
            }
        }
        ip = ip + 1;
        if (equal)
            memory[sp] = 1;
        else
            memory[sp] = 0;
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
    void NotEqual()
    {
        int length = memory[ip];
        int y = sp - length + 1;
        int x = sp - length;
        sp = x;

        memory[sp] = 0;
        for (int i = 0; i < length; i++)
        {
            if (memory[x + i] != memory[y + i])
            {
                memory[sp] = 1;
                break;
            }
        }
        ip = ip + 1;
    }

    // TestForLessThan: r = v1 < v2    before: [v1, v2, ...    after:  [r, ...
    void TestForLessThan()
    {
        v2 = memory[sp--];
        memory[sp] = (memory[sp] < v2) ? 1 : 0;
    }

    // TestForGreaterOrEqual: r = v1 >= v2    before: [v1, v2, ...    after:  [r, ...
    void TestForGreaterOrEqual()
    {
        v2 = memory[sp--];
        memory[sp] = (memory[sp] >= v2) ? 1 : 0;
    }

    // TestForGreater: r = v1 >= v2    before: [v1, v2, ...    after:  [r, ...
    void TestForGreater()
    {
        v2 = memory[sp--];
        memory[sp] = (memory[sp] > v2) ? 1 : 0;
    }

    // TestForGreaterOrEqual: r = v1 <= v2    before: [v1, v2, ...    after:  [r, ...
    void TestForLessOrEqual()
    {
        v2 = memory[sp--];
        memory[sp] = (memory[sp] <= v2) ? 1 : 0;
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
    void Assign()
    {
        int length = memory[ip];
        sp = sp - length - 1;
        int x = memory[sp + 1];
        int y = sp + 2;
        for (int i = 0; i < length; i++)
        {
            memory[x + i] = memory[y + i];
        }
        ip = ip + 1;
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

    void ProcCall()
    {
        int displacement = memory[ip];
        //t         printStack();

        memory[sp + 1] = bp;
        memory[sp + 3] = pe;
        memory[sp + 4] = ip + 1; // next instruction
        bp = sp;
        sp = sp + 4;
        ip = ip + displacement - 1;
        //t         printStack();
    }

    void ProcArg()
    {
        int displacement = memory[ip++];
        //t        printStack();
        memory[++sp] = displacement;
        //t        printStack();
    }

    void printStack()
    {
        printf("ip=%02x [", ip - 1024);
        for (int n = 0; n < 8; n++)
        {
            printf("%02x, ", memory[sp + n]);
        }
        printf("...\n");
    }
    void ParamCall()
    {
        //t        printStack();
        int displacement = memory[ip];
        int addr = memory[sp] + displacement;
        int dest = memory[addr + 1];

        //t        printStack();

        memory[sp] = memory[addr];
        memory[sp + 1] = bp;
        memory[sp + 3] = pe;
        memory[sp + 4] = ip + 1; // next instruction
        bp = sp;
        sp = sp + 4;
        ip = dest;
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
    void Do()
    {
        int displacement = memory[ip];
        if (memory[sp] == 1)
            ip++;
        else
            ip = ip + displacement - 1;
        sp--;
    }

    /**
        *           Program
        *           --------------------------
        *           |   ElseOpcode           |
        *           --------------------------
        *           |   displacement         | <-- ip
        *           --------------------------
        */
    void Else()
    {
        int displacement = memory[ip];
        ip = ip + displacement - 1;
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
    void Wait()
    {
        int displacement = memory[ip];
        ip = ip + displacement - 1;
        preempt();
        if (taskCurrent >= (numberOfTasks - 1))
            taskCurrent = 0;
        else
            taskCurrent++;
        resume();
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
    void Process()
    {
        int tempLength = memory[ip];
        lineNo = memory[ip + 1];

        if ((sp + tempLength) > pe)
        {
            runError("Not enough memory to run a process sp");
        }
        ip += 2;
    }

    /**
        *           Program
        *           --------------------------
        *           |   AlsoOpcode           |
        *           --------------------------
        *           |   displacement         | <-- ip
        *           --------------------------
        */
    void Also()
    {
        int displacement = memory[ip];
        if (numberOfTasks > 1)
        {
            while (taskCurrent < (numberOfTasks - 1))
            {
                taskQueue[taskCurrent] = taskQueue[taskCurrent + 1];
                taskCurrent++;
            }
            numberOfTasks = numberOfTasks - 1;
            taskCurrent = 0;
            resume();
        }
        else
        { // numberOfTasks = 1
            sp = taskStackTop;
            pe = taskProgTop;
            ip = ip + displacement - 1;
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
    void Cobegin()
    {
        int numOfTask = memory[ip];
        lineNo = memory[ip + 1];

        numberOfTasks = numOfTask;
        if (numberOfTasks > MAX_QUEUE)
            runError("Exceeds maximum number of processes");
        taskStackTop = sp;
        taskProgTop = pe;

        int length = (pe - sp) / numOfTask;
        for (int i = 0; i < numOfTask; i++)
        {
            pe = sp + length;
            taskQueue[i] = new Task();
            taskQueue[i]->sp = sp;
            taskQueue[i]->bp = bp;
            taskQueue[i]->pe = pe;
            taskQueue[i]->ip = ip + memory[ip + i * 2 + 3] - 1;
            sp = pe;
        }
        taskCurrent = 0;
        resume();
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
    void Index()
    {
        int index;

        int lower = memory[ip];
        int upper = memory[ip + 1];
        int length = memory[ip + 2];

        lineNo = memory[ip + 3];

        index = memory[sp];
        sp = sp - 1;

        if ((index < lower) || (index > upper))
            runError("index exceeds array dimension");

        memory[sp] = memory[sp] + (index - lower) * length;
        ip += 4;
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
    void Field()
    {
        int displ = memory[ip];
        memory[sp] = memory[sp] + displ;
        ip = ip + 1;
    }
    //------------------------------------------- Extras
    void PutInteger()
    {
        printf("%d", memory[sp]);
        sp = sp - 1;
    }
    void PutCharacter()
    {
        // #if defined(Host)
            printf("%c", (char)memory[sp]);
        // #else
        //     PutC((char)memory[sp]);
        // #endif
        
        sp = sp - 1;
    }
    void PutBoolean()
    {
        printf("%s", (memory[sp] == 0) ? "false" : "true");
        sp = sp - 1;
    }
    void PutLine()
    {
        printf("\n");
    }
    void EndCode()
    {
        //t     printf("The program terminates.\n");
        exit(0);
    }
};

static void usage()
{
    printf("Small Edison VM for Arduino Nano v1.0\n");
    printf("Usage: se <file>.pic\n");
    exit(1);
}

#if Nano
int main()
{
#else
int main(int argc, char **args)
{
#endif
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

    Kernel *kernel = new Kernel();
    kernel->load(file);
    kernel->run();

    return 0;
}
