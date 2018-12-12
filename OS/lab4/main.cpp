#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <random>
#include <cstdlib>
#include <ctime>
#include <w32api/synchapi.h>

char alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

pthread_mutex_t reader_lock;
pthread_mutex_t writer_lock;

struct Container {
    int size;
    int current_item;

    std::queue<char> items;
};

struct Attribute {
    int id_thread;
    int cycle_sum;

    std::queue<int> readers_queue;
    Container *container;
};

int get_top(Attribute *attribute) {
    int top = 0;

    pthread_mutex_lock(&reader_lock);
    top = attribute->readers_queue.front();
    pthread_mutex_unlock(&reader_lock);

    return top;
}

void *writer(void *args) {
    auto attribute = (Attribute *) args;

    std::random_device r;
    std::seed_seq seed { r() };
    std::mt19937 eng{ seed };
    std::uniform_int_distribution<> dist(0, 25);

    char symbol = alphabet[dist(eng)];


    for (int i = 0; i < attribute->cycle_sum;) {
        usleep(useconds_t(rand() % 500000 + 500000));

        pthread_mutex_lock(&reader_lock);

        if (!attribute->readers_queue.empty()) {
            pthread_mutex_unlock(&reader_lock);
            continue;
        }

        pthread_mutex_lock(&writer_lock);
        pthread_mutex_unlock(&reader_lock);

        if (attribute->container->current_item < attribute->container->size) {
            std::cout << "[" << attribute->id_thread << "] Записываю элемент..." << symbol <<
                      std::endl;
            attribute->container->items.push(symbol);
            attribute->container->current_item++;
            i++;
        } else {
            std::cout << "[" << attribute->id_thread << "] Хранилище полное." << std::endl;
        }

        pthread_mutex_unlock(&writer_lock);
    }

    return nullptr;
}

void *reader(void *args) {
    auto attribute = (Attribute *) args;

    for (int i = 0; i < attribute->cycle_sum;) {
        usleep(useconds_t(rand() % 500000 + 500000));

        pthread_mutex_lock(&reader_lock);
        attribute->readers_queue.push(attribute->id_thread);
        pthread_mutex_unlock(&reader_lock);

        while (get_top(attribute) != attribute->id_thread);

        pthread_mutex_lock(&writer_lock);

        if (attribute->container->current_item > 0) {
            std::cout << "[" << attribute->id_thread << "] Читаю элемент..." <<
                      attribute->container->items.front()<< std::endl;
            attribute->container->items.pop();
            attribute->container->current_item--;
            i++;
        } else {
            std::cout << "[" << attribute->id_thread <<"] Хранилище пустое." << std::endl;
        }

        pthread_mutex_unlock(&writer_lock);

        pthread_mutex_lock(&reader_lock);
        attribute->readers_queue.pop();
        pthread_mutex_unlock(&reader_lock);
    }

    return nullptr;
}

int main() {
    int container_size = 0, number_readers = 0, number_writers = 0, r_chunks_amount = 0,
            w_chunks_amount = 0;

    std::cout << "Размер хранилища: ";
    std::cin >> container_size;
    std::cout << "Кол-во писателей: ";
    std::cin >> number_writers;
    std::cout << "Кол-во порций данных писателя: ";
    std::cin >> w_chunks_amount;
    std::cout << "Кол-во читателей: ";
    std::cin >> number_readers;
    std::cout << "Кол-во порций данных читателя: ";
    std::cin >> r_chunks_amount;

    if ((number_writers * w_chunks_amount - number_readers * r_chunks_amount) > container_size
        || number_readers * r_chunks_amount > number_writers * w_chunks_amount) {
        std::cout << "Неверные параметры!" << std::endl;
        return -1;
    }

    pthread_mutex_init(&reader_lock, nullptr);
    pthread_mutex_init(&writer_lock, nullptr);

    auto writers = new pthread_t[number_writers];
    auto readers = new pthread_t[number_readers];

    auto attribute_writer = new Attribute[number_writers];
    auto attribute_reader = new Attribute[number_readers];

    auto container = new Container[container_size];
    container->current_item = 0;
    container->size = container_size;

    for (int i = 0; i < number_writers; i++) {
        attribute_writer[i].id_thread = i;
        attribute_writer[i].cycle_sum = w_chunks_amount;
        attribute_writer[i].container = container;
        pthread_create(&writers[i], nullptr, writer, (void *) &attribute_writer[i]);
    }

    for (int i = 0; i < number_readers; i++) {
        attribute_reader[i].id_thread = i + number_writers;
        attribute_reader[i].cycle_sum = r_chunks_amount;
        attribute_reader[i].container = container;
        pthread_create(&readers[i], nullptr, reader, (void *) &attribute_reader[i]);
    }

    for (int i = 0; i < number_writers; i++) {
        pthread_join(writers[i], nullptr);
    }

    for (int i = 0; i < number_readers; i++) {
        pthread_join(readers[i], nullptr);
    }

    std::cout << "Остался: " << container->current_item << std::endl;

    pthread_mutex_destroy(&reader_lock);
    pthread_mutex_destroy(&writer_lock);

    return 0;
}