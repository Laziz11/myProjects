#include <iostream>
#include <iomanip>

#include "car.h"

using namespace std;


int main(int argc, char* argv[])
{
    char cmd;
    unsigned long h,m;	// Время
    double sec;
    double s;	// Скорость
    double r;	// Радиус поворота
    double t;	// Время в секундах

    Car c;

    do {
        cout << endl << "(" << c.x << "," << c.y << ") Enter command (s m r): ";
        cin >> cmd;
        if ( cmd == 's' ) 		// Команда стоп
            ;
        else if ( cmd == 'm' ) {		// Команда движения
            cin >> h >> m >> sec;	// Время отправки команды
            if ( !c.CheckTime(h, m, sec) ) {
                cin.clear();
                cout << "Wrong time format" << endl;
                continue;
            }

            t = c.Time2Sec(h, m, sec);
            if ( t < c.now ) {
                cin.clear();
                cout << "Chronology violation" << endl;
                continue;
            }

            cin >> s;	// Новая скорость
            if ( s < 0 ) {
                cin.clear();
                cout << "Wrong speed" << endl;
                continue;
            }
            cin.clear();

            double per = t - c.now;	// Период времени от последней команды
            c.move(per);	// Передвигаемся с предыдущей скоростью
            c.speed = s;	// Устанавливаем новую скорость
            c.now = t;	// Устанавливаем текущее время
        }
        else if ( cmd == 'r' ) {		// Команда поворота
            cin >> h >> m >> sec;	// Время отправки команды
            if ( !c.CheckTime(h, m, sec) ) {
                cin.clear();
                cout << "Wrong time format" << endl;
                continue;
            }

            t = c.Time2Sec(h, m, sec);
            if ( t < c.now ) {
                cin.clear();
                cout << "Chronology violation" << endl;
                continue;
            }

            cin >> cmd;	// налево или направо
            if ( cmd != 'r' && cmd != 'l' ) {
                cin.clear();
                cout << "Turn to 'left' or turn to 'right'" << endl;
                continue;
            }

            cin >> r;	// Радиус поворота
            if ( r < 0 ) {
                cin.clear();
                cout << "Wrong radius" << endl;
                continue;
            }

            cin.clear();

            double per = t - c.now;	// Период времени от последней команды
            c.move(per);	// Передвигаемся с предыдущей скоростью
            if ( cmd == 'l' )
                c.rotateLeft(r);	// Поворачиваем налево
            else
                c.rotateRight(r);	// Поворачиваем направо

            c.now = t;	// Устанавливаем текущее время
        }
    } while(cmd != 's');
    return 0;
}
