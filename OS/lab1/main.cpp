#include <iostream>
#include <pthread.h>

#define LENGTH 3

struct DATA {
    int *array1;
    int *array2;
    int amount1;
    int amount2;
};

void *threadFunction1(void *arguments) {
    auto *data = (DATA *) arguments;
    int amount = 0;

    for (int i = 0; i < LENGTH; i++) {
        amount = amount + data->array1[i];
    }
    data->amount1 = amount;

    return nullptr;
}

void *threadFunction2(void *arguments) {
    auto *data = (DATA *) arguments;
    int amount = 0;

    for (int i = 0; i < LENGTH; i++) {
        amount = amount + data->array2[i];
    }
    data->amount2 = amount;

    return nullptr;
}

int main() {
    DATA data{};
    data.array1 = new int[LENGTH] { 1, 3, 9 };
    data.array2 = new int[LENGTH] { 2, 4, 6 };

    pthread_t thread1, thread2;

    pthread_create(&thread1, nullptr, threadFunction1, (void *) &data);
    pthread_create(&thread2, nullptr, threadFunction2, (void *) &data);

    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    std::cout << "Результат первого потока: " << data.amount1 << std::endl;
    std::cout << "Результат второго потока: " << data.amount2 << std::endl;

    return 0;
}