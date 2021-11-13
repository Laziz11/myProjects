#include "winners.h"

// Вернуть трех призеров
winners::top winners::podium() {
    top tops;
    int top_values[3] = {0, 0, 0};

    MyHash<QString, int>::iterator it;

    // Перебираем всех студентов
    for( it = students.begin(); it != students.end(); ++it ) {
        if ( it.value() > top_values[0] ) {
            // Перемещаем на первое место
            tops.third = tops.second; tops.second = tops.first;
            top_values[2] = top_values[1];
            top_values[1] = top_values[0];
            tops.first = it.key();
            top_values[0] = it.value();
        }
        else if ( it.value() > top_values[1] ) {
            // Перемещаем на второе место
            tops.third = tops.second;
            top_values[2] = top_values[1];

            tops.second = it.key();
            top_values[1] = it.value();
        }
        else if ( it.value() > top_values[2] ) {
            // Перемещаем на третье место
            tops.third = it.key();
            top_values[2] = it.value();
        }
    }
    return tops;	// Ключи первых трех возвращаем
}


