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

#include "syscall_map.h" // Archivo de encabezado que contiene el mapeo de syscalls

#define MAX_SYSCALLS 452 // Número máximo de syscalls

int verbose = 0; // Modo detallado
int wait_key = 0; // Esperar tecla


/*
 * Función: parse_args
 * -------------------
 * Analiza los argumentos proporcionados en la línea de comandos,
 * configurando las variables globales 'verbose' y 'wait_key' según
 * las opciones detectadas. También separa el nombre del programa 
 * y sus argumentos correspondientes.
 *
 * Parámetros:
 *   - argc: número total de argumentos en la línea de comandos.
 *   - argv: arreglo de cadenas que contiene los argumentos.
 *   - prog: puntero a una cadena donde se almacenará el nombre del programa.
 *   - prog_args: puntero a un arreglo donde se almacenarán los argumentos del programa.
 */

void parse_args(int argc, char *argv[], char **prog, char ***prog_args) { // Función para analizar los argumentos
    int i;
    for (i = 1; i < argc; i++) { // Iterar sobre los argumentos
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-V") == 0) {
            verbose = 1; // Modo detallado
            wait_key = 1; // Esperar tecla
        } else {
            break;
        }
    }
    *prog = argv[i]; // Guardar el nombre del programa
    *prog_args = &argv[i]; // Guardar los argumentos del programa
}

/*
 * Función: trace_syscalls
 * -----------------------
 * Rastrea las llamadas al sistema realizadas por un proceso hijo
 * utilizando ptrace. Muestra información detallada sobre cada llamada
 * al sistema y un resumen al finalizar.
 *
 * Parámetros:
 *   - pid: ID del proceso hijo a rastrear.
 */
void trace_syscalls(pid_t pid) { // Función para rastrear syscalls
    int status, syscall_num, in_syscall = 0;
    unsigned long long count[MAX_SYSCALLS] = {0}; // Contador de syscalls

    waitpid(pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD); // Configurar opciones de ptrace

    while (1) { 
        ptrace(PTRACE_SYSCALL, pid, 0, 0); // Esperar syscall
        waitpid(pid, &status, 0);
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            syscall_num = ptrace(PTRACE_PEEKUSER, pid, sizeof(long) * ORIG_RAX); // Obtener el número de syscall 
            if (syscall_num >= 0 && syscall_num < MAX_SYSCALLS) {
                if (!in_syscall) { 
                    count[syscall_num]++; // Incrementar el contador
                    if (verbose && syscall_names[syscall_num]) { // Mostrar información detallada
                        printf("Syscall: %s (%d)\n", syscall_names[syscall_num], syscall_num);
                        if (wait_key) { // Esperar tecla
                            printf("Presione una tecla para continuar...");
                            getchar();
                        }
                    }
                    in_syscall = 1; // Indicar que estamos dentro de una syscall
                } else {
                    in_syscall = 0;
                }
            }
        } else if (WIFEXITED(status)) { // Proceso hijo ha terminado
            break;
        }
    }

    // Mostrar resumen
    printf("\nResumen de Syscalls:\n");
    for (int i = 0; i < MAX_SYSCALLS; i++) { // Mostrar resumen de syscalls
        if (count[i] > 0 && syscall_names[i]) {
            printf("%-20s: %llu\n", syscall_names[i], count[i]); // Mostrar el nombre y el contador
        }
    }
}

/*
 * Función: main
 * -------------
 * Punto de entrada del programa. Maneja los argumentos de la línea
 * de comandos y llama a las funciones para rastrear syscalls.
 *
 * Parámetros:
 *   - argc: número total de argumentos en la línea de comandos.
 *   - argv: arreglo de cadenas que contiene los argumentos.
 *
 * Retorna:
 *   - 0 si se ejecuta correctamente, 1 en caso de error.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) { // Verificar si hay suficientes argumentos
        fprintf(stderr, "Uso: %s [-v|-V] Prog [opciones de Prog]\n", argv[0]); // Mostrar uso
        return 1;
    }

    char *prog; // Nombre del programa
    char **prog_args; // Argumentos del programa
    parse_args(argc, argv, &prog, &prog_args);

    pid_t pid = fork(); // Crear un nuevo proceso
    if (pid == 0) {
        // Proceso hijo
        ptrace(PTRACE_TRACEME, 0, NULL, NULL); // Permitir que el padre rastree este proceso
        execvp(prog, prog_args); // Ejecutar el programa
        perror("execvp");
        exit(1); // Salir si hay error
    } else {
        // Proceso padre
        trace_syscalls(pid);
    }
    return 0; 
}
