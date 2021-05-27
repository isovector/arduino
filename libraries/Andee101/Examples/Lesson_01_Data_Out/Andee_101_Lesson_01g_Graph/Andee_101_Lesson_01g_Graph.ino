/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Lesson 01g
  Display your values in a graph

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Always include these libraries.  Andee needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>

// Every object that appears on your smartphone's screen
// needs to be declared like this:
Andee101Helper objectA;
Andee101Helper objectB;
// We're creating two objects
float pi = 3.142;
int counter = 0;

// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup()
{
  Andee101.setName("Andee101"); // Max 8 characters only
  Andee101.begin();  // Setup the Arduino 101 to start broadcasting as an Annikken Andee101 peripheral
  setInitialData();  // Define the UI objects and customise their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{

  // The First Data_Out Widget
  objectA.setId(0);
  objectA.setType(DATA_OUT);
  objectA.setCoord(0, 0, 100, 20);
  objectA.setTitle("Instruction");
  objectA.setData("Long Press the UI Below");
  objectA.setUnit("to view Graph");
  objectA.setTitleTextColor("FFFFFFFF");
  objectA.setTitleColor("50FFFFFF");
  objectA.setTextColor("FFFFFFFF");
  objectA.setColor("FFFF8000");

  // This widget will plot a sin graph
  objectB.setId(1);
  objectB.setType(DATA_OUT);
  objectB.setCoord(35, 50, 30, 30);
  objectB.setTitle("Sin Graph");
  objectB.setData("0");
  objectB.setUnit("");
  objectB.setTitleTextColor("FF53868B");
  objectB.setTitleColor("FF00C5CD");
  objectB.setTextColor("FF53868B");
  objectB.setColor("FF00F5FF");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  if (Andee101.isConnected() == true)
  {
    objectA.update(); 
    float sinValue = sin(((float)counter / 20) * (2 * pi));
    objectB.setData(sinValue,2); // 2 decimal places

    counter++;
    if (counter == 20) {
      counter = 0;
    }
    objectB.update();
    delay(200);//delay is needed or else Arduino 101 will be crash
  }  
}



