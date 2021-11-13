#ifndef LIST_H
#define LIST_H

#include "object.h"
#include <iostream>

class List
{
private:
    struct ListItem {
        Object * item;
        ListItem * prev;
        ListItem * next;
    } ;

    int listCount;
    ListItem * first;
    ListItem * last;
    static ListItem * createListItem(const Object & object, ListItem * prior,
                                     ListItem * next);
public:    
    List();
    List(const List & list);
    List &operator=(const List &list);
    void AddTail(Object &object);
    ~List() ;
    void addObjectToItems(const Object & object) ;
    void deleteFirstItem() ;
    void deleteLastItem() ;
    void deleteItemByIndex(int i) ;
    int getListCount() const;
    void clearList();
    bool saveListToFile(const string & str) const;
    bool loadListFromFile(const string & str) ;
    bool isEqual(const List & list) const ;

    class Iterator{
    private:
        ListItem *head;
    public:
        Iterator(ListItem *head) { this->head = head; }
        Iterator operator++() {
            Iterator itr = *this; head = head->next; return itr; }
        Iterator & operator++(int) {
            Iterator * tmp = this ; head = head->next; return *tmp; }
        Object & operator*() { return *(head->item); }
        Object * operator->() { return (head->item); }
        bool operator==(const Iterator &itr) const { return itr.head==head ; }
        bool operator!=(const Iterator &itr) const { return itr.head!=head ; }
    };

    Iterator begin() const { return Iterator(first); }
    Iterator end() const { return Iterator(nullptr); }
};

#endif // LIST_H
