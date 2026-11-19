#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <string>
#include <windows.h> 

using namespace std;
using namespace std::chrono;

const int N = 100;

bool inputInt(int& value) {
    string input;
    while (cin >> input) {
        istringstream stream(input);
        char extra;
        if (stream >> value && !(stream >> extra)) return true;
        cout << "Ошибка: введите целое число: ";
    }
    return false;
}

// =========================================================================
// ЗАДАНИЕ 1: СОЗДАНИЕ И ВЫВОД МАССИВА
// =========================================================================
void generateRandomArray(int arr[]) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(-99, 99);
    for (int i = 0; i < N; i++) arr[i] = dist(gen);
}

bool inputArrayManually(int arr[]) {
    cout << "Введите " << N << " целых чисел:" << endl;
    for (int i = 0; i < N; i++) {
        if (!inputInt(arr[i])) return false;
    }
    return true;
}

void printArray(int arr[]) {
    for (int i = 0; i < N; i++) cout << arr[i] << " ";
    cout << endl;
}

// =========================================================================
// ЗАДАНИЕ 2: СОРТИРОВКИ (4 простые O(n^2) и 1 быстрая O(n log n))
// =========================================================================
void bubbleSort(int arr[]) {
    for (int i = 0; i < N - 1; i++)
        for (int j = 0; j < N - i - 1; j++)
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
}

void insertionSort(int arr[]) {
    for (int i = 1; i < N; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(int arr[]) {
    for (int i = 0; i < N - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < N; j++)
            if (arr[j] < arr[minIndex]) minIndex = j;
        if (minIndex != i) swap(arr[i], arr[minIndex]);
    }
}

void cocktailSort(int arr[]) {
    int left = 0, right = N - 1;
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (int i = left; i < right; i++)
            if (arr[i] > arr[i + 1]) { swap(arr[i], arr[i + 1]); swapped = true; }
        right--;
        for (int i = right; i > left; i--)
            if (arr[i] < arr[i - 1]) { swap(arr[i], arr[i - 1]); swapped = true; }
        left++;
    }
}

void quickSort(int arr[], int low, int high) {
    if (low >= high) return;
    int pivot = arr[low + (high - low) / 2];
    int left = low, i = low, right = high;
    while (i <= right) {
        if (arr[i] < pivot) swap(arr[left++], arr[i++]);
        else if (arr[i] > pivot) swap(arr[i], arr[right--]);
        else i++;
    }
    quickSort(arr, low, left - 1);
    quickSort(arr, right + 1, high);
}

// =========================================================================
// ЗАДАНИЕ 3: ПОИСК MIN/MAX С ЗАМЕРОМ И СРАВНЕНИЕМ
// =========================================================================
void task3_findMinMaxTime(int arr[]) {
    // 1. Замер на неотсортированном массиве
    auto start1 = steady_clock::now();
    int minVal = arr[0], maxVal = arr[0];
    for (int i = 1; i < N; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }
    auto end1 = steady_clock::now();
    auto time1 = duration_cast<nanoseconds>(end1 - start1).count();

    cout << "\n--- Задание 3: Поиск Min/Max ---\n";
    cout << "Min = " << minVal << ", Max = " << maxVal << "\n";
    cout << "Время поиска (один проход): " << time1 << " нс\n";

    // 2. Замер на отсортированном массиве
    int tempArr[N];
    copy(arr, arr + N, tempArr);
    quickSort(tempArr, 0, N - 1);

    auto start2 = steady_clock::now();
    int sortedMin = tempArr[0];
    int sortedMax = tempArr[N - 1];
    auto end2 = steady_clock::now();
    auto time2 = duration_cast<nanoseconds>(end2 - start2).count();

    cout << "В отсортированном: Min = " << sortedMin << ", Max = " << sortedMax << "\n";
    cout << "Время взятия по краям: " << time2 << " нс\n";
    cout << "Поиск по краям работает за O(1); время сортировки не включено. Замеры приблизительные.\n";
}

