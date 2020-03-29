
/*  Getting_BPM_to_Monitor prints the BPM to the Serial Monitor, using the least lines of code and PulseSensor Library.
 *  Tutorial Webpage: https://pulsesensor.com/pages/getting-advanced
 *
--------Use This Sketch To------------------------------------------
1) Displays user's live and changing BPM, Beats Per Minute, in Arduino's native Serial Monitor.
2) Print: "♥  A HeartBeat Happened !" when a beat is detected, live.
2) Learn about using a PulseSensor Library "Object".
4) Blinks LED on PIN 13 with user's Heartbeat.
--------------------------------------------------------------------*/
#include <SoftwareSerial.h>
SoftwareSerial s(5,6);
SoftwareSerial mySerial(9, 10);

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 700;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
int myBPM1;    
char msg;
char call;                 
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


void setup() {   
mySerial.begin(9600);
  s.begin(9600);          // For Serial Monitor
  Serial.begin(9600);
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
//  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    s.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  delay(100);
  pinMode(4, INPUT_PULLUP);
}



void loop() {

 int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                        // Print phrase "BPM: " 
                   // considered best practice in a simple sketch.
if(s.available()>0)
{
 s.write(myBPM);
}

//Serial.println(myBPM);
int isButtonPressed = !digitalRead(4);
  if (isButtonPressed) {
    Serial.println("Msg");
    SendMessage();
    
  }

 if (mySerial.available()>0)
   Serial.write(mySerial.read());
}
void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918279726649\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("SOS https://www.google.com/maps/@13.1291249,77.5875112,17z");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


void MakeCall()
{
  mySerial.println("ATD+918279726649;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  delay(1000);
}
  
