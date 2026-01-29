#include <iostream>
#include <vector>
#include <string>
#include <utility> 

// cтруктура одной группы 
template <typename KeyType, typename ElemType>
struct GroupInfo {
    KeyType key;      // ключ 
    int count;        // кол элементов
    ElemType minElem; // мин элемент группы (по <)
    ElemType maxElem; // макс элемент группы (по <)
    double sum;       // сумма valueFunc(x) по группе
    double avg;       // среднее sum / count (вещественное)
};
// шаблонная функция группировки
template <typename T, typename KeyFunc, typename ValueFunc>
auto groupStats(T* arr, int n, KeyFunc keyFunc, ValueFunc valueFunc)
{
    // тип ключа берём через decltype
    using KeyType = decltype(keyFunc(std::declval<T>()));

    std::vector<GroupInfo<KeyType, T>> groups;

    for (int i = 0; i < n; ++i) {
        T x = arr[i];
        KeyType key = keyFunc(x);
        double value = (double)valueFunc(x);

        // поиск группы по ключу 
        int index = -1;
        for (int j = 0; j < (int)groups.size(); ++j) {
            if (groups[j].key == key) {
                index = j;
                break;
            }
        }

        // если группы нет то создаём
        if (index == -1) {
            GroupInfo<KeyType, T> g;
            g.key = key;
            g.count = 1;
            g.minElem = x;
            g.maxElem = x;
            g.sum = value;
            g.avg = value; 

            groups.push_back(g);
        }
        // если группа есть то обновляем 
        else {
            groups[index].count++;

            // min/max 
            if (x < groups[index].minElem)
                groups[index].minElem = x;

            if (groups[index].maxElem < x)
                groups[index].maxElem = x;

            groups[index].sum += value;
            groups[index].avg = groups[index].sum / groups[index].count;
        }
    }

    return groups;
}

// вывод групп в формате: key, count, minElem, maxElem, sum, avg 
template <typename KeyType, typename ElemType>
void printGroups(const std::vector<GroupInfo<KeyType, ElemType>>& groups)
{
    std::cout << "key, count, minElem, maxElem, sum, avg\n";
    for (const auto& g : groups) {
        std::cout << g.key << ", "
                  << g.count << ", "
                  << g.minElem << ", "
                  << g.maxElem << ", "
                  << g.sum << ", "
                  << g.avg << "\n";
    }
}

int main()
{
    // 1) Массив int
    // ключ: последняя цифра x % 10
    // value: само число x
    int a[] = {12, 25, 30, 41, 52, 63, 70, 81, 92, 103};
    int n1 = sizeof(a) / sizeof(a[0]);

    auto groupsInt = groupStats(
        a, n1,
        [](int x) { return x % 10; },
        [](int x) { return x; }
    );

    std::cout << "\nINT ARRAY\n";
    printGroups(groupsInt);

    // 2) Массив double 
    // ключ: номер интервала (0: x<0, 1: 0<=x<=10, 2: x>10)
    // value: само число x
    double b[] = {-2.5, 0.0, 3.1, 10.0, 10.2, 25.7, -0.1, 7.7};
    int n2 = sizeof(b) / sizeof(b[0]);

    auto groupsDouble = groupStats(
        b, n2,
        [](double x) {
            if (x < 0) return 0;
            if (0 <= x && x <= 10) return 1;
            return 2;
        },
        [](double x) { return x; }
    );

    std::cout << "\n double array \n";
    printGroups(groupsDouble);

    // 3) Массив std::string 
    // ключ: первая буква строки (char)
    // value: длина строки
    std::string s[] = {"apple", "apricot", "banana", "blue", "cherry", "car", ""};
    int n3 = sizeof(s) / sizeof(s[0]);

    auto groupsString = groupStats(
        s, n3,
        [](const std::string& x) {
            // на случай пустой строки
            if (x.empty()) return '?';
            return x[0];
        },
        [](const std::string& x) {
            return (double)x.length();
        }
    );

    std::cout << "\n string array\n";
printGroups(groupsString);

    return 0;
}
