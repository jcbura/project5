#include <stdio.h>
#include "scheduler.h"
/*
 * Read data. To read task information from a file (i.e., file_descriptor) and to store data into task_list[].
 * The number of tasks stored in task_list[] is specified by size, which is an output.
 */
void read_file(FILE *file_descriptor, task_t task_list[], int *size) {
    int num = 0;
    while (fscanf(file_descriptor, "%u %u %u", &task_list[num].pid, &task_list[num].arrival_time, &task_list[num].burst_time)!= EOF) {
        task_list[num].initial_burst_time = task_list[num].burst_time;
        task_list[num].remaining_time = task_list[num].burst_time;
        num++;
    }
    *size = num;
}