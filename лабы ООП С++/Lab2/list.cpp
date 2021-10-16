#include "list.h"
#include <fstream>
#include <iostream>
#include <string.h>

List::ListItem *List::createLI(const Unit &unit, List::ListItem *prior, List::ListItem *next)
{
    // создаем новый обьект для хранения указателя на пред обьекта и на следующий обьект
    auto item = new ListItem() ;
    item->item = unit ;
    item->prior = prior ;
    item->next = next ;
    return item ;
}

List::List()
{
    // Конструктор по умолчанию
    first = nullptr ;
    last = nullptr ;
    cnt = 0 ;
}

List::List(const List &list)
{
    // Конструктор копирования
    first = nullptr ;
    last = nullptr ;
    cnt = 0 ;

    for (Unit & unit: list)
        insertUnit(unit) ;
}

List &List::operator=(const List &list)
{
    // Присваиваение
    if (this != &list) {
        first = nullptr ;
        last = nullptr ;
        cnt = 0 ;

        for (Unit & unit: list)
            insertUnit(unit) ;
    }
    return *this ;
}

List::~List()
{
    // Деструктор
    clear() ;
}

void List::insertUnit(const Unit &unit)
{
    // Процедура вставки элемента

    // Увеличиваем счетчик
    cnt++ ;

    // Случай пустого списка
    if (first==nullptr) {
        first = last = createLI(unit,nullptr,nullptr) ;
        return ;
    }

    // Случай списка из одного элемента
    if (first==last) {
        // Меньше, чем
        if (unit.getPrice()<first->item.getPrice()) {
            ListItem * oldfirst = first ;
            first = createLI(unit,nullptr,last) ;
            last->prior = first ;
            last = oldfirst ;
        }
        // Больше или равно, чем
        else {
            ListItem * oldlast = last ;
            last = createLI(unit,first,nullptr) ;
            first->next = last ;
            first = oldlast ;
        }
        return ;
    }

    // Остальное - случаи списка из двух и более элементов
    // Меньше, чем первый
    if (unit.getPrice()<first->item.getPrice()) {
        ListItem * oldfirst = first ;
        first = createLI(unit,nullptr,oldfirst) ;
        oldfirst->prior = first ;
        return ;
    }
    // Больше, чем последний
    if (unit.getPrice()>=last->item.getPrice()) {
        ListItem * oldlast = last ;
        last = createLI(unit,oldlast,nullptr) ;
        oldlast->next = last ;
        return ;
    }
    // Где-то посередине

    ListItem * tek = first ;
    while (tek->next!=nullptr) {
        if ((tek->item.getPrice()<unit.getPrice())&&(unit.getPrice()<=tek->next->item.getPrice())) {
            ListItem * newel = createLI(unit, tek, tek->next) ;
            tek->next = newel ;
            tek->next->prior = newel ;
            return ;
        }
        tek = tek->next ;
    }
}

void List::deleteFirst()
{
    // Случай пустого списка
    if (first==nullptr) return ;

    // Случай списка из одного элемента
    if (first==last) {
        delete first ;
        first = nullptr ;
        last = nullptr ;
        cnt = 0 ;
        return ;
    }

    // Остальные варианты
    ListItem * saved = first->next ;
    delete first ;
    first = saved ;
    first->prior = nullptr ;
    cnt-- ;
}

void List::deleteLast()
{
    // Случай пустого списка
    if (first==nullptr) return ;

    // Случай списка из одного элемента
    if (first==last) {
        delete first ;
        first = nullptr ;
        last = nullptr ;
        cnt = 0 ;
        return ;
    }

    // Остальные варианты
    ListItem * saved = last->prior ;
    delete last ;
    last = saved ;
    last->next = nullptr ;
    cnt-- ;
}

void List::deleteIndex(int index)
{
    // Удаление за границами
    if ((index < 0) || ( index >= cnt)) return ;

    // Сначала тривиальные случаи
    if (index==0) deleteFirst() ; else
        if (index==cnt-1) deleteLast() ; else {
            // А потом удаление в середине
            ListItem * tek = first ;
            while (index>1) {
                tek=tek->next ;
                index-- ;
            }
            tek->next = tek->next->next ;
            tek->next->prior = tek ;
            cnt-- ;
        }

}

int List::getCount() const
{
      // Количество записей
    return cnt ;
/*

    if (first==nullptr) return 0 ;

    int cnt = 1 ;
    ListItem * tek = first ;
    while (tek->next!=nullptr) {
        tek = tek->next ;
        cnt++ ;
    }
    return cnt ;
*/
}

void List::clear()
{
    // Очистка
    delete first ;
    first=nullptr ;
    delete last ;
    last=nullptr ;
    cnt=0 ;
}

bool List::saveToFile(const string &str) const
{
    // Запись в файл
    ofstream fout ;
    fout.open(str,ios_base::out) ;
    if (!fout) return false ;

    fout<<getCount()<<endl ;
    for (Unit & u: *this) {
        fout<<u.getName()<<endl ;
        fout<<u.getPrice()<<endl ;
        fout<<u.getVolume()<<endl ;
    }

    fout.close() ;
    return true ;
}

bool List::loadFromFile(const string &str)
{
    // Чтение из файла
    ifstream fin ;
    fin.open(str,ios_base::in) ;
    if (!fin) return false ;

    clear() ;

    char buf[255] ;
    fin.getline(buf,255) ;
    int cnt = atoi(buf) ;
    for (int i=0; i<cnt; i++) {
        Unit u ;
        fin.getline(buf,255) ;
        u.setName(buf) ;
        fin.getline(buf,255) ;
        u.setPrice(atoi(buf)) ;
        fin.getline(buf,255) ;
        u.setVolume(atoi(buf)) ;
        insertUnit(u) ;
    }

    fin.close() ;
    return true ;
}

bool List::isListEquals(const List & list) const
{
    // Сравнение коллекций
    if (list.getCount()!=getCount()) return false ;

    Iterator item1 = list.begin() ;
    for(Iterator item2 = begin(); item2!=end(); item2++) {
       if ((*item1).getName()!=(*item2).getName()) return false ;
       if ((*item1).getPrice()!=(*item2).getPrice()) return false ;
       if ((*item1).getVolume()!=(*item2).getVolume()) return false ;
       item1++ ;
    }

    return true ;
}
