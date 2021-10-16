#ifndef LIST_H
#define LIST_H

#include "unit.h"
#include <iostream>

class List
{
private:
    struct ListItem {
        Unit item ;
        ListItem * prior ;
        ListItem * next ;
    } ;

    int cnt ;
    ListItem * first ;
    ListItem * last ;
    static ListItem * createLI(const Unit & unit, ListItem * prior, ListItem * next) ;
public:    
    List();
    List(const List & list) ;
    List &operator=(const List &list) ;

    ~List() ;
    void insertUnit(const Unit & unit) ;
    void deleteFirst() ;
    void deleteLast() ;
    void deleteIndex(int i) ;
    int getCount() const ;
    void clear() ;
    bool saveToFile(const string & str) const;
    bool loadFromFile(const string & str) ;
    bool isListEquals(const List & list) const ;

    class Iterator
    {
    private:
        ListItem *head;
    public:
        Iterator(ListItem *head) { this->head = head; }
        Iterator operator++() { Iterator itr = *this; head = head->next; return itr; }
        Iterator & operator++(int) { Iterator * tmp = this ; head = head->next; return *tmp; }
        Unit& operator*() { return head->item; }
        bool operator==(const Iterator &itr) const { return itr.head==head ; }
        bool operator!=(const Iterator &itr) const { return itr.head!=head ; }
    };

    Iterator begin() const { return Iterator(first); }
    Iterator end() const { return Iterator(nullptr); }
};

#endif // LIST_H
