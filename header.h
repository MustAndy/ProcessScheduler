#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Job {
    int ArrivalTime, DurationTime;
    char JobContent[];
} Job, *JOB;

FILE* outputFile;

void scheduler();