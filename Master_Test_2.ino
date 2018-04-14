//************* Master Arduino Mega 2560 *********************
// Slave -> RTC
// Slave -> User Select Display
/*
   Flags
   Date: - Slot 1 500
           Slot 2 501
   Day : - Slot 1 502
   Day : - Slot 2 503

   Led Registers
   Date: -
   Slot 1 504
   Slot 2 505

   Day
   Slot 1 506
   Slot 2 507
*/

#include<Wire.h>
#include<DS3231.h>
#include<EEPROM.h>

DS3231  rtc(SDA, SCL);

Time  t;
int Hor;
int Min;
int Sec;
int Date;
int Month;
int Year;
int Day;
const int date_led_slot_1 = 2;
const int date_led_slot_2 = 3;
const int day_led_slot_1 = 4;
const int day_led_slot_2 = 5;

void day_alarm_1(void);
void day_alarm_2(void);
void commonregisters(void);

void setup()
{
  Serial.begin(9600);
  rtc.begin();
  pinMode(date_led_slot_1, OUTPUT);
  pinMode(date_led_slot_2, OUTPUT);
  pinMode(day_led_slot_1, OUTPUT);
  pinMode(day_led_slot_1, OUTPUT);
}


void loop()
{
  t = rtc.getTime();
  Hor = t.hour;
  Min = t.min;
  Sec = t.sec;
  Date = t.date;
  Month = t.mon;
  Year = t.year;
  Day = t.dow;

  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  Serial.println(rtc.getTimeStr());
  commonregisters();
  // On Time

  if (EEPROM.read(500) == 0)         // Date Slot 1
  {
    if (Year == (EEPROM.read(8) + 2000) && Month == EEPROM.read(7) && Date == EEPROM.read(6) && Hor == EEPROM.read(2) && Min == EEPROM.read(1) && Sec == EEPROM.read(0) )
    {
      digitalWrite(date_led_slot_1, HIGH);
    }
    // Off Time
    else if (Year == (EEPROM.read(8) + 2000) && Month == EEPROM.read(7) && Date == EEPROM.read(6) && Hor == EEPROM.read(5) && Min == EEPROM.read(4) && Sec == EEPROM.read(3))
    {
      digitalWrite(date_led_slot_1, LOW);  // Action
      Wire.beginTransmission(5);
      Wire.write('1');        // On Compition of Action
      Wire.endTransmission();
    }
  }

  if (EEPROM.read(501) ==  0)    // Date Slot 2
  {
    if (Year == (EEPROM.read(17) + 2000) && Month == EEPROM.read(16) && Date == EEPROM.read(15) && Hor == EEPROM.read(11) && Min == EEPROM.read(10) && Sec == EEPROM.read(9) )
    {
      digitalWrite(date_led_slot_2, HIGH);
    }
    // Off Time
    else if (Year == (EEPROM.read(17) + 2000) && Month == EEPROM.read(16) && Date == EEPROM.read(15) && Hor == EEPROM.read(14) && Min == EEPROM.read(13) && Sec == EEPROM.read(12))
    {
      digitalWrite(date_led_slot_2, LOW);  // Action
      Wire.beginTransmission(5);
      Wire.write('2');        // On Compition of Action
      Wire.endTransmission();
    }
  }
  if (EEPROM.read(502) == 0)
  {
    day_alarm_1();          // Slot 1 for By Date
  }
  if (EEPROM.read(503) == 0)
  {
    day_alarm_2();          // Slot 2 for By Date
  }
}

