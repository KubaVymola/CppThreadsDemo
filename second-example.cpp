#include <iostream>
#include <time.h>
#include <pthread.h>

#define THREAD_COUNT    2

using namespace std;

int returns[THREAD_COUNT];

void* simple_thread(void* arg)
{
    int arg_val = *((int *)arg);
    cout << "This is thread " << arg_val << "\n";
    returns[arg_val] = arg_val + 10;

    return NULL;
}

int main()
{   
    pthread_t threads[THREAD_COUNT];
    int values[] = {0, 1};
    
    pthread_create(&threads[0], NULL, simple_thread, &values[0]);
    pthread_create(&threads[1], NULL, simple_thread, &values[1]);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    cout << returns[0] << "\n";
    cout << returns[1] << "\n";

    cout << "Ending program" << "\n";


    return 0;
}