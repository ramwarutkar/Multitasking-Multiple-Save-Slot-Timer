//************* Slave Arduino Uno *********************
/*
   Flags
   Date: - Slot 1 500
           Slot 2 501

   Day: -  Slot 1 502
           Slot 2 503
*/
#include<Wire.h>
#include<EEPROM.h>
#include "Nokia_5110.h"                                   //Nokia 5110 Library
#define RST 2
#define CE 3
#define DC 7
#define DIN 5
#define CLK 6
Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK);



const int up = 8;                                         // Buttons
const int down = 9;
const int set_but = 10;

//  Global Integers
static int address = 0;
int tot_state = 0;                                       // Total State of Buttons
static int menu_set = 0;
static int by_date_count = 0 ;
static int by_day_count = 0 ;
static int view_menu = 0;

static int date_date = 1;
static int date_month = 1;
static int date_year = 2018;
static int on_sec  = 0 ;
static int on_min  = 0 ;
static int on_hour  = 0 ;
static int off_sec  = 0 ;
static int off_min  = 0 ;
static int off_hour  = 0 ;
static int sunday = 0;
static int monday = 0;
static int tuesday = 0;
static int wednesday = 0;
static int thursday = 0;
static int friday = 0;
static int saturday = 0;



//  *********  Functions    *******

void intro(void);

void menu_select(void);                                 // Main Functions
void by_date(void);
void by_days(void);
void view_saved(void);

void set_date(void);                                    // Secondary Functions
void transfer_date(void);
void set_days(void);
void transfer_days(void);
void clear_everything(void);
void view_type(void);
void type_date(void);
void type_day(void);
void view_date_slot_1(void);
void view_date_slot_2(void);
void view_day_slot_1(void);
void view_day_slot_2(void);

void on_time(void);                                    // Common Functions
void off_time(void);

void commonregisters(void);
void save_ladder(void);
void acknowledgement(void);

void setup()
{
  Serial.begin(9600);
  Wire.begin(5);
  Wire.onRequest(commonregisters);
  Wire.onReceive(acknowledgement);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  pinMode(set_but, INPUT);
  intro();
  delay(3000);
  lcd.setContrast(60);
  EEPROM.write(500, 1);
  EEPROM.write(501, 1);
  EEPROM.write(502, 1);
  EEPROM.write(503, 1);
}


void loop()
{
  if (menu_set == 0)                                 //  Main Menu
  {
    do
    {
      menu_select();
    } while (menu_set == 0);
  }
  else if (menu_set == 1)                           // By Date Menu
  {
    do
    {
      by_date();
    } while (menu_set == 1);
  }
  else if (menu_set == 2)                          // By Day Menu
  {
    do
    {
      by_days();
    } while (menu_set == 2);

  }
  else if (menu_set == 3)                          // View Saved Menu
  {
    do
    {
      view_saved();
    } while (menu_set == 3);
  }

}

void intro()
{
  lcd.setCursor(28, 1);
  lcd.print("SAMOM");
  lcd.setCursor(9, 2);
  lcd.print("Technologies");
  delay(1000);
}

void menu_select()                           //  Switch to Select Mode in the menu
{
  static int print_menu = 1;
  static int menu_select = 1;
  static int menu_switch = 1;

  if (print_menu == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SELECT MODE");
    lcd.setCursor(20, 2);
    lcd.print("ByDate");
    lcd.setCursor(20, 3);
    lcd.print("ByDay");
    lcd.setCursor(20, 4);
    lcd.print("View Saved");
    print_menu = 0;
  }
  tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
  delay(200);
  if (tot_state == 1 && (menu_switch > 1))
  {
    menu_switch--;
  }
  else if (tot_state == 2 && (menu_switch <= 2))
  {
    menu_switch++;
  }
  else if (tot_state == 3)
  {
    menu_switch = 4;
  }
  switch (menu_switch)
  {
    case 1:
      {
        lcd.clear(3, 0, 15);
        lcd.setCursor(0, 2);
        lcd.print(">>");
        menu_select = 1;
        break;
      }
    case 2:
      {
        lcd.clear(4, 0, 15);
        lcd.clear(2, 0, 15);
        lcd.setCursor(0, 3);
        lcd.print(">>");
        menu_select = 2;
        break;
      }
    case 3:
      {
        lcd.clear(3, 0, 15);
        lcd.setCursor(0, 4);
        lcd.print(">>");
        menu_select = 3;
        break;
      }
    case 4:
      {
        menu_set = menu_select;
        print_menu = 1;
        menu_switch = menu_select ;
        break;
      }
    default : break;
  }
}

void by_date()                                                //  Mode By_Date
{

  if (by_date_count == 0)
  {
    do
    {
      set_date();
    } while (by_date_count == 0);
  }
  else if (by_date_count == 1)
  {
    do
    {
      on_time();
    } while (by_date_count == 1);
  }
  else if (by_date_count == 2)
  {
    do
    {
      off_time();

    } while (by_date_count == 2);
  }
  else if (by_date_count == 3)
  {
    do
    {
      transfer_date();
    } while (by_date_count == 3);
  }

}

void by_days()                                                 //  Mode By Days
{

  if (by_day_count == 0)
  {
    do
    {
      set_days();
    } while (by_day_count == 0);
  }
  else if (by_day_count == 1)
  {
    do
    {
      on_time();
    } while (by_day_count == 1);
  }
  else if (by_day_count == 2)
  {
    do
    {
      off_time();
    } while (by_day_count == 2);
  }
  else if (by_day_count == 3)
  {
    do
    {
      transfer_days();
    } while (by_day_count == 3);
  }

}

