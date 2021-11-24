#ifndef Kernel_h
#define Kernel_h

#include <stdlib.h> // exit, EXIT_FAILURE
#include <stdint.h>
#include <string.h> // string
#include <stdio.h>

#include "COutForAUnit.h"

#define INVALID -1
#define ADD 0
#define ALSO 1
#define AND 2
#define ASSIGN 3
#define BLANK 4
#define COBEGIN 5
#define CONSTANT 6
#define CONSTRUCT 7
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
#define GOTO 21
#define GREATER 22
#define INDEX 24
#define INSTANCE 25
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

struct KernelDesc; // Forward ref
typedef struct KernelDesc *Kernel; // Opaque type

void Kernel_Init(Kernel k);
Kernel Kernel_New(void);
void Kernel_Delete(Kernel k);
void load(Kernel k, FILE *input);
void loadMem(Kernel k, uint8_t *mem);
void loadInput(Kernel k, int16_t *input);
void run(Kernel k);
void runError(const char *msg);
void preempt(Kernel k);
void resume(Kernel k);
void Goto(Kernel k);
void Procedure(Kernel k);
void EndProc(Kernel k);
void Instance(Kernel k);
void Variable(Kernel k);
void Construct(Kernel k);
void Constant(Kernel k);
void Value(Kernel k);
void ValSpace(Kernel k);
void Not(Kernel k);
void Mul(Kernel k);
void Div(Kernel k);
void Mod(Kernel k);
void And(Kernel k);
void Neg(Kernel k);
void Add(Kernel k);
void Subtract(Kernel k);
void Or(Kernel k);
void Equal(Kernel k);
void NotEqual(Kernel k);
void TestForLessThan(Kernel k);
void TestForGreaterOrEqual(Kernel k);
void TestForGreater(Kernel k);
void TestForLessOrEqual(Kernel k);
void Assign(Kernel k);
void ProcCall(Kernel k);
void ProcArg(Kernel k);
void printStack(Kernel k);
void ParamCall(Kernel k);
void Do(Kernel k);
void Else(Kernel k);
void When(Kernel k);
void Wait(Kernel k);
void EndWhen(Kernel k);
void Process(Kernel k);
void Also(Kernel k);
void Cobegin(Kernel k);
void Index(Kernel k);
void Field(Kernel k);
void PutInteger(Kernel k);
void PutCharacter(Kernel k);
void PutBoolean(Kernel k);
void PutLine(Kernel k);
void EndCode(Kernel k);



#endif