// =========================================================================
// ЗАДАНИЕ 4: СРЕДНЕЕ ЗНАЧЕНИЕ MIN/MAX И ПОИСК СОВПАДЕНИЙ
// =========================================================================
void task4_averageMinMax(int arr[]) {
    int minVal = arr[0], maxVal = arr[0];
    for (int i = 1; i < N; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    double avg = ((double)minVal + maxVal) / 2.0;
    int roundedAvg = (int)round(avg);

    cout << "\n--- Задание 4: Среднее Min/Max ---\n";
    cout << "Min: " << minVal << ", Max: " << maxVal << "\n";
    cout << "Среднее (округленное): " << roundedAvg << "\n";

    int indexes[N];
    auto start = steady_clock::now();
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (arr[i] == roundedAvg) {
            indexes[count] = i;
            count++;
        }
    }
    auto end = steady_clock::now();
    auto time_ns = duration_cast<nanoseconds>(end - start).count();

    cout << "Индексы в исходном массиве, равных " << roundedAvg << ": ";
    for (int i = 0; i < count; i++) cout << indexes[i] << " ";
    cout << "\nКоличество таких элементов: " << count << "\n";
    cout << "Время поиска: " << time_ns << " нс\n";
    int sortedArr[N], sortedIndexes[N];
    copy(arr, arr + N, sortedArr);
    quickSort(sortedArr, 0, N - 1);
    int sortedCount = 0;
    auto sortedStart = steady_clock::now();
    for (int i = 0; i < N; i++) {
        if (sortedArr[i] == roundedAvg) sortedIndexes[sortedCount++] = i;
    }
    auto sortedEnd = steady_clock::now();
    cout << "Индексы в отсортированном массиве: ";
    for (int i = 0; i < sortedCount; i++) cout << sortedIndexes[i] << " ";
    cout << "\nКоличество таких элементов: " << sortedCount << "\n";
    cout << "Время поиска: " << duration_cast<nanoseconds>(sortedEnd - sortedStart).count() << " нс (без сортировки)\n";
    cout << "Сравнение: время поиска линейно зависит от размера массива (O(n)).\n";
}

// =========================================================================
// ЗАДАНИЯ 5 и 6: ПОДСЧЕТ < a И > b (СРАВНЕНИЕ ЛИНЕЙНОГО И БИНАРНОГО)
// =========================================================================
void task5_6_countLessGreater(int arr[]) {
    int a, b;
    cout << "\n--- Задание 5-6: Подсчет < a и > b ---\n";
    cout << "Введите число a: "; if (!inputInt(a)) return;
    cout << "Введите число b: "; if (!inputInt(b)) return;

    int tempArr[N];
    copy(arr, arr + N, tempArr);
    quickSort(tempArr, 0, N - 1);

    // 1. Линейный поиск
    auto start1 = steady_clock::now();
    int countLessLinear = 0, countGreaterLinear = 0;
    for (int i = 0; i < N; i++) {
        if (tempArr[i] < a) countLessLinear++;
        if (tempArr[i] > b) countGreaterLinear++;
    }
    auto end1 = steady_clock::now();
    auto time1 = duration_cast<nanoseconds>(end1 - start1).count();

    // 2. Бинарный поиск
    auto start2 = steady_clock::now();
    int* lowA = lower_bound(tempArr, tempArr + N, a);
    int countLessBin = lowA - tempArr;
    int* highB = upper_bound(tempArr, tempArr + N, b);
    int countGreaterBin = (tempArr + N) - highB;
    auto end2 = steady_clock::now();
    auto time2 = duration_cast<nanoseconds>(end2 - start2).count();

    cout << "[Линейный] Меньше " << a << ": " << countLessLinear << " | Больше " << b << ": " << countGreaterLinear << "\n";
    cout << "Время линейного: " << time1 << " нс\n";
    cout << "[Бинарный] Меньше " << a << ": " << countLessBin << " | Больше " << b << ": " << countGreaterBin << "\n";
    cout << "Время бинарного: " << time2 << " нс\n";
    cout << "Сравнение: бинарный быстрее, так как работает за O(log n) вместо O(n).\n";
}

// =========================================================================
// ЗАДАНИЕ 7: ЛИНЕЙНЫЙ И БИНАРНЫЙ ПОИСК (УСРЕДНЕННЫЙ ЗАМЕР)
// =========================================================================
void task7_compareSearch(int arr[]) {
    int target;
    cout << "\n--- Задание 7: Линейный vs Бинарный поиск ---\n";
    cout << "Введите число для поиска: "; if (!inputInt(target)) return;

    int tempArr[N];
    copy(arr, arr + N, tempArr);
    quickSort(tempArr, 0, N - 1);

    int idxLinear = -1, idxBinary = -1;
    long long timeLinear = 0, timeBinary = 0;
    int iterations = 1000;

    // Линейный
    auto start1 = steady_clock::now();
    for (int k = 0; k < iterations; k++) {
        idxLinear = -1;
        for (int i = 0; i < N; i++) {
            if (tempArr[i] == target) { idxLinear = i; break; }
        }
    }
    auto end1 = steady_clock::now();
    timeLinear = duration_cast<nanoseconds>(end1 - start1).count() / iterations;

    // Бинарный
    auto start2 = steady_clock::now();
    for (int k = 0; k < iterations; k++) {
        idxBinary = -1;
        int left = 0, right = N - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (tempArr[mid] == target) { idxBinary = mid; break; }
            if (tempArr[mid] < target) left = mid + 1;
            else right = mid - 1;
        }
    }
    auto end2 = steady_clock::now();
    timeBinary = duration_cast<nanoseconds>(end2 - start2).count() / iterations;

    cout << "Линейный: " << (idxLinear != -1 ? "найден" : "не найден") << " (Среднее: " << timeLinear << " нс)\n";
    cout << "Бинарный: " << (idxBinary != -1 ? "найден" : "не найден") << " (Среднее: " << timeBinary << " нс)\n";
    if (idxLinear != -1) cout << "Индекс в исходном массиве: " << idxLinear << "\n";
    if (idxBinary != -1) cout << "Индекс в отсортированном массиве: " << idxBinary << "\n";
    cout << "Время бинарного поиска указано без сортировки массива.\n";
}

