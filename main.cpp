#include <iostream>
#include <algorithm> // Для std::swap

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Проверка левого дочернего элемента
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // Проверка правого дочернего элемента
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // Если наибольший элемент не корень
    if (largest != i) {
        std::cout << "Меняем " << arr[i] << " и " << arr[largest] << std::endl; // Дебаг вывод
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapsort(int arr[], int n) {
    // Построение кучи
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Сортировка
    for (int i = n - 1; i >= 0; i--) {
        std::cout << "Перемещаем корень " << arr[0] << " в конец массива" << std::endl; // Дебаг вывод
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

int main() {
    int arr[] = {12, 11, 13, 5, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    std::cout << "Исходный массив: ";
    for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
    std::cout << std::endl;

    heapsort(arr, n);

    std::cout << "Отсортированный массив: ";
    for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
    std::cout << std::endl;

    return 0;
}
