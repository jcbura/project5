/*
* COMP 3500: Project 5 Scheduling
* Jason Bura
* Version 1.0 4/19/2023
*
* This source code shows how to conduct separate compilation.
*
* How to compile using Makefile?
* $make
*
* How to manually compile?
* $gcc -c open.c
* $gcc -c read.c
* $gcc -c print.c
* $gcc open.o read.o print.o scheduler.c -o scheduler
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "scheduler.h"
#include "open.h"
#include "read.h"
#include "scheduling_algos.h"

int main(int argc, char *argv[]) {
    char *file_name; /* file name from the commandline */
    FILE *fp; /* file descriptor */
    task_t task_array[MAX_TASK_NUM];

    int error_code;
    u_int i;
    u_int count;

    if (argc < 3 || argc > 4) {
        printf("Usage: scheduler task_list_file [FCFS|RR|SRTF] [time_quantum]\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "FCFS") != 0 && strcmp(argv[2], "RR") != 0 && strcmp(argv[2], "SRTF") != 0) {
        printf("%s is not a valid policy. Please provide a valid policy.\n", argv[2]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "RR") == 0) {
        if (argc < 4 || atoi(argv[3]) < 1) {
            printf("%d is not a valid time_quantim. Please provide a valid time_quantum for the RR policy.\n", atoi(argv[3]));
            return EXIT_FAILURE;
        }
    }

    if (argc == 4 && strcmp(argv[2], "RR") != 0) {
        printf("Only the RR policy accepts time_quantum.\n");
        return EXIT_FAILURE;
    }

    error_code = open_file(argv[1], &fp);
    if (error_code == 1) {
        return EXIT_FAILURE;
    }

    read_file(fp, task_array, &count);

    printf("Scheduling Policy: %s\n", argv[2]);
    printf("There are %d tasks loaded from \"%s\". Press enter to continue...\n", count, argv[1]);
    getchar();

    if (strcmp(argv[2], "FCFS") == 0) {
        fcfs_policy(task_array, count);
    }
    else if (strcmp(argv[2], "RR") == 0) {
        rr_policy(task_array, count, atoi(argv[3]));
    }
    else if (strcmp(argv[2], "SRTF") == 0) {
        srtf_policy(task_array, count);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}