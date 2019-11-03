#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define THREADS_COUNT   2
#define ARRAY_SIZE      20
#define PIECE_SIZE      ARRAY_SIZE / THREADS_COUNT

int arr[ARRAY_SIZE];
int output_arr[ARRAY_SIZE];

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
        //printf("Starting with index %d\n", x);

        int minimum_index = 0;

        for(int y = 1; y < THREADS_COUNT; y++)
        {
            if(minimum_index_array[minimum_index] >= PIECE_SIZE)
            {
                //printf("Minimum index of out bounds\n");
                minimum_index = y;
                continue;
            }
            if(minimum_index_array[y] >= PIECE_SIZE)
            {
                //printf("Current index out of bounds\n");
                continue;
            }

            if(arr[minimum_index_array[y] + y * PIECE_SIZE] < 
                arr[minimum_index_array[minimum_index] + minimum_index * PIECE_SIZE])
            {
                //printf("Found lower index\n");
                minimum_index = y;   
            }
        }

        output_arr[x] = arr[minimum_index_array[minimum_index] + minimum_index * PIECE_SIZE];
        minimum_index_array[minimum_index]++;

        //break;
    }
}


int main()
{
    init_array();
    print_array();

    int thread0 = 0;
    int thread1 = 1;

    bubble_thread(&thread0);
    bubble_thread(&thread1);

    print_array();

    join_arrays(arr, output_arr);

    print_array(output_arr);

    return 0;
}