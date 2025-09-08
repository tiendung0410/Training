#include "OS.h"
#include "App_ButtonTask.h"
#include "App_CanTask.h"
#include "Dio.h"
#include "Can.h"
#include "IoHwAb.h"
#include "CanIf.h"

void System_Init(void)
{
    // MCAL Init
    Dio_Init(&Dio_Config);
    Can_Init(&Can_Config);

    // Ecu Abstraction/IoHwAb layer
    IoHwAb_Init();
    CanIf_Init();

    // Application layer Init
    App_ButtonTask_Init();
    App_CanTask_Init();
}

// Task cyclic 10ms
void CyclicTask10ms(void* arg)
{
    printf("CyclicTask10ms running...\n");
    int count=0;
    (void)arg;
    while (1) {
        App_ButtonTask_Run();
        App_CanTask_Run();
        Os_Sleep(100); // period 100ms
        if (count++ >= 15) break; // run 10s then exit
    }
    //pthread_exit(NULL);
    pthread_detach(pthread_self());
}


int main(void)
{
    // Init system
    System_Init();

    // OS task init
    OsTaskType cyclic10msTask;

    Os_CreateTask(&cyclic10msTask, CyclicTask10ms, NULL, 10, "Cyclic10msTask");
    // Start  task
    Os_StartTask(&cyclic10msTask);
    printf("===== ECU1 Init Done. =====\n");
    // Main idle loop
    Os_Sleep(5000);
    //pthread_join(cyclic10msTask.thread, NULL);
    return 0;
}


