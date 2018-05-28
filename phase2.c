/****************************************************
 * 
 *  1802 - CO004 - Group 2 - Job Schedulers
 *	Menber1:1509853G-I011-0202 WANG,JINGQING
 *  Menber2:1509853G-I011-0297 KUANG,YUHAO
 *  File: phase2.c
 *
 *  Copyright © 2018 WANG&KUANG. All rights reserved.
 *
 ****************************************************/

#include "header.h"
int timer = 0;
Boolean has_Job = 0;
int Job_Pointer = 0;

void RR(int Time_Quota)
{
    int status;
    if (Job_list[0] != NULL) //check the Job list is NUUL or not.
        has_Job = 1;
    //Job counter.
    int j = 0; //Job counter.

    while (has_Job)
    {
        pid_t ProcessControler = vfork();
        /*Using vfork to let child process share 
        the same memory with the parent process, 
        and it also can force the child process 
        running first.*/

        if (ProcessControler == 0)
        {
            /*Here is the Process controller.
            1. The controller will check that if current job has been created or not.
            2. If there is not any process create for the job, it will create 
                a child process to do exec() to create a new process for Job. 
            3. If there is a process create for the job, it will do a single round
                of RR. */
            if (Job_list[Job_Pointer]->Process_ID == 0)
            {
                /*If the current job doesn't have a process, it will go in this branch.*/
                Job_list[Job_Pointer]->Process_ID = fork(); //Fork one process, and record it into the struct.

                if (Job_list[Job_Pointer]->Process_ID == 0)
                {
                    kill(getpid(), SIGSTOP); //Suspended itself, and go back to the father process first.
                    printf("\nrunning %dth JOB\n", (Job_Pointer + 1));
                    execvp(Job_list[Job_Pointer]->Job_Name, Job_list[Job_Pointer]->Parameter);
                    //execl(Job_list[Job_Pointer]->Job_Name, Job_list[Job_Pointer]->Job_Name, NULL); //Create a process for the Job.
                    exit(0);
                }
                else
                {
                    /*Wait for the child process send the SIGSTOP signal.*/
                    waitpid(Job_list[Job_Pointer]->Process_ID, &status, WUNTRACED);
                    int Remaining_Time_1 = (Job_list[Job_Pointer]->Duration_Time - Job_list[Job_Pointer]->Running_Time);

                    /*This if-else statussment is using to judge whether the remaining time of 
                    the Job still enough for the single RR.
                    1. Larager: do a single RR.
                    2. Equal-Smaller: Just finish the remain part of the job.
                    3. Duration equals to 0.
                    */
                    if (Remaining_Time_1 > Time_Quota)
                    {
                        /*The following 4 line is use to calculate the global timer, and 
                        output the result to .txt*/
                        Job_list[Job_Pointer]->status = _PROCESS_RUNNING_;
                        Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);
                        Job_list[Job_Pointer]->Running_Time += Time_Quota;
                        timer += Time_Quota;

                        /*Running single RR.*/
                        kill(Job_list[Job_Pointer]->Process_ID, SIGCONT);
                        sleep(Time_Quota);
                        kill(Job_list[Job_Pointer]->Process_ID, SIGSTOP);

                        /*Change the Job's status and output result to .txt*/
                        Job_list[Job_Pointer]->status = _PROCESS_PAUSE_;
                        Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);
                    }
                    else if (Job_list[Job_Pointer]->Duration_Time == 0)
                    {
                        /*For those duration time equals to 0.*/
                        Job_list[Job_Pointer]->status = _PROCESS_RUNNING_;
                        Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);

                        kill(Job_list[Job_Pointer]->Process_ID, SIGCONT);
                        wait(NULL);
                        Job_list[Job_Pointer]->status = _PROCESS_KILLED_;
                        Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);
                    }
                    else
                    {
                        /*The following 4 line is use to calculate the global timer, and 
                        output the result to .txt*/
                        Job_list[Job_Pointer]->status = _PROCESS_RUNNING_;
                        Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);
                        timer += Remaining_Time_1;

                        /*Running the remain part of job.*/
                        kill(Job_list[Job_Pointer]->Process_ID, SIGCONT);
                        sleep(Remaining_Time_1);
                        kill(Job_list[Job_Pointer]->Process_ID, SIGKILL);

                        /*Kill the Job's process, output the result to the .txt*/
                        Job_list[Job_Pointer]->status = _PROCESS_KILLED_;
                        Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);
                        Job_list[Job_Pointer]->Running_Time += Remaining_Time_1;
                    }
                    exit(0); //vfork() ask the child process to do exit().
                }
            }
            else
            { /*If the current job have had a process, it will go in this branch.*/
                int Remaining_Time = (Job_list[Job_Pointer]->Duration_Time - Job_list[Job_Pointer]->Running_Time);
                if (Remaining_Time > Time_Quota)
                { /*If the remained time is still bigger than time quota,
                    the process controller will run single round, and stop
                    the process.*/

                    //Change the status and calculate the global timer.
                    Job_list[Job_Pointer]->status = _PROCESS_CONTINUE_;
                    Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);
                    timer += Time_Quota;
                    Job_list[Job_Pointer]->Running_Time += Time_Quota;

                    //Control
                    printf("\nrunning %dth JOB\n", (Job_Pointer + 1));
                    kill(Job_list[Job_Pointer]->Process_ID, SIGCONT);
                    sleep(Time_Quota);
                    kill(Job_list[Job_Pointer]->Process_ID, SIGSTOP);

                    //back to father process, and output the result.
                    Job_list[Job_Pointer]->status = _PROCESS_PAUSE_;
                    Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);
                }
                else if (Remaining_Time == 0)
                { /*The process has been finished.*/
                }
                else
                { /*If the remained time is less than time quota, 
                it will directly finish the remain part of job.*/

                    //calculate the timer.
                    Job_list[Job_Pointer]->status = _PROCESS_CONTINUE_;
                    Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);
                    timer += Remaining_Time;
                    Job_list[Job_Pointer]->Running_Time += Remaining_Time;

                    //Control!
                    printf("\nrunning %dth JOB\n", (Job_Pointer + 1));
                    kill(Job_list[Job_Pointer]->Process_ID, SIGCONT);
                    sleep(Remaining_Time);
                    kill(Job_list[Job_Pointer]->Process_ID, SIGKILL);

                    //back to father process and output result.
                    Job_list[Job_Pointer]->status = _PROCESS_KILLED_;
                    Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);
                }
                exit(0);
            }
        }
        else
        {
            wait(NULL); /*Wait for a single round of the controller finished,
                        but Job_Pointer use vfork() to create the controller process.
                        Whether this wait() is necessary, Job_Pointer don't know. =,=...
                        */

            if (Job_list[Job_Pointer + 1] == NULL)
            { /*If the Job_list[Job_Pointer+1]==NULL,it shows that the scheduler 
            comes to the last Job of the JOB list.*/

                /*The following next-job-finder is use to find a unfinished job.
                It will simply check every job from Job_Pointer th job to itself one times.*/
                for (j = Job_Pointer + 1; j != Job_Pointer; j++)
                {
                    if (Job_list[j] == NULL) //To form a checking loop.
                    {
                        j = 0;
                    }
                    if (Job_list[j]->Running_Time != Job_list[j]->Duration_Time)
                    //Running_Time less than Duration shows the job didn't finished.
                    {
                        Job_Pointer = j;
                        break;
                    }
                }

                /*After the above finder, if there is an unfinished job, the Job_Pointer-th job
                    repersent the unfinished job; if there is not a unfinished job, we need
                    to check if the Job_Pointer-th job is an unfinished job, so we need to recheck if
                    the Job_Pointer-th job is a real unfinished job. If Job_Pointer-th job is a unfinished job, 
                    we will continue to run Job_Pointer-th job, if it isn't, it shows all the jobs have
                    been finished*/
                if (Job_list[Job_Pointer]->Running_Time == Job_list[Job_Pointer]->Duration_Time)
                    has_Job = 0;
            }
            else
            { /*If the next Job haven't arrived, it shouldn't begin the next job. 
            If the global timer less than the next job's arrival time, it means the 
            next job haven't arrived.*/
                if (timer < Job_list[Job_Pointer + 1]->Arrival_Time)
                {
                    for (j = 0; j <= Job_Pointer; j++)
                    /*This for-loop is to check if those arrived jobs have been finished.
                    It use the same logic with the above next-job-finder.*/
                    {
                        if (Job_list[j]->Running_Time != Job_list[j]->Duration_Time)
                        {
                            Job_Pointer = j;
                            break;
                        }
                    }
                    /*Same as the above finder. if Job_Pointer-th job isn't an unfinished job, we need to 
                    hold on the scheduler to wait for the next Job arrival.*/
                    if (Job_list[Job_Pointer]->Running_Time == Job_list[Job_Pointer]->Duration_Time)
                    {
                        printf("\n\nWaiting for next job coming...\n");
                        sleep(Job_list[Job_Pointer + 1]->Arrival_Time - timer);
                        timer += ((Job_list[Job_Pointer + 1]->Arrival_Time) - timer);
                        ++Job_Pointer;
                    }
                }
                else
                {
                    /*If the next job have arrived, continue.*/
                    ++Job_Pointer;
                }
            }
        }
    }
}

