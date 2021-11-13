#include "test.h"
#include "object.h"
#include "list.h"
#include "objectstack.h"
#include <assert.h>
#include <iostream>
#include <cstring>

using namespace std;

void testobjectWithDefaultConstructor() {
    //Тестирование конструктора по умолчанию
    Object object;
    assert(Object::DEFAULT_OBJECT_NAME == object.getObjectName());
    assert(Object::DEFAULT_OBJECT_PRICE == object.getObjectPrice());
    assert(Object::DEFAULT_OBJECT_VOLUME == object.getObjectVolume());
}

void testobjectWithOneParameter() {
    //Тестирование инициализирующего конструктора с 1 параметром
    Object object("Item_1");
    assert("Item_1" == object.getObjectName());
    assert(Object::DEFAULT_OBJECT_PRICE == object.getObjectPrice());
    assert(Object::DEFAULT_OBJECT_VOLUME == object.getObjectVolume());
}

void testobjectWithTwoParameters() {
    //Тестирование инициализирующего конструктора с 2 параметрами
    Object object("Item_2", 2300);
    assert("Item_2" == object.getObjectName());
    assert(2300 == object.getObjectPrice());
    assert(Object::DEFAULT_OBJECT_VOLUME == object.getObjectVolume());
}

void testobjectWithThreeParameters() {
    //Тестирование инициализирующего конструктора с 3 параметрами
    Object object("Item_3", 2300,500);
    assert("Item_3" == object.getObjectName());
    assert(2300 == object.getObjectPrice());
    assert(500 == object.getObjectVolume());
}

void testobjectWithCopyConstructor() {
    //Тестирование копирующего конструктора
    Object object1("Item_1", 1500, 1000);
    Object object2(object1);
    assert("Item_1" == object2.getObjectName());
    assert(1500 == object2.getObjectPrice());
    assert(1000 == object2.getObjectVolume());
}

void testobjectWithCopyAssign() {
    //Тестирование присваивания
    Object object1("Item_1", 1500, 1000);
    Object object2 = object1;
    assert("Item_1" == object2.getObjectName());
    assert(1500 == object2.getObjectPrice());
    assert(1000 == object2.getObjectVolume());
}

void testobjectSetName() {
    //Тестирование установки значения названия
    Object lamp;
    lamp.setObjectName("Item_1");
    assert("Item_1" == lamp.getObjectName());
}

void testobjectSetPrice() {
    //Тестирование установки значения цены
    Object object;

    object.setObjectPrice(31500);
    assert(31500 == object.getObjectPrice());
    object.setObjectPrice(Object::MIN_OBJECT_PRICE-1);
    assert(Object::MIN_OBJECT_PRICE == object.getObjectPrice());
    object.setObjectPrice(Object::MAX_OBJECT_PRICE+1);
    assert(Object::MAX_OBJECT_PRICE == object.getObjectPrice());
}

void testobjectSetVolume() {
    //Тестирование установки значения объема
    Object object;
    object.setObjectVolume(930);
    assert(930 == object.getObjectVolume());
    object.setObjectVolume(Object::MIN_OBJECT_VOLUME-1);
    assert(Object::MIN_OBJECT_VOLUME == object.getObjectVolume());
    object.setObjectVolume(Object::MAX_OBJECT_VOLUME+1);
    assert(Object::MAX_OBJECT_VOLUME == object.getObjectVolume());
}

void testobjectLimits() {
    //Тестирование установки предельных значений
    Object object("Item_1", Object::MAX_OBJECT_PRICE,Object::MAX_OBJECT_VOLUME);
    assert("Item_1" == object.getObjectName());
    assert(Object::MAX_OBJECT_PRICE == object.getObjectPrice());
    assert(Object::MAX_OBJECT_VOLUME == object.getObjectVolume());
    object.setObjectPrice(Object::MIN_OBJECT_PRICE);
    object.setObjectVolume(Object::MIN_OBJECT_VOLUME);
    assert(Object::MIN_OBJECT_PRICE == object.getObjectPrice());
    assert(Object::MIN_OBJECT_VOLUME == object.getObjectVolume());
}

void testListDefault() {
    //Тестирование конструктора по умолчанию
    List list;
    assert(0==list.getListCount());
}

