#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>

const int MAX_SIZE = 100;

// Структура для передачи данных потокам
struct ThreadData {
    int* array;
    int size;
};

// Структура для передачи данных потокам marker
struct MarkerThreadData {
    int thread_id;
    int* array;
    int size;
    pthread_mutex_t* mutex;
    pthread_cond_t* cond;
    bool* continue_work;
    bool* finished;
};

// Функция потока marker
void* markerThread(void* arg) {
    MarkerThreadData* data = static_cast<MarkerThreadData*>(arg);

    srand(data->thread_id);

    while (*(data->continue_work)) {
        pthread_mutex_lock(data->mutex);

        // Ждем, пока все потоки marker не подадут сигналы о невозможности продолжения работы
        while (*(data->continue_work)) {
            int index = rand() % data->size;
            if (data->array[index] == 0) {
                // Если элемент массива равен нулю, помечаем его
                usleep(5000); // Поспать 5 миллисекунд
                data->array[index] = data->thread_id;
                usleep(5000); // Поспать 5 миллисекунд
                break;
            }
        }

        if (*(data->continue_work)) {
            // Выводим информацию и даем сигнал потоку main
            std::cout << "Поток " << data->thread_id << ": " << "Помечено " << data->thread_id << ", элемент массива, который невозможно пометить: -1" << std::endl;
            *(data->continue_work) = false;
            pthread_cond_signal(data->cond);
        } else {
            // Ждем ответный сигнал от потока main
            pthread_cond_wait(data->cond, data->mutex);
        }

        pthread_mutex_unlock(data->mutex);
    }

    *(data->finished) = true;

    pthread_exit(NULL);
}

int main() {
    int array[MAX_SIZE];
    int size;
    int num_threads;

    std::cout << "Введите размер массива: ";
    std::cin >> size;

    if (size > MAX_SIZE) {
        std::cerr << "Ошибка: Размер массива превышает максимальный размер." << std::endl;
        return 1;
    }

    std::fill_n(array, size, 0); // Инициализируем массив нулями

    std::cout << "Введите количество потоков marker: ";
    std::cin >> num_threads;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    bool continue_work = true;
    bool finished = false;

    // Создаем потоки marker
    std::vector<pthread_t> markerThreads(num_threads);
    std::vector<MarkerThreadData> markerData(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        markerData[i].thread_id = i + 1;
        markerData[i].array = array;
        markerData[i].size = size;
        markerData[i].mutex = &mutex;
        markerData[i].cond = &cond;
        markerData[i].continue_work = &continue_work;
        markerData[i].finished = &finished;

        pthread_create(&markerThreads[i], NULL, markerThread, &markerData[i]);
    }

    // Даем сигнал на начало работы всех потоков marker
    pthread_cond_broadcast(&cond);

    while (!finished) {
        pthread_mutex_lock(&mutex);

        // Ждем, пока все потоки marker не подадут сигналы о невозможности продолжения работы
        while (!continue_work) {
            pthread_cond_wait(&cond, &mutex);
        }

        // Выводим содержимое массива на консоль
        std::cout << "Массив: ";
        for (int i = 0; i < size; ++i) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;

        int marker_id;
        std::cout << "Введите порядковый номер потока marker для завершения работы: ";
        std::cin >> marker_id;

        if (marker_id <= num_threads && marker_id > 0) {
            // Посылаем сигнал потоку marker для завершения работы
            continue_work = false;
            pthread_cond_signal(&cond);

            // Ждем завершения работы потока marker
            pthread_join(markerThreads[marker_id - 1], NULL);

            // Выводим содержимое массива на консоль
            std::cout << "Массив: ";
            for (int i = 0; i < size; ++i) {
                std::cout << array[i] << " ";
            }
            std::cout << std::endl;

            // Посылаем сигнал о продолжении работы оставшимся потокам marker
            continue_work = true;
            pthread_cond_broadcast(&cond);
        } else {
            std::cout << "Ошибка: Неверный порядковый номер потока marker." << std::endl;
        }

        pthread_mutex_unlock(&mutex);
    }

    // Ожидаем завершения работы оставшихся потоков marker
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(markerThreads[i], NULL);
    }

    return 0;
}