#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define THREADS_COUNT   2
#define ARRAY_SIZE      20 
#define SLICE_SIZE      ARRAY_SIZE / THREADS_COUNT
#define MAX_NUM         1000


int input_arr[ARRAY_SIZE];
int output_arr[ARRAY_SIZE];
int threads_id[THREADS_COUNT];


void init_rand_arr(int *arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        arr[i] = rand() % MAX_NUM;
    }
}

void init_identity(int *arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        arr[i] = i;
    }
}

void print_arr(int *arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%d: %d\n", i, arr[i]);
    }
}

void* sort_thread(void *arg)
{
    int arg_val = *(int*)arg;


    int thread_start = arg_val * ARRAY_SIZE / THREADS_COUNT;
    int thread_end = thread_start + ARRAY_SIZE / THREADS_COUNT;

    for(int i = thread_start; i < thread_end; i++)
    {   
        int min_index = i;
        for(int y = i + 1; y < thread_end; y++)
        {
            if(input_arr[y] < input_arr[min_index])
            {
                min_index = y;
            }
        }

        int tmp = input_arr[i];
        input_arr[i] = input_arr[min_index];
        input_arr[min_index] = tmp;
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
            if(minimum_index_array[minimum_index] >= SLICE_SIZE)
            {
                minimum_index = y;
                continue;
            }
            if(minimum_index_array[y] >= SLICE_SIZE)
            {
                continue;
            }

            if(arr[minimum_index_array[y] + y * SLICE_SIZE] < 
                arr[minimum_index_array[minimum_index] + minimum_index * SLICE_SIZE])
            {
                minimum_index = y;   
            }
        }

        output_arr[x] = arr[minimum_index_array[minimum_index] + minimum_index * SLICE_SIZE];
        minimum_index_array[minimum_index]++;

        //break;
    }
}

int main()
{
    srand(time(NULL));

    pthread_t thread1, thread2;

    init_rand_arr(input_arr, ARRAY_SIZE);
    init_identity(threads_id, THREADS_COUNT);

    print_arr(input_arr, ARRAY_SIZE);

    //sort_thread(&threads_id[0]);
    //sort_thread(&threads_id[1]);

    pthread_create(&thread1, NULL, sort_thread, &threads_id[0]);
    pthread_create(&thread2, NULL, sort_thread, &threads_id[1]);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    print_arr(input_arr, ARRAY_SIZE);

    join_arrays(input_arr, output_arr);

    print_arr(output_arr, ARRAY_SIZE);

    return 0;
}
