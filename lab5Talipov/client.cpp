#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME "fifo_channel"

struct Employee {
    int num;
    char name[10];
    double hours;
};

int main() {
    mkfifo(FIFO_NAME, 0666); // Создание именованного канала
    int fifo_fd = open(FIFO_NAME, O_WRONLY); // Открытие канала для записи

    Employee employee;

    std::cout << "Enter employee ID: ";
    std::cin >> employee.num;
    std::cout << "Enter employee name: ";
    std::cin >> employee.name;
    std::cout << "Enter employee hours: ";
    std::cin >> employee.hours;

    write(fifo_fd, &employee, sizeof(Employee)); // Запись данных в канал

    close(fifo_fd); // Закрытие канала

    return 0;
}
