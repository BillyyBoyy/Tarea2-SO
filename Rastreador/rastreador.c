/*
 Tarea 2 - Sistemas Operativos

 Nombre: William Gerardo Alfaro Quiros
 Carné: 2022437996
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <signal.h>


#define MAX_SYSCALLS 452

int verbose = 0;
int wait_key = 0;

void parse_args(int argc, char *argv[], char **prog, char ***prog_args) {
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-V") == 0) {
            verbose = 1;
            wait_key = 1;
        } else {
            break;
        }
    }
    *prog = argv[i];
    *prog_args = &argv[i];
}
