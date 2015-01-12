#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <stdio.h>
#include <DS1302.h>

Adafruit_PCD8544 lcd = Adafruit_PCD8544(7, 6, 5, 4, 3);

//        RST,DAT,CLK
DS1302 rtc(8, 9, 10);

void setup()
{
  Serial.begin(9600);
  
  lcd.begin();
  
  startLCD();
  //startClock();  
}

void startLCD()
{
  lcd.setContrast(20);
  lcd.clearDisplay();
}

void startClock()
{
  rtc.writeProtect(false);
  rtc.halt(false);
  
  //  year, month, day, hour, minites, seconds, day of week   
  Time t (2015, 1, 11, 21, 02, 00, Time::kSunday);
  rtc.time(t);  
}

void loop()
{
  printTime();
  
  delay(1000);
}

String dayAsString(const Time::Day day)
{
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}

void printTime()
{
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);
  
  char date[20];  
  snprintf(date, sizeof(date), "%04d-%02d-%02d", t.yr, t.mon, t.date);
  char time[20];
  snprintf(time, sizeof(time), "%02d:%02d:%02d", t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.println(buf);
  
  lcd.clearDisplay();
  lcd.setTextSize(2);
  lcd.println(day.c_str());
  lcd.setTextSize(1);
  lcd.println(date);
  lcd.println(time);
  lcd.display();
}
