#include <iostream>
#include "object.h"


Object::Object(const string name, const int price, const int volume){
    setObjectName(name);
    setObjectPrice(price);
    setObjectVolume(volume);
};

Object::Object(const Object &object){
    setObjectName(object.getObjectName());
    setObjectPrice(object.getObjectPrice());
    setObjectVolume(object.getObjectVolume());
}

Object &Object::operator=(const Object &object)
{
    if (this != &object) {
        setObjectName(object.getObjectName());
        setObjectPrice(object.getObjectPrice());
        setObjectVolume(object.getObjectVolume());
    }
    return *this;
}

Object::~Object()
{

};

void Object::printObjectData() const {
    cout << "Name: " << objectName << endl;
    cout << "Price: " << objectPrice << endl;
    cout << "Volume: " << objectVolume << endl;
};

const string & Object::getObjectName() const {
    //Возвращет название
    return objectName;
};

int Object::getObjectPrice() const {
    //Возвращает цену
    return objectPrice;
};

int Object::getObjectVolume() const {
    //Возвращает объем
    return objectVolume;
};

void Object::setObjectPrice(const int price) {
    //Проверяет корректность значения цены, устанавливает значение
    if (price <= MIN_OBJECT_PRICE)
        objectPrice = MIN_OBJECT_PRICE;
    else if (price >= MAX_OBJECT_PRICE)
        objectPrice = MAX_OBJECT_PRICE;
    else
        objectPrice = price;
};

void Object::setObjectVolume(const int volume) {
    //Проверяет корректность значения объема, устанавливает значение
    if (volume <= MIN_OBJECT_VOLUME)
        objectVolume = MIN_OBJECT_VOLUME;
    else if (volume >= MAX_OBJECT_VOLUME)
        objectVolume = MAX_OBJECT_VOLUME;
    else
        objectVolume = volume;
};

void Object::setObjectName(string name){
    //Устанавливает значение названия
   objectName=std::move(name);
};

string Object::getTypeName() const{
    return TYPE;
}

string Object::getObjectTitle() const{
    return getTypeName()+" "+getObjectName()+", price "+to_string(getObjectPrice())+", volume "+to_string(getObjectVolume()) ;
}

Object *Object::createPointer() const{
    return new Object(*this) ;
};

