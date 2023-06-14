
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>
#include <DHT.h>
#include <MFRC522.h>
#include <math.h>

#include <LinxArduinoMega2560.h>
#include <LinxSerialListener.h>

#define DHT_TYPE DHT11
#define DHT11_PIN 4
DHT dht(DHT11_PIN, DHT_TYPE);


LinxArduinoMega2560* LinxDevice;



//cua cuon + dan phoi
const byte RxCuaCuon = 14;
const byte TxCuaCuon = 15;
const byte RxDanPhoi = 16;
const byte TxDanPhoi = 17;
const byte RxcanhBaoMua = 18;

// ====================
const byte RxCuaNgoai = 13;
const byte TxCuaNgoai = 12;
// =========================================
const byte coi = 3;
const byte bedroomLightPin = 8;
const byte livingRoomLightPin = 9;
const byte kitchenLightPin = 10   ;
// ========================================
const byte fanKitchen = 2;
int DHT11_RFID();
void setup() {
  pinMode(RxcanhBaoMua, INPUT);
  pinMode(TxCuaCuon, OUTPUT);
  pinMode(RxCuaCuon, INPUT);
  pinMode(TxDanPhoi, OUTPUT);
  pinMode(RxDanPhoi, INPUT);
  pinMode(fanKitchen, OUTPUT);
  pinMode(TxCuaNgoai, OUTPUT);
  pinMode(RxCuaNgoai, INPUT);
  pinMode(bedroomLightPin, OUTPUT);
  pinMode(livingRoomLightPin, OUTPUT);
  pinMode(kitchenLightPin, OUTPUT);
  LinxDevice = new LinxArduinoMega2560();
  dht.begin();
  LinxSerialConnection.Start(LinxDevice, 0);
  LinxSerialConnection.AttachCustomCommand(0, DHT11_DS18B20_RFID);

}

void loop() {
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();

  //Your Code Here, But It will Slow Down The Connection With LabVIEW
}

//Custom Command LINX - LabVIEW

int DHT11_RFID(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response) {
 
  if (input[1] == 1) {
    tone(coi, 1000, 1000);
  } else {
    noTone(coi);
  }
  digitalWrite(fanKitchen, input[0]);
 

  //Gui tin hieu dieu khien tu labVIEW den cac hẹ thong
  digitalWrite(TxCuaNgoai, input[2]);     
  digitalWrite(bedroomLightPin, input[3]); 
  digitalWrite(livingRoomLightPin, input[4]);
  digitalWrite(kitchenLightPin, input[5]);
  digitalWrite(TxCuaCuon, input[6]);
  digitalWrite(TxDanPhoi, input[7]);

  //gui tin hieu doc duoc len labVIEW
  float hum = dht.readHumidity();
  float tem = dht.readTemperature();
  byte roundHum = round(hum);
  byte roundTem = round(tem);
  response[0] = (unsigned char) roundHum;
  response[1] = (unsigned char) roundTem;
  response[2] = (unsigned char) digitalRead(RxCuaNgoai);
  response[3] = (unsigned char) digitalRead(RxCuaCuon);
  response[4] = (unsigned char) digitalRead(RxDanPhoi);
  response[5] = (unsigned char) digitalRead(RxcanhBaoMua);
  *numResponseBytes = 6;
  
  delay(80);
}
