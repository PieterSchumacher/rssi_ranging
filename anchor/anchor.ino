#include <SPI.h>
#include <RH_RF95.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
unsigned long current_millis = millis();

TinyGPS gps;
SoftwareSerial ss(4, 3);

static void smartdelay(unsigned long ms);

struct Config {
  uint8_t sf;
  float freq;
  float flat;
  float flong;
};

Config con;

float flat = TinyGPS::GPS_INVALID_F_ANGLE;
float flong = TinyGPS::GPS_INVALID_F_ANGLE;
unsigned long age = 0;

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!rf95.init()) while(1);
  rf95.setFrequency(863.1);
  rf95.setSpreadingFactor(7);
  rf95.setCodingRate4(4);
  rf95.setTxPower(14);  
  while(latitude == TinyGPS::GPS_INVALID_F_ANGLE) {flat = 5.31; flong = 5.31;}//gps.f_get_position(&flat, &flong, &age);}
  Serial.print("flat:"); Serial.println(String(flat)); Serial.print("flong:"); Serial.println(String(flong));
}

void loop()
{
  if (rf95.available()) {
  uint8_t len = sizeof(con);
    if (rf95.recv((uint8_t*)&con, &len)) {
      rf95.setSpreadingFactor(con.sf);
      rf95.setFrequency(con.freq);
      uint8_t mess[1] = {1}; 
      rf95.send(mess, sizeof(mess));
      rf95.waitPacketSent();
      uint8_t message[1];
      Serial.write(con);
//      int sample_counter = 0;
      current_millis = millis();
//      unsigned long start_sampling = millis();
      while(millis() - current_millis < 1200) {
        if (rf95.recv(message, &len)) {
          Serial.write(rf95.lastRssi());
          Serial.write(rf95.lastSNR());
//          sample_counter += 1;
        }
      }
//      unsigned long end_sampling = millis();
//      Serial.print(String(sample_counter)); Serial.println("/100 samples were collected");
    }
    rf95.init();
    rf95.setFrequency(863.1);
    rf95.setSpreadingFactor(7);
    rf95.setCodingRate4(4);
    rf95.setTxPower(14);  
    delay(1);
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
