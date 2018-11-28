#include <iostream>
#include <cstring>
#include <unistd.h>

char *result;
pthread_mutex_t lock;

void *thread_function(void *args)
{
    char *symbol = new char;
    symbol[0] = *(char *) args;

    usleep(100000);
    pthread_mutex_lock(&lock);

    for (int i = 0; i < 10; i++)
    {
        strcat(result, symbol);
        usleep(10000);
    }

    pthread_mutex_unlock(&lock);
}

void main_evaluation(int number)
{
    auto *threads = new pthread_t[number];
    char symbol = 65;
    char *symbols = new char[number];

    for (int i = 0; i < number; i++, symbol++)
    {
        symbols[i] = symbol;
        pthread_create(&threads[i], nullptr, thread_function, (void *) &symbols[i]);
    }

    for (int i = 0; i < number; i++)
    {
        pthread_join(threads[i], nullptr);
    }
}

int main()
{
    int number;
    printf("Enter the number of threads:\n");
    std::cin >> number;

    result = new char[number * 10] { '\0' };

    pthread_mutex_init(&lock, nullptr);

    main_evaluation(number);
    puts(result);

    pthread_mutex_destroy(&lock);

    return 0;
}