#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "scheduler.h"

/*
 * Merge algorithm for merge sort.
 */
void merge(task_t arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    task_t L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i].remaining_time <= R[j].remaining_time) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/*
 * Merge sort algorithm for scheduling policies.
 */
void merge_sort(task_t arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}


/*
 * First Come First Serve (FCFS) scheduling policy.
 */
void fcfs_policy(task_t arr[], int count) {
    printf("============================================================\n");

    int time = 0;
    int arr_count = 0;
    int finished_tasks = 0;

    task_t new_arr[count];

    /* can't set indexes of task_t array to null so we have empty_task */
    task_t empty_task = {.pid = 0, .arrival_time = 0, .burst_time = 0, .remaining_time = 0};

    int response_times[count];
    for (int i = 0; i < count; i++) {
        new_arr[i] = empty_task;
        response_times[i] = -1;
    }

    int total_waiting = 0;
    int total_response = 0;
    int total_turnaround = 0;
    int cpu_usage = 0;

    while (finished_tasks < count) {
        for (int i = arr_count; i < count; i++) {
            if (arr[i].arrival_time == time) {
                new_arr[arr_count] = arr[i];
                arr_count++;
            }
        }

        if (new_arr[0].remaining_time > 0) {
            if (response_times[new_arr[0].pid - 1] == -1) {
                response_times[new_arr[0].pid - 1] = time - new_arr[0].arrival_time;
                total_response += response_times[new_arr[0].pid - 1];
            }

            printf("<time %d> process %d is running\n", time, new_arr[0].pid);
            new_arr[0].remaining_time--;
            time++;
            cpu_usage++;
        }

        if (new_arr[0].remaining_time == 0) {
            printf("<time %d> process %d is finished...\n", time, new_arr[0].pid);
            finished_tasks++;

            int turnaround = time - new_arr[0].arrival_time;
            int waiting = turnaround - new_arr[0].burst_time;
            total_turnaround += turnaround;
            total_waiting += waiting;

            /* shift the remaining tasks to the left */
            for (int i = 1; i < arr_count; i++) {
                new_arr[i - 1] = new_arr[i];
            }
            new_arr[arr_count -1] = empty_task;
            arr_count--;
        }
    }
    printf("<time %d> All processes finish ......\n", time);

    float avg_waiting = (float) total_waiting / count;
    float avg_response = (float) total_response / count;
    float avg_turnaround = (float) total_turnaround / count;
    float cpu_usage_percent = (float) cpu_usage / time * 100;

    printf("============================================================\n");
    printf("Average waiting time: %.2f\n", avg_waiting);
    printf("Average response time: %.2f\n", avg_response);
    printf("Average turnaround time: %.2f\n", avg_turnaround);
    printf("Overall CPU usage: %.2f%%\n", cpu_usage_percent);
    printf("============================================================\n");
}

/*
 * Round Robin (RR) scheduling policy.
 */
