#ifndef MYHASH_H
#define MYHASH_H

// Шаблон хэш таблицы
template <class K, class T>
class MyHash {
    typedef unsigned char hash_value;	// Тип хеша

    // Простая функция хэширования
    hash_value hash(K key) const {
        // Тип ключа должен реализовывать функцию qHash
        return (hash_value) (qHash(key));
    }

    class chainEntry {	// Элемент таблицы цепочек - односвязный список
    public:
        chainEntry(K k, T v, chainEntry *n, chainEntry **t, hash_value h) {
            key = k;	// Копия ключа
            value = v;	// Копия значения
            next = n;	// Следующий элемент
            table = t;	// Вся таблица цепочек
            hash = h;	// Хеш
        }


        K key;		// Ключ
        T value;	// Значение
        hash_value hash;	// Значение хэша

        chainEntry *next;	// Указатель на следующий элемент в цепочке
        chainEntry **table;	// Указатель на всю таблицу хэшей

        // Функции последовательного перемещения по списку
        chainEntry *_firstNode(int top) {
            // Ищем первый узел в цепочках
            for ( int i = top; i < 256; i++ ) {
                chainEntry *c = table[i];
                if ( c != NULL )
                    return c;
            }
            return NULL;	// Ничего не найдено
        }

        chainEntry *_lastNode(int bottom) {
            // Ищем последний узел в цепочках
            for ( int i = bottom; i >= 0; i-- ) {
                chainEntry *c = table[i];
                if ( c != NULL ) {
                    // Нашли непустую цепочку - ищем в ней последний элемент
                    while(c->next != NULL) {
                        c = c->next;
                    }
                    return c;
                };
            }
            return NULL;
        }

        chainEntry *_nextNode(chainEntry *node) {
            // Ищем следующий по очереди
            if ( node->next != NULL )
                return node->next;	// В цепочке есть следующий

            // Ищем дальше по списку
            if ( hash == 256 ) return NULL;	// Это был последний

            return _firstNode(hash+1);
        }

        chainEntry *_prevNode(chainEntry *node) {
            // Находим узел, предшествующий текущему
            chainEntry *c = table[hash];
            if ( c == node ) { // Текущий был самым первым
                if ( hash == 0 ) return NULL;	// Это самая первая цепочка
                return _lastNode(hash-1);	// Ищем выше
            }
            else {
                // Ищем в этой же цепочке
                while(c->next != node) {
                    c = c->next;
                }
                return c;
            }
        }
    };


public:
    // Итератор в стиле STL
    class iterator {
        chainEntry *i;	// Указатель на узел в цепочке

    public:
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;

        inline iterator() { i = NULL; }
        explicit inline iterator(void *node) { i = (chainEntry *) node; }

        inline const K &key() const { return i->key; }
        inline T &value() const { return i->value; }

        // Перегрузка операторов
        inline T &operator* () const { return i->value; }
        inline T *operator-> () const { return &(i->value); }
        inline bool operator==(const iterator &other) const { return i == other.i; }
        inline bool operator!=(const iterator &other) const { return i != other.i; }

        // Перемещение по итератору
        inline iterator &operator++() {	// Префиксный оператор
            i = i->_nextNode(i);
            return *this;
        }
        inline iterator &operator++(int) {	// Постфиксный оператор
            iterator r = *this;
            i = i->_nextNode(i);
            return r;
        }
        inline iterator &operator--() {	// Префиксный оператор
            i = i->_prevNode(i);
            return *this;
        }
        inline iterator &operator--(int) {	// Постфиксный оператор
            iterator r = *this;
            i = i->_prevNode(i);
            return r;
        }

        inline iterator &operator+(int j) const {	// Сложение
            iterator r = *this;
            if (j > 0)
                while (j--) ++r;
            else
                while (j++) --r;
            return r;
        }

        inline iterator &operator-(int j) const {	// Вычитание
            return operator+(-j);
        }

        inline iterator &operator += (int j) {
            return *this = *this + j;
        }

        inline iterator &operator-=(int j) {
            return *this = *this - j;
        }
    };


    ////////////////////////////////////////////////////////////////////////////
    MyHash();	// Конструктор
    MyHash(const MyHash &other);	// Конструктор копирования

    ~MyHash(); // Деструктор

    int count() const;	// Вернуть количество элементов

    // Манипуляция с данными
    void insert(const K &key, const T &value);	// Вставка
    void unite(const MyHash &other);	// Объединение

    int remove(const K& key);	// Удаление
    iterator erase(iterator pos);	// Удаление по итератору
    T take(const K& key);	// Вернуть и удалить
    void clear();	// Очистить

    // Доступ по ключу
    const T value(const K &key) const;
    T& operator[](const K &key);
    const T operator[](const K &key) const;

