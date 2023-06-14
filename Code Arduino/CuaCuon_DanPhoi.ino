
#include <Adafruit_MotorShield.h>
#include <SPI.h>
#include <MFRC522.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(3);          //động cơ cửa cuốn gara
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);          //động cơ dàn phơi

unsigned long curTimeCuaCuon = 0;

unsigned long stTimeCuaCuon = 0;

unsigned long curTimeDanPhoi = 0;

unsigned long stTimeDanPhoi = 0;

unsigned long curTimeCanhBaoMua = 0;

unsigned long stTimeCanhBaoMua = 0;
//labVIEW
byte TxCuaCuon = 3;
byte RxCuaCuon = 4;
byte TxDanPhoi = 5;
byte RxDanPhoi = 6;
byte CanhBaoMua = 2;

#define RST_PIN   9
#define SS_PIN    10
MFRC522 mfrc522(SS_PIN, RST_PIN);

byte rfid[4];

//setup cửa cuốn
int delay_cuaCuon = 1000;
byte trangThaiCuaCuon = 0;
void dong_cua_cuon();
void mo_cua_cuon();

//setup dàn phơi
int trangThaiDanPhoi = 0;
void thu_dan_phoi();
void mo_dan_phoi();

//labVIEW
byte checkCuaCuon = 0;
byte checkDanPhoi = 0;
byte checkCanhBaoMua = 0;
byte controlDanPhoi = 0;
byte controlCanhBaoMua = 0;
byte controlCuaCuon = 0;
byte enableReadCuaCuon = 0;
byte enableReadDanPhoi = 0;
byte enableReadCanhBaoMua = 0;
void setup() {
  pinMode(TxCuaCuon, OUTPUT);
  pinMode(RxCuaCuon, INPUT);
  pinMode(TxDanPhoi, OUTPUT);
  pinMode(RxDanPhoi, INPUT);
  pinMode(CanhBaoMua, INPUT);
  Serial.begin(9600);  // Initialize serial communications with the PC
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  AFMS.begin();
  stTimeCuaCuon = millis();
  stTimeDanPhoi = millis();
  stTimeCanhBaoMua = millis();
}

void loop() {
  digitalWrite(TxCuaCuon, trangThaiCuaCuon);
  digitalWrite(TxDanPhoi, trangThaiDanPhoi);
  curTimeCuaCuon = millis();
  curTimeDanPhoi = millis();
  curTimeCanhBaoMua = millis();
  unsigned long elapsedTime1 = curTimeCuaCuon - stTimeCuaCuon;
  unsigned long elapsedTime2 = curTimeDanPhoi - stTimeDanPhoi;
  unsigned long elapsedTime3 = curTimeCanhBaoMua - stTimeCanhBaoMua;
  if (elapsedTime1 > 500) {
    enableReadCuaCuon = 1;
  }
  if (elapsedTime2 > 500) {
    enableReadDanPhoi = 1;
  }
  if (elapsedTime2 > 500) {
    enableReadCanhBaoMua = 1;
  }
  checkCanhBaoMua = digitalRead(CanhBaoMua);
  checkCuaCuon = digitalRead(RxCuaCuon);
  checkDanPhoi = digitalRead(RxDanPhoi);
  if (checkCuaCuon == 1 && enableReadCuaCuon == 1) {
    controlCuaCuon = 1;
    enableReadCuaCuon = 0;
    stTimeCuaCuon = millis();
  } else {
    controlCuaCuon = 0;
  }
  if (checkDanPhoi == 1 && enableReadDanPhoi == 1) {
    controlDanPhoi = 1;
    enableReadDanPhoi = 0;
    stTimeDanPhoi = millis();
  } else {
    controlDanPhoi = 0;
  }
  if (checkCanhBaoMua == 1 && enableReadCanhBaoMua == 1) {
    enableReadCanhBaoMua = 0;
    controlCanhBaoMua = 1;
    stTimeCanhBaoMua = millis();
  } else {
    controlCanhBaoMua = 0;
  }

  //DÀN PHƠI
if (controlCanhBaoMua == 1) {
    thu_dan_phoi();
  } else {
    if (controlDanPhoi == 1) {
      if (trangThaiDanPhoi == 1) {
        thu_dan_phoi();
      }
      else {
        mo_dan_phoi();
      }

    }
  }

  //CỬA CUỐN
  if ( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ) {
    for (int i = 0 ; i < 4 ; i ++) {
      rfid[i] = mfrc522.uid.uidByte[i];
    }

    delay(100);
    if (rfid[0] == 147 && rfid[1] == 192 && rfid[2] == 165 && rfid[3] == 169) {
      if (trangThaiCuaCuon == 1) {
        dong_cua_cuon();
      } else {
        mo_cua_cuon();
      }
    }
  }
  if (controlCuaCuon == 1) {
    if (trangThaiCuaCuon == 1) {
      dong_cua_cuon();
    } else {
      mo_cua_cuon();
    }
  }



}

void dong_cua_cuon() {
  trangThaiCuaCuon = 0;
  myMotor2->run(BACKWARD);
  for (int i = 150 ; i > 100 ; i -- ) {
    myMotor2->setSpeed(i);
    delay(10);
  }
  myMotor2->run(RELEASE);
}

void mo_cua_cuon() {
  trangThaiCuaCuon = 1;

  myMotor2->run(FORWARD);
  for (int i = 150 ; i > 100 ; i -- ) {
    myMotor2->setSpeed(i);
    delay(10);
  }
  myMotor2->run(RELEASE);
}
void thu_dan_phoi() {
  trangThaiDanPhoi = 0;
  myMotor1->run(BACKWARD);
  for (int i = 155 ; i > 100 ; i -- ) {
    myMotor1->setSpeed(i);
    delay(25);
  }
  myMotor1->setSpeed(0);
}

void mo_dan_phoi() {
  trangThaiDanPhoi = 1;
  myMotor1->run(FORWARD);
  for (int i = 155 ; i > 100 ; i -- ) {
    myMotor1->setSpeed(i);
    delay(25);
  }
  myMotor1->setSpeed(0);
}



