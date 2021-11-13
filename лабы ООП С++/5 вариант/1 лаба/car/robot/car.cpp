// Реализация класса Car

#include "car.h"
#include <qmath.h>

// Конструктор
Car::Car() {
    x = 0;	// Начальные координаты
    y = 0;

    speed = 0; // Начальная скорость
    angle = 0;	// Начальный угол - вдоль оси Ox
    now = 0;	// Начальная точка отсчета времени
}

// Передвинуться вперед на заданное расстояние
void Car::forward(double d) {
    double dx = cos(angle)*d;	// смещение по оси x
    double dy = sin(angle)*d;	// смещение по оси y

    x += dx;
    y += dy;
}

// Передвинуть автомобиль в текущем направлении за время t
void Car::move(double t) {
    double d = t*speed;	// Расстояние
    forward(d);	// Продвинуться вперед
}

// Повернуть на заданный угол (в радианах)
void Car::rotate(double a) {
    angle += a;
}

// Изменить скорость
void Car::accelerate(double s) {
    speed += s;
}

// Повернуть влево по заданному радиусу
void Car::rotateLeft(double r) {
    forward(r);	// Продвинуться вперед
    rotate(-M_PI_2);	// Повернуть против часовой стрелки на угол пи пополам
    forward(r);	// Продвинуться вперед
}

// Повернуть влево по заданному радиусу
void Car::rotateRight(double r) {
    forward(r);	// Продвинуться вперед
    rotate(M_PI_2);	// Повернуть по часовой стрелке на угол пи пополам
    forward(r);	// Продвинуться вперед
}

///////////////////////////////////////////////////////////////////
/// // Вспомогательные методы

// Перевод времени в секунды
double Car::Time2Sec(unsigned long h, unsigned long m, double s) {
    return h*3600 + m*60+s;
}

// Перевод секунд во время (чч:мм:сс)
void Car::Sec2Time(double t, unsigned long &h, unsigned long &m, double &s) {
    h = (unsigned long)(t/3600);	// Целое число часов
    t = t-h*3600;
    m = (unsigned long) (t/60);	// Целое число минут
    s = t-m*60;
}

// Градусы в радианы
double Car::Grad2Rad(double grad) {
    return (grad*M_PI)/180.;
}

// Радианы в градусы
double Car::Rad2Grad(double rad) {
    return (rad*180.)/M_PI;
}

// Проверить корректность времени
bool Car::CheckTime(unsigned long h, unsigned long m, double s) {
    if ( h < 0 ) return false;
    if ( m < 0 || m > 60 ) return false;
    if ( s < 0 || s > 60 ) return false;
    return true;
}