    // Поиск
    bool contains (const K& key) const;  // есть ли ключ key
    iterator find (const K& key) {
        chainEntry *ptr = (chainEntry *) _find(key);
        return iterator(ptr);
    }

    // Начало и конец для итератора в стиле STL
    iterator begin() { return iterator(_start()); };
    iterator end() { return iterator(); };

private:
    chainEntry *m_table[256];	// Список цепочек

    const void *_find(const K &key) const;		// Поиск элемента цепочки

    chainEntry *_start() {	// Начало списка
        for ( int i = 0; i < 256; i++ ) {
            chainEntry *c = m_table[i];
            if ( c != NULL )
                return c;
        }
        return NULL;	// Ничего не найдено
    }
};

// Конструктор
template <class K, class T>
MyHash<K,T>::MyHash() {
    // Инициализируем все цепочки пустыми указателями
    for ( int i = 0; i < 256; i++  )
        m_table[i] = NULL;
}

// Конструктор копирования
template <class K, class T>
MyHash<K,T>::MyHash(const MyHash &other) {
    // Инициализируем все цепочки пустыми указателями
    for ( int i = 0; i < 256; i++  )
        m_table[i] = NULL;

    iterator it;
    for( it = other.begin(); it != other.end(); ++it )
        this->insert(it->key, it->value);
}

// Деструктор
template <class K, class T>
MyHash<K,T>::~MyHash() {
    clear();
}

// Вернуть количество элементов
template <class K, class T>
int MyHash<K,T>::count() const {
    int cnt = 0;
    for ( int i = 0; i < 256; i++ ) {
        chainEntry *c = m_table[i];
        while(c) {
            cnt++;
            c = c->next;
        }
    }
    return cnt;
}

// Вставка
template <class K, class T>
void MyHash<K,T>::insert(const K& key, const T& value) {
    unsigned int h;
    chainEntry *c;

    h = hash(key);	// рассчитываем хэш

    // Ищем ключ в цепочке
    c = m_table[h];
    while (c) {
        if ( key == c->key ) {	// Найдена ячейка с таким ключом
            c->value = value;
            return;
        }
        c = c->next;
    }
    // Создаем новую ячейку
    m_table[h] = new chainEntry(key, value, m_table[h], m_table, h);
}

// Объединение
template <class K, class T>
void MyHash<K,T>::unite(const MyHash &other) {
    iterator it;
    for( it = other.begin(); it != other.end(); ++it )
        this->insert(it->key, it->value);
}

// Удаление
template <class K, class T>
int MyHash<K,T>::remove(const K &key)		// Удалить
{
    unsigned int h;
    chainEntry *c, *d, **p;

    h = hash(key);	// рассчитываем хэш

    // Ищем ключ в цепочке
    c = m_table[h];
    p = &(m_table[h]);
    while (c) {
        if ( key == c->key ) {	// Найдена ячейка с таким ключом
            d = c;
            *p = c->next;
            delete d;

            return 1;	// Удалена  запись
        }
        p = &(c->next);
        c = c->next;
    }
    return 0;	// Не найдена
}

// Очистить
template <class K, class T>
void MyHash<K,T>::clear() {
    chainEntry *c, *d;
    for ( int i = 0; i < 256; i++  ) {	// Удаляем все цепочки
        c = m_table[i];
        while ( c ) {
            d = c;
            c = c->next;
            delete d;
        }
    }
}

template <class K, class T>
const void *MyHash<K,T>::_find(const K &key) const	// Поиск по ключу
{
    unsigned int h;
    chainEntry *c;

    h = hash(key);	// рассчитываем хэш

    // Ищем ключ в цепочке
    c = m_table[h];
    while (c) {
        if ( key == c->key ) {	// Найдена ячейка с таким ключом
            return (void *)c;
        }
        c = c->next;
    }
    return NULL;	// Не найдена
}

// Операторы взятия элемента массива по ключу
template <class K, class T>
const T MyHash<K,T>::value(const K &key) const
{
    chainEntry *ptr = (chainEntry *) _find(key);
    if ( ptr == NULL )
        return T();
    else
        return ptr->value;
}

template <class K, class T>
T& MyHash<K,T>::operator[](const K &key)
{
    chainEntry *ptr = (chainEntry *) _find(key);
    return ptr->value;
}

template <class K, class T>
const T MyHash<K,T>::operator[](const K &key) const
{
    chainEntry *ptr = (chainEntry *) _find(key);
    return ptr->value;
}

template <class K, class T>
bool MyHash<K,T>::contains (const K& key) const
{
    chainEntry *ptr = (chainEntry *) _find(key);
    return (key != NULL);
}

// Вернуть и удалить
template <class K, class T>
T MyHash<K,T>::take(const K& key) {
    chainEntry *ptr = (chainEntry *) _find(key);
    if ( ptr == NULL )
        return T();

    T value = ptr->value;
    remove(key);
    return value;
}

#endif // MYHASH_H
