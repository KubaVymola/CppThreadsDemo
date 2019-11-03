#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define THREADS_COUNT   4
#define ARRAY_SIZE      20
#define PIECE_SIZE      ARRAY_SIZE / THREADS_COUNT

int arr[ARRAY_SIZE];
int output_arr[ARRAY_SIZE];
int thread_id[THREADS_COUNT];
pthread_t threads[THREADS_COUNT];

void init_array()
{
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        arr[i] = rand() % 10000;
    }
}

void print_array()
{
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("%d: %d\n", i, arr[i]);
    }
}


void print_array(int *arr)
{
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("%d: %d\n", i, arr[i]);
    }
}

void* bubble_thread(void* arg)
{
    int arg_val = *(int*)arg;

    int interval_start = arg_val * ARRAY_SIZE / THREADS_COUNT;
    int interval_end = interval_start + ARRAY_SIZE / THREADS_COUNT;

    printf("Starting sort from %d to %d at thread %d\n", interval_start, interval_end, arg_val);

    for(int i = interval_end - 1; i >= interval_start; i--)
    {
        for(int y = interval_start; y < i; y++)
        {
            if(arr[y] > arr[y + 1])
            {
                int temp = arr[y];
                arr[y] = arr[y + 1];
                arr[y + 1] = temp;
            }
        }
    }
    
    return NULL;
}

void join_arrays(int *arr, int *output_arr)
{
    int minimum_index_array[THREADS_COUNT];

    for(int i = 0; i < THREADS_COUNT; i++)
    {
        minimum_index_array[i] = 0;
    } 

    for(int x = 0; x < ARRAY_SIZE; x++)
    {
        int minimum_index = 0;

        for(int y = 1; y < THREADS_COUNT; y++)
        {
            if(minimum_index_array[minimum_index] >= PIECE_SIZE)
            {
                minimum_index = y;
                continue;
            }
            if(minimum_index_array[y] >= PIECE_SIZE)
            {
                continue;
            }

            if(arr[minimum_index_array[y] + y * PIECE_SIZE] < 
                arr[minimum_index_array[minimum_index] + minimum_index * PIECE_SIZE])
            {
                minimum_index = y;   
            }
        }

        output_arr[x] = arr[minimum_index_array[minimum_index] + minimum_index * PIECE_SIZE];
        minimum_index_array[minimum_index]++;

        //break;
    }
}

void init_thread_id()
{
    for(int i = 0; i < THREADS_COUNT; i++)
    {
        thread_id[i] = i;
    }
}

int main()
{
    srand(time(NULL));

    init_array();
    init_thread_id();
    print_array();

    for(int i = 0; i < THREADS_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, bubble_thread, &thread_id[i]);
    }
    for(int i = 0; i < THREADS_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Sorted parts:\n");
    print_array();

    join_arrays(arr, output_arr);
    printf("Merged array:\n");
    print_array(output_arr);

    return 0;
}