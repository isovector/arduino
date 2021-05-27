/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Vibration
  
  Making the smartdevice vibrate using the Annikken Andee
  
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
  // The First Data_Out Widget
  objectA.setId(0);           // Each object must have a unique ID number
  objectA.setType(DATA_OUT);  // This defines your object as a display box
  objectA.setCoord(0, 0, 100, 20);  
  objectA.setTitle("Vibration");
  objectA.setData("This sketch is to test vibration on phone");
  objectA.setTitleTextColor("FFFFFFFF"); 
  objectA.setTitleColor("50FFFFFF");
  objectA.setTextColor("FFFFFFFF");
  objectA.setColor("FFFF8000");  
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  
  if (Andee101.isConnected() == true)
  {
    objectA.update();// Call update() to refresh this UI on your screen

    Andee101.vibrate();//Use this function to tell your connected device to vibrate

    delay(2000);//delay for 2 seconds so taht the vibration on the device occurs every 2 seconds
  }
}



