#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class thread_job
{
private:
    int from, to;
    int job_id;
public:
    thread_job(/* args */);
    ~thread_job();
};

thread_job::thread_job(/* args */)
{
}

thread_job::~thread_job()
{
}



int main()
{


    printf("%d\n", 10);

    return 0;
}