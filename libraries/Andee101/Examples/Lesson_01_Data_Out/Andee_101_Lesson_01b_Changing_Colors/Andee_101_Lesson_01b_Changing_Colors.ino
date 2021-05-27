/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Lesson 01b
  Changing UI Color

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
  objectA.setId(0);           // Each object must have a unique ID number
  objectA.setType(DATA_OUT);  // This defines your object as a display box
  objectA.setCoord(25, 25, 50, 50);
  objectA.setTitle("Speed");
  objectA.setData("50");
  objectA.setUnit("m/s");
  objectA.setTitleTextColor("FFFFFFFF"); 
  objectA.setTitleColor("50FFFFFF"); // White color at 31% alpha
  objectA.setTextColor("FFFFFFFF");
  objectA.setColor("FFFF9912");
  /* Color is in AARRGGBB format
   * examples
   * WHITE: "FFFFFFFF"
   * BLACK: "FF000000"
   * RED:   "FFFF0000"
   * GREEN: "FF00FF00"
   * BLUE:  "FF0000FF"
   */

  
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  if (Andee101.isConnected() == true)
  {
    
    objectA.setColor("FFFF0000");
    objectA.update(); 
    delay(2000);
    
    objectA.setColor("FFFFFF00");
    objectA.update(); 
    delay(2000);
    
    objectA.setColor("FFFF00FF");
    objectA.update(); 
    delay(2000);
    
    objectA.setColor("FFFF9912");
    objectA.update(); 
    delay(2000);
    
    objectA.setColor("FF50FF50");
    objectA.update(); 
    delay(2000);
    
    objectA.setColor("FFFF5000");
    objectA.update(); 
    delay(2000);
  }
}



