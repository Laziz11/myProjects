#include <iostream>
#include "unit.h"


Unit::Unit(const string name, const int price, const int volume){
    /*
    Конструктор по умолчанию:
    Название = Name
    Цена = 1000
    Объем = 200
    Конструктор иницализации:
    1) с 1 параметром (Название - задается, Цена = 1000, Объем = 200)
    2) с 2 параметрами (Название - задается, Цена - задается, Объем = 200)
    3) с 3 параметрами (Название, Цена, Объем - задаются)
    */
    setName(name);
    setPrice(price);
    setVolume(volume);
};

Unit::Unit(const Unit &unit){
    //Конструктор копирования
    setName(unit.getName());
    setPrice(unit.getPrice());
    setVolume(unit.getVolume());
}

Unit &Unit::operator=(const Unit &unit)
{
    if (this != &unit) {
        setName(unit.getName());
        setPrice(unit.getPrice());
        setVolume(unit.getVolume());
    }
    return *this;
}

Unit::~Unit()
{
};

void Unit::printParameters() const {
    //Вывод на экран параметров
    cout << "Name: " << name_ << endl;
    cout << "Price: " << price_ << endl;
    cout << "Volume: " << volume_ << endl;
};

const string & Unit::getName() const {
    //Возвращет название
    return name_;
};

int Unit::getPrice() const {
    //Возвращает цену
    return price_;
};

int Unit::getVolume() const {
    //Возвращает объем
    return volume_;
};

void Unit::setPrice(const int price) {
    //Проверяет корректность значения цены, устанавливает значение
    if (price <= PRICE_MIN)
        price_ = PRICE_MIN;
    else if (price >= PRICE_MAX)
        price_ = PRICE_MAX;
    else
        price_ = price;
};

void Unit::setVolume(const int volume) {
    //Проверяет корректность значения объема, устанавливает значение
    if (volume <= VOLUME_MIN)
        volume_ = VOLUME_MIN;
    else if (volume >= VOLUME_MAX)
        volume_ = VOLUME_MAX;
    else
        volume_ = volume;
};

void Unit::setName(string name){
    //Устанавливает значение названия
   name_=std::move(name);
};

string Unit::getTypeName() const
{
    return "Вещь" ;
}

string Unit::getTitle() const
{
    return getTypeName()+" "+getName()+", цена "+to_string(getPrice())+", объем "+to_string(getVolume()) ;
}

Unit *Unit::createPointer() const
{    
    return new Unit(*this) ;
};

