#ifndef SHEDULER
#define SHEDULER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// tempo max e minimo de um processo dos conjutos padrões na cpu em ms
#define MAX_TIME 2000
#define MIN_TIME 500
#define MAX_CICLES 1
#define MIN_CICLES 15

struct process
{
    int id;
    char name[50];
    int priority; // prioridade de execução
    int state;    // state 0 = em execucao, 1 = na fila de execução, 2 = em espera
    int time_cpu; // tempo medio de processamento do processo a cada vez que é chamado em ms
    int cicles;   // Ciclos restantes para conclusao do processo
} typedef Process;

char *list_process[10] = {
    "systemd",
    "ModemManager",
    "NetworkManager",
    "bluetoothd",
    "dde-file-manage",
    "crond",
    "gnome-keyring-d",
    "lightdm",
    "mysqld",
    "systemd-journal"};

void setMicroProcess(Process *process_list, int min, int max, int begin, int size);
void setBasicProcess(Process *process_list, int min, int max, int begin, int size);
void setMediumProcess(Process *process_list, int min, int max, int begin, int size);
void setMegaProcess(Process *process_list, int min, int max, int begin, int size);

void start_sheduler(Process *process_list, int min, int max, int set_start, int begin, int size)
{
    // inicia o escalonador com um conjunto (micro, basic, medium, mega) definido pelo usuario
    switch (set_start)
    {
    case 0:
        setMicroProcess(process_list, min, max, begin, size);
        break;
    case 1:
        setBasicProcess(process_list, min, max, begin, size);
        break;
    case 2:
        setMediumProcess(process_list, min, max, begin, size);
        break;
    case 3:
        setMegaProcess(process_list, min, max, begin, size);
        break;
    default:
        setBasicProcess(process_list, min, max, begin, size);
        break;
    }
}

void setMicroProcess(Process *process_list, int min, int max, int begin, int size)
{
    // conjunto micro de processos iniciados com o sistema;
    for (int i = begin; i < size; i++)
    {
        process_list[i].id = i;
        strcpy(process_list[i].name, list_process[i - 1]);
        process_list[i].priority = min + rand() % (max - min + 1);
        process_list[i].state = 1;
        process_list[i].time_cpu = MIN_TIME + rand() % (MAX_TIME - MIN_TIME + 1);
        process_list[i].cicles = MIN_CICLES + rand() % (MAX_CICLES - MIN_CICLES + 1);
    }
}

void setBasicProcess(Process *process_list, int min, int max, int begin, int size)
{
}

void setMediumProcess(Process *process_list, int min, int max, int begin, int size)
{
}

void setMegaProcess(Process *process_list, int min, int max, int begin, int size)
{
}

#endif