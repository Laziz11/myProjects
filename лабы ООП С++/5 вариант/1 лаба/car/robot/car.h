#ifndef CAR_H
#define CAR_H

// Радиоуправляемый автомобиль
class Car {
public:
    double x,y;	// Текущие координаты
    double speed;	// Текущая скорость м/с
    double angle;	// Текущий угол
    double now;	// Текущее время в секундах

    Car();  // Конструктор

    void forward(double d); // Передвинуться вперед на заданное расстояние
    void move(double t);    // Передвинуть автомобиль в текущем направлении за время t
    void rotate(double a);  // Повернуть на заданный угол (в радианах)
    void accelerate(double s);  // Изменить скорость
    void rotateLeft(double r);  // Повернуть влево по заданному радиусу
    void rotateRight(double r); // Повернуть вправо по заданному радиусу

    ///////////////////////////////////////////////////////////////////
    // Вспомогательные методы

    // Перевод времени в секунды
    double Time2Sec(unsigned long h, unsigned long m, double s);    // Перевод времени в секунды
    void Sec2Time(double t, unsigned long &h, unsigned long &m, double &s); // Перевод секунд во время (чч:мм:сс)
    double Grad2Rad(double grad);   // Градусы в радианы
    double Rad2Grad(double rad);    // Радианы в градусы
    bool CheckTime(unsigned long h, unsigned long m, double s); // Проверить корректность времени
};


#endif // CAR_H
