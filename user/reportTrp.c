// A Simple C program
#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "kernel/spinlock.h"
#include "kernel/proc.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    for (int i = 0; i < 2; i++)
    {
        int p1 = fork();
        if (p1 == 0)
        {
            int p2 = fork();
            if (p2 == 0)
            {
                int *x = 0;
                *x = 4;
                sleep(4);
            }
            else
            {
                printf("i:%d\tsecond pid: %d\n", i, p2);
                int *x = 0;
                *x = 4;
                wait((int *)0);
                sleep(3);
            }
            exit(0);
        }
        printf("i:%d\tfirst pid: %d\n", i, p1);
    }
    sleep(3);

    struct report_traps rps;
    int error = reportTrp(&rps);

    printf("number of exceptions: %d\n", rps.count);
    printf("PID\tPNAME\t\tscause\t\t\tsepc\t\t\tstval\n");
    for (struct report *rp = rps.reports; rp < &rps.reports[rps.count]; rp++)
    {
        printf("%d\t%s\t0x00000000000000%lx\t0x0000000000000%ld\t0x00000000000000%ld\n",
        rp->pid,
        rp->pname,
        rp->scause,
        rp->sepc,
        rp->stval);
    }
    wait((int *)0);
    wait((int *)0);

    exit(error);
}