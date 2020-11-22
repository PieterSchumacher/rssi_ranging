#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

#define SF 7
#define Freq 868.1

// Singleton instance of the radio driver
RH_RF95 rf95;
RHReliableDatagram manager(rf95, 2);

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!manager.init())
    Serial.println("init failed");
}

void loop()
{
  if (manager.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      Serial.print("dist:");
      Serial.println((char*)buf);
      Serial.print("RSSI:");
      Serial.println(rf95.lastRssi(), DEC);
      Serial.print("SNR:");
      Serial.println(rf95.lastSNR(), DEC);
      Serial.print("SF:");
      Serial.println(SF, DEC);
    }
  }
}
