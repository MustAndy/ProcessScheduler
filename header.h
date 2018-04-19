#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define _PROCESS_CREATED_ 0;
#define _PROCESS_PAUSE_ 1;
#define _PROCESS_RUNNING_ 2;
#define _PROCESS_KILLED_ 3;
#define _PROCESS_CONTINUE_ 4;

typedef enum {
    FALSE,
    TRUE
} Boolean;

/*  Struct introduction.  ******
    
    For one line of Job.txt.
    3 ls -R / 5.
    The corresponding struct JOB is: 
    int ArrivalTime = 3;
    int DurationTime = 5;
    int RunningTime = 0;
    JobName = "ls";
    char **Parameter = {"ls","-R","/",NULL}
    int state = _PROCESS_CREATED_;
*/
typedef struct _Job
{
    int ArrivalTime, DurationTime, RunningTime, ParameterCounte;
    char JobName[256];
    char** Parameter;
    int state;
    pid_t ProcessID;
} Job, *JOB;

void scheduler();
void RR(JOB arr[], int TimeQuota);
void FCFS(JOB arr[]);
void OutputSchedulingReport(int JobNo, int state);
void PauseProcess(int sig);
extern JOB* Joblist; //0,1,2,3,4...
extern int timer;
extern FILE* outputFile;
extern Boolean hasJob;
void printJobState();