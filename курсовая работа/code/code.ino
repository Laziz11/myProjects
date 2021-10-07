
// Радиоприемник с поиском станций
// на TEA5767 и LCD Keypad shield

// Подключение библиотек


#include <TEA5767.h>
#include <Wire.h>
#include <LiquidCrystal.h>


// создание экземпляров



TEA5767 Radio;
LiquidCrystal lcd(8,9,4,5,6,7);


//  переменные
double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;


void setup() {  
  Wire.begin(); 
  // запуск радио
  Radio.init();
  // станция по умолчанию
  Radio.set_frequency(101.0); 
  Serial.begin(9600);
  // запуск дисплея
  lcd.begin(16,2);
  lcd.clear();
}


void loop() {

  unsigned char buf[5];
  int stereo;
  int signal_level;
  double current_freq;
  unsigned long current_millis = millis();
  
 if (Radio.read_status(buf) == 1) {
   current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
   stereo = Radio.stereo(buf);
   signal_level = Radio.signal_level(buf);
    // вывод информации на дисплей
    lcd.setCursor(0,0);
    lcd.print("FM: "); lcd.print(current_freq);
    lcd.setCursor(0,1);
    if (stereo) 
       {lcd.print("STEREO ");} 
    else 
       {lcd.print("MONO ");}

  }
  
  // поиск станции
  if (search_mode == 1) {
      if (Radio.process_search (buf, search_direction) == 1) {
          search_mode = 0;
          Serial.println("search ok");
      }
  }
  // поиск вверх по частоте
  if (analogRead(A0)<100) {
    Serial.println("RIGHT");
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    delay(300);
  }
  // поиск вниз по частоте
  if (analogRead(A0)>400 && analogRead(A0)<600) {
    Serial.println("LEFT");
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_DOWN;
    Radio.search_down(buf);
    delay(300);
  } 
  delay(50);
}