void testListWithCopyConstructor() {
    //Тестирование копирующего конструктора
    List list1 ;
    Object object1("Item_1", 1500, 1000);
    list1.addObjectToItems(object1) ;
    Object object2("Item_2", 2500, 2000);
    list1.addObjectToItems(object2) ;

    List list2(list1);
    assert(list1.getListCount() == list2.getListCount());
}

void testListAdd() {
    //Тестирование добавления
    List list1 ;
    assert(0 == list1.getListCount());

    Object object1("Item_1", 1500, 1000);
    list1.addObjectToItems(object1) ;    
    assert(1 == list1.getListCount());

    Object object2("Item_2", 2500, 2000);
    list1.addObjectToItems(object2) ;
    assert(2 == list1.getListCount());
}

void testListDeleteFirst() {
    //Тестирование удаления с начала
    List list1 ;
    Object object1("Item_1", 1500, 1000);
    Object object2("Item_2", 2500, 2000);
    Object object3("Item_3", 3500, 3000);
    list1.addObjectToItems(object1) ;
    list1.addObjectToItems(object2) ;
    list1.addObjectToItems(object3) ;

    list1.deleteFirstItem() ;
    List::Iterator itr = list1.begin() ;
    assert((*itr).getObjectName()=="Item_2") ;

    list1.deleteFirstItem() ;
    itr = list1.begin() ;
    assert((*itr).getObjectName()=="Item_3") ;

    list1.deleteFirstItem() ;
    assert(list1.getListCount()==0) ;
}

void testListDeleteLast() {
    //Тестирование удаления с конца
    List list1 ;
    Object object1("Item_1", 1500, 1000);
    Object object2("Item_2", 2500, 2000);
    Object object3("Item_3", 3500, 3000);
    list1.addObjectToItems(object1) ;
    list1.addObjectToItems(object2) ;
    list1.addObjectToItems(object3) ;

    list1.deleteLastItem() ;
    List::Iterator itr = list1.begin() ;
    itr++ ;
    assert((*itr).getObjectName()=="Item_2") ;

    list1.deleteLastItem() ;
    itr = list1.begin() ;
    assert((*itr).getObjectName()=="Item_1") ;

    list1.deleteLastItem() ;
    assert(list1.getListCount()==0) ;
}

void testListDeleteIndex() {
    //Тестирование удаления индекса
    List list1 ;
    Object object1("Item_1", 1500, 1000);
    Object object2("Item_2", 2500, 2000);
    Object object3("Item_3", 3500, 3000);
    list1.addObjectToItems(object1) ;
    list1.addObjectToItems(object2) ;
    list1.addObjectToItems(object3) ;

    list1.deleteItemByIndex(1) ;
    List::Iterator itr = list1.begin() ;
    assert((*itr).getObjectName()=="Item_1") ;
    itr++ ;
    assert((*itr).getObjectName()=="Item_3") ;
}

// Встроенная процедура для проверки упорядоченности
void _subTestOrdered(const List & list) {
    List::Iterator itr = list.begin();
    assert(itr!=list.end()) ;
    assert((*itr).getObjectName()=="Item_1") ;

    itr++ ;
    assert(itr!=list.end()) ;
    assert((*itr).getObjectName()=="Item_2") ;

    itr++ ;
    assert(itr!=list.end()) ;
    assert((*itr).getObjectName()=="Item_3") ;

    itr++ ;
    assert(itr==list.end()) ;
}

void testListAddOrdered() {
    //Тестирование добавления в порядке возрастания
    List list1;
    Object object1("Item_1", 1500, 1000);
    Object object2("Item_2", 2500, 2000);
    Object object3("Item_3", 3500, 3000);

    //Тестирование добавления в порядке возрастания
    list1.addObjectToItems(object1) ;
    list1.addObjectToItems(object2) ;
    list1.addObjectToItems(object3) ;
    _subTestOrdered(list1) ;

    //Тестирование добавления в порядке убывания
    list1.clearList() ;
    list1.addObjectToItems(object3) ;
    list1.addObjectToItems(object2) ;
    list1.addObjectToItems(object1) ;
    _subTestOrdered(list1) ;

    //Тестирование добавления в 1-3-2
    list1.clearList() ;
    list1.addObjectToItems(object1) ;
    list1.addObjectToItems(object3) ;
    list1.addObjectToItems(object2) ;
    _subTestOrdered(list1) ;

    // Тестирование случайных данных
    const int N = 100 ;
    Object u("ItemX",0,0) ;
    list1.clearList() ;
    for (int i=0; i<N; i++) {
        u.setObjectPrice(rand() % 1000) ;
        list1.addObjectToItems(u) ;
    }

    List::Iterator itr = list1.begin();
    int oldprice = (*itr).getObjectPrice() ;
    itr++ ;
    for (int i=0; i<N-2; i++) {
        assert(oldprice<=(*itr).getObjectPrice()) ;
        oldprice = (*itr).getObjectPrice() ;
        itr++ ;
    }

}

