#include "test.h"
#include "unit.h"
#include "list.h"
#include <assert.h>
#include <iostream>
#include <cstring>

using namespace std;

void testUnitWithDefaultConstructor() {
    //Тестирование конструктора по умолчанию
    Unit unit;
    assert(Unit::NAME_DEFAULT == unit.getName());
    assert(Unit::PRICE_DEFAULT == unit.getPrice());
    assert(Unit::VOLUME_DEFAULT == unit.getVolume());
}

void testUnitWithOneParameter() {
    //Тестирование инициализирующего конструктора с 1 параметром
    Unit unit("Item_1");
    assert("Item_1" == unit.getName());
    assert(Unit::PRICE_DEFAULT == unit.getPrice());
    assert(Unit::VOLUME_DEFAULT == unit.getVolume());
}

void testUnitWithTwoParameters() {
    //Тестирование инициализирующего конструктора с 2 параметрами
    Unit unit("Item_2", 2300);
    assert("Item_2" == unit.getName());
    assert(2300 == unit.getPrice());
    assert(Unit::VOLUME_DEFAULT == unit.getVolume());
}

void testUnitWithThreeParameters() {
    //Тестирование инициализирующего конструктора с 3 параметрами
    Unit unit("Item_3", 2300,500);
    assert("Item_3" == unit.getName());
    assert(2300 == unit.getPrice());
    assert(500 == unit.getVolume());
}

void testUnitWithCopyConstructor() {
    //Тестирование копирующего конструктора
    Unit unit1("Item_1", 1500, 1000);
    Unit unit2(unit1);
    assert("Item_1" == unit2.getName());
    assert(1500 == unit2.getPrice());
    assert(1000 == unit2.getVolume());
}

void testUnitWithCopyAssign() {
    //Тестирование присваивания
    Unit unit1("Item_1", 1500, 1000);
    Unit unit2 = unit1;
    assert("Item_1" == unit2.getName());
    assert(1500 == unit2.getPrice());
    assert(1000 == unit2.getVolume());
}

void testUnitSetName() {
    //Тестирование установки значения названия
    Unit lamp;
    lamp.setName("Item_1");
    assert("Item_1" == lamp.getName());
}

void testUnitSetPrice() {
    //Тестирование установки значения цены
    Unit unit;

    unit.setPrice(31500);
    assert(31500 == unit.getPrice());
    unit.setPrice(Unit::PRICE_MIN-1);
    assert(Unit::PRICE_MIN == unit.getPrice());
    unit.setPrice(Unit::PRICE_MAX+1);
    assert(Unit::PRICE_MAX == unit.getPrice());
}

void testUnitSetVolume() {
    //Тестирование установки значения объема
    Unit unit;
    unit.setVolume(930);
    assert(930 == unit.getVolume());
    unit.setVolume(Unit::VOLUME_MIN-1);
    assert(Unit::VOLUME_MIN == unit.getVolume());
    unit.setVolume(Unit::VOLUME_MAX+1);
    assert(Unit::VOLUME_MAX == unit.getVolume());
}

void testUnitLimits() {
    //Тестирование установки предельных значений
    Unit unit("Item_1", Unit::PRICE_MAX, Unit::VOLUME_MAX);
    assert("Item_1" == unit.getName());
    assert(Unit::PRICE_MAX == unit.getPrice());
    assert(Unit::VOLUME_MAX == unit.getVolume());
    unit.setPrice(Unit::PRICE_MIN);
    unit.setVolume(Unit::VOLUME_MIN);
    assert(Unit::PRICE_MIN == unit.getPrice());
    assert(Unit::VOLUME_MIN == unit.getVolume());
}

void testListDefault() {
    //Тестирование конструктора по умолчанию
    List list;
    assert(0==list.getCount());
}

void testListWithCopyConstructor() {
    //Тестирование копирующего конструктора
    List list1 ;
    Unit unit1("Item_1", 1500, 1000);
    list1.insertUnit(unit1) ;
    Unit unit2("Item_2", 2500, 2000);
    list1.insertUnit(unit2) ;

    List list2(list1);
    assert(list1.getCount() == list2.getCount());
}

void testListAdd() {
    //Тестирование добавления
    List list1 ;
    assert(0 == list1.getCount());

    Unit unit1("Item_1", 1500, 1000);
    list1.insertUnit(unit1) ;    
    assert(1 == list1.getCount());

    Unit unit2("Item_2", 2500, 2000);
    list1.insertUnit(unit2) ;
    assert(2 == list1.getCount());
}

void testListDeleteFirst() {
    //Тестирование удаления с начала
    List list1 ;
    Unit unit1("Item_1", 1500, 1000);
    Unit unit2("Item_2", 2500, 2000);
    Unit unit3("Item_3", 3500, 3000);
    list1.insertUnit(unit1) ;
    list1.insertUnit(unit2) ;
    list1.insertUnit(unit3) ;

    list1.deleteFirst() ;
    List::Iterator itr = list1.begin() ;
    assert((*itr).getName()=="Item_2") ;

    list1.deleteFirst() ;
    itr = list1.begin() ;
    assert((*itr).getName()=="Item_3") ;

    list1.deleteFirst() ;
    assert(list1.getCount()==0) ;
}

