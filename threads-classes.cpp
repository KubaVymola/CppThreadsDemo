#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define THREAD_COUNT    2
#define ARRAY_SIZE      20
#define TYPE            int
#define MAX_NUM         10000

class thread_job
{
public:
    int from, length;
    int job_id;
    int merge_counter;
    TYPE *array;
    
    thread_job(int from, int length, int job_id, int *array) : 
        from(from), length(length), job_id(job_id), array(array), merge_counter(0) {}
    
    thread_job() {}
};



thread_job jobs[THREAD_COUNT];
pthread_t threads[THREAD_COUNT];
TYPE array[ARRAY_SIZE];

void init_array(TYPE *arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        arr[i] = rand() % MAX_NUM;
    }
}

void print_array(TYPE *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d: %d\n", i, arr[i]);
    }
}

void init_jobs()
{
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        jobs[i] = thread_job((i == 0 ? 0 : jobs[i - 1].from + jobs[i - 1].length), 
            ARRAY_SIZE / THREAD_COUNT, i, array);
    }
}

void *select_sort(void *arg)
{
    thread_job job = *(thread_job*)arg;

    for (int i = 1; i < job.length; i++)
    {
        for (int x = i; x > 0; x--)
        {
            if(job.array[job.from + x] < job.array[job.from + x - 1])
            {
                int tmp = array[job.from + x];
                array[job.from + x] = array[job.from + x - 1];
                array[job.from + x - 1] = tmp;
            }
        }
    }

    return NULL;
}

void merge_arrays(int *input_array, thread_job *jobs, int *output_array, 
    int jobs_count, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        int min_index = 0;
        for (int x = 1; x < jobs_count; x++)
        {
            if(jobs[min_index].merge_counter >= jobs[min_index].length)
            {
                min_index = x;
                continue;
            }
            if(jobs[x].merge_counter >= jobs[x].length)
            {
                continue;
            }

            if(input_array[jobs[x].from + jobs[x].merge_counter] < 
                input_array[jobs[min_index].from + jobs[min_index].merge_counter])
            {
                min_index = x;
            }
        }

        output_array[i] = input_array[jobs[min_index].from + jobs[min_index].merge_counter];
        jobs[min_index].merge_counter++;
    }
    
}

int main(int argc, int *argv)
{
    srand(time(NULL));

    init_array(array, ARRAY_SIZE);
    init_jobs();

    printf("Generated array...:\n");
    print_array(array, ARRAY_SIZE);

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, select_sort, &jobs[i]);
    }
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("Sorted array with threads...:\n");
    print_array(array, ARRAY_SIZE);

    int output_array[ARRAY_SIZE];
    merge_arrays(array, jobs, output_array, THREAD_COUNT, ARRAY_SIZE);

    printf("Merged array:\n");
    print_array(output_array, ARRAY_SIZE);


    return 0;
    //--------------------


    printf("Sequential approach\n");
    init_array(array, ARRAY_SIZE);
    thread_job job = { 0, ARRAY_SIZE, 0, array };
    select_sort((void*)&job);
    
    printf("Sorted array with sequential approach");
    print_array(array, ARRAY_SIZE);    

    return 0;
}