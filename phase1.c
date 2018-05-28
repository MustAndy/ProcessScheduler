/****************************************************
 * 
 *  1802 - CO004 - Group 2 - Job Schedulers
 *	Menber1:1509853G-I011-0202 WANG,JINGQING
 *  Menber2:1509853G-I011-0297 KUANG,YUHAO
 *  File: phase1.c
 *
 *  Copyright © 2018 WANG&KUANG. All rights reserved.
 *
 ****************************************************/

#include "header.h"

void ReadFile()
{
    char buff[270];

    while (fgets(buff, sizeof(buff), Input_File) != NULL)
    {
        job_counter++;
    }

    rewind(Input_File);
    Job_list = (JOB*)malloc(job_counter * sizeof(struct _Job));

    //Successfully getting file
    int j;
    for (j = 0; j < job_counter; j++)
    {
        fgets(buff, sizeof(buff), Input_File);
        splits(buff, j);
    }
    fclose(Input_File);
}

void splits(char buff[270], int Job_No)
{
    int at; //Array for the Arrival Time
    char cmd[256]; //Array for the Command Line
    int duration; //Array for the Duration Time
    char** parameter;
    int o;
    char* buf = buff;
    char seg[] = "\t";

    int i = 0;
    char* sub = strtok(buf, seg);

    while (sub != NULL)
    {
        //Put the first token into arrival time
        if (i == 0)
        {
            at = atoi(sub);
        }

        //Put the second token into command line
        if (i == 1)
        {
            strcpy(cmd, sub);
        }

        //Put the third token into duration time
        if (i == 2)
        {
            duration = *sub - '0';
        }

        sub = strtok(NULL, seg);
        i++;
    }

    //Divide the command line by space

    int Parameter_Counter = 0;
    int j;
    for (j = 0; j < strlen(cmd); j++)
    {
        if (cmd[j] == 32)
        {
            Parameter_Counter++;
        }
    }
    Parameter_Counter++;

    /*After count the parameter number, 
    * the following code is to initilize
    */
    parameter = (char**)malloc(sizeof(char*) * Parameter_Counter);
    for (o = 0; o < Parameter_Counter; o++)
    {
        parameter[o] = (char*)malloc(256 * sizeof(char));
    }
    //getting the first token
    char* sep = strtok(cmd, " ");
    strcpy(parameter[0], sep);

    //getting the rest token
    int k;
    for (k = 1; k < Parameter_Counter; k++)
    {
        sep = strtok(NULL, " ");
        strcpy(parameter[k], sep);
    }

    Job_list[Job_No] = (JOB)malloc(sizeof(struct _Job));
    Job_list[Job_No]->Parameter_Counte = Parameter_Counter;
    Job_list[Job_No]->Parameter = (char**)malloc(sizeof(char*) * Parameter_Counter);
    for (o = 0; o < Parameter_Counter; o++)
    {
        Job_list[Job_No]->Parameter[o] = (char*)malloc(256 * sizeof(char));
        strcpy(Job_list[Job_No]->Parameter[o], parameter[o]);
    }
    Job_list[Job_No]->Arrival_Time = at;
    Job_list[Job_No]->Duration_Time = duration;
    strcpy(Job_list[Job_No]->Job_Name, parameter[0]);
    Job_list[Job_No]->Running_Time = 0;
    Job_list[Job_No]->status = _PROCESS_CREATED_;
}