void commonregisters()             // Function that Receives Data form Slave
{
  static int address = 0;    // Address header for sequential transfer (comman to both Master and Slave)
  Wire.requestFrom(5, 1);
  while (Wire.available())
  {
    int c = Wire.read();
    if (address == 0)
    {
      if (c == 255)                 // Start Bit
      {
        address = 1;
      }
      else
      {
        address = 0;
      }
    }
    else if (address == 1)
    {
      EEPROM.write(0, c);
      address = 2;
    }
    else if (address == 2)
    {
      EEPROM.write(1, c);
      address = 3;
    }
    else if (address == 3)
    {
      EEPROM.write(2, c);
      address = 4;
    }
    else if (address == 4)
    {
      EEPROM.write(3, c);
      address = 5;
    }
    else if (address == 5)
    {
      EEPROM.write(4, c);
      address = 6;
    }
    else if (address == 6)
    {
      EEPROM.write(5, c);
      address = 7;
    }
    else if (address == 7)
    {
      EEPROM.write(6, c);
      address = 8;
    }
    else if (address == 8)
    {
      EEPROM.write(7, c);
      address = 9;
    }
    else if (address == 9)
    {
      EEPROM.write(8, c);
      address = 10;
    }
    else if (address == 10)
    {
      EEPROM.write(9, c);
      address = 11;
    }
    else if (address == 11)
    {
      EEPROM.write(10, c);
      address = 12;
    }
    else if (address == 12)
    {
      EEPROM.write(11, c);
      address = 13;
    }
    else if (address == 13)
    {
      EEPROM.write(12, c);
      address = 14;
    }
    else if (address == 14)
    {
      EEPROM.write(13, c);
      address = 15;
    }
    else if (address == 15)
    {
      EEPROM.write(14, c);
      address = 16;
    }
    else if (address == 16)
    {
      EEPROM.write(15, c);
      address = 17;
    }
    else if (address == 17)
    {
      EEPROM.write(16, c);
      address = 18;
    }
    else if (address == 18)
    {
      EEPROM.write(17, c);
      address = 19;
    }
    else if (address == 19)
    {
      EEPROM.write(500, c);
      address = 20;
    }
    else if (address == 20)
    {
      EEPROM.write(501, c);
      address = 21;
    }

    else if (address == 21)
    {
      EEPROM.write(300, c);
      address = 22;
    }
    else if (address == 22)
    {
      EEPROM.write(301, c);
      address = 23;
    }
    else if (address == 23)
    {
      EEPROM.write(302, c);
      address = 24;
    }
    else if (address == 24)
    {
      EEPROM.write(303, c);
      address = 25;
    }
    else if (address == 25)
    {
      EEPROM.write(304, c);
      address = 26;
    }
    else if (address == 26)
    {
      EEPROM.write(305, c);
      address = 27;
    }
    else if (address == 27)
    {
      EEPROM.write(306, c);
      address = 28;
    }
    else if (address == 28)
    {
      EEPROM.write(307, c);
      address = 29;
    }
    else if (address == 29)
    {
      EEPROM.write(308, c);
      address = 30;
    }
    else if (address == 30)
    {
      EEPROM.write(309, c);
      address = 31;
    }
    else if (address == 31)
    {
      EEPROM.write(310, c);
      address = 32;
    }
    else if (address == 32)
    {
      EEPROM.write(311, c);
      address = 33;
    }
    else if (address == 33)
    {
      EEPROM.write(312, c);
      address = 34;
    }
    else if (address == 34)
    {
      EEPROM.write(313, c);
      address = 35;
    }
    else if (address == 35)
    {
      EEPROM.write(314, c);
      address = 36;
    }
    else if (address == 36)
    {
      EEPROM.write(315, c);
      address = 37;
    }
    else if (address == 37)
    {
      EEPROM.write(316, c);
      address = 38;
    }
    else if (address == 38)
    {
      EEPROM.write(317, c);
      address = 39;
    }
    else if (address == 39)
    {
      EEPROM.write(318, c);
      address = 40;
    }
    else if (address == 40)
    {
      EEPROM.write(319, c);
      address = 41;
    }
    else if (address == 41)
    {
      EEPROM.write(320, c);
      address = 42;
    }
    else if (address == 42)
    {
      EEPROM.write(321, c);
      address = 43;
    }
    else if (address == 43)
    {
      EEPROM.write(322, c);
      address = 44;
    }
    else if (address == 44)
    {
      EEPROM.write(323, c);
      address = 45;
    }
    else if (address == 45)
    {
      EEPROM.write(324, c);
      address = 46;
    }
    else if (address == 46)
    {
      EEPROM.write(325, c);
      address = 47;
    }
    else if (address == 47)
    {
      EEPROM.write(502, c);
      address = 48;
    }
    else if (address == 48)
    {
      EEPROM.write(503, c);
      address = 49;
    }
    else if (address == 49)
    {
      Serial.println("");
      Serial.println("Date Slot 1  --");
      Serial.print("On Sec --");
      Serial.println(EEPROM.read(0));
      Serial.print("On Min --");
      Serial.println(EEPROM.read(1));
      Serial.print("On Hour --");
      Serial.println(EEPROM.read(2));
      Serial.print("Off Sec --");
      Serial.println(EEPROM.read(3));
      Serial.print("Off Min --");
      Serial.println(EEPROM.read(4));
      Serial.print("Off Hour --");
      Serial.println(EEPROM.read(5));
      Serial.print("Date --");
      Serial.println(EEPROM.read(6));
      Serial.print("Month --");
      Serial.println(EEPROM.read(7));
      Serial.print("Year --");
      Serial.println(EEPROM.read(8));

      Serial.println("");
      Serial.println("Date Slot 2 --");
      Serial.print("On Sec --");
      Serial.println(EEPROM.read(9));
      Serial.print("On Min --");
      Serial.println(EEPROM.read(10));
      Serial.print("On Hour --");
      Serial.println(EEPROM.read(11));
      Serial.print("Off Sec --");
      Serial.println(EEPROM.read(12));
      Serial.print("Off Min --");
      Serial.println(EEPROM.read(13));
      Serial.print("Off Hour --");
      Serial.println(EEPROM.read(14));
      Serial.print("Date --");
      Serial.println(EEPROM.read(15));
      Serial.print("Month --");
      Serial.println(EEPROM.read(16));
      Serial.print("Year --");
      Serial.println(EEPROM.read(17));

      Serial.println("");
      Serial.println("Day Slot 1 --");
      Serial.print("Sunday --");
      Serial.println(EEPROM.read(300));
      Serial.print("Monday--");
      Serial.println(EEPROM.read(301));
      Serial.print("Tuesday --");
      Serial.println(EEPROM.read(302));
      Serial.print("Wednesday --");
      Serial.println(EEPROM.read(303));
      Serial.print("Thursday --");
      Serial.println(EEPROM.read(304));
      Serial.print("Friday --");
      Serial.println(EEPROM.read(305));
      Serial.print("Saturday--");
      Serial.println(EEPROM.read(306));
      Serial.print("On Sec --");
      Serial.println(EEPROM.read(307));
      Serial.print("On Min --");
      Serial.println(EEPROM.read(308));
      Serial.print("On Hour --");
      Serial.println(EEPROM.read(309));
      Serial.print("Off Sec --");
      Serial.println(EEPROM.read(310));
      Serial.print("Off Min --");
      Serial.println(EEPROM.read(311));
      Serial.print("Off Hour --");
      Serial.println(EEPROM.read(312));

      Serial.println("");
      Serial.println("Day Slot 2 --");
      Serial.print("Sunday --");
      Serial.println(EEPROM.read(313));
      Serial.print("Monday--");
      Serial.println(EEPROM.read(314));
      Serial.print("Tuesday --");
      Serial.println(EEPROM.read(315));
      Serial.print("Wednesday --");
      Serial.println(EEPROM.read(316));
      Serial.print("Thursday --");
      Serial.println(EEPROM.read(317));
      Serial.print("Friday --");
      Serial.println(EEPROM.read(318));
      Serial.print("Saturday--");
      Serial.println(EEPROM.read(319));
      Serial.print("On Sec --");
      Serial.println(EEPROM.read(320));
      Serial.print("On Min --");
      Serial.println(EEPROM.read(321));
      Serial.print("On Hour --");
      Serial.println(EEPROM.read(322));
      Serial.print("Off Sec --");
      Serial.println(EEPROM.read(323));
      Serial.print("Off Min --");
      Serial.println(EEPROM.read(324));
      Serial.print("Off Hour --");
      Serial.println(EEPROM.read(325));

      Serial.println("");
      Serial.println("Flags --");
      Serial.print("Date Slot 1 --");
      Serial.println(EEPROM.read(500));
      Serial.print("Date Slot 2 --");
      Serial.println(EEPROM.read(501));
      Serial.print("Day Slot 1 --");
      Serial.println(EEPROM.read(502));
      Serial.print("Day Slot 2 --");
      Serial.println(EEPROM.read(503));
      Serial.println("");

      address = 0;
    }
    delay(5);
  }
}