void view_saved()                       //  View Saved
{
  if (view_menu == 0)
  {
    do
    {
      view_type();
    } while (view_menu == 0);
  }
  else if (view_menu == 1)
  {
    do
    {
      type_date();
    } while (view_menu == 1);
  }
  else if (view_menu == 2)
  {
    do
    {
      type_day();
    } while (view_menu == 2);
  }
}

void set_date()
{
  static int print_date = 1;
  static int duration_count = 1;
  static int month_datelimit = 0;

  if (print_date == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SET DATE:-");
    lcd.setCursor(8, 3);
    lcd.print("1   / 1  /2018");
    print_date = 0;
  }
  if (duration_count == 1)             //   FOR SETTING YEAR
  {
    do
    {
      lcd.setCursor(57, 4);
      lcd.print("^");
      tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(200);
      switch (tot_state)
      {
        case 1:
          {
            if (date_year <= 2050)
            {
              date_year++;
              lcd.clear(3, 50, 84);
              lcd.print(date_year);
            }
            break;
          }
        case 2:
          {
            if (date_year >= 2019)
            {
              date_year--;
              lcd.clear(3, 50, 84);
              lcd.print(date_year);
            }
            break;
          }
        case 3:
          {
            duration_count = 2;
            lcd.clear(4, 57, 75);
            break;
          }

        default: break;
      }
    } while (duration_count == 1);
  }
  else if (duration_count == 2)         //   FOR SETTING MINUTES
  {
    do
    {
      lcd.setCursor(32, 4);
      lcd.print("^");
      tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(200);
      switch (tot_state)
      {
        case 1:
          {
            if (date_month <= 11)
            {
              date_month++;
              lcd.clear(3, 30, 40);
              lcd.print(date_month);
            }
            break;
          }
        case 2:
          {
            if (date_month != 1)
            {
              date_month--;
              lcd.clear(3, 30, 40);
              lcd.print(date_month);
            }
            break;
          }
        case 3:
          {
            duration_count = 3;
            lcd.clear(4, 32, 46);
            break;
          }
        default: break;

      }
    } while (duration_count == 2);
  }
  else if (duration_count == 3)                  // FOR SETTING Date
  {
    if (date_month == 1 || date_month == 3 || date_month == 12 || date_month == 5 || date_month == 7 || date_month == 8 || date_month == 10 || date_month == 12)
      month_datelimit = 30;
    else if (date_month == 4 || date_month == 6 || date_month == 9 || date_month == 11 )
      month_datelimit = 29;
    else if (date_month == 2 && date_year == 2020)
      month_datelimit = 28;
    else if (date_month == 2)
      month_datelimit = 27;

    do
    {
      lcd.setCursor(8, 4);
      lcd.print("^");

      tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(200);
      switch (tot_state)
      {
        case 1:
          {
            if (date_date <= month_datelimit )
            {
              date_date++;
              lcd.clear(3, 8, 20);
              lcd.print(date_date);
            }
            break;
          }
        case 2:
          {
            if (date_date != 1)
            {
              date_date--;
              lcd.clear(3, 8, 20);
              lcd.print(date_date);
            }
            break;
          }
        case 3:
          {
            print_date = 1;
            duration_count = 1;
            by_date_count = 1;
            break;
          }
        default: break;
      }
    } while (duration_count == 3);
  }
}


void set_days()                            // For Setting Multiple Days
{
  static int print_days = 1;
  static int travers = 1;
  if (print_days == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SET DAYS -");
    lcd.setCursor(5, 2);
    lcd.print("Su");
    lcd.setCursor(26, 2);
    lcd.print("Mo");
    lcd.setCursor(45, 2);
    lcd.print("Tu");
    lcd.setCursor(65, 2);
    lcd.print("We");
    lcd.setCursor(5, 4);
    lcd.print("Th");
    lcd.setCursor(26, 4);
    lcd.print("Fr");
    lcd.setCursor(45, 4);
    lcd.print("Sa");
    lcd.setCursor(65, 4);
    lcd.print("SET");
    print_days = 0;
  }
  tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
  delay(200);
  if (tot_state == 2)
  {
    if (travers <= 7)
    {
      travers++;
    }
  }
  else if (tot_state == 1)
  {
    if (travers != 1)
    {
      travers--;
    }
  }

  switch (travers)
  {
    case 1:
      {
        lcd.clear(3, 26, 31);
        lcd.setCursor(7, 3);
        lcd.print("^");
        delay(200);
        if (tot_state == 3)
        {
          lcd.setCursor(0, 2);
          lcd.print(">");
          sunday = 7;
        }
        break;
      }
    case 2:
      {
        lcd.clear(3, 7, 12);
        lcd.clear(3, 47, 53);
        lcd.setCursor(26, 3);
        lcd.print("^");
        if (tot_state == 3)
        {
          lcd.setCursor(21, 2);
          lcd.print(">");
          monday = 1;
        }

        break;
      }
    case 3:
      {
        lcd.clear(3, 26, 31);
        lcd.clear(3, 67, 73);
        lcd.setCursor(47, 3);
        lcd.print("^");
        if (tot_state == 3)
        {
          lcd.setCursor(40, 2);
          lcd.print(">");
          tuesday = 2;
        }

        break;
      }
    case 4:
      {
        lcd.clear(3, 47, 53);
        lcd.clear(5, 7, 12);
        lcd.setCursor(67, 3);
        lcd.print("^");
        if (tot_state == 3)
        {
          lcd.setCursor(60, 2);
          lcd.print(">");
          wednesday = 3;
        }

        break;
      }

    case 5:
      {
        lcd.clear(3, 67, 73);
        lcd.clear(5, 26, 31);
        lcd.setCursor(7, 5);
        lcd.print("^");
        if (tot_state == 3)
        {
          lcd.setCursor(0, 4);
          lcd.print(">");
          thursday = 4;
        }
        break;
      }
    case 6:
      {
        lcd.clear(5, 7, 12);
        lcd.clear(5, 47, 53);
        lcd.setCursor(26, 5);
        lcd.print("^");
        if (tot_state == 3)
        {
          lcd.setCursor(21, 4);
          lcd.print(">");
          friday = 5;
        }
        break;
      }
    case 7:
      {
        lcd.clear(5, 26, 31);
        lcd.clear(5, 70, 76);
        lcd.setCursor(47, 5);
        lcd.print("^");
        if (tot_state == 3)
        {
          lcd.setCursor(41, 4);
          lcd.print(">");
          saturday = 6;
        }
        break;
      }
    case 8:
      {
        lcd.clear(5, 47, 53);
        lcd.setCursor(70, 5);
        lcd.print("^");
        if (tot_state == 3)
        {
          print_days = 1;
          travers = 1;
          by_day_count = 1;
        }
        break;
      }

    default: break;
  }
}



