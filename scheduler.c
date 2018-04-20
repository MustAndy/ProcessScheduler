#include "header.h"
int timer = 0;
Boolean hasJob = 0;
void PauseProcess(int sig)
{
    printf("The Process has been paused.\n");
}

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
void handler(int sig)
{
}
void RR(JOB arr[], int TimeQuota)
{
    int status;
    if (arr[0] != NULL)
        hasJob = 1;
    int i = 0;
    int j = 0;

    while (hasJob)
    {
        pid_t ProcessControler = vfork();
        if (ProcessControler == 0)
        {
            if (arr[i]->ProcessID == 0)
            {
                arr[i]->ProcessID = fork();

                if (arr[i]->ProcessID == 0)
                {
                    kill(getpid(), SIGTSTP);
                    //sighold
                    //kill(getppid(), SIGCHLD);
                    //signal(SIGCONT, PauseProcess);
                    printf("running %dth JOB\n", (i + 1));
                    execl(arr[i]->JobName, arr[i]->JobName, NULL);
                    exit(0);
                }
                else
                {
                    //waitpid(arr[i]->ProcessID, &status, 0);
                    //waitpid(arr[i]->ProcessID, 0);
                    //wait(NULL);
                    printf("I go first~\n");
                    int RemainingTime1 = (arr[i]->DurationTime - arr[i]->RunningTime);
                    if (RemainingTime1 > TimeQuota)
                    {
                        arr[i]->state = _PROCESS_RUNNING_;
                        OutputSchedulingReport((i + 1), arr[i]->state);
                        arr[i]->RunningTime += TimeQuota;
                        timer += TimeQuota;

                        kill(arr[i]->ProcessID, SIGCONT);
                        sleep(TimeQuota);
                        //nanosleep(TimeQuota * 1000 - 100);
                        kill(arr[i]->ProcessID, SIGSTOP);
                        arr[i]->state = _PROCESS_PAUSE_;
                        OutputSchedulingReport((i + 1), arr[i]->state);
                    }
                    else
                    {
                        arr[i]->state = _PROCESS_RUNNING_;
                        OutputSchedulingReport((i + 1), arr[i]->state);
                        timer += RemainingTime1;
                        kill(arr[i]->ProcessID, SIGCONT);
                        sleep(RemainingTime1);
                        kill(arr[i]->ProcessID, SIGKILL);
                        arr[i]->state = _PROCESS_KILLED_;
                        OutputSchedulingReport((i + 1), arr[i]->state);

                        //OutputSchedulingReport((i + 1), arr[i]->state);
                        arr[i]->RunningTime += RemainingTime1;
                    }
                    exit(0);
                }
            }
            else
            {
                int RemainingTime = (arr[i]->DurationTime - arr[i]->RunningTime);
                //printf("remaining: %d\n", RemainingTime);
                if (RemainingTime >= TimeQuota)
                {
                    arr[i]->state = _PROCESS_CONTINUE_;
                    OutputSchedulingReport((i + 1), arr[i]->state);
                    timer += TimeQuota;
                    printf("running %dth JOB\n", (i + 1));
                    arr[i]->RunningTime += TimeQuota;
                    kill(arr[i]->ProcessID, SIGCONT);
                    sleep(TimeQuota);
                    kill(arr[i]->ProcessID, SIGSTOP);
                    arr[i]->state = _PROCESS_KILLED_;
                    OutputSchedulingReport((i + 1), arr[i]->state);
                }
                else if (RemainingTime == 0)
                {
                    kill(arr[i]->ProcessID, SIGKILL);
                    arr[i]->state = _PROCESS_KILLED_;
                    OutputSchedulingReport((i + 1), arr[i]->state);
                }
                else
                {
                    arr[i]->state = _PROCESS_CONTINUE_;
                    OutputSchedulingReport((i + 1), arr[i]->state);
                    timer += RemainingTime;
                    printf("running %dth JOB\n", (i + 1));
                    kill(arr[i]->ProcessID, SIGCONT);
                    sleep(RemainingTime);
                    arr[i]->RunningTime += RemainingTime;
                    kill(arr[i]->ProcessID, SIGKILL);
                    arr[i]->state = _PROCESS_KILLED_;
                    OutputSchedulingReport((i + 1), arr[i]->state);
                }
                exit(0);
            }
        }
        else
        {
            wait(NULL);
            //printJobState();
            //printf("timer:%d i: %d\n ", timer, i);
            if (arr[i + 1] != NULL)
            {
                if (timer < arr[i + 1]->ArrivalTime)
                {
                    for (j = 0; j <= i; j++)
                    {
                        if (arr[j]->RunningTime != arr[j]->DurationTime)
                        {
                            i = j;
                            break;
                        }
                    }
                    if (arr[i]->RunningTime == arr[i]->DurationTime)
                    {
                        sleep(arr[i + 1]->ArrivalTime - timer);
                        timer += ((arr[i + 1]->ArrivalTime) - timer);
                        ++i;
                    }
                }
                else
                {
                    ++i;
                }
            }
            else
            {
                for (j = i + 1; j != i; j++)
                {
                    if (arr[j] == NULL)
                    {
                        j = 0;
                    }
                    if (arr[j]->RunningTime != arr[j]->DurationTime)
                    {
                        i = j;
                        break;
                    }
                }
                if (arr[i]->RunningTime == arr[i]->DurationTime)
                    hasJob = 0;
            }
        }
    }
}

void FCFS(JOB arr[])
{
    if (arr[0] != NULL)
        hasJob = 1;
    int i = 0;
    while (hasJob)
    {
        arr[i]->ProcessID = vfork();
        if (arr[i]->ProcessID == 0)
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
            kill(arr[i]->ProcessID, SIGKILL);
            printf("The %dth job has finished\n\n", (i + 1));
            arr[i]->state = _PROCESS_KILLED_;
            arr[i]->RunningTime = arr[i]->DurationTime;
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
    int a[6] = { 4, 3, 2, 1, 0, 20 }, b[6] = { 5, 4, 3, 2, 1, 6 };
    //char c[6][10] = { "ls -R / 2", "./timer 2", "./timer 4", "./while1", "./timer 2", "./timer 2" };
    char c[6][10] = { "./while1", "./while1", "./while1", "./while1", "./while1", "./while1" };
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
    printJobState();
    RR(Joblist, 3);
    //FCFS(Joblist);
    printJobState();
}

void printJobState()
{
    printf("\nArrival %25c JobContent %9c JobNo %5c State %5c ProcessID %5c ArrivalTime %5c Duration %5c RunningTime \n", ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    int i;
    for (i = 0; Joblist[i] != NULL; i++)
    {
        printf("%5d%40s%15d%15d%15d%15d%15d%15d\n", Joblist[i]->ArrivalTime, Joblist[i]->JobName, (i + 1), Joblist[i]->state, Joblist[i]->ProcessID, Joblist[i]->ArrivalTime, Joblist[i]->DurationTime, Joblist[i]->RunningTime);
    }
}