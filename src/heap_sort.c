#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sheduler.c"
#include <unistd.h>
#include <semaphore.h>

#define ONE_SECOND 1000 // 1000ms

sem_t mutex; // semaforo para pausar a simulacao da heap_sort

struct heap_atributes
{
    Process *process_list;
    int size_list;
} typedef HeapAtributes;

void swap(Process *val_p1, Process *val_p2)
{
    // Troca os valores de posição na lista
    Process aux;
    aux = *val_p1;
    *val_p1 = *val_p2;
    *val_p2 = aux;
}

void heapify(Process *process_list, int index, int heap_size)
{
    // funcao para colocar as prioridades no inicio da heap, heap sort decrescente no número
    int left_child = 2 * index;
    int right_child = 2 * index + 1;
    int smaller = index;

    // prioridade por priority e depois por tempo
    if (left_child < heap_size && process_list[left_child].priority < process_list[smaller].priority)
        smaller = left_child;
    else if (left_child < heap_size && process_list[left_child].priority == process_list[smaller].priority)
        if (process_list[left_child].time_cpu < process_list[smaller].time_cpu)
            smaller = left_child;

    if (right_child < heap_size && process_list[right_child].priority < process_list[smaller].priority)
        smaller = right_child;
    else if (right_child < heap_size && process_list[right_child].priority == process_list[smaller].priority)
        if (process_list[right_child].time_cpu < process_list[smaller].time_cpu)
            smaller = right_child;

    if (smaller != index)
    {
        swap(&process_list[index], &process_list[smaller]);
        heapify(process_list, smaller, heap_size);
    }
}

void build_heap(Process *process_list, int heap_size)
{
    for (int i = heap_size / 2; i > 0; i--)
    {
        heapify(process_list, i, heap_size);
    }
}

void show_root(Process *process_list)
{
    printf("id = %d\t", process_list[1].id);
    printf("name = %s\t", process_list[1].name);
    printf("priority = %d\t", process_list[1].priority);
    printf("time = %d\t", process_list[1].time_cpu);
    printf("ciclos = %d\n", process_list[1].cicles);
    sleep((float)process_list[1].time_cpu / ONE_SECOND);
}

void heap_sort(void *atributes)
{
    HeapAtributes *atributes_cp = (HeapAtributes *)atributes;
    int heap_size = atributes_cp->size_list;
    Process *process_list = atributes_cp->process_list;

    // funcao da heap_sort modificada, para mostrar as iterações
    for (int i = heap_size - 1; i > 1; i--)
    {
        sem_wait(&mutex);
        process_list[1].cicles -= 1; // decrementa a quantidade de ciclos restantes
        show_root(process_list);
        swap(&process_list[1], &process_list[i]);
        // se o processo q acabou de ser executado já terminou seus ciclos, marca como concluido
        if (process_list[i].cicles <= 0)
        {
            printf("Removendo %s\n", process_list[heap_size - 1].name);
            process_list[heap_size - 1].state = 0;
            swap(&process_list[heap_size - 1], &process_list[atributes_cp->size_list - 1]);
            atributes_cp->size_list -= 1;
        }
        heap_size -= 1;
        heapify(process_list, 1, heap_size);
        sem_post(&mutex);
    }
    process_list[1].cicles -= 1;
    show_root(process_list);
    if (process_list[1].cicles <= 0)
    {
        printf("Removendo %s\n", process_list[heap_size - 1].name);
        process_list[heap_size - 1].state = 0;
        swap(&process_list[heap_size - 1], &process_list[atributes_cp->size_list - 1]);
        atributes_cp->size_list -= 1;
    }
    build_heap(process_list, atributes_cp->size_list); // construir heap
    // chama recursivo até que a fila de execucao esteja vazia
    if (atributes_cp->size_list > 0)
    {
        printf("\n");
        heap_sort(atributes_cp);
    }
    else
    {
        printf("\nFIM da execução da Lista!!!\n");
    }
}
