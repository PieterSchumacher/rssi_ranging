#include <SPI.h>
#include <RH_RF95.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define SF 7
#define default_freq 863.1

RH_RF95 rf95;
TinyGPS gps;
SoftwareSerial ss(4, 3);

float latitude = 5.31;
float longitude = 5.31;
unsigned long age = 0;
const uint8_t message[1] = {1};

static void smartdelay(unsigned long ms);

struct Config {
  uint8_t sf;
  uint8_t cr;
  uint8_t p;
  float freq;
  float flat;
  float flong;
};

Config con;

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!rf95.init()) Serial.println("init failed");
  rf95.setFrequency(863.1);
  rf95.setSpreadingFactor(7);
  rf95.setCodingRate4(5);
  rf95.setTxPower(18);
  ss.begin(9600);
}

void loop()
{
  for (uint8_t sf = 7; sf < 13; sf += 1) {
    for (uint8_t cr = 5; cr < 9; cr += 1) {
      for (uint8_t p = 18; p < 19; p += 1) {
        for (float freq = 863.1; freq < 870; freq += 0.2) {
          float current_time = millis();
          rf95.setFrequency(863.1);
          rf95.setSpreadingFactor(7);
          rf95.setCodingRate4(5);
          rf95.setTxPower(18);
          while(latitude == TinyGPS::GPS_INVALID_F_ANGLE) {latitude = 5.31; longitude = 5.31;}//gps.f_get_position(&latitude, &longitude, &age);}
          con = {sf, cr, p, freq, latitude, longitude};
          rf95.send((uint8_t*)&con, sizeof(con));
          rf95.waitPacketSent();
          rf95.setSpreadingFactor(sf);
          rf95.setCodingRate4(cr);
          rf95.setFrequency(freq);
          rf95.setTxPower(p);
          smartdelay(100);
//          Serial.println("send samples");
          for (int i = 0; i < 10; i++) {
            rf95.send(message, sizeof(message));
            rf95.waitPacketSent();
          }
          latitude = TinyGPS::GPS_INVALID_F_ANGLE; longitude = TinyGPS::GPS_INVALID_F_ANGLE;
          smartdelay(2500);
          Serial.print("one iteration took "); Serial.print(String((millis() - current_time)/1000.0)); Serial.println(" seconds");
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
