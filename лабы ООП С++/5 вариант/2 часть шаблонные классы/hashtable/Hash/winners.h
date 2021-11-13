#ifndef WINNERS_H
#define WINNERS_H

#include <QString>
#include "myhash.h"

typedef MyHash<QString, int> WinHash;

// Класс - студенты, победители соревнований
class winners {
public:
    MyHash<QString, int> students;  // Список студентов

    // Структура для расчета победителей
    typedef struct {
        QString first, second, third;
    } top;

    top podium();   // Вернуть трех призеров
};

#endif // WINNERS_H
