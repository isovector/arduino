/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Lesson 03a
  Creating A Text Input Button

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>


// Every object that appears on your smartphone's screen
// needs to be declared like this:
Andee101Helper objectA;
Andee101Helper objectB;
bool makeUI = false;
char text[64];

// We're creating two objects

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
  memset(text, 0x00, 32);
  objectA.setId(0);  // Each object must have a unique ID number
  objectA.setType(DATA_OUT);  // This defines your object as a display box
  objectA.setCoord(0, 10, 100, 20);
  /* setCoord (x-coord, y-coord, width, height)
     x-coord: From 0.0 (left-most) to 100.0 (right-most)
     y-coord: From 0.0 (top-most)  to 100.0 (bottom-most)
     width:   10.0 (10% of screen width) to 100.0 (100% of screen width)
     height:  10.0 (10% of screen height) to 100.0 (100% of screen height)
  */

  objectA.setTitle("Text Entered");
  objectA.setData(text);
  objectA.setUnit(" ");
  objectA.setTitleTextColor("FFFFFFFF");
  objectA.setTitleColor("50FFFFFF");
  objectA.setTextColor("FFFFFFFF");
  objectA.setColor("FFFF8000");

  //// Let's draw the Keyboard Button
  objectB.setId(1);  // Each object must have a unique ID number
  objectB.setType(KEYBOARD_IN);  // This defines your object as a Text Input Button
  objectB.setCoord(25, 40, 50, 50);
  objectB.setTitle("Enter a Msg");
  objectB.setColor("FF00FF7F");
}

// Arduino will run instructions here repeatedly until you power it off.

void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  if (Andee101.isConnected() == true)
  {     
    if (makeUI == true) {
      objectA.update(); // Call update() to refresh the display on your screen
      objectB.update(); // If you forgot to call update(), your object won't appear
      makeUI = false;
    }
    if (objectB.isPressed()) {         
      objectB.ack();
      objectB.getKeyboardMessage(text);
      Serial.println(text);
      objectA.setData(text);
       objectA.update(); 
    }
    delay(100);//delay is needed or else Arduino 101 will be crash
  }
  else {
    makeUI = true;
  }
}



