/****************************************************
 * 
 *  1802 - CO004 - Group 2 - Job Schedulers
 *	Menber1:1509853G-I011-0202 WANG,JINGQING
 *  Menber2:1509853G-I011-0297 KUANG,YUHAO
 *  File: util.c
 *
 *  Copyright © 2018 WANG&KUANG. All rights reserved.
 *
 ****************************************************/

#include "header.h"

void print_Job_State()
{
    printf("\nArrival %28c JobContent %9c JobNo %8c State %5c Process_ID %5c Arrival_Time %5c Duration %5c Running_Time \n", ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    int i;
    for (i = 0; Job_list[i] != NULL; i++)
    {
        printf("%5d%40s%15d%15d%15d%15d%15d%15d\n", Job_list[i]->Arrival_Time, Job_list[i]->Job_Name, (i + 1), Job_list[i]->status, Job_list[i]->Process_ID, Job_list[i]->Arrival_Time, Job_list[i]->Duration_Time, Job_list[i]->Running_Time);
    }
}

void print_Job_Outline()
{
    printf("\n JobNo %5c Arrival Time %32c Job_Name %9c Duration Time %5c Parameter_Counter %5c Parameter  %5c  %5c  %5c  \n", ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    int i, j;
    for (i = 0; Job_list[i] != NULL; i++)
    {
        printf("%5d%15d%46s%18d%22d              ", (i + 1), Job_list[i]->Arrival_Time, Job_list[i]->Job_Name, Job_list[i]->Duration_Time, Job_list[i]->Parameter_Counte);
        if (Job_list[i]->Parameter != NULL)
            for (j = 0; j < Job_list[i]->Parameter_Counte; j++)
            {
                printf("%5s", Job_list[i]->Parameter[j]);
                if (j != Job_list[i]->Parameter_Counte - 1)
                    printf(",");
            }
        putchar('\n');
    }
}

void Output_Scheduling_Report(int JobNo, int status)
{
    Output_File = fopen("result.txt", "a+");
    if (Output_File == NULL)
    {
        printf("Fail to open the file...\n");
        exit(-1);
    }
    switch (status)
    {
    case 1:
        fprintf(Output_File, "%3d sec: Job %d suspended\n\n", timer, JobNo);
        break;
    case 2:
        fprintf(Output_File, "%3d sec: Job %d run\n", timer, JobNo);
        break;
    case 3:
        fprintf(Output_File, "%3d sec: Job %d terminated\n\n", timer, JobNo);
        break;
    case 4:
        fprintf(Output_File, "%3d sec: Job %d continue\n", timer, JobNo);
        break;
    }
    fclose(Output_File);
}

/*Make sure the job.txt is in the right order.*/
void bubble_Sort(int length)
{
    if (Job_list == NULL)
        return;
    int i, j;
    for (i = 0; i < length; i++)
    {
        for (j = i + 1; j < length; j++)
        {
            if ((Job_list[i]->Arrival_Time) > (Job_list[j]->Arrival_Time))
            {
                JOB temp = Job_list[i];
                Job_list[i] = Job_list[j];
                Job_list[j] = temp;
            }
            else if ((Job_list[i]->Arrival_Time) == (Job_list[j]->Arrival_Time))
            {
                if ((Job_list[i]->Duration_Time) > (Job_list[j]->Duration_Time))
                {
                    JOB temp = Job_list[i];
                    Job_list[i] = Job_list[j];
                    Job_list[j] = temp;
                }
            }
        }
    }
}