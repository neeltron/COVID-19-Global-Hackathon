

#include <SoftwareSerial.h>
#include <dht11.h>
#include <FirebaseArduino.h>
SoftwareSerial s(D6, D5);
dht11 DHT11;
#define DHT11PIN 2

#define WIFI_SSID "MediSpect"
#define WIFI_PASSWORD "gvih4808"
#define FIREBASE_HOST "detectnow-227d2.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyDd93w_dMSgyYLvkAyisHsNYTft1KuxM98 "

int myBPM;
int sensorValue;
#define BLYNK_PRINT Serial
int breathCount;
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

WiFiServer server(80);
String header;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "7a30d5838c674934a17181cad68fe3ec";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MediSpect";
char pass[] = "gvih4808";
String apiKey = "2QMOS2V4UMNKJTZ2";
WiFiClient client;
const char *ssid1 =  "MediSpect";     // replace with your wifi ssid and wpa2 key
const char *pass1 =  "gvih4808";
const char* server1 = "api.thingspeak.com";
// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);
BlynkTimer timer;
// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
BLYNK_WRITE(V1)
{

  // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
  if (String("Marco") == param.asStr()) {
    terminal.println("You said: 'Marco'") ;
    terminal.println("I said: 'Polo'") ;
  } else {

    // Send it back
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }

  // Ensure everything is sent
  terminal.flush();
}
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.

}
void setup() {
  pinMode( 3, INPUT);
  s.begin(9600);
  Serial.begin(9600);
  // Debug console
  Serial.begin(9600);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println(F("Type 'Marco' and get a reply, or type"));
  terminal.println(F("anything else and get it printed back."));
  terminal.flush();
  timer.setInterval(1000L, myTimerEvent);
  WiFi.begin(ssid1, pass1);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid1, pass1);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  s.write("s");
  if (s.available() > 0)
  {
    myBPM = s.read();
    Serial.println(myBPM);
    terminal.print("BPM: ");
    terminal.println(myBPM / 4);
  }
  Firebase.pushString("", DHT11.temperature);
  Firebase.pushString("", DHT11.humidity); 
  Blynk.virtualWrite(V1, DHT11.temperature);
  sensorValue = analogRead(0); // read analog input pin 0
  Serial.print("Air Quality: ");
  Serial.println(sensorValue, DEC); // prints the value read
  Blynk.virtualWrite(V2, DHT11.humidity);
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Temperature: ");
  Serial.println(DHT11.temperature);
  Blynk.virtualWrite(V3, sensorValue);
  Serial.print("Humidity: ");
  Serial.println(DHT11.humidity);
  Blynk.virtualWrite(V4, myBPM / 2);


  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off


            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Smart Clothes for Security Forces</h1>");
            client.println("<iframe width=450 height=260 style=border: 1px solid #cccccc; src=https://thingspeak.com/channels/978008/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15></iframe>");
            client.println("<iframe width=450 height=260 style=border: 1px solid #cccccc; src=https://thingspeak.com/channels/978008/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15></iframe>");
            client.println("<iframe width=450 height=260 style=border: 1px solid #cccccc; src=https://thingspeak.com/channels/978008/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15></iframe>");
            client.println("<iframe width=450 height=260 style=border: 1px solid #cccccc; src=https://thingspeak.com/channels/978008/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15></iframe>");
            client.println("<iframe width=450 height=260 style=border: 1px solid #cccccc; src=https://thingspeak.com/channels/978008/widgets/144369></iframe>");
            client.println("<iframe width=450 height=260 style=border: 1px solid #cccccc; src=https://thingspeak.com/channels/978008/widgets/144370></iframe>");
            /*if (DHT11.temperature > 38) {
              client.println("<br>Fever Detected. High Possibility of Coronavirus if this is the result since last 7-14 days. Please visit your nearest hospital for further assistance.");
              }
              else if (sensorValue > 200) {
              client.println("<br>Respiratory Problems Detected. High Possibility of Coronavirus if this is the result since last 7-14 days. Please visit your nearest hospital for further assistance.");

              }
              else {
              client.println("<br>Very low chances of Coronavirus.");
              }*/
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }


  if (client.connect(server1, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(DHT11.temperature);
    postStr += "&field2=";
    postStr += String(DHT11.humidity);
    postStr += "&field3=";
    postStr += String(sensorValue);
    postStr += "&field4=";
    postStr += String(myBPM);

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

}
