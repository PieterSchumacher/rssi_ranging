#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

#define SF 7
#define FREQ 868.1

// Singleton instance of the radio driver
RH_RF95 rf95;
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t data[] = "success";


void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!rf95.init())
    Serial.println("init failed");
  rf95.setFrequency(FREQ);
}

void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf95.recv(buf, &len))
    {
      Serial.print("dist:");
      Serial.println((char*)buf);
      Serial.print("RSSI:");
      Serial.println(rf95.lastRssi(), DEC);
      Serial.print("SNR:");
      Serial.println(rf95.lastSNR(), DEC);
      Serial.print("SF:");
      Serial.println(SF, DEC);
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
    }
  }
}
