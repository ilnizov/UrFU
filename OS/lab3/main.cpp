#include <iostream>
#include <cstring>
#include <unistd.h>
#include "petersons.h"

struct Process
{
    int number;
    char symbol;
};

char *result;

void *thread_function(void *args) {
    auto *process = (Process *) args;
    usleep(100000);

    enter_region(process->number);

    for (int i = 0; i < 10; i++) {
        strcat(result, &process->symbol);
        usleep(10000);
    }

    leave_region(process->number);
}

void main_evaluation() {
    auto *threads = new pthread_t[n];
    auto *process = new Process[n];

    char sym = 65;

    for (int i = 0; i < n; i++, sym++) {
        process[i].number = i;
        process[i].symbol = sym;

        pthread_create(&threads[i], nullptr, thread_function, (void *) &process[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], nullptr);
    }
}

int main() {
    printf("Enter the number of threads:\n");
    std::cin >> n;

    last = new int[n];
    stage = new int[n];

    for (int i = 0; i < n; i++) {
        stage[i] = 0;
    }


    for (int i = 0; i < n; i++) {
        last[i] = 0;
    }

    result = new char[n * 10] { '\0' };

    main_evaluation();
    puts(result);

    return 0;
}