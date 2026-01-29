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
