#include "Task.h"
#include <stdint.h>
#include <stdlib.h> // malloc, free

typedef struct TaskDesc
{
    int8_t bp;
    int8_t sp;
    int8_t pe;
    int8_t ip;
} TaskDesc, *Task;

void Task_Init(Task t) {
    t->bp=0;
    t->sp=0;
    t->pe=0;
    t->ip=0;
}

Task Task_New () {
    Task t = (Task) malloc(sizeof(TaskDesc)); // Allocate space to the task
    Task_Init( t );
    return t;
}

void Task_Set(Task t, int8_t bp, int8_t sp, int8_t pe, int8_t ip) {
    t->bp = bp;
    t->sp = sp;
    t->pe = pe;
    t->ip = ip;
}

int8_t Task_Get_bp(Task t) {
    return t->bp;
}

int8_t Task_Get_sp(Task t) {
    return t->sp;
}

int8_t Task_Get_pe(Task t) {
    return t->pe;
}

int8_t Task_Get_ip(Task t) {
    return t->ip;
}

void Task_Delete(Task t) {
    free(t);
}
