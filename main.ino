//By HomeMade Electronics
//Subscribe to my channel https://www.youtube.com/channel/UC8isJR_71x1wIfw6jB106pg
//for more tutorial videos
//like, share and leave a comment if you need help

#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

char keys[4][3]={
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'*','0','#'}};
 
byte rowPin[4]={6,7,8,9};
byte colPin[3]={3,4,5};

String password = "2233";  // The Pin Code.
String data = "";
int position = 0;
bool dooropen = false;

//int wrong = 0; // Variable to calculate the wrong inputs.

int redPin = 10;
int greenPin = 11;
int buzzer = 12;
 
Keypad keypad=Keypad(makeKeymap(keys),rowPin,colPin,4,3);
// MAPPING THE KEYPAD.


int total_attempts = 0; // Variable to determine the number of wrong attempts.

void setup()
{
  
  //pinMode(redPin,OUTPUT);
  //pinMode(greenPin,OUTPUT);
  //pinMode(buzzer, OUTPUT);
 
  lcd.begin(16, 2);
  lcd.print(" Arduino Door");
  lcd.setCursor(0, 1);
  lcd.print("--Lock project--");
  delay(3000);
  lcd.clear();
}

void loop()
{
  
//   lcd.clear();
//   lcd.print("  Enter Code:");
//   delay(100);
  
  //char pressed=keypad.getKey();
  //String key[3];

  if (dooropen == true)
  {
    char pressed = keypad.getKey();

    if (pressed == '#' || pressed == '*')

    {
      lcd.clear();
      setLocked(true);
      lcd.print("  Door Closed");
      delay(2000);
      dooropen = false;
    }
  }

  else Open();


  
//  if(pressed)
//  {
//   lcd.clear();
//   lcd.print(" Enter Unlock Code: ");
//   lcd.setCursor(position,2);
//   lcd.print(pressed);
//   delay(500);
//     if(pressed == '*' || pressed == '#')
//       {
//           position = 0;
//           setLocked(true);
//           lcd.clear();
//       }

//     else if(pressed == password[position])
//       {
//           key[position]=pressed;
//           position++;
//       }
 
//     else if (pressed != password[position] )
//       {
//           wrong++;
//           position ++;
//       }

//     if(position == 4){
//           if( wrong >0)
//             {
//                 total++;
//                 wrong = 0;
//                 position = 0;
//                 lcd.clear();
//                 lcd.setCursor(0,2);
//                 lcd.print("    Wrong Code!     ");
//                 delay(1000);
//                 setLocked(true);
//             }

//           else if(position == 4 && wrong == 0)
//             {
//                 position = 0;
//                 wrong = 0;
//                 lcd.clear();
//                 lcd.setCursor(0,1);
//                 lcd.print("      Welcome!     ");
//                 lcd.setCursor(5,2);
//                 lcd.print(" Door Open");
//                 delay(2000);
//                 setLocked(false);
//             }

//              if(total ==3)
//             {
//                 total=0;
//                 buzzer_beep();
//                 delay(500);
//             }

//         }

//    }

   
}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password");

  char pressed = keypad.getKey();
  if (pressed) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    data += pressed; // store char into data string
    lcd.setCursor(position, 1); // move cursor to show each new char
    lcd.print("*"); // print char at said cursor
    //data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
    position++;
  }

  if(data.length() == 4)
  {
    if(data == password)
    {
      lcd.clear();
      setLocked(false);
      lcd.print("  Access Granted");
      dooropen = true;
    }
    else 
    {
      lcd.clear();
      lcd.print("  Access Denied");
      delay(1000);
      dooropen = false;
      total_attempts++;
      if(total_attempts==4)
      {
        total_attempts=0;
        buzzer_beep();
        delay(500);
      }
    }
    data = "";
    position = 0;
  }

}

void setLocked(int locked)
  {
    if (locked)
    {
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      delay(1000);
    }
    else
    {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      delay(1000);
      //digitalWrite(redPin, HIGH);
      //digitalWrite(greenPin, LOW);
    }
  }
void buzzer_beep()
{
  //  lcd.clear();
  //  lcd.setCursor(0,1);
  //  lcd.print("    WARNING  !!!!   ");
  //  lcd.setCursor(0,2);
  //  lcd.print("   Access Denied");
  for (int i=0;i<10;i++)
  {
    digitalWrite(buzzer,HIGH);
    delay(5000);
    digitalWrite(buzzer,LOW);
    delay(1000);
  }
}
