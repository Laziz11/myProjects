#include <iostream>
#include "test.h"
#include "unit.h"
#include "list.h"

int main()
{    
    //Тестирование класса и пример использования
    testClasses();

    Unit unit("Item", 4800, 1200);
    unit.printParameters();
    unit.setName("NewItem");
    unit.setPrice(1000);
    unit.setVolume(5100);
    unit.printParameters();

    List list ;

    for(List::Iterator item = list.begin(); item!=list.end(); item++)
       (*item).printParameters() ;

    cout << endl << "List count: " << list.getCount() << endl;
    printNumeredCollection(list) ;

    list.insertUnit(unit) ;
    cout << endl << "List count: " << list.getCount() << endl;
    printNumeredCollection(list) ;

    cout << endl << "List count: " << list.getCount() << endl;
    printNumeredCollection(list) ;

    unit.setName("NewItem1");
    unit.setPrice(10001);
    unit.setVolume(51001);
    list.insertUnit(unit) ;
    unit.setName("NewItem2");
    unit.setPrice(10002);
    unit.setVolume(51002);
    list.insertUnit(unit) ;

    cout << endl << "List count: " << list.getCount() << endl;
    printNumeredCollection(list) ;

    cout << endl << "List count: " << list.getCount() << endl;
    printNumeredCollection(list) ;

    List list2 = list ;

    cout << endl << "List2 count: " << list2.getCount() << endl;
    printNumeredCollection(list2) ;

    list2.saveToFile("coll.dat") ;

    List list3 ;
    list3.loadFromFile("coll.dat") ;
    cout << endl << "List3 count: " << list3.getCount() << endl;
    printNumeredCollection(list3) ;

    return 0;
}
