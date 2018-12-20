#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

void *write_thread(void *args) {
    std::cout << "Enter your messages: " << std::endl;

    int pipe = *(int *) args;
    char *message = new char[255];

    do {
        gets(message);
        write(pipe, message, 255);
    } while (strcmp(message, "close") != 0);
}

void *read_thread(void *args) {
    int pipe = *(int *) args;
    char *message = new char[255];

    do {
        int done = static_cast<int>(read(pipe, message, 255));

        if (strcmp(message, "close") == 0) {
            std::cout << "Server has finished writing" << std::endl;
        } else {
            if (done > 0) {
                std::cout << "Received a message: " << message << std::endl;
            }
        }
    } while (strcmp(message, "close") != 0);
}

void main_evaluation(int *pipes) {
    pthread_t writer;
    pthread_t reader;

    pthread_create(&writer, nullptr, write_thread, (void *) &pipes[1]);
    pthread_create(&reader, nullptr, read_thread, (void *) &pipes[0]);

    pthread_join(writer, nullptr);
    pthread_join(reader, nullptr);
}

int main() {
    char writer_path[] =  "/tmp/client_to_server";
    char reader_path[] = "/tmp/server_to_client";

    int *pipes = new int[2];
    pipes[0] = open(reader_path, O_RDONLY | O_NONBLOCK);
    std::cout << "Waiting for the server to start" << std::endl;

    while(pipes[0] < 0) {
        pipes[0] = open(reader_path, O_RDONLY | O_NONBLOCK);
    }
    std::cout << "Opened pipe for reading" << std::endl;

    pipes[1] = open(writer_path, O_WRONLY | O_NONBLOCK);
    while (pipes[1] < 0) {
        pipes[1] = open(writer_path, O_WRONLY | O_NONBLOCK);
    }
    std::cout << "Opened pipe for writing" << std::endl;

    main_evaluation(pipes);

    close(pipes[0]);
    close(pipes[1]);

    return 0;
}