void on_time()                                          //    TO SET ON TIME
{
  static int  duration_print = 1;
  static int  duration_count = 1;

  if (duration_print == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SET ON TIME");
    lcd.setCursor(15, 3);
    lcd.print("00 : 00 : 00");
    duration_print = 0;
  }

  if (duration_count == 1)             //   FOR SETTING SECONDS
  {
    do
    {
      lcd.setCursor(57, 4);
      lcd.print("^");
      tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(200);
      switch (tot_state)
      {
        case 1:
          {
            if (on_sec <= 58)
            {
              on_sec++;
              lcd.clear(3, 57, 70);
              lcd.print(on_sec);
            }
            break;
          }
        case 2:
          {
            if (on_sec != 0)
            {
              on_sec--;
              lcd.clear(3, 57, 70);
              lcd.print(on_sec);
            }
            break;
          }
        case 3:
          {
            duration_count = 2;
            lcd.clear(4, 57, 75);
            break;
          }

        default: break;
      }
    } while (duration_count == 1);
  }
  else if (duration_count == 2)         //   FOR SETTING MINUTES
  {
    do
    {
      lcd.setCursor(36, 4);
      lcd.print("^");
      tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(200);
      switch (tot_state)
      {
        case 1:
          {
            if (on_min <= 58)
            {
              on_min++;
              lcd.clear(3, 36, 46);
              lcd.print(on_min);
            }
            break;
          }
        case 2:
          {
            if (on_min != 0)
            {
              on_min--;
              lcd.clear(3, 36, 46);
              lcd.print(on_min);
            }
            break;
          }
        case 3:
          {
            duration_count = 3;
            lcd.clear(4, 36, 46);
            break;
          }
        default: break;

      }
    } while (duration_count == 2);
  }
  else if (duration_count == 3)                  // FOR SETTING HOURS
  {
    do
    {
      lcd.setCursor(15, 4);
      lcd.print("^");

      tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(200);
      switch (tot_state)
      {
        case 1:
          {
            if (on_hour <= 23)
            {
              on_hour++;
              lcd.clear(3, 15, 25);
              lcd.print(on_hour);
            }
            break;
          }
        case 2:
          {
            if (on_hour != 0)
            {
              on_hour--;
              lcd.clear(3, 15, 25);
              lcd.print(on_hour);
            }
            break;
          }
        case 3:
          {
            duration_count = 1;
            duration_print = 1;
            by_day_count = 2;
            by_date_count = 2;
            break;
          }
        default: break;
      }
    } while (duration_count == 3);
  }
}

void off_time()                                       //   For Settinf Off Time
{
  static int  duration_print = 1;
  static int  duration_count = 1;

  if (duration_print == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SET OFF TIME");
    lcd.setCursor(15, 3);
    lcd.print("00 : 00 : 00");
    duration_print = 0;
  }

  if (duration_count == 1)             //   FOR SETTING SECONDS
  {
    do
    {
      lcd.setCursor(57, 4);
      lcd.print("^");
      tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(200);
      switch (tot_state)
      {
        case 1:
          {
            if (off_sec <= 58)
            {
              off_sec++;
              lcd.clear(3, 57, 70);
              lcd.print(off_sec);
            }
            break;
          }
        case 2:
          {
            if (off_sec != 0)
            {
              off_sec--;
              lcd.clear(3, 57, 70);
              lcd.print(off_sec);
            }
            break;
          }
        case 3:
          {
            duration_count = 2;
            lcd.clear(4, 57, 75);
            break;
          }

        default: break;
      }
    } while (duration_count == 1);
  }
  else if (duration_count == 2)         //   FOR SETTING MINUTES
  {
    do
    {
      lcd.setCursor(36, 4);
      lcd.print("^");
      tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(200);
      switch (tot_state)
      {
        case 1:
          {
            if (off_min <= 58)
            {
              off_min++;
              lcd.clear(3, 36, 46);
              lcd.print(off_min);
            }
            break;
          }
        case 2:
          {
            if (off_min != 0)
            {
              off_min--;
              lcd.clear(3, 36, 46);
              lcd.print(off_min);
            }
            break;
          }
        case 3:
          {
            duration_count = 3;
            lcd.clear(4, 36, 46);
            break;
          }
        default: break;

      }
    } while (duration_count == 2);
  }
  else if (duration_count == 3)                  // FOR SETTING HOURS
  {
    do
    {
      lcd.setCursor(15, 4);
      lcd.print("^");

      tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(200);
      switch (tot_state)
      {
        case 1:
          {
            if (off_hour <= 23)
            {
              off_hour++;
              lcd.clear(3, 15, 25);
              lcd.print(off_hour);
            }
            break;
          }
        case 2:
          {
            if (off_hour != 0)
            {
              off_hour--;
              lcd.clear(3, 15, 25);
              lcd.print(off_hour);
            }
            break;
          }
        case 3:
          {
            duration_count = 1;
            duration_print = 1;
            by_day_count = 3;
            by_date_count = 3;
            lcd.clear();
            break;
          }
        default: break;
      }

    } while (duration_count == 3);
  }
}

