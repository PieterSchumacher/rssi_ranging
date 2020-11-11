#include <SPI.h>
#include <RH_RF95.h>

#define SF 7
#define Freq 868.1

// Singleton instance of the radio driver
RH_RF95 rf95;

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
}

void loop()
{
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  uint8_t data[] = "Hello World!";
  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();
  delay(1000);
}
