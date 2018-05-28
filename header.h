/****************************************************
 * 
 *  1802 - CO004 - Group 2 - Job Schedulers
 *	Menber1:1509853G-I011-0202 WANG,JINGQING
 *  Menber2:1509853G-I011-0297 KUANG,YUHAO
 *  File: header.h
 *
 *  Copyright © 2018 WANG&KUANG. All rights reserved.
 *
 ****************************************************/
#pragma once
#ifndef MY_H_FILE
#define MY_H_FILE

#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define _PROCESS_CREATED_ 0
#define _PROCESS_PAUSE_ 1
#define _PROCESS_RUNNING_ 2
#define _PROCESS_KILLED_ 3
#define _PROCESS_CONTINUE_ 4

typedef enum
{
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
    int status = _PROCESS_CREATED_;
*/
typedef struct _Job
{
    int Arrival_Time, Duration_Time, Running_Time, Parameter_Counte;
    char Job_Name[256];
    char** Parameter;
    int status;
    pid_t Process_ID;
} Job, *JOB;

/*
 *The struct array to store the information of Job.
 */
extern JOB* Job_list;

/*
 *The global timer.
 */
extern int timer;

/*
 *The global job pointer.
 */
extern int Job_Pointer;

/*
 *The input and output file.
 */
extern FILE* Output_File;
extern FILE* Input_File;

/*
 *The bool for the loop in the scheduler algorithm.
 */
extern Boolean has_job;

/*
 *The counter for recording of job number.
 */
extern int job_counter;

/*
* The entrance of the phase 1.
*/
void ReadFile();
/*
* Handle the input job file, split them into every parameter and store into the struct.
*/
void splits(char buff[270], int Job_No);

/*
*The entrance of the phase 2.
*/
void scheduler(int policy, int time_Select);

/*
*  The scheduler policy.
*/
void RR(int Time_Quota);
void FCFS();

/*
* Print out the outline for phase1
*/
void print_Job_Outline();

/*
*  Print out the finish state of every job.
*/
void print_Job_State();

/*
*  Output the report to the result.txt file.
*/
void Output_Scheduling_Report(int Job_No, int status);

void bubble_Sort(int length);

#endif