// =========================================================================
// ЗАДАНИЕ 8: ОБМЕН ЭЛЕМЕНТОВ (СРАВНЕНИЕ ОДИНОЧНОГО И УСРЕДНЕННОГО)
// =========================================================================
void task8_swapElements(int arr[]) {
    int idx1, idx2;
    cout << "\n--- Задание 8: Обмен элементов ---\n";
    cout << "Введите индексы 1 и 2: ";
    if (!inputInt(idx1) || !inputInt(idx2)) return;

    if (idx1 < 0 || idx1 >= N || idx2 < 0 || idx2 >= N) {
        cout << "Ошибка: неверные индексы!\n"; return;
    }

    int tempArr[N];
    copy(arr, arr + N, tempArr);

    // Одиночный обмен
    auto start1 = steady_clock::now();
    swap(tempArr[idx1], tempArr[idx2]);
    auto end1 = steady_clock::now();
    auto timeSingle = duration_cast<nanoseconds>(end1 - start1).count();

    // 100 000 обменов
    int iterations = 100000;
    auto start2 = steady_clock::now();
    for (int i = 0; i < iterations; i++) {
        swap(tempArr[idx1], tempArr[idx2]);
    }
    auto end2 = steady_clock::now();
    auto total_ns = duration_cast<nanoseconds>(end2 - start2).count();
    double avgTime = (double)total_ns / iterations;

    cout << "Время одного обмена: " << timeSingle << " нс\n";
    cout << "Среднее время обмена (" << iterations << " итераций): " << avgTime << " нс\n";
    swap(arr[idx1], arr[idx2]);
    cout << "Основной массив после обмена:\n";
    printArray(arr);
    cout << "Замеры времени являются приблизительными.\n";
}

// =========================================================================
// ИНДИВИДУАЛЬНОЕ ЗАДАНИЕ №4: СДВИГ ВПРАВО НА K ПОЗИЦИЙ
// =========================================================================
void individualTask4(int arr[]) {
    int k;
    cout << "\n--- Индивидуальное задание 4: Сдвиг вправо ---\n";
    cout << "Введите количество позиций K: "; if (!inputInt(k)) return;
    k = ((k % N) + N) % N;

    auto start = steady_clock::now();
    int tempArr[N];
    for (int i = 0; i < N; i++) {
        tempArr[(i + k) % N] = arr[i];
    }
    auto end = steady_clock::now();
    auto time_ns = duration_cast<nanoseconds>(end - start).count();

    cout << "Массив после сдвига:\n";
    printArray(tempArr);

    long long sumEven = 0;
    for (int i = 0; i < N; i += 2) {
        sumEven += tempArr[i];
    }

    cout << "Сумма элементов на четных позициях: " << sumEven << "\n";
    cout << "Время сдвига: " << time_ns << " нс (алгоритм O(n)).\n";
}

