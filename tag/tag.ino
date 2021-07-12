#include <SPI.h>
#include <RH_RF95.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define SF 7
#define default_freq 863.1

RH_RF95 rf95;
TinyGPS gps;
SoftwareSerial ss(4, 3);

float latitude = TinyGPS::GPS_INVALID_F_ANGLE;
float longitude = TinyGPS::GPS_INVALID_F_ANGLE;
unsigned long age = 0;
uint8_t message[1] = {1};
const float frequencies[7] = { 863.1 };
uint8_t recv_message[2];
float packet_id = 0.0;

static void smartdelay(unsigned long ms);

struct Config {
  float freq;
  float flat;
  float flong;
  uint8_t sf;
};

Config con;

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!rf95.init()) while(1);
  rf95.setFrequency(863.1);
  rf95.setSpreadingFactor(12);
  rf95.setCodingRate4(4);
  rf95.setTxPower(20);
  ss.begin(9600);
}

void loop()
{ Serial.println("start iteration");
  float start_loop = millis();
  for (uint8_t sf = 7; sf < 12; sf += 1) {
        rf95.setFrequency(863.1);
        rf95.setSpreadingFactor(12);
        while(latitude == TinyGPS::GPS_INVALID_F_ANGLE) {smartdelay(100);gps.f_get_position(&latitude, &longitude, &age);}
        con = {frequencies[0], latitude, longitude, sf};
        rf95.send((uint8_t*)&con, sizeof(con));
        rf95.waitPacketSent();
        smartdelay(200);
        rf95.send((uint8_t*)&con, sizeof(con));
        rf95.waitPacketSent();
        rf95.setSpreadingFactor(sf);
        rf95.setFrequency(frequencies[0]);
        smartdelay(2000);
        for (uint8_t i = 0; i < 100; i++) {
          message[0] = i+1;
          rf95.send(message, sizeof(message));
        }
        latitude = TinyGPS::GPS_INVALID_F_ANGLE; longitude = TinyGPS::GPS_INVALID_F_ANGLE;
        smartdelay(7000);
  }
  Serial.print("one iteration took "); Serial.print(String((millis() - start_loop)/1000.0)); Serial.println(" seconds");
  Serial.flush();
  smartdelay(60000);
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
