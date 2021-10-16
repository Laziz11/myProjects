#include "unitstack.h"
#include <iostream>

using namespace std ;

UnitStack::UnitStack(const string name, const int price):Unit(name,price)
{
}

UnitStack::UnitStack(const UnitStack &unit):Unit(unit)
{
}

UnitStack &UnitStack::operator=(const UnitStack &unit)
{
    if (this != &unit) {
        setName(unit.getName());
        setPrice(unit.getPrice());
    }
    return *this;
}

UnitStack::~UnitStack()
{
}

void UnitStack::printParameters() const {
    //Вывод на экран списка монет
    cout << "Name: " << name_ << endl;
    cout << "Price: " << price_ << endl;
    cout << "Volume: " << getVolume() << endl;
};

int UnitStack::getVolume() const {
    //Возвращает объем
    int ws[5] = {100,50,10,5,1} ;

    int vol = 0 ;
    int ost = price_ ;
    while (ost>0)
        for (int i=0; i<5; i++)
            if (ost>=ws[i]) {
                ost-=ws[i] ;
                vol++ ;
                break ;
            }

    return vol;
};

void UnitStack::setVolume(const int volume) {
    // Пустой метод
}

string UnitStack::getTypeName() const
{
    return "Стопка монет" ;
}

Unit *UnitStack::createPointer() const
{    
    return new UnitStack(*this) ;
};

