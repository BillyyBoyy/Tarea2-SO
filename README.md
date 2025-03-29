# Tarea2-SO

**Repositorio de William Alfaro**  
**Carné: 2022437996**

Este repositorio contiene el código fuente y los archivos necesarios para la Tarea 2 del curso de **Principios de Sistemas Operativos**. El objetivo es desarrollar un programa en C que rastree las llamadas al sistema (**system calls**) realizadas por otro programa, mostrando tanto un resumen al final de la ejecución como la posibilidad de ver las llamadas en tiempo real y/o paso a paso.

---

## Comandos para Compilar y Ejecutar

### 1. Compilar el código

Para compilar el programa, basta con utilizar el `Makefile` provisto:

```bash
make
```

Esto generará el ejecutable llamado `rastreador`.

---

### 2. Ejecutar el programa

La sintaxis general para ejecutar el rastreador es:

```bash
./rastreador [opciones rastreador] Prog [opciones de Prog]
```

**Opciones disponibles:**

- `-v`: Despliega un mensaje cada vez que detecta una llamada al sistema realizada por `Prog`.
- `-V`: Además de desplegar los mensajes, espera a que el usuario presione una tecla para continuar (modo paso a paso).

> Las `[opciones de Prog]` son simplemente los argumentos para el programa objetivo; `rastreador` no los interpreta.

#### Ejemplos:

```bash
./rastreador ls
```

Ejecuta `ls` y al final imprime un resumen de las syscalls detectadas.

```bash
./rastreador -v ls -l
```

Ejecuta `ls -l` y muestra cada syscall a medida que ocurre, además del resumen final.

```bash
./rastreador -V sleep 1
```

Ejecuta `sleep 1` y muestra cada syscall pausando entre cada una, hasta completar la ejecución.

---

## Estructura del Repositorio

```
Tarea2-SO/
│
├── rastreador.c             # Código fuente principal del rastreador
├── syscall_map.c            # Implementación del arreglo con nombres de syscalls
├── syscall_map.h            # Cabecera que expone el arreglo de syscalls
├── Makefile                 # Script de compilación
├── README.md                # Instrucciones y explicación del proyecto
└── Documentación/
    ├── 2022437996_tarea2.pdf       # Documento en PDF
    ├── doc_fuente_latex            # Archivos fuente de la documentación en latex
```
