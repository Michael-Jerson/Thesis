#include <Keypad.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <TM1637Display.h>
#include <Wire.h>

#define CLK 10
#define DIO 11
#define buzzer 15

#define TEST_DELAY   1000
const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

const uint8_t SEG_STOP[] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  //0
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  //0
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  //0
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  //0
};
const uint8_t SEG_ON[] = {
  SEG_F,                                           //i
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   //O
  SEG_C | SEG_E | SEG_G,                           //n
  SEG_B,                                           //i
};
int cont = 3000;
TM1637Display display(CLK, DIO);
int c = 3000;

int in1 = 4;
int data;
int led = 13;
char password[4];
char pass[4],pass1[4];
int i=0;
char customKey=0;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.println("  Electronic Keypad lock ");
  delay(2000);
  Serial.println("  Enter your Passkey:");
  pinMode (in1, OUTPUT);
  digitalWrite(in1, HIGH);
  display.setBrightness(0xF);
  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display.setSegments(data);
  display.setSegments(SEG_ON);
  //for(int j=0;j<4;j++)
  //EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
  pass[j]=EEPROM.read(j);
}
  
void loop()
{
  customKey = customKeypad.getKey();
  if(customKey=='#')
  change();
  if (customKey)
  {
     password[i++]=customKey;
     Serial.print(customKey);
     beep();
    // Serial.print("|");
  }
  if(i==4)
  {
    delay(500);
    for(int j=0;j<4;j++)
    pass[j]=EEPROM.read(j);
    if(!(strncmp(password, pass,4)))
    {
      digitalWrite(led, LOW);
      beep();
      dooropen();
      Serial.println("  Passkey Accepted");
      delay(2000);
      Serial.print("n");
      delay(5000);
      Serial.println("  #.Change Passkey");
      delay(2000);
      Serial.println("  Enter Passkey:");
      i=0;
      digitalWrite(led, LOW);
    }
    
    else
    {
      digitalWrite(buzzer, HIGH);
      Serial.println("  Access Denied...");
      Serial.println("  #.Change Passkey");
      delay(2000);
      Serial.println("  Enter Passkey:");
      i=0;
      digitalWrite(buzzer, LOW);
    }
  }
}

void change()
{
  int j=0;
  Serial.println("  You're Current Passkey");
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      Serial.println(key);
      beep();
    }
    key=0;
  }
  delay(500);
  
  if((strncmp(pass1, pass, 4)))
  {
    Serial.println("  Wrong Passkey...");
    Serial.println("  Better Luck Again");
    delay(1000);
  }
  else
  {
    j=0;
    
  Serial.println("  Enter New Passk:");
  //Serial.print("|");
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass[j]=key;
      Serial.println(key);
      EEPROM.write(j,key);
      j++;
      beep();
    }
  }
  Serial.println("  Done......");
  delay(1000);
  }
  Serial.println("  Enter your Passk:");
  customKey=0;
}

void beep()
{
  digitalWrite(buzzer, HIGH);
  delay(20);
  digitalWrite(buzzer, LOW);
}
void dooropen()
{
  while (Serial.available()> 0)
  {
    int data = Serial.read();
    if (data == 'a') {
      Serial.println("On");
      digitalWrite(13, HIGH);
      digitalWrite(in1, LOW);
      segments();
    }
    else if (data == 'b') 
  { Serial.println("off");
      digitalWrite(13, LOW);
      display.setSegments(SEG_STOP);
      digitalWrite(in1, HIGH);
      timee();
      break;
    }
  }
}
void segments()
{

  while (c > 0) {
    uint8_t segto;
  
    display.showNumberDec(cont, true);
  
    delay(500);

    if (cont == 3100 || cont == 3000 || cont == 2900 || cont == 2800 || cont == 2700 || cont == 2600 || cont == 2500 || cont == 2400 || cont == 2300 || cont == 2200 || cont == 2100 || cont == 2000 || cont == 1900 || cont == 1800 || cont == 1700 || cont == 1600 || cont == 1500 || cont == 1400 || cont == 1300 || cont == 1200 || cont == 1100 || cont == 1000) {
      cont -= 40;
      loop();
    }
    loop();
    cont--;
    loop();
    segto = 0x80 | display.encodeDigit((cont / 100) % 10);
    loop();
    display.setSegments(&segto, 1, 1);
    loop();
    delay(500);
    loop();
    if (cont == 900 || cont == 800 || cont == 700 || cont == 600 || cont == 500 || cont == 400 || cont == 300 || cont == 200 || cont == 100) {
      loop();
      cont -= 41;
      loop();
    }
    c--;
    if (cont == 0) {
      display.setSegments(SEG_DONE);
      processes();

      break;
    }
  }

}
void processes()
{
  digitalWrite(in1, HIGH);
  /** if(Data == 'a')
    {

     digitalWrite(13,HIGH);
     Serial.println("Charging...");
     segments(true);

    }

    else if(Data == 'b')
    {
     digitalWrite(13,LOW);
     Serial.println("Turning off....Please Wait..");
     timee();

    }
    }
    }**/
  timee();
}
void timee()
{

  display.setSegments(SEG_STOP);

  do {


    timee();

  }
  while ( SEG_STOP);

}
void ilaw()
{
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}
