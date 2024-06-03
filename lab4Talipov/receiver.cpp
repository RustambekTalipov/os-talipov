#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <csignal>

#define BUFFER_SIZE 20

void signalHandler(int signal) {
    std::cout << "Received signal from Sender\n";
}

int main() {
    std::string file_name;
    int num_senders;

    std::cout << "Enter the name of the binary file: ";
    std::cin >> file_name;

    std::cout << "Enter the number of sender processes: ";
    std::cin >> num_senders;

    std::ofstream outfile(file_name, std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Failed to create binary file\n";
        return EXIT_FAILURE;
    }
    outfile.close();

    signal(SIGUSR1, signalHandler);

    std::cout << "Waiting for all sender processes to be ready...\n";
    for (int i = 0; i < num_senders; ++i) {
        pause(); // Wait for SIGUSR1 from sender
    }

    std::ifstream infile(file_name, std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Failed to open binary file\n";
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    while (true) {
        if (!infile.read(buffer, BUFFER_SIZE)) {
            std::cout << "No messages in file. Waiting...\n";
            sleep(1);
        } else {
            std::cout << "Received message: " << buffer << std::endl;
        }
    }

    infile.close();
    return 0;
}
