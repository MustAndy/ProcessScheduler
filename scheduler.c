#include "header.h"

void bubbleSort(JOB arr[], int length)
{
    if (arr == NULL)
        return;

    for (int i = 0; i < length; i++)
    {
        for (int j = i + 1; j < length; j++)
        {
            if ((arr[i]->ArrivalTime) > (arr[j]->ArrivalTime))
            {
                JOB temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void FCFS(JOB arr[], int length)
{
    int status = 0;
    for (int i = 0; i < length; i++)
    {
        int child = fork();
        // if (child == 0)
        // {
        //     printf("wwww\n");
        //     exit(0);
        // }
        if (child == 0)
        {
            int child1 = fork();
            if (child1 == 0)
            {
                printf("running while1\n");
                char* aimFile = arr[i]->JobContent;
                execl(aimFile, aimFile, NULL);
                exit(0);
            }
            else
            {
                sleep(arr[i]->DurationTime);
                kill(child1, SIGUSR1);
                exit(0);
            }
        }
        else
        {
            wait(NULL);
            printf("Parent process with pid: %d\n", getpid());
            printf("The task is completed!\n");
        }
        wait(&status);
    }
}

void scheduler()
{
    int a[6] = { 4, 3, 2, 1, 0, 15 }, b[6] = { 0, 2, 4, 3, 2, 2 };
    //char c[6][10] = { "ls -R / 2", "./timer 2", "./timer 4", "./while1", "./timer 2", "./timer 2" };
    char c[6][10] = { "ls", "./timer", "./timer", "./while1", "./timer", "./timer" };
    int jobcounter = 6;
    JOB Joblist[jobcounter];
    for (int i = 0; i < jobcounter; i++)
    {
        Joblist[i] = (JOB)malloc(sizeof(struct _Job));
        Joblist[i]->ArrivalTime = a[i];
        Joblist[i]->DurationTime = b[i];
        strcpy(Joblist[i]->JobContent, c[i]);
    }
    bubbleSort(Joblist, jobcounter);

    for (int i = 0; i < jobcounter; i++)
    {
        printf("%3d  %20s  %3d\n", Joblist[i]->ArrivalTime, Joblist[i]->JobContent, Joblist[i]->DurationTime);
    }

    FCFS(Joblist, jobcounter);
}