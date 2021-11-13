#include "objectstack.h"
#include <iostream>

using namespace std ;

ObjectStack::ObjectStack(const string name, const int price):Object(name,price){
}

ObjectStack::ObjectStack(const ObjectStack &object):Object(object){
}

ObjectStack &ObjectStack::operator=(const ObjectStack &object){
    if (this != &object) {
        setObjectName(object.getObjectName());
        setObjectPrice(object.getObjectPrice());
    }
    return *this;
}

ObjectStack::~ObjectStack()
{
}

int ObjectStack::getObjectVolume() const {
    //Возвращает объем
    int ws[5] = {100,50,10,5,1} ;

    int vol = 0 ;
    int ost = objectPrice ;
    while (ost>0)
        for (int i=0; i<5; i++)
            if (ost>=ws[i]) {
                ost-=ws[i] ;
                vol++ ;
                break ;
            }

    return vol;
};

void ObjectStack::setObjectVolume(const int volume) {
    // Пустой метод
}

string ObjectStack::getTypeName() const
{
    return TYPE;
}

Object *ObjectStack::createPointer() const
{    
    return new ObjectStack(*this) ;
};

