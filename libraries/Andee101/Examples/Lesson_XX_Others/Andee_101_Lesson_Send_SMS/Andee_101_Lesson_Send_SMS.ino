/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Send an SMS (please don't abuse this and prank your friend!!)
  [IMPORTANT]
  Only Works if the connected phone is on Android and contains a usable SIM card
  SMS sent via this method will cost you money (by the telco)
  since it is an SMS sent by you personally (Messaging, SMS etc)
  Please change the destination telephone number in the code below 

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>  

Andee101Helper objectA;
Andee101Helper objectB;
void setup()
{
  Andee101.begin();  // Setup communication between Andee 101 and Arduino 
  Andee101.setName("Andee101");
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{ 
  objectA.setId(0);  // Each object must have a unique ID number
  objectA.setType(DATA_OUT);  // This defines your object as a display box
  objectA.setCoord(0, 10, 100  , 20);  
  objectA.setTitle("Send an SMS to your friend!!");
  objectA.setUnit("Require telco connection, cost $$");
  objectA.setData("For Android only");
  objectA.setTitleTextColor("FFFFFFFF");
  objectA.setTitleColor("50FFFFFF");
  objectA.setTextColor("FFFFFFFF");
  objectA.setColor("FFFF8000");
  //// Let's draw the Button
  objectB.setId(1);  // Each object must have a unique ID number
  objectB.setType(BUTTON_IN);  // This defines your object as a BUTTON
  objectB.setCoord(25, 40, 50, 50);
  objectB.setTitle("Click me");
  objectB.setColor("FFFF8000"); 
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  if (Andee101.isConnected() == true) {
    objectA.update(); // Call update() to refresh the display on your screen
    objectB.update(); // If you forgot to call update(), your object won't appear
    delay(200);
    
    if (objectB.isPressed()) {
      objectB.ack();
      Andee101.sendSMS("12345678", "This msg is sent from Arduino 101!"); 
      // 12345678 is the destination telephone number
      // the 2nd Parameters is the message
    }  
  }
}