void day_alarm_1()
{
  if (Day == EEPROM.read(300))                           // Sunday
  {
    Serial.println("Sunday");
    if (Hor == EEPROM.read(309) && Min == EEPROM.read(308) && Sec == EEPROM.read(307))
    {
      digitalWrite(day_led_slot_1, HIGH);
    }
    else if (Hor == EEPROM.read(312) && Min == EEPROM.read(311) && Sec == EEPROM.read(310))
    {
      digitalWrite(day_led_slot_1, LOW);
      EEPROM.write(300, 0);
    }
  }

  else if (Day == EEPROM.read(301))                         // Monday
  {
    Serial.println("Monday");
    if (Hor == EEPROM.read(309) && Min == EEPROM.read(308) && Sec == EEPROM.read(307))
    {
      digitalWrite(day_led_slot_1, HIGH);
    }
    else if (Hor == EEPROM.read(312) && Min == EEPROM.read(311) && Sec == EEPROM.read(310))
    {
      digitalWrite(day_led_slot_1, LOW);
      EEPROM.write(301, 0);
    }
  }

  else if (Day == EEPROM.read(302))                       // Tuesday
  {
    Serial.println("Tuesday");
    if (Hor == EEPROM.read(309) && Min == EEPROM.read(308) && Sec == EEPROM.read(307))
    {
      digitalWrite(day_led_slot_1, HIGH);
    }
    else if (Hor == EEPROM.read(312) && Min == EEPROM.read(311) && Sec == EEPROM.read(310))
    {
      digitalWrite(day_led_slot_1, LOW);
      EEPROM.write(302, 0);
    }
  }

  else if (Day == EEPROM.read(303))                      // Wedneday
  {
    Serial.println("Wednesday");
    if (Hor == EEPROM.read(309) && Min == EEPROM.read(308) && Sec == EEPROM.read(307))
    {
      digitalWrite(day_led_slot_1, HIGH);
    }
    else if (Hor == EEPROM.read(312) && Min == EEPROM.read(311) && Sec == EEPROM.read(310))
    {
      digitalWrite(day_led_slot_1, LOW);
      EEPROM.write(303, 0);
    }
  }

  else if (Day == EEPROM.read(304))                      // Thursday
  {
    Serial.println("Thursday");
    if (Hor == EEPROM.read(309) && Min == EEPROM.read(308) && Sec == EEPROM.read(307))
    {
      digitalWrite(day_led_slot_1, HIGH);
    }
    else if (Hor == EEPROM.read(312) && Min == EEPROM.read(311) && Sec == EEPROM.read(310))
    {
      digitalWrite(day_led_slot_1, LOW);
      EEPROM.write(304, 0);
    }
  }

  else if (Day == EEPROM.read(305))                     // Friday
  {
    Serial.println("Friday");
    if (Hor == EEPROM.read(309) && Min == EEPROM.read(308) && Sec == EEPROM.read(307))
    {
      digitalWrite(day_led_slot_1, HIGH);
    }
    else if (Hor == EEPROM.read(312) && Min == EEPROM.read(311) && Sec == EEPROM.read(310))
    {
      digitalWrite(day_led_slot_1, LOW);
      EEPROM.write(305, 0);
    }
  }

  else if (Day == EEPROM.read(306))                     // Saturday
  {
    Serial.println("Saturday");
    if (Hor == EEPROM.read(309) && Min == EEPROM.read(308) && Sec == EEPROM.read(307))
    {
      digitalWrite(day_led_slot_1, HIGH);
    }
    else if (Hor == EEPROM.read(312) && Min == EEPROM.read(311) && Sec == EEPROM.read(310))
    {
      digitalWrite(day_led_slot_1, LOW);
      EEPROM.write(306, 0);
    }
  }
  else if (EEPROM.read(300) == 0 && EEPROM.read(301) == 0 && EEPROM.read(302) == 0 && EEPROM.read(303) == 0 && EEPROM.read(304) == 0 && EEPROM.read(305) == 0 && EEPROM.read(306) == 0  )
  { // TOTAL ALARM OVER
    Wire.beginTransmission(5);
    Wire.write('3');               // On complition of action
    Wire.endTransmission();
  }
}

