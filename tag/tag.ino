#include <SPI.h>
#include <RH_RF95.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define SF 7
#define Freq 868.1

RH_RF95 rf95;
TinyGPS gps;
SoftwareSerial ss(4,3);

static void smartdelay(unsigned long ms);

uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);
bool newData;
String datastring="";
String datastring1="";
char gps_lon[20]={"\0"};  
char gps_lat[20]={"\0"}; 

void setup() 
{
  Serial.begin(9600);
  ss.begin(9600);
  while (!Serial);
  if (!rf95.init()) Serial.println("init failed");
  rf95.setFrequency(868.1);
      ss.print("Simple TinyGPS library v. "); ss.println(TinyGPS::library_version());

}

void loop()
{
  uint8_t data[sizeof(float)];
  for (unsigned long start = millis(); millis() - start < 1000;) while (Serial.available()) newData = gps.encode(Serial.read());
  if (newData) {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    // Once the GPS fixed,send the data to the server.
    datastring +=dtostrf(flat, 0, 6, gps_lat); 
    datastring1 +=dtostrf(flon, 0, 6, gps_lon);
    ss.println(strcat(strcat(gps_lon,","),gps_lat));
    strcpy(gps_lat,gps_lon);
    ss.println(gps_lat); //Print gps_lon and gps_lat
    strcpy((char *)data,gps_lat); 
  }
//  while (Serial.available() == 0) {}
//  String b = Serial.readString();
//  b.toCharArray(data, sizeof(data));
  for (int i = 0; i < 50; i++) {
    Serial.println(i);
    Serial.print("Sending ");
//    Serial.print(b);
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();
    if (rf95.waitAvailableTimeout(3000)) { 
      if (rf95.recv(buf, &len)) {
        Serial.print("got reply: ");
        Serial.println((char*)buf);
      }
      else Serial.println("recv failed");
    }
    else Serial.println("No reply, is rf95_server running?");
    smartdelay(1000);
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
