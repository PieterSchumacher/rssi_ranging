#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>\
#include <stdlib.h>
#include <string.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
unsigned long current_millis = millis();
//char* token1 = "%";
//char* token2 = ";";
float lastRssi;
float lastSNR;
float freq;
uint8_t p;
uint8_t cr;
uint8_t sf;
float flat;
float flong;

int change_config(char* buf) {
  return !strcmp(buf + (strlen(buf)-strlen("%")), "%");
}

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!rf95.init())
    Serial.println("init failed");
}

void loop()
{
//  if (true) {
//    current_millis = millis();
//    Serial.print("RSSI:");
//    Serial.println(1.1111111, DEC);
//    Serial.print("SNR:");
//    Serial.println(1.1111111, DEC);
//    Serial.println(millis()-current_millis, DEC);
//  } else {
  rf95.init();
  rf95.setFrequency(863.1);
  rf95.setSpreadingFactor(7);
  rf95.setCodingRate4(5);
  rf95.setTxPower(18);
  current_millis = millis();
  Serial.println("default config");
  while(!rf95.available()) {}
  while(millis() - current_millis < 5000) {
    if (rf95.available()) {
      uint8_t len = sizeof(buf);
      if (rf95.recv(buf, &len)) {
        current_millis = millis();
        if (change_config(buf)) {
          Serial.println("hi!");
          sscanf(buf, "%u;%u;%f;%u;%f,%f%", &sf, &cr, &freq, &p, &flat, &flong);
          rf95.init();
          rf95.setSpreadingFactor(sf);
          rf95.setCodingRate4(cr);
          rf95.setFrequency(freq);
          rf95.setTxPower(p);
          Serial.println(sf, DEC);
          Serial.println(cr, DEC);
          Serial.println(freq, DEC);
          Serial.println(p, DEC);
          Serial.println(flat, DEC);
          Serial.println(flong, DEC);
        } else {
          lastRssi = rf95.lastRssi();
          lastSNR = rf95.lastSNR();
          Serial.print("RSSI:");
          Serial.println(rf95.lastRssi(), DEC);
          Serial.print("SNR:");
          Serial.println(rf95.lastSNR(), DEC);
          Serial.print("FREQ:");
          Serial.println(freq, DEC);
          Serial.print("TxPower:");
          Serial.println(p, DEC);
          Serial.print("CR:");
          Serial.println(cr, DEC);
          Serial.print("SF:");
          Serial.println(sf, DEC);
        }
      }
    }
  }
//  }
}
