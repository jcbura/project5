/* 
 * COMP 3500: Project 5 Scheduling
 * Jason Bura
 * Version 1.0 4/19/2023
 *
 * This source code shows how to conduct separate compilation.
 *
 * scheduler.h: The header file of scheduler.c
 */
#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#define MAX_TASK_NUM 32

typedef unsigned int u_int;

typedef struct task_info {
    u_int pid;
    u_int arrival_time;
    u_int burst_time;
    u_int initial_burst_time;
    u_int remaining_time;
} task_t;

#endif