#include <SPI.h>
#include <RH_RF95.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
unsigned long current_millis = millis();
float lastRssi;
float lastSNR;

TinyGPS gps;
SoftwareSerial ss(4, 3);

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
  if (!rf95.init())
  Serial.println("init failed");
  rf95.setFrequency(863.1);
  rf95.setSpreadingFactor(7);
  rf95.setCodingRate4(5);
  rf95.setTxPower(18);   
}

void loop()
{
  if (rf95.available()) {
  uint8_t len = sizeof(con);
    if (rf95.recv((uint8_t*)&con, &len)) {
      Serial.println("CONFIG PACKET: ");
      Serial.println(con.sf, DEC);
      Serial.println(con.cr, DEC);
      Serial.println(con.p, DEC);
      Serial.println(con.freq, 1);
      Serial.println(con.flat, 6);
      Serial.println(con.flong, 6);
      rf95.setSpreadingFactor(con.sf);
      rf95.setCodingRate4(con.cr);
      rf95.setFrequency(con.freq);
      rf95.setTxPower(con.p);
      uint8_t mess[1] = {1}; 
      rf95.send(mess, sizeof(mess));
      rf95.waitPacketSent();
      uint8_t message[1];
      int sample_counter = 0;
      current_millis = millis();
      unsigned long start_sampling = millis();
      while(millis() - current_millis < 2000) {
        if (rf95.recv(message, &len)) {
          current_millis = millis();
          lastRssi = rf95.lastRssi();
          lastSNR = rf95.lastSNR();
          sample_counter += 1;
        }
      }
      unsigned long end_sampling = millis();
      Serial.print(String(sample_counter)); Serial.println("/10 samples were collected");
  //        Serial.print("RSSI:");
  //        Serial.println(rf95.lastRssi(), 3);
  //        Serial.print("SNR:");
  //        Serial.println(rf95.lastSNR(), 3);
  //        Serial.print("FREQ:");
  //        Serial.println(freq, 3);
  //        Serial.print("TxPower:");
  //        Serial.println(p, DEC);
  //        Serial.print("CR:");
  //        Serial.println(cr, DEC);
  //        Serial.print("SF:");
  //        Serial.println(sf, DEC);   
    }
    if (!rf95.init()) Serial.println("init failed");
    rf95.setFrequency(863.1);
    rf95.setSpreadingFactor(7);
    rf95.setCodingRate4(5);
    rf95.setTxPower(18);  
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
