#include <Arduino.h>
#include <DHT.h>
#include <HT1621.h> // https://github.com/valerionew/ht1621-7-seg

// --- Название проекта ---
#define PROJECT_NAME "MeteoClimate"

// --- Пины ---
#define DHTPIN     2      // DHT11 к D2
#define DHTTYPE    DHT11
#define TOUCH_PIN  A1     // Touch sensor
#define BL_PIN     9      // Подсветка (через MOSFET)
#define CS_PIN     6      // HT1621 Chip Select
#define WR_PIN     7      // HT1621 Write
#define DATA_PIN   8      // HT1621 Data

// --- Объекты ---
DHT dht(DHTPIN, DHTTYPE);
HT1621 lcd;//CS_PIN, WR_PIN, DATA_PIN);

bool backlightOn = false;
bool lastTouchState = false;
bool is_print_temp = true;

void setup() {
  Serial.begin(9600);

  pinMode(TOUCH_PIN, INPUT);
  pinMode(BL_PIN, OUTPUT);
  digitalWrite(BL_PIN, LOW); // подсветка отключена по умолчанию

  // Инициализация дисплея
  lcd.begin(CS_PIN, WR_PIN, DATA_PIN, BL_PIN);
  lcd.clear();
  lcd.noBacklight();
  
  // Надпись и старт
  //lcd.print(PROJECT_NAME);
  dht.begin();
}

void loop() {
  // --- Обработка сенсорной кнопки ---
  bool touchState = analogRead(TOUCH_PIN) > 500; // Можно скорректировать порог по конкретному сенсору
  if (touchState && !lastTouchState) {
    backlightOn = !backlightOn;
    digitalWrite(BL_PIN, backlightOn ? HIGH : LOW);
  if(backlightOn)
    lcd.backlight();
    else 
    lcd.noBacklight();
    
    delay(200); // Антидребезг
    Serial.println("LED light ON/OFF");
  }
  lastTouchState = touchState;

  // --- Получение данных DHT11 ---
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();

  // Для HT1621 LCD: используйте библиотеку с print(float) или с форматом sprintf
 lcd.clear();
 if(is_print_temp) {
  lcd.printCelsius(temp);
 }
 else {
  lcd.print(humi, 0);
 }
 is_print_temp = !is_print_temp;
  // // Температура
  // lcd.setCursor(0, 0);
  // lcd.print("T:");
  // lcd.print((int)temp);
  // lcd.print("C");
  // // Влажность
  // lcd.setCursor(0, 1);
  // lcd.print("H:");
  // lcd.print((int)humi);
  // lcd.print("%");
  Serial.print("T: ");
  Serial.print(temp,1);
  Serial.print("C;  ");
  Serial.print("H: ");
  Serial.print(humi,0);
  Serial.print("%");
  Serial.println();

  delay(1500); // Обновление раз в 1.5 сек
}
