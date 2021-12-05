#ifndef MYHASH_H
#define MYHASH_H

#include <QHash>
#include <exception>
#include <QFile>
#include <QTextStream>

template <typename K,typename V>
class MyHash {
private:
    struct HashItem {
        K k ;
        V v ;
        HashItem * next ;
    };
    static HashItem * newHashItem(K k, V v) {
        HashItem * hi = new HashItem() ;
        hi->k = k ;
        hi->v = v ;
        hi->next = nullptr ;
        return hi ;
    }
    static const int HTSIZE = 128;
    int size ;
    HashItem * table[HTSIZE] ;
    static int calcHash(K k) {
        return qHash(k) % HTSIZE ;
    }
    V getValue(const K k) const {
        int p = calcHash(k) ;
        HashItem * tek = table[p] ;
        while (tek!=nullptr) {
            if (tek->k==k) return tek->v ;
            tek=tek->next ;
        }
        throw new std::runtime_error(QString("Not found key "+k).toStdString()) ;
    }
public:
    MyHash() {
        size = 0 ;
        for (int i=0; i<HTSIZE; i++)
            table[i]=nullptr ;
    }
    MyHash(const MyHash<K,V> &src) {
        size = 0 ;
        for (K k: src.getAllKeys())
            add(k,src[k]) ;
    }
    MyHash operator=(const MyHash<K,V> &src)
    {
        if (this != &src) {
            deleteAll() ;
            for (K k: src.getAllKeys())
                add(k,src[k]) ;
        }
        return *this ;
    }

    ~MyHash() {
        deleteAll() ;
    }
    V operator[] (const K k) const {
        return getValue(k) ;
    }
    bool operator==(const MyHash<K,V> & obj) {
        if (getCount()!=obj.getCount()) return false ;
        for (K k: getAllKeys()) {
            if (!obj.isContain(k)) return false ;
            if (obj[k]!=getValue(k)) return false ;
        }
        return true ;
    }
    void add(const K k, int v) {

        int p = calcHash(k) ;
        if (table[p]==nullptr) {
            table[p] = newHashItem(k,v) ;
            size++ ;
        }
        else {
            HashItem * tek = table[p] ;
            HashItem * parent = table[p] ;
            while (tek!=nullptr) {
                if (tek->k==k) {
                    table[p]->v=v ;
                    return ;
                }
                else {
                    parent=tek ;
                    tek=tek->next ;
                }
            }
            parent->next = newHashItem(k,v) ;
        }
    }
    void deleteKey(const K k) {
        int p = calcHash(k) ;
        if (table[p]==nullptr) return ;

        if (table[p]->k==k) {
            HashItem * next = table[p]->next ;
            delete table[p] ;
            table[p]=next ;
            size-- ;
        }
        else {
            HashItem * tek = table[p] ;
            while (tek->next!=nullptr) {
                HashItem * parent = tek ;
                if (tek->next->k==k) {
                    HashItem * next = tek->next->next ;
                    delete tek->next ;
                    tek->next = next ;
                    size-- ;
                    break ;
                }
                tek=tek->next ;
            }
        }
    }
    void deleteAll() {
        for (int i=0; i<HTSIZE; i++)
            if (table[i]!=nullptr) {
                HashItem * tek = table[i] ;
                while (tek!=nullptr) {
                    HashItem * parent = tek ;
                    tek=tek->next ;
                    delete parent ;
                }
                table[i] = nullptr ;
            }
        size=0 ;
    }
    std::vector<K> getAllKeys() const {
        std::vector<K> res ;
        for (int i=0; i<HTSIZE; i++) {
            HashItem * tek = table[i] ;
            while (tek!=nullptr) {
                res.push_back(tek->k) ;
                tek=tek->next ;
            }
        }
        return res ;
    }
    bool isContain(const K k) const {
        int p = calcHash(k) ;
        HashItem * tek = table[p] ;
        while (tek!=nullptr) {
            if (tek->k==k) return true ;
            tek=tek->next ;
        }
        return false ;
    }
    int getCount() const {
        return size ;
    }
    V getSum() const {
        V sum = 0 ;
        for (K k: getAllKeys())
            sum+=getValue(k) ;
        return sum ;
    }
    void loadFromFile(const QString & filename) {
        deleteAll() ;
        QFile file(filename);
        file.open(QFile::ReadOnly) ;
        QTextStream stm(&file) ;
        int cnt ;
        K k ;
        V v ;
        stm>>cnt ;
        for (int i=0; i<cnt; i++) {
            if (typeid(K)==typeid(QString)) {
                stm.skipWhiteSpace() ;
                k = stm.readLine() ;
            }
            else
                stm>>k ;
            stm>>v ;
            add(k,v) ;
        }
        file.close() ;
    }
    void saveToFile(const QString & filename) {
        QFile file(filename);
        file.open(QFile::WriteOnly) ;
        QTextStream stm(&file) ;
        stm<<size<<"\n" ;
        for (K k: getAllKeys())
            stm<<k<<"\n"<<getValue(k)<<"\n" ;
        stm.flush() ;
        file.close() ;
    }
} ;

#endif // MYHASH_H
