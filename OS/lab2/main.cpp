#include <iostream>
#include <math.h>
#include <pthread.h>

struct DATA {
    int start;
    int end;
    int stepThread;
    double amount;
};

double calculateMember(int n) {
    double member = 1.0 / ((3.0 * n - 2.0) * (3.0 * n + 1.0));

    return member;
}

void *calculateRow(void *arguments) {
    auto *data = (DATA *) arguments;
    data->amount = 0.0;

    for (int i = data->start; i <= data->end; i += data->stepThread) {
        data->amount += calculateMember(i);
    }

    return nullptr;
}

void mainEvaluation(int n, int countThreads, double *amount) {
    auto *threads = new pthread_t[countThreads];
    auto *data = new DATA[countThreads];

    for (int i = 0; i < countThreads && i < n; i++) {
        data[i].start = i + 1;
        data[i].end = n;
        data[i].stepThread = countThreads;

        pthread_create(&threads[i], nullptr, calculateRow, (void *) &data[i]);
    }

    for (int i = 0; i < countThreads; i++) {
        pthread_join(threads[i], nullptr);
    }

    for (int i = 0; i < countThreads; i++) {
        *amount += data[i].amount;
    }
}

int main() {
    int n;
    int threads;
    auto amount = new double();

    std::cout << "Введите количество членов ряда: ";
    std::cin >> n;

    std::cout << "Введите количество потоков: ";
    std::cin >> threads;

    mainEvaluation(n, threads, amount);

    std::cout << "Сумма ряда: " << *amount;

    return 0;
}