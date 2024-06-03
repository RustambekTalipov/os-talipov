#include <iostream>
#include <fstream>
#include <string>

// Структура employee
struct employee {
    int num;        // идентификационный номер сотрудника
    char name[10];  // имя сотрудника
    double hours;   // количество отработанных часов
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <binary file> <number of records>" << std::endl;
        return 1;
    }

    std::string binary_file = argv[1];
    int num_records = std::stoi(argv[2]);

    std::ofstream outfile(binary_file, std::ios::binary);
    if (!outfile) {
        std::cerr << "Не удалось открыть бинарный файл для записи." << std::endl;
        return 1;
    }

    for (int i = 0; i < num_records; ++i) {
        employee emp;
        std::cout << "Введите идентификационный номер сотрудника: ";
        std::cin >> emp.num;
        std::cout << "Введите имя сотрудника: ";
        std::cin >> emp.name;
        std::cout << "Введите количество отработанных часов: ";
        std::cin >> emp.hours;

        outfile.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    outfile.close();
    std::cout << "Бинарный файл успешно создан." << std::endl;
    return 0;
}
