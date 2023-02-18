#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define max 10
int min = -1;

int flag[2];
int turn, buffer[max];

void lock_init()
{
    flag[0] = flag[1] = 0;
    turn = 0;
}

void lock(int thread_num)
{
    flag[thread_num] = 1;
    turn = 1 - thread_num;
    while (flag[turn] == 1 && turn == 1 - thread_num)
        ;
}

void unlock(int thread_num)
{
    flag[thread_num] = 0;
}

void produce(int num, int item)
{
    int thread_num = num;
    printf("Thread Entered: %d\n", thread_num);
    if (min == max - 1)
    {
        printf("Can't Produce, Buffer is Full!!!\n");
        exit(NULL);
    }
    else
    {
        buffer[++min] = item;
        sleep(item);
        printf("Item produced = %d\n", buffer[min]);
    }
}

void *producer(int thread_num)
{
    do
    {
        lock(thread_num);
        produce(thread_num, rand() % 10);
        unlock(thread_num);
    } while (turn == thread_num);
}

void consume(int num)
{
    int thread_num = num;
    printf("Thread Entered: %d\n", thread_num);

    if (min < 0)
    {
        printf("Can't Consume, Buffer is empty!!!\n");
    }
    else
    {
        int item = buffer[0];
        for (int i = 0; i < min; i++)
        {
            buffer[i] = buffer[i + 1];
        }
        min--;
        sleep(item);
        printf("Item consumed = %d\n", item);
    }
}

void *consumer(int num)
{
    int thread_num = num;
    do
    {
        lock(thread_num);
        consume(thread_num);
        unlock(thread_num);
    } while (turn == thread_num);
}

int main()
{
    pthread_t t1, t2;
    lock_init();
    pthread_create(&t1, NULL, producer, 0);
    pthread_create(&t2, NULL, consumer, 1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
