#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>\
#include <stdlib.h>
#include <string.h>

#define SF 7
#define FREQ 860.0

// Singleton instance of the radio driver
RH_RF95 rf95;
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t data[] = "success";
uint8_t token1[2] = "%";
uint8_t token2[2] = ";";
char *ptr;
double freq = 860.10;
int counter = 0;
unsigned long current_millis = millis();

int changeFreq(char* buf) {
  return !strcmp(buf + (strlen(buf)-strlen(token1)), token1);
}

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
  if (millis() - current_millis > 10000) {
    rf95.init();
    rf95.setFrequency(860.00);
    current_millis = millis();
    delay(2000);
  }
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf95.recv(buf, &len))
    {
      if (changeFreq(buf)) {
        freq = strtod(buf, &ptr);
        Serial.print("float value: ");
        Serial.println(freq);
        rf95.setFrequency(freq); 
        uint8_t message[] = "frequency changed";
        rf95.send(message, sizeof(message));
      } else {
        Serial.print("message content:");
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
      current_millis = millis();
    }
  }
}
