#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//====================================================================================================================
LiquidCrystal_I2C lcd(0X27, 16, 2);
Servo myservo; // tao bien.
const byte ROWS = 4;
const byte COLS = 4;
char password[5];
const char correctPassword[] = "1234";
const int servoPin = 10;
const int Tx = 12;
const int Rx = 13;
const int buttonPin = 11;

int i = 0; 
int on = 0;
char mes = 'x'; // mes sent labView.
int ledState = LOW; // Trạng thái ban đầu của LED.
int lastButtonState = LOW; // Trạng thái của nút nhấn lần trước.
int lastButtonStateLabView = LOW; // Trạng thái của nút nhấn lần trước.
int controlData;

////////////////////
unsigned long startTime;
unsigned long currentTime;
int checkDoor = 1;
int maxAttemptsPassword = 2; // so lan nhap mat khau toi da.
int failedAttempts = 0; // so lan nhap sai mat khau.
byte isScreenLocked = 0; // bien dieu khien khoa man hinh.
unsigned long lockStartTime; // thoi gian bat dau khoa man hinh.

///////////////////
char MatrixKey[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //R1 R2 R3 R4
byte colPins[COLS] = {6, 7, 8, 9}; // C1, C2, C3, C4
Keypad Mykeys = Keypad(makeKeymap(MatrixKey), rowPins, colPins, ROWS, COLS);
// ============================================================================================================
int Message();
void displayEnterPassword();
void openDoor();
void closeDoor();
void hidePassword(int x); 
void lockScreen(); // khoa man hinh khi nhap qua so lan.
void  receiveControlSignal();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(Rx, INPUT);
  pinMode(Tx, OUTPUT);
  lcd.init(); // khoi tao man hinh
  lcd.backlight(); // bat den man hinh.

  myservo.attach(servoPin);
  myservo.write(0);

  lcd.print("Enter Password");
  startTime = millis();
}
// =================================================================================================
void loop() {
  currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  if(elapsedTime > 1000) {
    checkDoor = 1;
  }
  int controlSignal = digitalRead(Rx);
  //delay(500);
  if(controlSignal == 1 && checkDoor == 1) {
      ledState = !ledState; // Hoán đổi trạng thái của LED.
      myservo.write(ledState * 90);
      checkDoor = 0;
      startTime = millis();
  }
  //Serial.println(controlSignal);

  char EnterKey = Mykeys.getKey();
  int buttonState = digitalRead(buttonPin);
  delay(100);
   if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      ledState = !ledState; // Hoán đổi trạng thái của LED.
      myservo.write(ledState * 90);
      delay(200);
    }
    lastButtonState = buttonState;
  }

  if(EnterKey && ledState == LOW && isScreenLocked == 0) {
    password[i] = EnterKey;
    // Nhap mat khau.
    if(i==0) {
      hidePassword(0);
    }
    if(i==1) {
      hidePassword(1);
    }
    if(i==2) {
      hidePassword(2);
    }
    if(i==3) {
      hidePassword(3);
    }
    if(password[i] == '#') {
      i = -1;
      on = -1;
      lcd.clear();
      lcd.print("Enter Password");
    }
    i++;
    on++;
  }
  if(on == 4) {
    if(!strcmp(password, correctPassword)) {
      openDoor();
    }
    if(strcmp(password, correctPassword) && isScreenLocked == 0) {
      failedAttempts++;
      if (failedAttempts < maxAttemptsPassword) {
        // nhap sai mat khau in ra man hinh sai mat khau.
        closeDoor();
      } else {
        // khoa man hinh trong vong 5s.
        lockStartTime =  millis();
        isScreenLocked = 1;
        // In ra màn hình "Hãy thử lại sau".
        lcd.clear();
        lcd.print("Please try again");
      }
    }
  }
  if(isScreenLocked = 1 && on == 4) {
     // isScreenLocked = 0;
      unsigned long unlockTime = millis() - lockStartTime;
      Serial.println(unlockTime);
      lcd.setCursor(4, 1);
      lcd.print("after  s");
      lcd.setCursor(10, 1);
      lcd.print(5 - unlockTime / 1000);
      if(unlockTime >= 5000) {
        failedAttempts = 0;
        isScreenLocked = 0;
        displayEnterPassword();
      }
  }
  digitalWrite(Tx, ledState);
}

void displayEnterPassword() {
  lcd.clear();
  lcd.print("Enter Password");
  i = 0;
  on = 0;
}

void openDoor() {
  ledState = HIGH; // Hoán đổi trạng thái của LED.
  myservo.write(90);
  digitalWrite(13, ledState);
 // digitalWrite(Tx, ledState);
  displayEnterPassword();
}

void closeDoor() {
  lcd.clear();
  lcd.print("   Incorrect!");
  delay(500);
  lcd.clear();
  lcd.print("   Try Again!");
  delay(500);
  displayEnterPassword();
}

void hidePassword(int x) {
  lcd.setCursor(6 + x, 1);
  lcd.print(password[x]);
  delay(300);
  lcd.setCursor(6 + x, 1);
  lcd.print("*");
}

void  receiveControlSignal() {
  int receivedData = digitalRead(Rx);
  if(receivedData == 1) {
     ledState = !ledState; // Hoán đổi trạng thái của LED.
     myservo.write(ledState * 90);
  }
  controlData = ledState;
}