void testListDeleteLast() {
    //Тестирование удаления с конца
    List list1 ;
    Unit unit1("Item_1", 1500, 1000);
    Unit unit2("Item_2", 2500, 2000);
    Unit unit3("Item_3", 3500, 3000);
    list1.insertUnit(unit1) ;
    list1.insertUnit(unit2) ;
    list1.insertUnit(unit3) ;

    list1.deleteLast() ;
    List::Iterator itr = list1.begin() ;
    itr++ ;
    assert((*itr).getName()=="Item_2") ;

    list1.deleteLast() ;
    itr = list1.begin() ;
    assert((*itr).getName()=="Item_1") ;

    list1.deleteLast() ;
    assert(list1.getCount()==0) ;
}

void testListDeleteIndex() {
    //Тестирование удаления индекса
    List list1 ;
    Unit unit1("Item_1", 1500, 1000);
    Unit unit2("Item_2", 2500, 2000);
    Unit unit3("Item_3", 3500, 3000);
    list1.insertUnit(unit1) ;
    list1.insertUnit(unit2) ;
    list1.insertUnit(unit3) ;

    list1.deleteIndex(1) ;
    List::Iterator itr = list1.begin() ;
    assert((*itr).getName()=="Item_1") ;
    itr++ ;
    assert((*itr).getName()=="Item_3") ;
}

// Встроенная процедура для проверки упорядоченности
void _subTestOrdered(const List & list) {
    List::Iterator itr = list.begin();
    assert(itr!=list.end()) ;
    assert((*itr).getName()=="Item_1") ;

    itr++ ;
    assert(itr!=list.end()) ;
    assert((*itr).getName()=="Item_2") ;

    itr++ ;
    assert(itr!=list.end()) ;
    assert((*itr).getName()=="Item_3") ;

    itr++ ;
    assert(itr==list.end()) ;
}

void testListAddOrdered() {
    //Тестирование добавления в порядке возрастания
    List list1 ;
    Unit unit1("Item_1", 1500, 1000);
    Unit unit2("Item_2", 2500, 2000);
    Unit unit3("Item_3", 3500, 3000);

    //Тестирование добавления в порядке возрастания
    list1.insertUnit(unit1) ;
    list1.insertUnit(unit2) ;
    list1.insertUnit(unit3) ;
    _subTestOrdered(list1) ;

    //Тестирование добавления в порядке убывания
    list1.clear() ;
    list1.insertUnit(unit3) ;
    list1.insertUnit(unit2) ;
    list1.insertUnit(unit1) ;
    _subTestOrdered(list1) ;

    //Тестирование добавления в 1-3-2
    list1.clear() ;
    list1.insertUnit(unit1) ;
    list1.insertUnit(unit3) ;
    list1.insertUnit(unit2) ;
    _subTestOrdered(list1) ;

    // Тестирование случайных данных
    const int N = 100 ;
    Unit u("ItemX",0,0) ;
    list1.clear() ;
    for (int i=0; i<N; i++) {
        u.setPrice(rand() % 1000) ;
        list1.insertUnit(u) ;
    }

    List::Iterator itr = list1.begin();
    int oldprice = (*itr).getPrice() ;
    itr++ ;
    for (int i=0; i<N-2; i++) {
        assert(oldprice<=(*itr).getPrice()) ;
        oldprice = (*itr).getPrice() ;
        itr++ ;
    }

}

void testListClear() {
    //Тестирование очистки
    List list1 ;
    Unit unit1("Item_1", 1500, 1000);
    list1.insertUnit(unit1) ;
    Unit unit2("Item_2", 2500, 2000);
    list1.insertUnit(unit2) ;
    assert(2 == list1.getCount());

    list1.clear() ;
    assert(0 == list1.getCount());
}

void testListSaveLoad() {
    //Тестирование загрузки/очистки
    List list1 ;
    Unit unit1("Item_1", 1500, 1000);
    list1.insertUnit(unit1) ;
    Unit unit2("Item_2", 2500, 2000);
    list1.insertUnit(unit2) ;
    list1.saveToFile("test.dat") ;

    List list2 ;
    list2.loadFromFile("test.dat") ;

    assert(list2.getCount() == list1.getCount());
}

void testListCompare() {
    //Тестирование сравнения
    List list1 ;
    Unit unit1("Item_1", 1500, 1000);
    list1.insertUnit(unit1) ;
    Unit unit2("Item_2", 2500, 2000);
    list1.insertUnit(unit2) ;

    List list2 ;
    assert(!list1.isListEquals(list2)) ;

    List list3(list1) ;
    assert(list1.isListEquals(list3)) ;

    Unit unit3("Item_3", 2500, 2000);
    list3.insertUnit(unit3) ;
    assert(!list1.isListEquals(list3)) ;

    list1.insertUnit(unit3) ;
    assert(list1.isListEquals(list3)) ;
}

void printNumeredCollection(const List & coll) {
    // Нумерованный вывод коллекции
    int i = 1 ;
    for (Unit & u : coll)
        cout << i++ << "." << u.getName() << " "<<u.getPrice()<<" "<< u.getVolume()<<endl ;
}

void testClasses() {
    //Тестирование класса Лампа
    testUnitWithDefaultConstructor();
    testUnitWithOneParameter();
    testUnitWithTwoParameters();
    testUnitWithThreeParameters();
    testUnitWithCopyConstructor();
    testUnitWithCopyAssign() ;
    testUnitSetName();
    testUnitSetPrice();
    testUnitSetVolume();
    testUnitLimits();

    testListDefault() ;
    testListWithCopyConstructor() ;
    testListAdd() ;
    testListAddOrdered() ;
    testListClear() ;
    testListSaveLoad() ;
    testListCompare() ;    
    testListDeleteFirst() ;
    testListDeleteLast() ;
    testListDeleteIndex() ;

    cout << "All tests are passed" << endl;
}
