#include <iostream>
#include <pthread.h>
#include <unistd.h> // Для функции sleep

const int MAX_SIZE = 100;

// Структура для передачи данных потокам
struct ThreadData {
    int* array;
    int size;
};

// Функция для нахождения минимального и максимального элементов массива
void* minMaxThread(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    int min = data->array[0];
    int max = data->array[0];

    for (int i = 1; i < data->size; ++i) {
        if (data->array[i] < min) {
            min = data->array[i];
        }
        if (data->array[i] > max) {
            max = data->array[i];
        }
        sleep(7); // "спать" 7 миллисекунд
    }

    std::cout << "Минимальный элемент: " << min << std::endl;
    std::cout << "Максимальный элемент: " << max << std::endl;

    pthread_exit(NULL);
}

// Функция для нахождения среднего значения элементов массива
void* averageThread(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    int sum = 0;
    for (int i = 0; i < data->size; ++i) {
        sum += data->array[i];
        sleep(12); // "спать" 12 миллисекунд
    }

    float average = static_cast<float>(sum) / data->size;

    std::cout << "Среднее значение элементов: " << average << std::endl;

    pthread_exit(NULL);
}

int main() {
    int array[MAX_SIZE];
    int size;

    std::cout << "Введите размер массива: ";
    std::cin >> size;

    if (size > MAX_SIZE) {
        std::cerr << "Ошибка: Размер массива превышает максимальный размер." << std::endl;
        return 1;
    }

    std::cout << "Введите элементы массива: ";
    for (int i = 0; i < size; ++i) {
        std::cin >> array[i];
    }

    pthread_t minMaxThreadID, averageThreadID;

    ThreadData data = {array, size};

    // Создание потоков
    pthread_create(&minMaxThreadID, NULL, minMaxThread, &data);
    pthread_create(&averageThreadID, NULL, averageThread, &data);

    // Ожидание завершения потоков
    pthread_join(minMaxThreadID, NULL);
    pthread_join(averageThreadID, NULL);

    // Нахождение среднего значения
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += array[i];
    }
    float average = static_cast<float>(sum) / size;

    // Замена минимального и максимального элементов на среднее значение
    for (int i = 0; i < size; ++i) {
        if (array[i] == data.array[0] || array[i] == data.array[size - 1]) {
            array[i] = average;
        }
    }

    // Вывод измененного массива
    std::cout << "Измененный массив: ";
    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
