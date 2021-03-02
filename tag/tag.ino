#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define SF 7
#define default_freq 863.1

RH_RF95 rf95;
TinyGPS gps;
SoftwareSerial ss(4, 3);
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

float latitude = 5.31;
float longitude = 5.31;
unsigned long age = 0;

static void smartdelay(unsigned long ms);

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!rf95.init()) Serial.println("init failed");
  rf95.setFrequency(default_freq);
  ss.begin(9600);
}

void loop()
{
  for (uint8_t sf = 7; sf < 13; sf += 1) {
    for (uint8_t cr = 5; cr < 9; cr += 1) {
      for (float freq = 863.1; freq < 870; freq += 0.2) {
        for (uint8_t p = 2; p < 21; p += 1) {
          Serial.println("new config");
          rf95.init(); 
          rf95.setFrequency(default_freq);
          rf95.setSpreadingFactor(7);
          rf95.setCodingRate4(5);
          rf95.setTxPower(18);
          String b_freq = String(sf); b_freq += ";";
          b_freq += String(cr); b_freq += ";";
          b_freq += String(freq); b_freq += ";";
          b_freq += String(p); b_freq += ";";
          while(latitude == TinyGPS::GPS_INVALID_F_ANGLE) {smartdelay(1000); gps.f_get_position(&latitude, &longitude, &age);}
          
          b_freq += String(latitude,6); b_freq += String(","); b_freq += String(longitude,6);
          b_freq += String("%"); 
          b_freq.toCharArray(buf, sizeof(buf));
          Serial.println(b_freq);
          rf95.send(buf, sizeof(buf));
          rf95.waitPacketSent();
          rf95.init();
          rf95.setSpreadingFactor(sf);
          rf95.setCodingRate4(cr);
          rf95.setFrequency(freq);
          rf95.setTxPower(p);
          smartdelay(2000);
          for (int i = 0; i < 10; i++) {
            Serial.println("send new data packet");
            rf95.send(buf, sizeof(buf));
            rf95.waitPacketSent();
          }
          latitude = TinyGPS::GPS_INVALID_F_ANGLE; longitude = TinyGPS::GPS_INVALID_F_ANGLE; 
          smartdelay(10000);
        }
      }
    }
  }
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
