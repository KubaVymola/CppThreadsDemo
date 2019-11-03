#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <pthread.h>

#define ARRAY_SIZE		20
#define THREADS_COUNT	4

using namespace std;


int arr[ARRAY_SIZE];
int max_indeces[THREADS_COUNT];
int thread_indeces[THREADS_COUNT];
pthread_t threads[THREADS_COUNT];
	

void create_arr()
{
	for(int i = 0; i < ARRAY_SIZE; i++)
	{
		arr[i] = rand() % 1000;
	}
}

void print_arr()
{
	for(int i = 0; i < ARRAY_SIZE; i++)
	{
		cout << i << ": " << arr[i] << '\n';
	}
}

void* hello_thread(void* thread_id)
{
	int t_id = *((int*)thread_id);
	cout << t_id;

	return NULL;
}

int find_max_index(int* index_arr)
{
	int max_index = 0;

	for(int i = 1; i < THREADS_COUNT; i++)
	{
		if(arr[index_arr[i]] > arr[index_arr[max_index]])
		{
			max_index = i;
		}
	}

	return index_arr[max_index];
}

void* finder_thread(void* arg)
{
	int arg_value = *(int*)arg;

	cout << "Starting search at thread " << arg_value << "\n";

	int interval_start = arg_value * ARRAY_SIZE / THREADS_COUNT;
	int interval_end = interval_start + ARRAY_SIZE / THREADS_COUNT;

	int max_index = interval_start;

	for(int i = interval_start + 1; i < interval_end; i++)
	{
		if(arr[i] > arr[max_index])
		{
			max_index = i;
		}
	}

	cout << "Setting max index from thread " << arg_value << " to " << max_index << " (value " << arr[max_index] << ")" << "\n";
	max_indeces[arg_value] = max_index;
	return NULL;
}


int main()
{
	srand(time(NULL));

	create_arr();
	print_arr();

	for(int i = 0; i < THREADS_COUNT; i++)
	{
		thread_indeces[i] = i;
	}

	for(int i = 0; i < THREADS_COUNT; i++)
	{
		pthread_create(&threads[i], NULL, finder_thread, &thread_indeces[i]);
	}	

	for(int i = 0; i < THREADS_COUNT; i++)
	{
		pthread_join(threads[i], NULL);
	}

	int max_index = find_max_index(max_indeces);
	cout << "Max index is " << max_index << " with value " << arr[max_index] << "\n";  

	return 0;
}
