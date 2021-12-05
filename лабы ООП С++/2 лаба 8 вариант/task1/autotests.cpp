#include "autotests.h"
#include "MyHash.h"

#include <QString>
//1-Размер коллекции, созданной конструктором по умолчанию, равен нулю
void testSize() {
    MyHash<QString,int> hash ;
    assert(hash.getCount()==0) ;
    hash.add("Item1",100) ;
    assert(hash.getCount()==1) ;
}
//4-При добавлении новой пары ключ-значение K, V размер коллекции увеличивается на единицу, а при добавлении имеющейся пары ключ-значение K, V размер не изменяется
void testAddExist() {
    MyHash<QString,int> hash ;
    hash.add("Item1",100) ;
    assert(hash.getCount()==1) ;
    hash.add("Item2",200) ;
    assert(hash.getCount()==2) ;
    hash.add("Item1",100) ;
    assert(hash.getCount()==2) ;
    hash.add("Item1",150) ;
    assert(hash.getCount()==2) ;
}
//5-Метод, проверяющий наличие ключа принимает добавленный ключ K и возвращает истину
void testExist() {
    MyHash<QString,int> hash ;
    assert(!hash.isContain("Item1")) ;    
    hash.add("Item1",100) ;
    assert(hash.isContain("Item1")) ;
}
//6-Оператор [] принимает добавленный ключ K и возвращает его значение V
void testGetValue() {
    MyHash<QString,int> hash ;
    hash.add("Item1",100) ;
    hash.add("Item2",200) ;
    assert(hash["Item1"]==100) ;
    assert(hash["Item2"]==200) ;
}
//7 and 8 - При удалении существующего ключа K размер коллекции уменьшается на единицу, а при удалении отсутствующего ключа K размер не изменяется
void testDeleteKey() {
    MyHash<QString,int> hash ;
    hash.add("Item1",100) ;
    hash.add("Item2",200) ;
    assert(hash.getCount()==2) ;
    assert(hash.isContain("Item1")) ;
    hash.deleteKey("Item1") ;
    assert(hash.getCount()==1) ;
    assert(!hash.isContain("Item1")) ;
    hash.deleteKey("Item3") ;
    assert(hash.getCount()==1) ;
}
//2-Непустая коллекция после удаления всех объектов равна коллекции, созданной при помощи конструктора по умолчанию)
void testCompareEmpty() {
    MyHash<QString,int> hash ;
    hash.add("Item1",100) ;
    hash.add("Item2",200) ;
    hash.deleteAll() ;
    MyHash<QString,int> hashnew ;
    assert(hash==hashnew) ;
}
//9-Копия непустой коллекции равна оригиналу, после изменения одного из значений V равенство нарушается
void testCompareModify() {
    MyHash<QString,int> hash ;
    hash.add("Item1",100) ;
    hash.add("Item2",200) ;
    MyHash<QString,int> hashnew = hash;
    assert(hash==hashnew) ;
    hashnew.add("Item1",150) ;
    assert(!(hash==hashnew)) ;
}
//3-Непустая коллекция, сохранённая в файл, равна новой коллекции, загруженной из того же файл
void testSaveLoad() {
    MyHash<QString,int> hash ;
    hash.add("Item 1",100) ;
    hash.add("Item 2",200) ;
    hash.saveToFile("hash.tmp") ;
    MyHash<QString,int> hashnew ;
    hashnew.loadFromFile("hash.tmp") ;
    assert(hash==hashnew) ;
}

void testSum() {
    MyHash<QString,int> hash ;
    assert(hash.getSum()==0) ;
    hash.add("Item 1",100) ;
    assert(hash.getSum()==100) ;
    hash.add("Item 2",200) ;
    assert(hash.getSum()==300) ;
    hash.add("Item 3",150) ;
    assert(hash.getSum()==450) ;
}

void collissionTest() {
    MyHash<QString,int> hash ;
    int sum = 0 ;
    int sumodd= 0 ;
    for (int i=0; i<1000; i++) {
        hash.add(QString::number(i,2),i) ;
        sum+=i ;
        if (i % 2 == 0) sumodd+=i ;
    }
    for (int i=0; i<1000; i++) {
        assert(hash[QString::number(i,2)]==i) ;
        assert(hash.isContain(QString::number(i,2))) ;
    }
    assert(hash.getSum()==sum) ;
    for (int i=0; i<1000; i++)
        if (i % 2 != 0) hash.deleteKey(QString::number(i,2)) ;
    for (int i=0; i<1000; i++) {
        if (i % 2==0) {
            assert(hash[QString::number(i,2)]==i) ;
            assert(hash.isContain(QString::number(i,2))) ;
        }
        else
            assert(!hash.isContain(QString::number(i,2))) ;
    }
    assert(hash.getSum()==sumodd) ;
}

bool testAll()
{
    testSize() ; // test 1
    testAddExist() ; // test 2
    testExist() ; // test 3
    testGetValue() ; // test 4
    testDeleteKey() ; // test 5 and 6
    testCompareEmpty() ; // test 7
    testCompareModify() ; // test 8
    testSaveLoad() ; // test 9
    testSum() ;
    collissionTest() ;
}
