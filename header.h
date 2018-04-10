#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct _Job
{
    int ArrivalTime, DurationTime, ParameterCount;
    char JobContent[256];
    char** Parameter;
} Job, *JOB;

FILE* outputFile;

void scheduler();