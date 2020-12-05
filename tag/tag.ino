#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

#define SF 7
#define Freq 868.1

RH_RF95 rf95;
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!rf95.init())
    Serial.println("init failed");
    rf95.setFrequency(868.1);
}

void loop()
{
  while (Serial.available() == 0) {}
  uint8_t data[sizeof(float)];
  String b = Serial.readString();
  b.toCharArray(data, sizeof(data));
  for (int i = 0; i < 50; i++) {
    Serial.println(i);
    Serial.print("Sending ");
    Serial.print(b);
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();
    if (rf95.waitAvailableTimeout(3000)) { 
      if (rf95.recv(buf, &len)) {
        Serial.print("got reply: ");
        Serial.println((char*)buf);
      }
      else
        Serial.println("recv failed");
    }
  else
    Serial.println("No reply, is rf95_server running?");
   delay(1000);
  }
}