/*Because we sort the job list before scheduler, we can directly
use a while-loop to run every job one by one */
void FCFS()
{
    if (Job_list[0] != NULL) //check the Job list is NUUL or not.
        has_Job = 1;

    while (has_Job)
    { /*Using vfork to let child process share 
        the same memory with the parent process, 
        and it also can force the child process 
        running first.*/
        Job_list[Job_Pointer]->Process_ID = vfork();

        if (Job_list[Job_Pointer]->Process_ID == 0)
        { /*Child process.*/

            /*Change the status and output the result.*/
            printf("running %dth JOB\n", (Job_Pointer + 1));
            Job_list[Job_Pointer]->status = _PROCESS_RUNNING_;
            Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);

            /*Using alarm() to stop the process after the durationTime.*/
            alarm(Job_list[Job_Pointer]->Duration_Time);
            execvp(Job_list[Job_Pointer]->Job_Name, Job_list[Job_Pointer]->Parameter);
            //execl(Job_list[Job_Pointer]->Job_Name, Job_list[Job_Pointer]->Job_Name, NULL);
            exit(0);
        }
        else
        {
            wait(NULL);
            /*To make sure there is no any zombine left.*/
            kill(Job_list[Job_Pointer]->Process_ID, SIGKILL);
            printf("The %dth job has finished\n\n", (Job_Pointer + 1));

            /*Change the status and output the result.*/
            Job_list[Job_Pointer]->status = _PROCESS_KILLED_;
            Job_list[Job_Pointer]->Running_Time = Job_list[Job_Pointer]->Duration_Time;
            timer += Job_list[Job_Pointer]->Duration_Time;
            Output_Scheduling_Report((Job_Pointer + 1), Job_list[Job_Pointer]->status);

            /*To check the exit status, if there is not any job, exit the scheduler*/
            if (Job_list[Job_Pointer + 1] == NULL)
            {
                has_Job = FALSE;
            }
            else
            {
                /*Check if next job has been arrived. If it doesn;t arrived, wait for it coming.*/
                if (timer < Job_list[Job_Pointer + 1]->Arrival_Time)
                {
                    printf("\nWaiting for next job coming...\n\n");
                    int j = Job_list[Job_Pointer + 1]->Arrival_Time - timer;
                    timer += ((Job_list[Job_Pointer + 1]->Arrival_Time) - timer);
                    sleep(j);
                }

                /*continue to next job.*/
                Job_Pointer++;
            }
        }
    }
}

void scheduler(int policy, int time_Select)
{
    Output_File = fopen("result.txt", "w");
    if (Output_File == NULL)
    {
        printf("Fail to open the file...\n");
        exit(-1);
    }
    fclose(Output_File);

    bubble_Sort(job_counter);
    print_Job_Outline();

    if (policy == 0)
        FCFS();
    else if (policy == 1)
        RR(time_Select);

    print_Job_State();
}
