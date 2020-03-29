#include <SoftwareSerial.h>
#include <dht11.h>
SoftwareSerial s(D6, D5);
dht11 DHT11;
#define DHT11PIN 0
int myBPM;
int sensorValue;

#include <ESP8266WiFi.h>
String apiKey = "J1RE2A92X2BGR6C2";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Trikuldham";
char pass[] = "jaihanuman1981";
const char* server = "api.thingspeak.com";
WiFiClient client;
void setup() {
  pinMode( 3, INPUT);
  s.begin(9600);
  Serial.begin(9600);
}

void loop() {
  s.write("s");
  if (s.available() > 0)
  {
    myBPM = s.read();
    Serial.println(myBPM);

  }
  sensorValue = analogRead(0); // read analog input pin 0
  Serial.print("Air Quality: ");
  Serial.println(sensorValue, DEC); // prints the value read
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Temperature: ");
  Serial.println(DHT11.temperature);
  Serial.print("Humidity: ");
  Serial.println(DHT11.humidity);
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(myBPM/4);
    postStr += "&field2=";
    postStr += String(sensorValue);
    postStr += "\r\n\r\n";
    postStr += "&field3=";
    postStr += String(DHT11.temperature);
    postStr += "&field4=";
    postStr += String(DHT11.humidity);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(DHT11.temperature);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(DHT11.humidity);
    Serial.println("%. Send to Thingspeak.");
  }
  client.stop();

  Serial.println("Waiting...");

  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  
}
