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

union Data {
  uint8_t buf[sizeof(struct Config)];
  Config con;
} data;


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
{if (rf95.available()) {
  uint8_t len = sizeof(data.buf);

 if (rf95.recv(data.buf, &len)) {
    Serial.println("CONFIG PACKET: ");
    Serial.println(data.con.sf, DEC);
    Serial.println(data.con.cr, DEC);
    Serial.println(data.con.p, DEC);
    Serial.println(data.con.freq, 1);
    Serial.println(data.con.flat, 6);
    Serial.println(data.con.flong, 6);
//    rf95.init();
    rf95.setSpreadingFactor(data.con.sf);
    rf95.setCodingRate4(data.con.cr);
    rf95.setFrequency(data.con.freq);
    rf95.setTxPower(data.con.p);
    smartdelay(3000);

    current_millis = millis();
    
    uint8_t message[1]; 
    len = sizeof(message);
    while(millis() - current_millis < 10000) {
      if (rf95.recv(message, &len)) {
        current_millis = millis();
        lastRssi = rf95.lastRssi();
        lastSNR = rf95.lastSNR();
        Serial.println(message[0]);
      }
    }
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
//    rf95.init();
    rf95.setFrequency(863.1);
    rf95.setSpreadingFactor(7);
    rf95.setCodingRate4(5);
    rf95.setTxPower(18);   
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