// =========================================================================
// ГЛАВНАЯ ФУНКЦИЯ (МЕНЮ)
// =========================================================================
int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);

    int originalArray[N];

    // Выбор способа создания первого массива
    int startChoice;
    cout << "Как создать первый массив?\n";
    cout << "1 - Сгенерировать случайно\n";
    cout << "2 - Ввести вручную\n";
    cout << "Ваш выбор: ";
    if (!inputInt(startChoice)) {
        cout << "\nВвод завершен.\n";
        return 0;
    }
    cout << endl;

    if (startChoice == 1) {
        generateRandomArray(originalArray);
        cout << "Сгенерированный массив:\n";
        printArray(originalArray);
    }
    else if (startChoice == 2) {
        if (!inputArrayManually(originalArray)) {
            cout << "\nВвод завершен.\n";
            return 0;
        }
        cout << "Ваш массив:\n";
        printArray(originalArray);
    }
    else {
        cout << "Неверный выбор. Создан случайный массив по умолчанию.\n";
        generateRandomArray(originalArray);
        printArray(originalArray);
    }

    int choice;
    do {
        cout << "\n================ МЕНЮ ================\n";
        cout << "1. Пузырьковая сортировка (замер)\n";
        cout << "2. Сортировка вставками (замер)\n";
        cout << "3. Сортировка выбором (замер)\n";
        cout << "4. Шейкерная сортировка (замер)\n";
        cout << "5. Быстрая сортировка (замер)\n";
        cout << "6. Поиск Min/Max (Задание 3)\n";
        cout << "7. Среднее Min/Max (Задание 4)\n";
        cout << "8. Элементы < a и > b (Задание 5-6)\n";
        cout << "9. Линейный/Бинарный поиск (Задание 7)\n";
        cout << "10. Обмен элементов (Задание 8)\n";
        cout << "11. Сдвиг массива вправо (Инд. задание 4)\n";
        cout << "12. Создать/Ввести новый массив\n";
        cout << "13. Показать текущий массив\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";
        if (!inputInt(choice)) {
            cout << "\nВвод завершен.\n";
            break;
        }

        int copyArray[N];

        switch (choice) {
        case 1: {
            copy(originalArray, originalArray + N, copyArray);
            auto start = steady_clock::now();
            bubbleSort(copyArray);
            auto end = steady_clock::now();
            cout << "Время (Пузырьковая): " << duration_cast<microseconds>(end - start).count() << " мкс\n";
            break;
        }
        case 2: {
            copy(originalArray, originalArray + N, copyArray);
            auto start = steady_clock::now();
            insertionSort(copyArray);
            auto end = steady_clock::now();
            cout << "Время (Вставками): " << duration_cast<microseconds>(end - start).count() << " мкс\n";
            break;
        }
        case 3: {
            copy(originalArray, originalArray + N, copyArray);
            auto start = steady_clock::now();
            selectionSort(copyArray);
            auto end = steady_clock::now();
            cout << "Время (Выбором): " << duration_cast<microseconds>(end - start).count() << " мкс\n";
            break;
        }
        case 4: {
            copy(originalArray, originalArray + N, copyArray);
            auto start = steady_clock::now();
            cocktailSort(copyArray);
            auto end = steady_clock::now();
            cout << "Время (Шейкерная): " << duration_cast<microseconds>(end - start).count() << " мкс\n";
            break;
        }
        case 5: {
            copy(originalArray, originalArray + N, copyArray);
            auto start = steady_clock::now();
            quickSort(copyArray, 0, N - 1);
            auto end = steady_clock::now();
            cout << "Время (Быстрая): " << duration_cast<microseconds>(end - start).count() << " мкс\n";
            break;
        }
        case 6: task3_findMinMaxTime(originalArray); break;
        case 7: task4_averageMinMax(originalArray); break;
        case 8: task5_6_countLessGreater(originalArray); break;
        case 9: task7_compareSearch(originalArray); break;
        case 10: task8_swapElements(originalArray); break;
        case 11: individualTask4(originalArray); break;
        case 12: {
            int method;
            cout << "Выберите способ создания массива:\n";
            cout << "1 - Сгенерировать случайно\n";
            cout << "2 - Ввести вручную\n";
            cout << "Ваш выбор: ";
            if (!inputInt(method)) {
                cout << "\nВвод завершен.\n";
                return 0;
            }

            if (method == 1) {
                generateRandomArray(originalArray);
                cout << "Сгенерирован новый случайный массив:\n";
                printArray(originalArray);
            }
            else if (method == 2) {
                if (!inputArrayManually(originalArray)) {
                    cout << "\nВвод завершен.\n";
                    return 0;
                }
                cout << "Ваш массив принят:\n";
                printArray(originalArray);
            }
            else {
                cout << "Неверный выбор. Массив остался прежним.\n";
            }
            break;
        }
        case 13:
            cout << "Текущий массив:\n";
            printArray(originalArray);
            break;
        case 0: cout << "Выход из программы.\n"; break;
        default: cout << "Неверный выбор. Попробуйте снова.\n";
        }
        if (choice >= 1 && choice <= 5) {
            cout << "Отсортированный массив:\n";
            printArray(copyArray);
            cout << (is_sorted(copyArray, copyArray + N) ? "Массив отсортирован правильно.\n" : "Ошибка: массив не отсортирован.\n");
        }
    } while (choice != 0);

    return 0;
}
