#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TYPE            int
#define ARRAY_SIZE      20
#define THREAD_COUNT    2
#define NUM_MAX         1e7
#define NUM_MIN         -1e7
#define SORT_DESC       true


struct description
{
    int thread_id;
    int from;
    int length;
    TYPE *array;
    int merge_pointer = 0;
};


void init_array(TYPE *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = (rand() % (TYPE)(NUM_MAX - NUM_MIN)) + (TYPE)NUM_MIN;
    }
}


void init_descriptions(description* des, int size, TYPE *array, int array_size)
{
    for(int i = 0; i < size; i++)
    {
        des[i] = { i, i == 0 ? 0 : des[i - 1].from + des[i - 1].length,
            i == size - 1 ? array_size - des[i - 1].from - des[i - 1].length : array_size / size, array  };

        printf("Initialized description for thread %d with length %d, from %d, to %d\n",
            des[i].thread_id, des[i].length, des[i].from, des[i].from + des[i].length);
    }
}


void print_array(TYPE *array, int size)
{
    printf("\n\nArray:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d: %d\n", i, array[i]);
    }
}

void insert_sort(description d)
{
    for(int i = 1; i < d.length; i++)
    {
        for(int x = i; x >= 1; x--)
        {
            if(SORT_DESC ^ (d.array[d.from + x] < d.array[d.from + x - 1]))
            {
                int tmp = d.array[d.from + x];
                d.array[d.from + x] = d.array[d.from + x - 1];
                d.array[d.from + x - 1] = tmp;
            }
        }
    }
}

void merge_arrays(TYPE *input_array, TYPE *output_array, int array_size,
    description *des, int des_size)
{
    for (int x = 0; x < array_size; x++)
    {
        int extreme_index = 0;

        for (int i = 1; i < des_size; i++)
        {
            if(des[extreme_index].merge_pointer >= des[extreme_index].length)
            {
                extreme_index = i;
                continue;
            }
            if(des[i].merge_pointer >= des[i].length)
            {
                continue;
            }

            if(SORT_DESC ^ (input_array[des[i].from + des[i].merge_pointer] <
                input_array[des[extreme_index].from + des[extreme_index].merge_pointer]))
            {
                extreme_index = i;
            }
        }

        output_array[x] = input_array[des[extreme_index].from + des[extreme_index].merge_pointer];
        des[extreme_index].merge_pointer++;
    }

}

bool check_sorted(TYPE *array, int size)
{
    for (int i = 1; i < size; i++)
    {
        if(SORT_DESC ^ (array[i - 1] > array[i]))
        {
            printf("Not sorted!!!\n");
            return false;
        }
    }

    printf("Sorted\n");
    return true;
}


int main(int argc, char const *argv[])
{
    //srand(time(NULL));

    TYPE array[ARRAY_SIZE], output_array[ARRAY_SIZE];
    description d[THREAD_COUNT];

    init_descriptions(d, THREAD_COUNT, array, ARRAY_SIZE);
    init_array(array, ARRAY_SIZE);

    printf("Initial array:\n");
    print_array(array, ARRAY_SIZE);

    printf("Sorting...\n");
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        insert_sort(d[i]);
        print_array(array, ARRAY_SIZE);
    }

    printf("Output\n");
    merge_arrays(array, output_array, ARRAY_SIZE, d, THREAD_COUNT);
    print_array(output_array, ARRAY_SIZE);

    //init_array(output_array, ARRAY_SIZE);

    return !check_sorted(output_array, ARRAY_SIZE);
}
