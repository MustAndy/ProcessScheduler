/****************************************************
 * 
 *  1802 - CO004 - Group 2 - Job Schedulers
 *	Menber1:1509853G-I011-0202 WANG,JINGQING
 *  Menber2:1509853G-I011-0297 KUANG,YUHAO
 *  File: main.c
 *
 *  Copyright © 2018 WANG&KUANG. All rights reserved.
 *
 ****************************************************/

#include "header.h"
JOB* Job_list;
FILE* Output_File;
FILE* Input_File;
int job_counter = 0;
int main(int argc, char* argv[])
{
    printf("\033[35;1m"
           "\n\nWelcome to our job scheduler ~!\n\n"
           "\033[0m");

    char pgm[120]; // input micro code file name
    if (argc < 2 || argc > 4)
    {
        printf("\033[31;1m"
               "usage: ./scheduler [input Filename] [Policy] [if-RR-time]\n"
               "\033[0m");
        exit(1);
    }
    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".txt");

    Input_File = fopen(pgm, "r");
    if (Input_File == NULL)
    {
        printf("\033[31;1m"
               "File %s not found\n"
               "\033[0m",
            pgm);
        exit(1);
    }

    ReadFile();
    if (argc == 2)
    {
        printf("Only running phase 1 \n");
        print_Job_Outline();
        printf("\nYou totally input %d files.\n\n\
Using './scheduler %s FCFS' to running scheduler in FCFS policy.\n\n\
Using './scheduler %s RR [Time quota]' to running scheduler in RR policy.\n ",
            job_counter,
            pgm, pgm);
    }
    else if (argc == 3)
    {
        if (strcmp(argv[2], "FCFS") == 0 || strcmp(argv[2], "fcfs") == 0)
        {
            printf("You select to run FCFS!\n Using Ctrl+c to stop the scheduler and check the state of it.\n");
            scheduler(0, 0);
        }
        else
        {
            printf("\033[31;1m"
                   "Please input correct policy! \n"
                   "\033[0m");
        }
    }
    else if (argc == 4)
    {
        if (strcmp(argv[2], "RR") == 0 || strcmp(argv[2], "rr") == 0)
        {
            int timeSelect = atoi(argv[3]);
            printf("You select to run RR, with the timequoat %d\n\nUsing Ctrl+c to stop the scheduler and check the state of it.\n", timeSelect);
            scheduler(1, timeSelect);
        }
        else
        {
            printf("\033[31;1m"
                   "Please input correct policy! \n"
                   "\033[0m");
        }
    }
    return 0;
}