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
