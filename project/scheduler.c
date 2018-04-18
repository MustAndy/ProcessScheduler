#include "header.h"
int timer = 0;
Boolean hasJob = 0;
void OutputSchedulingReport(int JobNo, int state)
{
    // #define _PROCESS_CREATED_ 0;
    // #define _PROCESS_PAUSE_ 1;
    // #define _PROCESS_RUNNING_ 2;
    // #define _PROCESS_KILLED_ 3;
    // #define _PROCESS_CONTINUE_ 4;
    outputFile = fopen("result.txt", "a+");
    if (outputFile == NULL)
    {
        printf("Fail to open the file...\n");
        exit(-1);
    }
    switch (state)
    {
    case 1:
        fprintf(outputFile, "%3d sec: Job %d suspended\n", timer, JobNo);
        break;
    case 2:
        fprintf(outputFile, "%3d sec: Job %d run\n", timer, JobNo);
        break;
    case 3:
        fprintf(outputFile, "%3d sec: Job %d terminated\n", timer, JobNo);
        break;
    case 4:
        fprintf(outputFile, "%3d sec: Job %d continue\n", timer, JobNo);
        break;
    }
    fclose(outputFile);
}
void bubbleSort(JOB arr[], int length)
{
    if (arr == NULL)
        return;
    int i, j;
    for (i = 0; i < length; i++)
    {
        for (j = i + 1; j < length; j++)
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

void RR(JOB arr[], int JobCounter, int TimeQuota)
{
}

void FCFS(JOB arr[])
{
    if (arr[0] != NULL)
        hasJob = 1;
    int i = 0;
    while (hasJob)
    {
        pid_t process_id = fork();
        if (process_id == 0)
        {
            printf("running %dth JOB\n", (i + 1));
            arr[i]->state = _PROCESS_RUNNING_;
            OutputSchedulingReport((i + 1), arr[i]->state);

            alarm(arr[i]->DurationTime);
            execl(arr[i]->JobName, arr[i]->JobName, NULL);
            exit(0);
        }
        else
        {
            wait(NULL);
            kill(process_id, SIGKILL);
            printf("The %dth job has finished\n\n", (i + 1));
            arr[i]->state = _PROCESS_KILLED_;
            timer += arr[i]->DurationTime;
            OutputSchedulingReport((i + 1), arr[i]->state);
            if (arr[i + 1] == NULL)
            {
                hasJob = FALSE;
            }
            else
            {
                i++;
                if (timer < arr[i]->ArrivalTime)
                {
                    int j = arr[i]->ArrivalTime - timer;
                    timer += ((arr[i]->ArrivalTime) - timer);
                    sleep(j);
                }
            }
        }
    }
}

void scheduler()
{
    int a[6] = { 4, 3, 2, 1, 0, 15 }, b[6] = { 0, 2, 4, 3, 2, 2 };
    //char c[6][10] = { "ls -R / 2", "./timer 2", "./timer 4", "./while1", "./timer 2", "./timer 2" };
    char c[6][10] = { "ls", "./while1", "./while1", "./while1", "./while1", "./while1" };
    int jobcounter = 6;
    int i;
    Joblist = (JOB*)malloc(jobcounter * sizeof(struct _Job));
    for (i = 0; i < jobcounter; i++)
    {
        Joblist[i] = (JOB)malloc(sizeof(struct _Job));
        Joblist[i]->ArrivalTime = a[i];
        Joblist[i]->DurationTime = b[i];
        strcpy(Joblist[i]->JobName, c[i]);
        Joblist[i]->RunningTime = 0;
        Joblist[i]->state = _PROCESS_CREATED_;
    }
    bubbleSort(Joblist, jobcounter);
    printf("\nArrival           Job content     Duration\n");
    for (i = 0; i < jobcounter; i++)
    {
        printf("%5d  %20s  %8d\n", Joblist[i]->ArrivalTime, Joblist[i]->JobName, Joblist[i]->DurationTime);
    }

    FCFS(Joblist);
    printf("\nArrival           Job content     State\n");
    for (i = 0; i < jobcounter; i++)
    {
        printf("%5d  %20s  %8d\n", Joblist[i]->ArrivalTime, Joblist[i]->JobName, Joblist[i]->state);
    }
}