void transfer_date()   // Function to save timer Setting in EEPROM (if elseif ladder for slot selection)
{
  int temp = 0;
  lcd.clear();
  if (EEPROM.read(500) == 1)  // 1 = Empty 0 = Not Empty
  {
    lcd.setCursor(0, 0);
    lcd.print("Saving Slot 1");
    delay(2000);
    EEPROM.write(0, on_sec);
    EEPROM.write(1, on_min);
    EEPROM.write(2, on_hour);
    EEPROM.write(3, off_sec);
    EEPROM.write(4, off_min);
    EEPROM.write(5, off_hour);
    EEPROM.write(6, date_date);
    EEPROM.write(7, date_month);
    temp = date_year - 2000 ;
    EEPROM.write(8, temp);
    EEPROM.write(500, 0);
  }
  else if (EEPROM.read(501) == 1)  // 1 = Empty 0 = Not Empty
  {
    lcd.setCursor(0, 0);
    lcd.print("Saving Slot 2");
    delay(2000);
    EEPROM.write(9, on_sec);
    EEPROM.write(10, on_min);
    EEPROM.write(11, on_hour);
    EEPROM.write(12, off_sec);
    EEPROM.write(13, off_min);
    EEPROM.write(14, off_hour);
    EEPROM.write(15, date_date);
    EEPROM.write(16, date_month);
    temp = date_year - 2000 ;
    EEPROM.write(17, temp);
    EEPROM.write(501, 0);
  }
  by_date_count = 0;
  by_day_count = 0;
  clear_everything();
  delay(1000);
  menu_set = 0;
}

void transfer_days() // Function to save timer Setting in EEPROM (if elseif ladder for slot selection)
{
  lcd.clear();
  if (EEPROM.read(502) == 1) // 1 = Empty 0 = Not Empty
  {
    lcd.setCursor(0, 0);
    lcd.print("Saving Slot 1");
    EEPROM.write(307, on_sec);
    EEPROM.write(308, on_min);
    EEPROM.write(309, on_hour);
    EEPROM.write(310, off_sec);
    EEPROM.write(311, off_min);
    EEPROM.write(312, off_hour);
    EEPROM.write(300, sunday);
    EEPROM.write(301, monday);
    EEPROM.write(302, tuesday);
    EEPROM.write(303, wednesday);
    EEPROM.write(304, thursday);
    EEPROM.write(305, friday);
    EEPROM.write(306, saturday);
    EEPROM.write(502, 0);
    Serial.print(EEPROM.read(502));
  }
  else if (EEPROM.read(503) == 1)  // 1 = Empty 0 = Not Empty
  {
    lcd.setCursor(0, 0);
    lcd.print("Saving Slot 2");
    EEPROM.write(320, on_sec);
    EEPROM.write(321, on_min);
    EEPROM.write(322, on_hour);
    EEPROM.write(323, off_sec);
    EEPROM.write(324, off_min);
    EEPROM.write(325, off_hour);
    EEPROM.write(313, sunday);
    EEPROM.write(314, monday);
    EEPROM.write(315, tuesday);
    EEPROM.write(316, wednesday);
    EEPROM.write(317, thursday);
    EEPROM.write(318, friday);
    EEPROM.write(319, saturday);
    EEPROM.write(503, 0);
  }
  by_day_count = 0;
  by_date_count = 0;
  delay(990);
  clear_everything();
  menu_set = 0;
}

