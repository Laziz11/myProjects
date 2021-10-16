#ifndef UNIT_H
#define UNIT_H

#include <string>
using namespace std;

class Unit {
public:
    static constexpr int PRICE_MIN = 1;
    static constexpr int PRICE_MAX = 100000;
    static constexpr int PRICE_DEFAULT = 1000;
    static constexpr int VOLUME_MIN = 1;
    static constexpr int VOLUME_MAX = 100000;
    static constexpr int VOLUME_DEFAULT = 200;
    static constexpr auto NAME_DEFAULT = "Name";
    Unit(const string name=NAME_DEFAULT, const int price=PRICE_DEFAULT, const int volume=VOLUME_DEFAULT);
    Unit(const Unit &unit);
    Unit & operator=(const Unit &unit) ;
    ~Unit() ;
    void printParameters() const ;
    const string & getName() const ;
    int getPrice() const ;
    int getVolume() const ;
    void setPrice(const int price);
    void setVolume(const int volume);
    void setName(string name);

private:
    string name_;
    int price_, volume_;
};

#endif // UNIT_H
