#include "hal_task.h"
#include <stdlib.h>

typedef struct TaskDesc {
    uint16_t bp;
    uint16_t sp;
    uint16_t pe;
    uint16_t ip;
} TaskDesc, *Task;

Task createTask(uint16_t size){
     
    Task t = (Task)malloc(sizeof(TaskDesc) * size);
    
    t->bp = t->sp = t->pe = t->ip = 0;

    return t;
}

Task **createDoubleArrayTask(uint16_t size) {
    
    Task **t = (Task**)malloc(sizeof(TaskDesc) * size);

    return t;
}

uint16_t GetBp(Task t){
    return t->bp;
};
uint16_t GetSp(Task t){
    return t->sp;
};
uint16_t GetPe(Task t){
    return t->pe;
};
uint16_t GetIp(Task t){
    return t->ip;
};


void SetBp(Task t, uint16_t value){
    t->bp = value;
}
void SetSp(Task t, uint16_t value){
    t->sp = value;
}
void SetPe(Task t, uint16_t value){
    t->pe = value;
}
void SetIp(Task t, uint16_t value){
    t->ip = value;
}

// void Task_Delete(Task t){
//     free(t);
// }
