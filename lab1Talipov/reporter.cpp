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
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <binary file> <report file> <hourly rate>" << std::endl;
        return 1;
    }

    std::string binary_file = argv[1];
    std::string report_file = argv[2];
    double hourly_rate = std::stod(argv[3]);

    std::ifstream infile(binary_file, std::ios::binary);
    if (!infile) {
        std::cerr << "Не удалось открыть бинарный файл: " << binary_file << std::endl;
        return 1;
    }

    std::ofstream outfile(report_file);
    if (!outfile) {
        std::cerr << "Не удалось открыть файл отчета: " << report_file << std::endl;
        return 1;
    }

    outfile << "Отчет по файлу «" << binary_file << "»\n";
    outfile << "Номер сотрудника, имя сотрудника, часы, зарплата\n";

    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        double salary = emp.hours * hourly_rate;
        outfile << emp.num << ", " << emp.name << ", " << emp.hours << ", " << salary << "\n";
    }

    infile.close();
    outfile.close();

    // Чтение отчета
    std::ifstream report(report_file);
    if (!report) {
        std::cerr << "Не удалось открыть файл отчета: " << report_file << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(report, line)) {
        std::cout << line << std::endl;
    }

    report.close();

    return 0;
}