void commonregisters()   // Function when Master calls for Data
{
  save_ladder();
}
void save_ladder()
{
  if (address == 0)
  {
    Wire.write(255);                 // Start Bit
    address++;
  }
  else if (address == 1)               // Date Slot 1
  {
    Wire.write(EEPROM.read(0));
    address++;
  }
  else if (address == 2)
  {
    Wire.write(EEPROM.read(1));
    delay(10);
    address++;
  }
  else if (address == 3)
  {
    Wire.write(EEPROM.read(2));
    address++;
  }
  else if (address == 4)
  {
    Wire.write(EEPROM.read(3));
    address++;
  }
  else if (address == 5)
  {
    Wire.write(EEPROM.read(4));
    address++;
  }
  else if (address == 6)
  {
    Wire.write(EEPROM.read(5));
    address++;
  }
  else if (address == 7)
  {
    Wire.write(EEPROM.read(6));
    address++;
  }
  else if (address == 8)                       ////////////////
  {
    Wire.write(EEPROM.read(7));
    address++;
  }
  else if (address == 9)                     //  Date Slot 2
  {
    Wire.write(EEPROM.read(8));
    address++;
  }
  else if (address == 10)
  {
    Wire.write(EEPROM.read(9));
    address++;
  }
  else if (address == 11)
  {
    Wire.write(EEPROM.read(10));
    delay(10);
    address++;
  }
  else if (address == 12)
  {
    Wire.write(EEPROM.read(11));
    address++;
  }
  else if (address == 13)
  {
    Wire.write(EEPROM.read(12));
    address++;
  }
  else if (address == 14)
  {
    Wire.write(EEPROM.read(13));
    address++;
  }
  else if (address == 15)
  {
    Wire.write(EEPROM.read(14));
    address++;
  }
  else if (address == 16)
  {
    Wire.write(EEPROM.read(15));
    address++;
  }
  else if (address == 17)
  {
    Wire.write(EEPROM.read(16));
    address++;
  }
  else if (address == 18)
  {
    Wire.write(EEPROM.read(17));                       //////////////
    address++;
  }
  else if (address == 19)
  {
    Wire.write(EEPROM.read(500));
    address++;
  }
  else if (address == 20)
  {
    Wire.write(EEPROM.read(501));
    address++;
  }
  else if  (address == 21)                     //  Day Slot 1
  {
    Wire.write(EEPROM.read(300));
    address++;
  }
  else if  (address == 22)
  {
    Wire.write(EEPROM.read(301));
    address++;
  }
  else if  (address == 23)
  {
    Wire.write(EEPROM.read(302));
    address++;
  }
  else if  (address == 24)
  {
    Wire.write(EEPROM.read(303));
    address++;
  }
  else if  (address == 25)
  {
    Wire.write(EEPROM.read(304));
    address++;
  }
  else if  (address == 26)
  {
    Wire.write(EEPROM.read(305));
    address++;
  }
  else if  (address == 27)
  {
    Wire.write(EEPROM.read(306));
    address++;
  }
  else if  (address == 28)
  {
    Wire.write(EEPROM.read(307));
    address++;
  }
  else if  (address == 29)
  {
    Wire.write(EEPROM.read(308));
    address++;
  }
  else if  (address == 30)
  {
    Wire.write(EEPROM.read(309));
    address++;
  }
  else if  (address == 31)
  {
    Wire.write(EEPROM.read(310));
    address++;
  }
  else if  (address == 32)
  {
    Wire.write(EEPROM.read(311));
    address++;
  }
  else if  (address == 33)
  {
    Wire.write(EEPROM.read(312));                 ////////////////////
    address++;
  }
  else if  (address == 34)                   // Day Slot 2
  {
    Wire.write(EEPROM.read(313));
    address++;
  }
  else if  (address == 35)
  {
    Wire.write(EEPROM.read(314));
    address++;
  }
  else if  (address == 36)
  {
    Wire.write(EEPROM.read(315));
    address++;
  }
  else if  (address == 37)
  {
    Wire.write(EEPROM.read(316));
    address++;
  }
  else if  (address == 38)
  {
    Wire.write(EEPROM.read(317));
    address++;
  }
  else if  (address == 39)
  {
    Wire.write(EEPROM.read(318));
    address++;
  }
  else if  (address == 40)
  {
    Wire.write(EEPROM.read(319));
    address++;
  }
  else if  (address == 41)
  {
    Wire.write(EEPROM.read(320));
    address++;
  }
  else if  (address == 42)
  {
    Wire.write(EEPROM.read(321));
    address++;
  }
  else if  (address == 43)
  {
    Wire.write(EEPROM.read(322));
    address++;
  }
  else if  (address == 44)
  {
    Wire.write(EEPROM.read(323));
    address++;
  }
  else if  (address == 45)
  {
    Wire.write(EEPROM.read(324));
    address++;
  }
  else if  (address == 46)                         //////////////////
  {
    Wire.write(EEPROM.read(325));
    address++;
  }
  else if  (address == 47)
  {
    Wire.write(EEPROM.read(502));
    address++;
  }
  else if (address == 48)
  {
    Wire.write(EEPROM.read(503));
    address++;
  }
  else if (address == 49)
  {
    address = 0;
  }
  delay(5);
}


void acknowledgement(int howMany) // Function called when Master sends Acknowledgement
{
  while (Wire.available())
  {
    int d = Wire.read();

    if (d == '1')
    {
      EEPROM.write(0, 0);
      EEPROM.write(1, 0);
      EEPROM.write(2, 0);
      EEPROM.write(3, 0);
      EEPROM.write(4, 0);
      EEPROM.write(5, 0);
      EEPROM.write(6, 0);
      EEPROM.write(7, 0);
      EEPROM.write(8, 0);
      EEPROM.write(500, 1);
    }
    else if (d == '2')
    {
      EEPROM.write(9, 0);
      EEPROM.write(10, 0);
      EEPROM.write(11, 0);
      EEPROM.write(12, 0);
      EEPROM.write(13, 0);
      EEPROM.write(14, 0);
      EEPROM.write(15, 0);
      EEPROM.write(16, 0);
      EEPROM.write(17, 0);
      EEPROM.write(501, 1);
    }
    else if (d == '3')
    {
      EEPROM.write(307, 0);
      EEPROM.write(308, 0);
      EEPROM.write(309, 0);
      EEPROM.write(310, 0);
      EEPROM.write(311, 0);
      EEPROM.write(312, 0);
      EEPROM.write(300, 0);
      EEPROM.write(301, 0);
      EEPROM.write(302, 0);
      EEPROM.write(303, 0);
      EEPROM.write(304, 0);
      EEPROM.write(305, 0);
      EEPROM.write(306, 0);
      EEPROM.write(502, 1);
    }
    else if (d == '4')
    {
      EEPROM.write(320, 0);
      EEPROM.write(321, 0);
      EEPROM.write(322, 0);
      EEPROM.write(323, 0);
      EEPROM.write(324, 0);
      EEPROM.write(325, 0);
      EEPROM.write(313, 0);
      EEPROM.write(314, 0);
      EEPROM.write(315, 0);
      EEPROM.write(316, 0);
      EEPROM.write(317, 0);
      EEPROM.write(318, 0);
      EEPROM.write(319, 0);
      EEPROM.write(503, 1);
    }

  }
}

