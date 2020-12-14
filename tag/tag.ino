#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define SF 7
#define Freq 868.1

RH_RF95 rf95;
TinyGPS gps;
SoftwareSerial ss(4, 3);
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

static void smartdelay(unsigned long ms);

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!rf95.init()) Serial.println("init failed");
  rf95.setFrequency(868.1);
  ss.begin(9600);
}

void loop()
{
  float flat = TinyGPS::GPS_INVALID_F_ANGLE; float flon = TinyGPS::GPS_INVALID_F_ANGLE; unsigned long age = 0;
  int nb_tries = 10;
  while(Serial.available() == 0){}
  while(flat == TinyGPS::GPS_INVALID_F_ANGLE) {smartdelay(3000); gps.f_get_position(&flat, &flon, &age); if (nb_tries>10) break;}
  if (nb_tries > 10) Serial.println("GPS READING FAILED"); else {
    Serial.println("GPS READING SUCCESS");
    for (int i = 0; i < 10; i++) {
      uint8_t data[sizeof(float)];
      String b = String(flat); b+=";";
      b += String(flon); b+=";";
      b += String(SF); b+=";";
      b += String(Freq); b+=";";
      Serial.println(b);
      b.toCharArray(data, sizeof(data));
      Serial.println(i);
      Serial.print("Sending ");
      Serial.println(b);
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      if (rf95.waitAvailableTimeout(3000)) { 
        if (rf95.recv(buf, &len)) {
          Serial.print("got reply: ");
          Serial.println((char*)buf);
        }
        else Serial.println("recv failed");
      } else Serial.println("No reply, is rf95_server running?");
    smartdelay(1000);
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