void day_alarm_2()
{
  if (Day == EEPROM.read(313))                           // Sunday
  {
    Serial.println("Sunday");
    if (Hor == EEPROM.read(322) && Min == EEPROM.read(321) && Sec == EEPROM.read(320))
    {
      digitalWrite(day_led_slot_2, HIGH);
    }
    else if (Hor == EEPROM.read(325) && Min == EEPROM.read(324) && Sec == EEPROM.read(323))
    {
      digitalWrite(day_led_slot_2, LOW);
      EEPROM.write(300, 0);
    }
  }

  else if (Day == EEPROM.read(314))                         // Monday
  {
    Serial.println("Monday");
    if (Hor == EEPROM.read(322) && Min == EEPROM.read(321) && Sec == EEPROM.read(320))
    {
      digitalWrite(day_led_slot_2, HIGH);
    }
    else if (Hor == EEPROM.read(325) && Min == EEPROM.read(324) && Sec == EEPROM.read(323))
    {
      digitalWrite(day_led_slot_2, LOW);
      EEPROM.write(314, 0);
    }
  }

  else if (Day == EEPROM.read(315))                       // Tuesday
  {
    Serial.println("Tuesday");
    if (Hor == EEPROM.read(322) && Min == EEPROM.read(321) && Sec == EEPROM.read(320))
    {
      digitalWrite(day_led_slot_2, HIGH);
    }
    else if (Hor == EEPROM.read(325) && Min == EEPROM.read(324) && Sec == EEPROM.read(323))
    {
      digitalWrite(day_led_slot_2, LOW);
      EEPROM.write(315, 0);
    }
  }

  else if (Day == EEPROM.read(316))                      // Wedneday
  {
    Serial.println("Wednesday");
    if (Hor == EEPROM.read(322) && Min == EEPROM.read(321) && Sec == EEPROM.read(320))
    {
      digitalWrite(day_led_slot_2, HIGH);
    }
    else if (Hor == EEPROM.read(325) && Min == EEPROM.read(324) && Sec == EEPROM.read(323))
    {
      digitalWrite(day_led_slot_2, LOW);
      EEPROM.write(316, 0);
    }
  }

  else if (Day == EEPROM.read(317))                      // Thursday
  {
    Serial.println("Thursday");
    if (Hor == EEPROM.read(322) && Min == EEPROM.read(321) && Sec == EEPROM.read(320))
    {
      digitalWrite(day_led_slot_2, HIGH);
    }
    else if (Hor == EEPROM.read(325) && Min == EEPROM.read(324) && Sec == EEPROM.read(323))
    {
      digitalWrite(day_led_slot_2, LOW);
      EEPROM.write(317, 0);
    }
  }

  else if (Day == EEPROM.read(318))                     // Friday
  {
    Serial.println("Friday");
    if (Hor == EEPROM.read(322) && Min == EEPROM.read(321) && Sec == EEPROM.read(320))
    {
      digitalWrite(day_led_slot_2, HIGH);
    }
    else if (Hor == EEPROM.read(325) && Min == EEPROM.read(324) && Sec == EEPROM.read(323))
    {
      digitalWrite(day_led_slot_2, LOW);
      EEPROM.write(318, 0);
    }
  }

  else if (Day == EEPROM.read(319))                     // Saturday
  {
    Serial.println("Saturday");
    if (Hor == EEPROM.read(322) && Min == EEPROM.read(321) && Sec == EEPROM.read(320))
    {
      digitalWrite(day_led_slot_2, HIGH);
    }
    else if (Hor == EEPROM.read(325) && Min == EEPROM.read(324) && Sec == EEPROM.read(323))
    {
      digitalWrite(day_led_slot_2, LOW);
      EEPROM.write(319, 0);
    }
  }
  else if (EEPROM.read(313) == 0 && EEPROM.read(314) == 0 && EEPROM.read(315) == 0 && EEPROM.read(316) == 0 && EEPROM.read(317) == 0 && EEPROM.read(318) == 0 && EEPROM.read(319) == 0)
  {
    // TOTAL ALARM OVER
    Wire.beginTransmission(5);
    Wire.write('4');   // On complition of action
    Wire.endTransmission();
  }
}


