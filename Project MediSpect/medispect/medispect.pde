import processing.serial.*;

Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph
float inByte = 0;
String val;     // Data received from the serial port

Table table;
int hours;
int minutes;
int seconds;
int id;
void setup () {
  // set the window size:
  size(1920, 1080);
  table = new Table();
  table.addColumn("id");
  //table.addColumn("time");
  table.addColumn("heart rate");
  //table.addColumn("status");
  // List all the available serial ports
  // if using Processing 2.1 or later, use Serial.printArray()

  // I know that the first port in the serial list on my Mac is always my
  // Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 9600);

  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');

  // set initial background:
  background(0);
}

void draw () {
  // draw the line:
  stroke(0, 0, 255);
  fill(0);
  line(xPos, height, xPos, height - inByte);

  // at the edge of the screen, go back to the beginning:
  if (xPos >= width) {
    xPos = 0;
    background(0);
  } else {
    // increment the horizontal position:
    xPos++;
  }
  textSize(20);
  fill(100);
  /*if (xPos == 20) {
    text(inByte/6, 20, 30);
  }
  if (xPos == 100) {
    text(inByte/6, 100, 30);
  }
  if (xPos == 180) {
    text(inByte/6, 180, 30);
  }
  if (xPos == 260) {
    text(inByte/6, 260, 30);
  }
  if (xPos == 340) {
    text(inByte/6, 340, 30);
  }
  if (xPos == 420) {
    text(inByte/6, 420, 30);
  }
  if (xPos == 500) {
    text(inByte/6, 500, 30);
  }
  if (xPos == 580) {
    text(inByte/6, 580, 30);
  }
  if (xPos == 660) {
    text(inByte/6, 660, 30);
  }
  if (xPos == 740) {
    text(inByte/6, 740, 30);
  }
  if (xPos == 820) {
    text(inByte/6, 820, 30);
  }
  if (xPos == 900) {
    text(inByte/6, 900, 30);
  }
  if (xPos == 980) {
    text(inByte/6, 980, 30);
  }
  if (xPos == 1060) {
    text(inByte/6, 1060, 30);
  }
  if (xPos == 1140) {
    text(inByte/6, 1140, 30);
  }
  if (xPos == 1220) {
    text(inByte/6, 1220, 30);
  }
  if (xPos == 1300) {
    text(inByte/6, 1300, 30);
  }
  if (xPos == 1380) {
    text(inByte/6, 1380, 30);
  }
  if (xPos == 1460) {
    text(inByte/6, 1460, 30);
  }
  if (xPos == 1540) {
    text(inByte/6, 1540, 30);
  }
  if (xPos == 1620) {
    text(inByte/6, 1620, 30);
  }
  if (xPos == 1700) {
    text(inByte/6, 1700, 30);
  }
  if (xPos == 1780) {
    text(inByte/6, 1780, 30);
  }
  if (xPos == 1860) {
    text(inByte/6, 1860, 30);
  }
  if (xPos == 1940) {
    text(inByte/6, 1940, 30);
  }
  */
  if (id % 60 == 0) {
  //hours = hour();
  //minutes = minute();
  //seconds = second();
  TableRow newRow = table.addRow();
  newRow.setInt("id", table.lastRowIndex());
  //newRow.setString("time", str(hours)+":"+str(minutes)+":"+str(seconds));
  newRow.setInt("heart rate", int(inByte/6));
  //newRow.setString("status", "healthy");
  
  }
  id++;
  saveTable(table, "data/new1.csv");
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);
    // convert to an int and map to the screen height:
    inByte = float(inString);
    println(inByte);
    inByte = map(inByte, 0, 512, 0, height);
  }
}
