/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================ 
  Text-to-Speech
  
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

// We're creating two objects

// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup()
{
  Andee101.setName("Andee101");
  Andee101.begin();  // Setup communication between  Andee and Arduino
  Andee101.clear();  // Clear the screen of any previous displays
  setInitialData();  // Define object types and their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  objectA.setId(0);  // Each object must have a unique ID number
  objectA.setType(DATA_OUT);  // This defines your object as a display box
  objectA.setCoord(2, 10, 96  , 20);
  objectA.setTitle("Text to Speech!");
  objectA.setData("Press the button below");
  objectA.setUnit("");
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
    delay(100);//delay is needed or else Arduino 101 will be crash
    
    if (objectB.isPressed()) {
      objectB.ack();
      // Normal speed, Normal Pitch (1.0f,1.0f)
      Andee101.textToSpeech("My heart is impressed, experienced", 1.0f, 1.0f, US );
      delay(2000);
      // High speed, Normal Pitch (1.2f,1.0f)
      Andee101.textToSpeech("broken and healed strong", 1.3f, 1.0f, US);
      delay(2000);
      // Low speed, Normal Pitch (0.8f,1.0f)
      Andee101.textToSpeech("fused with the world", 0.5f, 1.0f, US);
      delay(2000);
      // Normal speed, High Pitch (1.0f,1.2f)
      Andee101.textToSpeech("through the fire of passion", 1.0f, 1.3f, US);
      delay(2000);
      // Normal speed, Low Pitch (1.0f,0.8f)
      Andee101.textToSpeech("connected to my mind", 1.0f, 0.7f, US);
      delay(1000);
      // Normal speed, Normal Pitch (1.0f,1.0f)
      Andee101.textToSpeech("through my dreams", 1.0f, 1.0f , US);
    }
  }
}