void view_type()
{
  static int print_viewsaved = 1;
  static int view_set_select = 1;
  if (print_viewsaved == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SELECT TYPE");
    lcd.setCursor(20, 2);
    lcd.print("ByDate");
    lcd.setCursor(20, 3);
    lcd.print("ByDay");
    lcd.clear(3, 0, 15);
    lcd.setCursor(0, 2);
    lcd.print(">>");
    print_viewsaved = 0;
  }
  tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
  delay(200);
  switch (tot_state)
  {
    case 1:
      {
        lcd.clear(3, 0, 15);
        lcd.setCursor(0, 2);
        lcd.print(">>");
        view_set_select = 1;
        break;
      }
    case  2:
      {
        lcd.clear(4, 0, 15);
        lcd.clear(2, 0, 15);
        lcd.setCursor(0, 3);
        lcd.print(">>");
        view_set_select = 2;
        break;
      }
    case 3:
      {
        print_viewsaved = 1;
        view_menu = view_set_select;
        view_set_select = 1;
      }
    default: break;
  }
}

void type_date()             //  Display Saved Dates
{
  static int type_date_print = 1;
  static int type_date_select = 1;
  static int type_date_save_print = 1;
  static int view_stay = 1;

  if (type_date_print == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SELECT DATE");
    lcd.setCursor(0, 1);
    lcd.print("SLOT: -");
    type_date_print = 0;
    type_date_save_print = 1;
    view_stay = 1;
  }
  tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
  delay(200);
  if (tot_state == 2)
  {
    if (type_date_select <= 1  )
    {
      type_date_select++;
    }
  }
  else if (tot_state == 1)
  {
    if (type_date_select != 1)
    {
      type_date_select--;
    }
  }
  switch (type_date_select)
  {
    case 1:
      {
        lcd.clear(3, 20, 60);
        lcd.setCursor(15, 3);
        lcd.print("<  SLOT 1  >");
        if (tot_state == 3)
        {
          if (EEPROM.read(500) == 1)
          {
            lcd.clear();
            lcd.setCursor(15, 3);
            lcd.print("Empty Slot");
            delay(1000);
            type_date_print = 1;
            view_menu = 0;
            menu_set = 0;
          }
          else
          {
            do
            {
              if (type_date_save_print == 1)
              {
                view_date_slot_1();
                type_date_save_print = 0;
              }
              if (digitalRead(set_but) == HIGH)
              {
                EEPROM.write(0, 0);
                EEPROM.write(1, 0);
                EEPROM.write(2, 0);
                EEPROM.write(3, 0);
                EEPROM.write(4, 0);
                EEPROM.write(5, 0);
                EEPROM.write(6, 0);
                EEPROM.write(7, 0);
                EEPROM.write(8, 0);
                EEPROM.write(500, 1);
                type_date_print = 1;
                view_menu = 0;
                menu_set = 0;
                view_stay = 0;
                delay(1000);
              }
              else if (digitalRead(down) == HIGH)
              {
                type_date_print = 1;
                view_menu = 0;
                menu_set = 0;
                view_stay = 0;
                delay(1000);
              }
            } while (view_stay == 1);
          }
        }
        break;
      }
    case 2:
      {
        lcd.clear(3, 20, 60);
        lcd.setCursor(15, 3);
        lcd.print("<  SLOT 2  >");
        if (tot_state == 3)
        {
          if (EEPROM.read(501) == 1)
          {
            lcd.clear();
            lcd.setCursor(15, 3);
            lcd.print("Empty Slot");
            delay(1000);
            type_date_print = 1;
            view_menu = 0;
            menu_set = 0;
          }
          else
          {
            do
            {
              if (type_date_save_print == 1)
              {
                view_date_slot_2();
                type_date_save_print = 0;
              }
              if (digitalRead(set_but) == HIGH)
              {
                EEPROM.write(9, 0);
                EEPROM.write(10, 0);
                EEPROM.write(11, 0);
                EEPROM.write(12, 0);
                EEPROM.write(13, 0);
                EEPROM.write(14, 0);
                EEPROM.write(15, 0);
                EEPROM.write(16, 0);
                EEPROM.write(17, 0);
                EEPROM.write(501, 1);
                type_date_print = 1;
                view_menu = 0;
                menu_set = 0;
                view_stay = 0;
                delay(1000);
              }
              else if (digitalRead(down) == HIGH)
              {
                type_date_print = 1;
                view_menu = 0;
                menu_set = 0;
                view_stay = 0;
                delay(1000);
              }
            } while (view_stay == 1);
          }
        }
        break;
      }
    default: break;
  }
}
void view_date_slot_1()
{
  int temp = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DETAILS:-");
  lcd.setCursor(0, 1);
  lcd.print("ON-");
  lcd.setCursor(28, 1);
  lcd.print(EEPROM.read(2));
  lcd.setCursor(40, 1);
  lcd.print(":");
  lcd.setCursor(47, 1);
  lcd.print(EEPROM.read(1));
  lcd.setCursor(63, 1);
  lcd.print(":");
  lcd.setCursor(67, 1);
  lcd.print(EEPROM.read(0));

  lcd.setCursor(0, 2);
  lcd.print("OFF-");
  lcd.setCursor(28, 2);
  lcd.print(EEPROM.read(5));
  lcd.setCursor(40, 2);
  lcd.print(":");
  lcd.setCursor(47, 2);
  lcd.print(EEPROM.read(4));
  lcd.setCursor(63, 2);
  lcd.print(":");
  lcd.setCursor(67, 2);
  lcd.print(EEPROM.read(3));
  lcd.setCursor(0, 3);
  lcd.print(EEPROM.read(6));
  lcd.setCursor(13, 3);
  lcd.print("/");
  lcd.setCursor(20, 3);
  lcd.print(EEPROM.read(7));
  lcd.setCursor(34, 3);
  lcd.print("/");
  temp = 2000 + EEPROM.read(8);
  lcd.setCursor(41, 3);
  lcd.print(temp);
  lcd.setCursor(0, 5);
  lcd.print("RESET  ::  BACK");
  delay(100);
}
void view_date_slot_2()
{
  int temp = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DETAILS:-");
  lcd.setCursor(0, 1);
  lcd.print("ON-");
  lcd.setCursor(28, 1);
  lcd.print(EEPROM.read(11));
  lcd.setCursor(40, 1);
  lcd.print(":");
  lcd.setCursor(47, 1);
  lcd.print(EEPROM.read(10));
  lcd.setCursor(63, 1);
  lcd.print(":");
  lcd.setCursor(67, 1);
  lcd.print(EEPROM.read(9));

  lcd.setCursor(0, 2);
  lcd.print("OFF-");
  lcd.setCursor(28, 2);
  lcd.print(EEPROM.read(14));
  lcd.setCursor(40, 2);
  lcd.print(":");
  lcd.setCursor(47, 2);
  lcd.print(EEPROM.read(13));
  lcd.setCursor(63, 2);
  lcd.print(":");
  lcd.setCursor(67, 2);
  lcd.print(EEPROM.read(12));
  lcd.setCursor(0, 3);
  lcd.print(EEPROM.read(15));
  lcd.setCursor(13, 3);
  lcd.print("/");
  lcd.setCursor(20, 3);
  lcd.print(EEPROM.read(16));
  lcd.setCursor(34, 3);
  lcd.print("/");
  temp = 2000 + EEPROM.read(17);
  lcd.setCursor(41, 3);
  lcd.print(temp);
  lcd.setCursor(0, 5);
  lcd.print("RESET  ::  BACK");
  delay(100);
}
void type_day()            // Display Saved Days
{
  static int type_date_print = 1;
  static int type_date_select = 1;
  static int type_date_save_print = 1;
  static int view_stay = 1;


  if (type_date_print == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SELECT DAYS");
    lcd.setCursor(0, 1);
    lcd.print("SLOT: -");
    type_date_print = 0;
    type_date_save_print = 1;
    view_stay = 1;
  }
  tot_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
  delay(200);
  if (tot_state == 2)
  {
    if (type_date_select <= 1  )
    {
      type_date_select++;
    }
  }
  else if (tot_state == 1)
  {
    if (type_date_select != 1)
    {
      type_date_select--;
    }
  }
  switch (type_date_select)
  {
    case 1:
      {
        lcd.clear(3, 20, 60);
        lcd.setCursor(15, 3);
        lcd.print("<  SLOT 1  >");
        if (tot_state == 3)
        {
          if (EEPROM.read(502) == 1)
          {
            lcd.clear();
            lcd.setCursor(15, 3);
            lcd.print("Empty Slot");
            delay(1000);
            type_date_print = 1;
            view_menu = 0;
            menu_set = 0;
          }
          else
          {
            do
            {
              if (type_date_save_print == 1)
              {
                view_day_slot_1();
                type_date_save_print = 0;
              }
              if (digitalRead(set_but) == HIGH)
              {
                EEPROM.write(307, 0);
                EEPROM.write(308, 0);
                EEPROM.write(309, 0);
                EEPROM.write(310, 0);
                EEPROM.write(311, 0);
                EEPROM.write(312, 0);
                EEPROM.write(300, 0);
                EEPROM.write(301, 0);
                EEPROM.write(302, 0);
                EEPROM.write(303, 0);
                EEPROM.write(304, 0);
                EEPROM.write(305, 0);
                EEPROM.write(306, 0);
                EEPROM.write(502, 1);
                type_date_print = 1;
                view_menu = 0;
                menu_set = 0;
                view_stay = 0;
                delay(1000);
              }
              else if (digitalRead(down) == HIGH)
              {
                type_date_print = 1;
                view_menu = 0;
                menu_set = 0;
                view_stay = 0;
                delay(1000);
              }
            } while (view_stay == 1);
          }
        }
        break;
      }
    case 2:
      {
        lcd.clear(3, 20, 60);
        lcd.setCursor(15, 3);
        lcd.print("<  SLOT 2  >");
        if (tot_state == 3)
        {
          if (EEPROM.read(503) == 1)
          {
            lcd.clear();
            lcd.setCursor(15, 3);
            lcd.print("Empty Slot");
            delay(1000);
            type_date_print = 1;
            view_menu = 0;
            menu_set = 0;
          }
          else
          {
            do
            {
              if (type_date_save_print == 1)
              {
                view_day_slot_2();
                type_date_save_print = 0;
              }
              if (digitalRead(set_but) == HIGH)
              {
                EEPROM.write(320, 0);
                EEPROM.write(321, 0);
                EEPROM.write(322, 0);
                EEPROM.write(323, 0);
                EEPROM.write(324, 0);
                EEPROM.write(325, 0);
                EEPROM.write(313, 0);
                EEPROM.write(314, 0);
                EEPROM.write(315, 0);
                EEPROM.write(316, 0);
                EEPROM.write(317, 0);
                EEPROM.write(318, 0);
                EEPROM.write(319, 0);
                EEPROM.write(503, 1);
                type_date_print = 1;
                view_menu = 0;
                menu_set = 0;
                view_stay = 0;
                delay(1000);
              }
              else if (digitalRead(down) == HIGH)
              {
                type_date_print = 1;
                view_menu = 0;
                menu_set = 0;
                view_stay = 0;
                delay(1000);
              }
            } while (view_stay == 1);
          }
        }
        break;
      }
    default: break;
  }
}

