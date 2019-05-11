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
#define MAX_PROCESS 50
#define MIN_PROCESS 0

struct process
{
    int id;
    char name[50];
    int priority; // prioridade de execução
    int state;    // state 0 = em execucao, 1 = na fila de execução, 2 = em espera
    int time_cpu; // tempo medio de processamento do processo a cada vez que é chamado em ms
    int cicles;   // Ciclos restantes para conclusao do processo
} typedef Process;

char *list_process[50] = {
    "systemd",
    "ModemManager",
    "NetworkManager",
    "bluetoothd",
    "dde-file-manage",
    "crond",
    "gnome-keyring-d",
    "lightdm",
    "mysqld",
    "systemd-journal",
    "Oracle",
    "Xorg",
    "Baidu",
    "systemd-logind",
    "gnome-terminal-",
    "udisksd",
    "wpa_supplicant",
    "cinnamon-screen",
    "csd-keyboard",
    "csd-media-keys",
    "csd-mouse",
    "csd-orientation─",
    "csd-power",
    "steam",
    "chrome",
    "spotify",
    "rocket.chat",
    "telegram",
    "csd-screensaver",
    "csd-sound",
    "gvfs-mtp-volume",
    "dbus-daemon",
    "blueberry-obex",
    "applet.py",
    "colord",
    "acpid",
    "agetty",
    "avahi-daemon",
    "cups-browsed",
    "cinnamon-killer",
    "python3",
    "python2",
    "java",
    "ruby",
    "node",
    "bash",
    "zombie",
    "code",
    "gnome",
    "graphviz",
};

void setProcess(Process *process_list, int min, int max, int begin, int size);

void start_sheduler(Process *process_list, int min, int max, int begin, int size)
{
    // inicia o escalonador com um conjunto (micro, basic, medium, mega) definido pelo usuario
    setProcess(process_list, min, max, begin, size);
}

void setProcess(Process *process_list, int min, int max, int begin, int size)
{
    int index_default_process = 0;
    // conjunto micro de processos iniciados com o sistema;
    for (int i = begin; i < size; i++)
    {
        process_list[i].id = i;
        index_default_process = MIN_PROCESS + rand() % (MAX_PROCESS - MIN_PROCESS);

        strcpy(process_list[i].name, list_process[index_default_process]);
        process_list[i].priority = min + rand() % (max - min + 1);
        process_list[i].state = 1;
        process_list[i].time_cpu = MIN_TIME + rand() % (MAX_TIME - MIN_TIME + 1);
        process_list[i].cicles = MIN_CICLES + rand() % (MAX_CICLES - MIN_CICLES + 1);
    }
}

#endif