/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Clear Screen

  Use the Clear Screen command to everything on the Dashboard

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Always include these libraries.  Andee needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>

Andee101Helper ClearButton;
Andee101Helper objectA;
Andee101Helper objectB;
Andee101Helper objectC;
void setup()
{
  Andee101.setName("Andee101"); // Max 8 characters only
  Andee101.begin();  // Setup the Arduino 101 to start broadcasting as an Annikken Andee101 peripheral
  setInitialData();  // Define the UI objects and customise their appearance
}
 
void setInitialData()
{ 
  ClearButton.setId(0);
  ClearButton.setType(BUTTON_IN);
  ClearButton.setCoord(79, 79, 20, 20);
  ClearButton.setTitle("Clear Screen");
  ClearButton.setColor("FFFF8000");

  objectA.setId(1);
  objectA.setType(DATA_OUT);
  objectA.setCoord(2, 2, 96, 23);
  objectA.setTitle("Obj A");
  objectA.setData("");
  objectA.setUnit("");
  objectA.setTitleTextColor("FF53868B");
  objectA.setTitleColor("FF00C5CD");
  objectA.setTextColor("FF53868B");
  objectA.setColor("FF00F5FF");

  objectB.setId(2);
  objectB.setType(DATA_OUT);
  objectB.setCoord(2, 27, 96, 23);
  objectB.setTitle("Obj B");
  objectB.setData("");
  objectB.setUnit("");
  objectB.setTitleTextColor("FF53868B");
  objectB.setTitleColor("FF00C5CD");
  objectB.setTextColor("FF53868B");
  objectB.setColor("FF00F5FF");

  objectC.setId(3);
  objectC.setType(DATA_OUT);
  objectC.setCoord(2, 52, 96, 23);
  objectC.setTitle("Obj C");
  objectC.setData("");
  objectC.setUnit("");
  objectC.setTitleTextColor("FF53868B");
  objectC.setTitleColor("FF00C5CD");
  objectC.setTextColor("FF53868B");
  objectC.setColor("FF00F5FF");
}
 
void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  if (Andee101.isConnected() == true)
  {    
    objectA.update();
    objectB.update();
    objectC.update();
    ClearButton.update();
    delay(200);//delay is needed or else Arduino 101 will be crash
   
    if (ClearButton.isPressed()) {
      ClearButton.ack();
      Andee101.clear(); // Remove all the UI on the Screen
      delay(2000); // Wait for 3s before redrawing
    }
  }
}



