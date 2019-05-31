#include "heap_sort.c"
#include "sheduler.c"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// nesta simulação, o escalonador vai dar preferência para os processos com melhor prioridade = 1, 2, 3...
// se os processos possuirem mesma prioridade, a preferência será para o processamento com menor tempo de cpu

#define MAX_TIME 2000
#define MIN_TIME 500
#define SIZE_MAX 100
#define SIZE_MIN 10
#define PRIORITY_MAX 20
#define PRIORITY_MIN 1
#define NUM_THREADS 2

void main_menu();
void simulation();
void await_simulation();
Process add_menu(Process *process_list);
int start_sheduler_menu(Process *process_list, int *begin, int *size_list);

int main()
{
    setlocale(LC_ALL, "Portuguese");
    main_menu();
    return 0;
}

void main_menu()
{
    // tamanho inicial da lista de processos (lembrando de nao usar pos 0, para facilitar)
    int size_list = 0;
    int set_group = 0; // grupo de inicio do escalonador
    int begin = 1;
    int op = 0;
    Process new_process;
    Process *process_list = NULL;

    process_list = malloc(sizeof(Process));

    // while ou thread? eis a questao
    while (1)
    {
        system("clear");
        srand(time(NULL));

        // MENU
        printf("\t\t\t*** ESCALONADOR DE PROCESSOS ***\n");
        printf("-------------------------------------------------------------------------\n");
        printf("\t(1) - Criar Lista de Processos\n");
        printf("\t(2) - Mostrar Simulação\n");
        printf("\t(3) - Adicionar Processo\n");
        // printf("\t(4) - Interromper/Remover Processo\n");
        printf("\t(0) - Sair do Sistema\n");
        printf("-------------------------------------------------------------------------\n");
        printf("Digite uma opção: ");
        scanf("%d", &op);

        switch (op)
        {
        case 0:
            return;
        case 1:
            set_group = start_sheduler_menu(process_list, &begin, &size_list);
            if (set_group == -1)
                break;
            process_list = realloc(process_list, size_list * sizeof(Process));
            start_sheduler(process_list, PRIORITY_MIN, PRIORITY_MAX, begin, size_list);
            break;
        case 2:
            simulation(process_list, begin, size_list);
            break;
        case 3:
            new_process = add_menu(process_list);
            process_list = realloc(process_list, (++size_list) * sizeof(Process));
            printf("tamanho = %d\n", size_list);
            process_list[size_list - 1] = new_process;
            break;
        default:
            return;
        }
    }
}

void simulation(Process *process_list, int begin, int size_list)
{
    pthread_t threads[NUM_THREADS];

    if (size_list == 0)
    {
        printf("Não há fila de processos!\n");
        printf("\tRetornando ao menu principal...\n");
        getchar();
        getchar();
        return;
    }

    build_heap(process_list, size_list); // construir heap

    printf("\nSimulação\n\n");
    HeapAtributes *atributes;
    atributes = malloc(sizeof(HeapAtributes));
    atributes->process_list = process_list;
    atributes->size_list = size_list;
    for (int i = 1; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, (void *)heap_sort, (void *)atributes);
    getchar();
    getchar();
}

void await_simulation()
{
    // funcao que aguarda um valor no teclado, enquanto simulação esta em andamento
    while (1)
    {
        scanf("%*d");
        printf("Simulação Pausada\n");
    }
}

Process add_menu(Process *process_list)
{
    Process new_process;
    int errors = 0;
    while (1)
    {
        printf("\t** Informe as configurações do Processo **\n\n");
        printf("\tNome = ");
        scanf("%s", new_process.name);
        printf("\tPrioridade = ");
        scanf("%d", &new_process.priority);
        printf("\tTempo de CPU(se valor nulo, tempo de cpu aleatório) = ");
        scanf("%d", &new_process.time_cpu);
        if (new_process.time_cpu == 0)
            new_process.time_cpu = MIN_TIME + rand() % (MAX_TIME - MIN_TIME + 1);
        printf("\tCiclos para termino do processo(se valor nulo, ciclos aleatório) = ");
        scanf("%d", &new_process.cicles);
        if (new_process.cicles == 0)
            new_process.cicles = MIN_CICLES + rand() % (MAX_CICLES - MIN_CICLES + 1);

        if (errors == 0)
            break;
    }
    return new_process;
}

int start_sheduler_menu(Process *process_list, int *begin, int *size_list)
{
    int op;
    int set_group = 0;
    printf("\t(1) - Micro (10 processos essênciais)\n");
    printf("\t(2) - Basic (Conjunto básico de 20 processos)\n");
    printf("\t(3) - Medium (Conjunto padrão com 30 processos)\n");
    printf("\t(4) - Mega (Conjunto completo com 50 processos)\n");
    printf("Digite com qual conjunto iniciar o Sistema: ");
    scanf("%d", &op);
    //
    if (op == 0)
        return -1;
    set_group = op - 1;
    if (op == 1)
        *size_list = 11;
    else if (op == 2)
        *size_list = 21;
    else if (op == 3)
        *size_list = 31;
    else
        *size_list = 51;
    return set_group;
}