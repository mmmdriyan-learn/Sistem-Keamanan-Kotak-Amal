#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#define passwordLength 5

boolean flag = false;
const int VibrationSensor = A0;
int VibrationSensorState = 0;
int buzzer = 12;
Servo ServoMotor;
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

char Pass[passwordLength];
char PassNew[passwordLength];
char Stored[passwordLength];
byte passCount = 0, storedCount = 0;
char key;
byte keyState = 0;
char lastPressedKey;
byte mode = 0;

long time_old = 0;
bool just_allowed_pass = 0;

char initialPassword[] = {'1', '2', '3', '4'}; //password standar


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  checkEEPROM();

  pinMode(VibrationSensor, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin();
  ServoMotor.attach(10); //Kunci Otomatis

}

void loop()
{
  VibrationSensorState = digitalRead(VibrationSensor);
  if (VibrationSensorState == HIGH) {
    if (mode != 3) {
      digitalWrite(buzzer, HIGH);      
      Serial.println("Getaran terdeteksi!");
    }else{
      digitalWrite(buzzer, LOW);
    }
  }
    else{
    digitalWrite(buzzer, LOW);
  }
  key = keypad.getKey();
  keyState = keypad.getState();

  if (key) {
    lastPressedKey = key;
    //Serial.println(key);
  }


  if ( mode == 3) {
    if (lastPressedKey == '#' && keyState == 2) {
      mode = 1; //mengunci pintu dengan cara tekan # dan tekan tombol di keyboard secara bersamaan
    }
    if (lastPressedKey == '*' && keyState == 2) {
      mode = 0;
      lcd.clear();
      lcd.setCursor(4, 0);
      //Timer Hitung Mundur
    //1

  lcd.setCursor(2,15);
  lcd.println(" ");
  lcd.setCursor(2,14);
  lcd.println(" ");
  lcd.setCursor(1,0);
  delay(200);
  lcd.println("will be locked:");

  lcd.setCursor(6,1);
  lcd.print("5");
  delay(200);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.println("will be locked:");
  delay(1000);
  //2
  lcd.setCursor(1,0);
  lcd.println("will be locked:");
  lcd.setCursor(6,1); //2
  lcd.print("4");
  delay(100);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.println("will be locked:");
  delay(1000);
  //3
  lcd.setCursor(1,0);
    lcd.println("will be locked:");
  lcd.setCursor(6,1); //3
  lcd.print("3");
  delay(100);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.println("will be locked:");
  delay(1000);
  //4
  lcd.setCursor(1,0);
  lcd.println("will be locked:");
  lcd.setCursor(6,1); //4
  lcd.print("2");
  delay(100);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.println("will be locked:");
  delay(1000);
  //5
  lcd.setCursor(6,1);
  lcd.print("1");
  delay(100);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.println("will be locked:");
  delay(1000);
    lcd.clear();
  lcd.setCursor(4,0);  
    lcd.print("LOCKED");
    delay(2000);
    ServoMotor.write(1); //derajat servo
    Serial.println("Kotak Amal Terkunci");
    }
  }

  if (mode == 0) {
    ServoMotor.write(11);
    lcd.setCursor(1, 0);
    lcd.print("Enter Password");

  } else if (mode == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Set New Password");
  } else if (mode == 2) {
    lcd.setCursor(0, 0);
    lcd.print("Password Again");
  } else if (mode == 3) {
    lcd.setCursor(4, 0);
    lcd.print("UNLOCKED");
  }


  if (key && key != '#' && mode != 3)
  {
    collectKey();
  }

  if (passCount == passwordLength - 1)
  {
    if (mode == 0) {
      lcd.clear();
      if (!strcmp(Pass, Stored)) {
        lcd.setCursor(2, 0);
        lcd.print("WELCOME BACK");
        just_allowed_pass = 1;
        time_old = millis();

        delay(2000);
        ServoMotor.write(90);
        Serial.println("Kotak Amal Terbuka");
        mode = 3;
      } else {
        digitalWrite(buzzer, HIGH);
        lcd.setCursor(2, 0);
        lcd.print("INCORRECT !");
        lcd.setCursor(4, 1);
        lcd.print("PASSWORD");
        digitalWrite(buzzer, HIGH);
        delay(2000);
        digitalWrite(buzzer, LOW);
      }
      delay(1000);
      lcd.clear();
      clearData();

      
    } else if ( mode == 1) {
      lcd.clear();
      mode = 2;
      for (int i = 0; i < passwordLength; i = i + 1) {
        PassNew[i] = Pass[i];
      }
      clearData();
    } else if (mode == 2) {
      if (!strcmp(Pass, PassNew)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("New Password is ");
        lcd.setCursor(4, 1);
        lcd.print(Pass);
        delay(2000);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Saving...");
        for (int i = 0; i <= 100; i =  i + 10) {
          lcd.setCursor(4, 1);
          lcd.print(i);
          lcd.setCursor(7, 1);
          lcd.print("%");
          delay(200);
        }
        EEPROM.put(0, Pass);
        EEPROM.get(0, Stored);
        delay(500);
      } else {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("PASSWORD");
        lcd.setCursor(3, 1);
        lcd.print("DIDN'T MATCH!");
        delay(2000);
      }
      mode = 3;
      clearData();
      lcd.clear();
    }
  }
}

void collectKey() {
  Pass[passCount] = key;
  lcd.setCursor(4 + passCount, 1);
  lcd.print("*");
  passCount++;
}

void clearData()
{
  while (passCount != 0)
  {
    Pass[passCount--] = 0;
  }
}


void checkEEPROM() {
  EEPROM.get(0, Stored);
  if (Stored[0] == 0 && Stored[1] == 0 && Stored[2] == 0 && Stored[3] == 0) {
    Serial.println("No EEPROM PASSWORD FOUND");
    EEPROM.put(0, initialPassword);
    EEPROM.get(0, Stored);
    delay(5);
  }
}
