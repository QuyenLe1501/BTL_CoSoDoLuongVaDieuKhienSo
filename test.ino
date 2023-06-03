//
//    FILE: dht11_test.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: DHT library test sketch for DHT11 && Arduino
//     URL: https://github.com/RobTillaart/DHTlib


#include <dht.h>
dht DHT;

#define DHT11_PIN     5


void setup()
{
  Serial.begin(9600);
}


void loop()
{
  // READ DATA
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("t1");
  Serial.println(DHT.humidity);
  Serial.print("t2");
  Serial.println(DHT.temperature);

  delay(1000);
}


// -- END OF FILE --