void view_day_slot_1()
{
  static int day_sequence_print;
  static int day_sequence;
  day_sequence = 0;
  day_sequence_print = 1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DETAILS:-");
  lcd.setCursor(0, 1);
  lcd.print("SLOT 1");
  lcd.setCursor(0, 2);
  lcd.print("ON-");
  lcd.setCursor(28, 2);
  lcd.print(EEPROM.read(309));
  lcd.setCursor(40, 2);
  lcd.print(":");
  lcd.setCursor(47, 2);
  lcd.print(EEPROM.read(308));
  lcd.setCursor(63, 2);
  lcd.print(":");
  lcd.setCursor(67, 2);
  lcd.print(EEPROM.read(307));

  lcd.setCursor(0, 3);
  lcd.print("OFF-");
  lcd.setCursor(28, 3);
  lcd.print(EEPROM.read(312));
  lcd.setCursor(40, 3);
  lcd.print(":");
  lcd.setCursor(47, 3);
  lcd.print(EEPROM.read(311));
  lcd.setCursor(63, 3);
  lcd.print(":");
  lcd.setCursor(67, 3);
  lcd.print(EEPROM.read(310));
  lcd.setCursor(0, 5);
  lcd.print("RESET  ::  BACK");
  delay(100);

  if (day_sequence_print == 1)
  {
    if (EEPROM.read(300) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("S");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(301) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("M");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(302) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("T");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(303) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("W");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(304) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("Th");
      day_sequence = day_sequence + 17;
    }
    if (EEPROM.read(305) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("F");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(306) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("Sa");
      day_sequence_print = 0;
    }
  }
}
void view_day_slot_2()                         
{
  static int day_sequence_print;
  static int day_sequence;
  day_sequence = 0;
  day_sequence_print = 1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DETAILS:-");
  lcd.setCursor(0, 1);
  lcd.print("SLOT 2");
  lcd.setCursor(0, 2);
  lcd.print("ON-");
  lcd.setCursor(28, 2);
  lcd.print(EEPROM.read(322));
  lcd.setCursor(40, 2);
  lcd.print(":");
  lcd.setCursor(47, 2);
  lcd.print(EEPROM.read(321));
  lcd.setCursor(63, 2);
  lcd.print(":");
  lcd.setCursor(67, 2);
  lcd.print(EEPROM.read(320));

  lcd.setCursor(0, 3);
  lcd.print("OFF-");
  lcd.setCursor(28, 3);
  lcd.print(EEPROM.read(325));
  lcd.setCursor(40, 3);
  lcd.print(":");
  lcd.setCursor(47, 3);
  lcd.print(EEPROM.read(324));
  lcd.setCursor(63, 3);
  lcd.print(":");
  lcd.setCursor(67, 3);
  lcd.print(EEPROM.read(323));
  lcd.setCursor(0, 5);
  lcd.print("RESET  ::  BACK");
  delay(100);

  if (day_sequence_print == 1)
  {
    if (EEPROM.read(313) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("S");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(314) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("M");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(315) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("T");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(316) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("W");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(317) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("Th");
      day_sequence = day_sequence + 17;
    }
    if (EEPROM.read(318) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("F");
      day_sequence = day_sequence + 10;
    }
    if (EEPROM.read(319) != 0)
    {
      lcd.setCursor(day_sequence, 4);
      lcd.print("Sa");
      day_sequence_print = 0;
    }
  }
}

void clear_everything()  // Fucntion to clear all Static Variables 
{
  date_date = 1;
  date_month = 1;
  date_year = 2018;
  on_sec  = 0 ;
  on_min  = 0 ;
  on_hour  = 0 ;
  off_sec  = 0 ;
  off_min  = 0 ;
  off_hour  = 0 ;
  sunday = 0;
  monday = 0;
  tuesday = 0;
  wednesday = 0;
  thursday = 0;
  friday = 0;
  saturday = 0;
}







