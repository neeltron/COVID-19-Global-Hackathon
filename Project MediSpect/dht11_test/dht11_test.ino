#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 0
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  int chk = DHT11.read(DHT11PIN);

  Serial.println(DHT11.temperature);
  Serial.println(DHT11.humidity);
  
  delay(10);
}
