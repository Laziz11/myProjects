#ifndef UNITSTACK_H
#define UNITSTACK_H

#include "unit.h"

class UnitStack : public Unit
{
public:    
    UnitStack(const string name=NAME_DEFAULT, const int price=PRICE_DEFAULT);
    UnitStack(const UnitStack &unit);
    UnitStack & operator=(const UnitStack &unit) ;
    virtual ~UnitStack() ;
    virtual void printParameters() const override ;
    virtual int getVolume() const override ;
    virtual void setVolume(const int volume) override ;
    virtual string getTypeName() const override ;
    virtual Unit * createPointer() const override ;
};

#endif // UNITSTACK_H
