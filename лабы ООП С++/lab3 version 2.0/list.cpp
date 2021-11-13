#include "list.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include "objectstack.h"

List::ListItem *List::createListItem(const Object &object, List::ListItem *prev,
                                     List::ListItem *next){
    auto item = new ListItem();
    item->item = object.createPointer();
    item->prev = prev;
    item->next = next;
    return item;
}

List::List(){
    first = nullptr;
    last = nullptr;
    listCount = 0;
}

List::List(const List &list){

    first = nullptr;
    last = nullptr;
    listCount = 0;

    for (Object & object: list)
       addObjectToItems(object);
}

void List::AddTail(Object &n){
   // Создаем новый элемент
   ListItem * temp = new ListItem;
   // Следующего нет
   temp->next = nullptr;
   // Заполняем данные
   temp->item = n.createPointer();
   // Предыдущий - бывший хвост
   temp->prev = last;

   // Если элементы есть?
   if(last != nullptr)
      last->next = temp;

   // Если элемент первый, то он одновременно и голова и хвост
   if(listCount == 0)
      first = last = temp;
   else
      // иначе новый элемент - хвостовой
   last = temp;

   listCount++;
}

List &List::operator=(const List &list){
    if (this != &list) {
        clearList();
        for (Object & object: list)
            addObjectToItems(object);
    }
    return *this;
}

List::~List(){
    clearList();
}

void List::addObjectToItems(const Object &object){

    listCount++;

    if (first==nullptr) {
        first = last = createListItem(object,nullptr,nullptr) ;
        return;
    }

    if (first==last) {
        if (object.getObjectPrice()<first->item->getObjectPrice()) {
            ListItem * oldfirst = first;
            first = createListItem(object,nullptr,last);
            last->prev = first;
            last = oldfirst;
        }
        else {
            ListItem * oldlast = last;
            last = createListItem(object,first,nullptr);
            first->next = last;
            first = oldlast;
        }
        return;
    }

    if (object.getObjectPrice()<first->item->getObjectPrice()) {
        ListItem * oldfirst = first;
        first = createListItem(object,nullptr,oldfirst);
        oldfirst->prev = first;
        return;
    }
    if (object.getObjectPrice()>=last->item->getObjectPrice()) {
        ListItem * oldlast = last;
        last = createListItem(object,oldlast,nullptr);
        oldlast->next = last;
        return;
    }

    ListItem * tek = first;
    while (tek->next!=nullptr) {
        if ((tek->item->getObjectPrice()<object.getObjectPrice())&&(object.getObjectPrice()<=tek->next->item->getObjectPrice())) {
            ListItem * newItem = createListItem(object, tek, tek->next);
            tek->next = newItem;
            tek->next->prev = newItem;
            return;
        }
        tek = tek->next;
    }
}

void List::deleteFirstItem(){
    if (first==nullptr) return;

    if (first==last) {
        delete first;
        first = nullptr;
        last = nullptr;
        listCount = 0;
        return;
    }

    first = first->next;
    delete first->prev;
    first->prev = nullptr;
    listCount--;
}

void List::deleteLastItem(){

    if (first==nullptr) return;

    if (first==last) {
        delete first;
        first = nullptr;
        last = nullptr;
        listCount = 0;
        return;
    }

    last = last->prev;
    delete last->next;
    last->next = nullptr;
    listCount--;
}

void List::deleteItemByIndex(int i){
    if ((i<0)||(i>=listCount)) return;

    if (i==0) deleteFirstItem();
        else if (i==listCount-1)
        deleteLastItem();
            else{
                ListItem * tek = first;
                while (i>1) {
                    tek=tek->next;
                    i--;
                    }
                tek->next = tek->next->next;
                tek->next->prev = tek;
                listCount--;
                }

}

int List::getListCount() const{
    return listCount ;
}

void List::clearList(){
    while(getListCount()!=0){
        deleteItemByIndex(0);
    }
}

bool List::saveListToFile(const string &fileName) const{
    ofstream fout;
    fout.open(fileName);
    if (!fout) return false;

    fout<<getListCount()<<endl;
    for (Object & objectItem: *this) {
        fout<<objectItem.getObjectName()<<endl;
        fout<<objectItem.getObjectPrice()<<endl;
        fout<<objectItem.getObjectVolume()<<endl;
    }

    fout.close();
    return true;
}

bool List::loadListFromFile(const string &fileName){
    ifstream fin ;
    fin.open(fileName,ios_base::in);
    if (!fin) return false;

    clearList();

    char buf[255] ;
    fin.getline(buf,255) ;
    int cnt = atoi(buf) ;
    for (int i=0; i<cnt; i++) {
        Object objectItem ;
        fin.getline(buf,255) ;
        objectItem.setObjectName(buf) ;
        fin.getline(buf,255) ;
        objectItem.setObjectPrice(atoi(buf)) ;
        fin.getline(buf,255) ;
        objectItem.setObjectVolume(atoi(buf)) ;
        addObjectToItems(objectItem) ;
    }

    fin.close() ;
    return true ;
}


bool List::isEqual(const List & list) const{
    if (list.getListCount()!=getListCount()) return false;

    Iterator item1 = list.begin();
    for(Iterator item2 = begin(); item2!=end(); item2++) {
       if ((*item1).getObjectName()!=(*item2).getObjectName()) return false ;
       if ((*item1).getObjectPrice()!=(*item2).getObjectPrice()) return false ;
       if ((*item1).getObjectVolume()!=(*item2).getObjectVolume()) return false ;
       item1++;
    }

    return true ;
}
