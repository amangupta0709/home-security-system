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

String data = "";
int position = 6;
bool dooropen = false;

int redPin = 10;
int greenPin = 11;
int buzzer = 12;
 
Keypad keypad=Keypad(makeKeymap(keys),rowPin,colPin,4,3);
// MAPPING THE KEYPAD.


int total_attempts = 0; // Variable to determine the number of wrong attempts.

void setup()
{
  Serial.begin(9600);
  
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(buzzer, OUTPUT);
 
  lcd.begin(16, 2);
  delay(1000);
  lcd.clear();
}

void loop()
{
  if (dooropen == true)
  {
    char pressed = keypad.getKey();

    if (pressed == '#' || pressed == '*')

    {
      lcd.clear();
      setLocked(true);
      lcd.print("  Door Closed");
      delay(1000);
      dooropen = false;
    }
  }

  else Open(); 
}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password");

  char pressed = keypad.getKey();
  if (pressed)
  {
    data += pressed;
    lcd.setCursor(position, 1);
    lcd.print("*");
    position++;
  }

  if(data.length() == 4)
  {
    delay(400);
    Serial.print(data);
    lcd.clear();
    lcd.print("Verifying");
    int pos = 9;
    while(!Serial.available()){
      if(pos==16){
        lcd.setCursor(9,0);
        lcd.print("       ");
        pos = 9;
      }
      lcd.setCursor(pos++,0);
      lcd.print(".");
      delay(400);
    }
    String incomingString = Serial.readString();
    if(incomingString == "Correct")
    {
      lcd.clear();
      lcd.print(" Access Granted");
      setLocked(false);
      dooropen = true;
    }
    else if(incomingString == "Incorrect")
    {
      lcd.clear();
      lcd.print("   Incorrect");
      delay(1000);
      dooropen = false;
      total_attempts++;
      if(total_attempts==3)
      {
        total_attempts=0;
        buzzer_beep();
        delay(500);
      }
    }
    data = "";
    position = 6;
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
      delay(3000);
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
    }
  }
void buzzer_beep()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   WARNING !!");
  lcd.setCursor(0,1);
  lcd.print(" Access Denied");      
  for (int i=0;i<3;i++)
  {
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    delay(1000);
  }
}
