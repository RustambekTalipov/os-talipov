#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_NAME "employee_data.bin"
#define FIFO_NAME "fifo_channel"

struct Employee {
    int num;
    char name[10];
    double hours;
};

int main() {
    mkfifo(FIFO_NAME, 0666); // Создание именованного канала
    int fifo_fd = open(FIFO_NAME, O_RDONLY); // Открытие канала для чтения

    std::ofstream file(FILE_NAME, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to create binary file\n";
        return EXIT_FAILURE;
    }

    std::cout << "Enter the number of clients: ";
    int num_clients;
    std::cin >> num_clients;

    std::cout << "Waiting for clients to request modifications...\n";

    // Ожидание запросов от клиентов
    for (int i = 0; i < num_clients; ++i) {
        Employee employee;
        read(fifo_fd, &employee, sizeof(Employee));
        file.write(reinterpret_cast<char*>(&employee), sizeof(Employee));
    }

    file.close();
    close(fifo_fd);
    unlink(FIFO_NAME); // Удаление именованного канала

    // Открытие файла для чтения и вывод содержимого на консоль
    std::ifstream infile(FILE_NAME, std::ios::binary);
    if (infile.is_open()) {
        Employee employee;
        std::cout << "Modified file contents:\n";
        while (infile.read(reinterpret_cast<char*>(&employee), sizeof(Employee))) {
            std::cout << "ID: " << employee.num << ", Name: " << employee.name << ", Hours: " << employee.hours << std::endl;
        }
        infile.close();
    } else {
        std::cerr << "Failed to open binary file\n";
        return EXIT_FAILURE;
    }

    return 0;
}
