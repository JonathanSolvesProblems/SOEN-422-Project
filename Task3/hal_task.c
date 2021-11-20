#include "hal_task.h"

typedef struct TaskDesc {
    int bp;
    int sp;
    int pe;
    int ip;
} TaskDesc, *Task;

Task t;

Task Task_New(){
    Task t = (Task)malloc(sizeof(TaskDesc));

    bp = sp = pe = ip = 0;

    return t;
}

Task Task_New(uint8_t size){
    Task t = (Task)malloc(sizeof(TaskDesc) * size);

    bp = sp = pe = ip = 0;

    return t;
}