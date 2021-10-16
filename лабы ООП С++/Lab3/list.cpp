#include "list.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include "unitstack.h"

List::ListItem *List::createLI(const Unit &unit, List::ListItem *prior, List::ListItem *next)
{
    auto item = new ListItem() ;
    item->item = unit.createPointer() ;
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
        if (unit.getPrice()<first->item->getPrice()) {
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
    if (unit.getPrice()<first->item->getPrice()) {
        ListItem * oldfirst = first ;
        first = createLI(unit,nullptr,oldfirst) ;
        oldfirst->prior = first ;
        return ;
    }
    // Больше, чем последний
    if (unit.getPrice()>=last->item->getPrice()) {
        ListItem * oldlast = last ;
        last = createLI(unit,oldlast,nullptr) ;
        oldlast->next = last ;
        return ;
    }
    // Где-то посередине

    ListItem * tek = first ;
    while (tek->next!=nullptr) {
        if ((tek->item->getPrice()<unit.getPrice())&&(unit.getPrice()<=tek->next->item->getPrice())) {
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
        first = nullptr ;
        last = nullptr ;
        cnt = 0 ;
        return ;
    }

    // Остальные варианты
    delete first ;
    first = first->next ;
    first->prior = nullptr ;
    cnt-- ;
}

void List::deleteLast()
{
    // Случай пустого списка
    if (first==nullptr) return ;

    // Случай списка из одного элемента
    if (first==last) {
        first = nullptr ;
        last = nullptr ;
        cnt = 0 ;
        return ;
    }

    // Остальные варианты
    delete last ;
    last = last->prior ;
    last->next = nullptr ;
    cnt-- ;
}

void List::deleteIndex(int i)
{
    // Удаление за границами
    if ((i<0)||(i>=cnt)) return ;

    // Сначала тривиальные случаи
    if (i==0) deleteFirst() ; else
        if (i==cnt-1) deleteLast() ; else {
            // А потом удаление в середине
            ListItem * tek = first ;
            while (i>1) {
                tek=tek->next ;
                i-- ;
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
}

void List::clear()
{
    // Очистка
    if (first!=nullptr) delete first ;
    if (last!=nullptr) delete last ;
    first=nullptr ;
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
        fout<<u.getTypeName()<<endl ;
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
        Unit unit ;
        UnitStack unitstack ;
        fin.getline(buf,255) ;
        if (string(buf)==unit.getTypeName()) {
            fin.getline(buf,255) ;
            unit.setName(buf) ;
            fin.getline(buf,255) ;
            unit.setPrice(atoi(buf)) ;
            fin.getline(buf,255) ;
            unit.setVolume(atoi(buf)) ;
            insertUnit(unit) ;
        }
        else {
            fin.getline(buf,255) ;
            unitstack.setName(buf) ;
            fin.getline(buf,255) ;
            unitstack.setPrice(atoi(buf)) ;
            fin.getline(buf,255) ;
            unitstack.setVolume(atoi(buf)) ;
            insertUnit(unitstack) ;
        }
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
