#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

// Определение структуры employee
struct employee {
    int num;        // идентификационный номер сотрудника
    char name[10];  // имя сотрудника
    double hours;   // количество отработанных часов
};

// Функция для запуска утилиты и ожидания завершения
void runUtility(const std::string &command) {
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error: Command failed: " << command << std::endl;
        exit(1);
    }
}

int main() {
    std::string binary_file;
    int num_records;
    std::string report_file;
    double hourly_rate;

    // Запрашиваем имя бинарного файла и количество записей
    std::cout << "Введите имя бинарного файла: ";
    std::cin >> binary_file;
    std::cout << "Введите количество записей: ";
    std::cin >> num_records;

    // Запускаем утилиту Creator
    std::string creator_command = "./creator " + binary_file + " " + std::to_string(num_records);
    runUtility(creator_command);

    // Выводим содержимое созданного бинарного файла
    std::ifstream infile(binary_file, std::ios::binary);
    if (!infile) {
        std::cerr << "Не удалось открыть бинарный файл." << std::endl;
        return 1;
    }
    std::cout << "Содержимое бинарного файла:" << std::endl;
    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        std::cout << "ID: " << emp.num << ", Имя: " << emp.name << ", Часы: " << emp.hours << std::endl;
    }
    infile.close();

    // Запрашиваем имя файла отчета и оплату за час работы
    std::cout << "Введите имя файла отчета: ";
    std::cin >> report_file;
    std::cout << "Введите оплату за час работы: ";
    std::cin >> hourly_rate;

    // Запускаем утилиту Reporter
    std::string reporter_command = "./reporter " + binary_file + " " + report_file + " " + std::to_string(hourly_rate);
    runUtility(reporter_command);

    // Выводим отчет на консоль
    std::ifstream report(report_file);
    if (!report) {
        std::cerr << "Не удалось открыть файл отчета." << std::endl;
        return 1;
    }
    std::cout << "Отчет:" << std::endl;
    std::string line;
    while (std::getline(report, line)) {
        std::cout << line << std::endl;
    }
    report.close();

    return 0;
}
