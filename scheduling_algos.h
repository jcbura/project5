#ifndef SCHEDULING_ALGOS_H
#define SCHEDULING_ALGOS_H

#include "scheduler.h"

/*
 * The prototype of functions implemented in scheduling_algos.c
 */
void fcfs_policy(task_t task_array[], int count);
void rr_policy(task_t task_array[], int count, int quantum);
void srtf_policy(task_t task_array[], int count);

#endif