void rr_policy(task_t arr[], int count, int quantum) {
    printf("============================================================\n");

    int time = 0;
    int arr_count = 0;
    int finished_tasks = 0;

    int total_waiting = 0;
    int total_response = 0;
    int total_turnaround = 0;
    int cpu_usage = 0;

    task_t circular_arr[count];
    int front = 0;
    int rear = -1;

    /* can't set indexes of task_t array to null so we have empty_task */
    task_t empty_task = {.pid = 0, .arrival_time = 0, .burst_time = 0, .remaining_time = 0};

    int response_times[count];
    for (int i = 0; i < count; i++) {
        circular_arr[i] = empty_task;
        response_times[i] = -1;
    }

    while (finished_tasks < count) {
        for (int i = 0; i < count; i++) {
            if (arr[i].arrival_time == time) {
                circular_arr[i] = arr[i];
                rear = i;
                arr_count++;
            }
        }

        task_t *current_task = &circular_arr[front];
        int task_quantum = quantum;

        while (current_task->remaining_time > 0 && task_quantum > 0) {
            if (response_times[current_task->pid - 1] == -1) {
                response_times[current_task->pid - 1] = time - current_task->arrival_time;
                total_response += response_times[current_task->pid - 1];
            }

            printf("<time %d> process %d is running\n", time, current_task->pid);
            time++;
            task_quantum--;
            current_task->remaining_time--;
            cpu_usage++;

            for (int i = arr_count; i < count; i++) {
                if (arr[i].arrival_time == time) {
                    rear = (rear + 1) % count;
                    circular_arr[rear] = arr[i];
                    arr_count++;
                }
            }
        }

        if (current_task->remaining_time == 0) {
            printf("<time %d> process %d is finished...\n", time, current_task->pid);
            finished_tasks++;

            int turnaround = time - current_task->arrival_time;
            int waiting = turnaround - current_task->burst_time;
            total_turnaround += turnaround;
            total_waiting += waiting;
        } else {
            rear = (rear + 1) % count;
            circular_arr[rear] = *current_task;
        }
        if (front != rear) {
            circular_arr[front] = empty_task;
        }
        front = (front + 1) % count;
    }
    printf("<time %d> All processes finish ......\n", time);

    float avg_waiting = (float)total_waiting / count;
    float avg_response = (float)total_response / count;
    float avg_turnaround = (float)total_turnaround / count;
    float cpu_usage_percent = (float)cpu_usage / time * 100;

    printf("============================================================\n");
    printf("Average waiting time: %.2f\n", avg_waiting);
    printf("Average response time: %.2f\n", avg_response);
    printf("Average turnaround time: %.2f\n", avg_turnaround);
    printf("Overall CPU usage: %.2f%%\n", cpu_usage_percent);
    printf("============================================================\n");
}

/*
 * Shortest Remaining Time First (SRTF) scheduling policy.
 */
void srtf_policy(task_t arr[], int count) {
    printf("============================================================\n");

    int time = 0;
    int arr_count = 0;
    int finished_tasks = 0;

    task_t new_arr[count];

    /* can't set indexes of task_t array to null so we have empty_task */
    task_t empty_task = {.pid = 0, .arrival_time = 0, .burst_time = 0, .remaining_time = 0};

    int total_waiting = 0;
    int total_response = 0;
    int total_turnaround = 0;
    int cpu_usage = 0;

    int response_times[count];
    for (int i = 0; i < count; i++) {
        response_times[i] = -1;
    }

    while (finished_tasks < count) {
        for (int i = arr_count; i < count; i++) {
            if (arr[i].arrival_time == time) {
                new_arr[arr_count] = arr[i];
                arr_count++;
            }
        }
        
        merge_sort(new_arr, 0, arr_count - 1);

        if (new_arr[0].remaining_time > 0) {
            if (response_times[new_arr[0].pid - 1] == -1) {
                response_times[new_arr[0].pid - 1] = time - new_arr[0].arrival_time;
                total_response += response_times[new_arr[0].pid - 1];
            }

            printf("<time %d> process %d is running\n", time, new_arr[0].pid);
            new_arr[0].remaining_time--;
            time++;
            cpu_usage++;
        }

        if (new_arr[0].remaining_time == 0) {
            printf("<time %d> process %d is finished...\n", time, new_arr[0].pid);
            finished_tasks++;

            int turnaround = time - new_arr[0].arrival_time;
            int waiting = turnaround - new_arr[0].burst_time;
            total_turnaround += turnaround;
            total_waiting += waiting;

            /* shift the remaining tasks to the left */
            for (int i = 1; i < arr_count; i++) {
                new_arr[i - 1] = new_arr[i];
            }
            new_arr[arr_count -1] = empty_task;
            arr_count--;
        }
    }
    printf("<time %d> All processes finish ......\n", time);

    float avg_waiting = (float) total_waiting / count;
    float avg_response = (float) total_response / count;
    float avg_turnaround = (float) total_turnaround / count;
    float cpu_usage_percent = (float) cpu_usage / time * 100;

    printf("============================================================\n");
    printf("Average waiting time: %.2f\n", avg_waiting);
    printf("Average response time: %.2f\n", avg_response);
    printf("Average turnaround time: %.2f\n", avg_turnaround);
    printf("Overall CPU usage: %.2f%%\n", cpu_usage_percent);
    printf("============================================================\n");
}