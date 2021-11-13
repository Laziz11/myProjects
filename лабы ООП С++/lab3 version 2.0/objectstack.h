#ifndef OBJECTSTACK_H
#define OBJECTSTACK_H

#include "object.h"

class ObjectStack : public Object
{
public:    
    ObjectStack(const string name=DEFAULT_OBJECT_NAME,
                const int price=DEFAULT_OBJECT_PRICE);
    ObjectStack(const ObjectStack &object);
    ObjectStack & operator=(const ObjectStack &object);
    virtual ~ObjectStack();
    virtual int getObjectVolume() const override;
    virtual void setObjectVolume(const int volume) override;
    virtual string getTypeName() const override;
    virtual Object * createPointer() const override;
protected:
    static constexpr auto TYPE = "OBJECT_STACK";
};

#endif // OBJECTSTACK_H
