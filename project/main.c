#include "header.h"
JOB* Joblist;
FILE* outputFile;
int main()
{

    printf("hello\n");
    // JOB a = NULL;
    // a->ArrivalTime = 3;
    // a->DurationTime = 3;
    // strcpy(a->JobContent, "aaa");
    // printf("%d", a->ArrivalTime);
    outputFile = fopen("result.txt", "w");
    if (outputFile == NULL)
    {
        printf("Fail to open the file...\n");
        exit(-1);
    }
    fclose(outputFile);
    scheduler();

    return 0;
}