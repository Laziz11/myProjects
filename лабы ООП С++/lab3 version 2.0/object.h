#ifndef OBJECT_H
#define OBJECT_H

#include <string>
using namespace std;

class Object {
public:
    static constexpr int MIN_OBJECT_PRICE = 1;
    static constexpr int MAX_OBJECT_PRICE = 100000;
    static constexpr int DEFAULT_OBJECT_PRICE = 1000;
    static constexpr int MIN_OBJECT_VOLUME = 1;
    static constexpr int MAX_OBJECT_VOLUME = 100000;
    static constexpr int DEFAULT_OBJECT_VOLUME = 200;
    static constexpr auto DEFAULT_OBJECT_NAME = "DEFAULT_NAME";
    Object(const string name=DEFAULT_OBJECT_NAME,
           const int price=DEFAULT_OBJECT_PRICE,
           const int volume=DEFAULT_OBJECT_VOLUME);
    Object(const Object &object);
    Object & operator=(const Object &object) ;
    virtual ~Object() ;
    virtual void printObjectData() const;
    const string & getObjectName() const;
    int getObjectPrice() const;
    virtual int getObjectVolume() const;
    void setObjectPrice(const int price);
    virtual void setObjectVolume(const int volume);
    void setObjectName(string name);
    virtual string getTypeName() const;
    virtual string getObjectTitle() const;
    virtual Object * createPointer() const;

protected:
    string objectName;
    int objectPrice, objectVolume;
    static constexpr auto TYPE = "OBJECT";
};

#endif // OBJECT_H