void testListClear() {
    //Тестирование очистки
    List list1 ;
    Object object1("Item_1", 1500, 1000);
    list1.addObjectToItems(object1) ;
    Object object2("Item_2", 2500, 2000);
    list1.addObjectToItems(object2) ;
    assert(2 == list1.getListCount());

    list1.clearList() ;
    assert(0 == list1.getListCount());
}

void testListSaveLoad() {
    //Тестирование загрузки/очистки
    List list1 ;
    Object object1("Item_1", 1500, 1000);
    list1.addObjectToItems(object1) ;
    Object object2("Item_2", 2500, 2000);
    list1.addObjectToItems(object2) ;
    list1.saveListToFile("test.txt") ;

    List list2 ;
    list2.loadListFromFile("test.txt") ;

    assert(list2.getListCount() == list1.getListCount());
}

void testListCompare() {
    //Тестирование сравнения
    List list1 ;
    Object object1("Item_1", 1500, 1000);
    list1.addObjectToItems(object1) ;
    Object object2("Item_2", 2500, 2000);
    list1.addObjectToItems(object2) ;

    List list2 ;
    assert(!list1.isEqual(list2)) ;

    List list3(list1) ;
    assert(list1.isEqual(list3)) ;

    Object object3("Item_3", 2500, 2000);
    list3.addObjectToItems(object3) ;
    assert(!list1.isEqual(list3)) ;

    list1.addObjectToItems(object3) ;
    assert(list1.isEqual(list3)) ;
}

void testobjectStackWithDefaultConstructor() {
    //Тестирование конструктора по умолчанию
    ObjectStack object;
    assert(Object::DEFAULT_OBJECT_NAME == object.getObjectName());
    assert(Object::DEFAULT_OBJECT_PRICE == object.getObjectPrice());
    assert(10 == object.getObjectVolume());
}

void testobjectStackWithParameters() {
    //Тестирование инициализирующего конструктора
    ObjectStack object("Item_1",123);
    assert("Item_1" == object.getObjectName());
    assert(123 == object.getObjectPrice());
    assert(6 == object.getObjectVolume());
}

void testobjectStackVolumes() {
    //Тестирование расчета объема
    ObjectStack object("Item_1",66);
    assert(4 == object.getObjectVolume());
    object.setObjectPrice(77) ;
    assert(6 == object.getObjectVolume());
    object.setObjectPrice(88) ;
    assert(8 == object.getObjectVolume());
    object.setObjectPrice(999) ;
    assert(9+1+4+1+4 == object.getObjectVolume());
}

void testobjectInfo() {
    //Тестирование метода информации о классе
    Object object1("Item_1",123);
    ObjectStack object2("Item_1",123);
    List list1 ;
    list1.addObjectToItems(object1) ;
    list1.addObjectToItems(object2) ;
    assert("OBJECT"==object1.getTypeName()) ;
    assert("OBJECT_STACK"==object2.getTypeName()) ;
}

void printItemsList(const List & coll) {
    // Нумерованный вывод коллекции
    int i = 1 ;
    for (Object & u : coll)
        cout << i++ << "." << u.getObjectName() << " "<<u.getObjectPrice()<<" "<< u.getObjectVolume()<<endl ;
}

void testClasses() {
    //Тестирование класса Лампа
    testobjectWithDefaultConstructor();
    testobjectWithOneParameter();
    testobjectWithTwoParameters();
    testobjectWithThreeParameters();
    testobjectWithCopyConstructor();
    testobjectWithCopyAssign() ;
    testobjectSetName();
    testobjectSetPrice();
    testobjectSetVolume();
    testobjectLimits();

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

    testobjectStackWithDefaultConstructor() ;
    testobjectStackWithParameters() ;
    testobjectStackVolumes() ;
    testobjectInfo() ;

    cout << "All tests are passed" << endl;
}
