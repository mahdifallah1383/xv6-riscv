#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "kernel/spinlock.h"
#include "kernel/proc.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    struct child_processes cp;

    for (int i = 0; i < 5; i++){
        int p1 = fork();
        if (p1 == 0)
        {
            int p2 = fork();
            if (p2 == 0)
                sleep(5);
            else
            {
                wait(0);
                sleep(3);
            }
            exit(0);    
        }
    }
    sleep(1);

    int error = childProc(&cp);

    if (error < 0) {
        printf("Error calling child_processes system call\n");
        exit(-1);
    }

    printf("Number of child processes: %d\n", cp.count);
    printf("PID\tPPID\tSTATE\t\tNAME\n");

    for (int i = 0; i < cp.count; i++) {
        char state[9];
        switch (cp.processes[i].state)
        {
            case 0:
                strcpy(state, "UNUSED  ");
                break;
            case 1:
                strcpy(state, "USED    ");
                break;
            case 2:
                strcpy(state, "SLEEPING");
                break;
            case 3:
                strcpy(state, "RUNNABLE");
                break;
            case 4:
                strcpy(state, "RUNNING ");
                break;
            case 5:
                strcpy(state, "ZOMBIE  ");   
                break;
            default:
                strcpy(state, "UNKNOWN ");
                break;
        }

        printf("%d\t%d\t%s\t%s\n",
            cp.processes[i].pid,
            cp.processes[i].ppid,
            state,
            cp.processes[i].name);
    }
    wait(0);

    exit(error);
    return error;
}