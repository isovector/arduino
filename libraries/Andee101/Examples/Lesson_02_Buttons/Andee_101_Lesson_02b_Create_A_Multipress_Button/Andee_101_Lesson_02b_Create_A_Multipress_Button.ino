/************************************************************
  [ Andee101 - with Arduino 101]
  ================
  Lesson 02b
  Creating a Multipress Button

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Sometimes, you may wish to press and hold a button to repeat
// a certain action instead or constantly pressing the button without delay.
// This lesson will teach you all you need to know to do this!

// To demonstrate this, we'll use the LED on the Arduino 101 and have the LED
// flash repeatedly as long as the button is constantly pressed

// Always include these libraries. Annikken Andee101 needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>

// We'll create a display and a button to show you how to
// program a button to do something
Andee101Helper displaybox;
Andee101Helper button;

const int pinLED = 13; // Set LED pin to output pin 13 to control the Arduino 101 LED
boolean LEDState = false;
boolean updateUI = false;
// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup()
{
  Andee101.begin();  // Setup communication between Annikken Andee101 and Arduino
  setInitialData(); // Define object types and their appearance

  pinMode(pinLED, OUTPUT); // Configures pin 13 for output.
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  // Let's draw a display box!
  displaybox.setId(0);  // Each object must have a unique ID number
  displaybox.setType(DATA_OUT);  // This defines your object as a display box
  displaybox.setCoord(0, 0, 100, 25);
  displaybox.setTitle("Press-and-Hold Button");
  displaybox.setData("Press and hold to make the LED blink continuously!");

  button.setId(1); // Don't forget to assign a unique ID number
  button.setType(BUTTON_IN); // Defines object as a button
  button.setCoord(0, 30, 100, 25);
  button.setTitle("Press me!");
  button.setInputMode(NO_ACK); // You need this line to allow for multiple button presses
  // You can't use setData() and setUnit() for buttons.
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  if (Andee101.isConnected() == true)
  {
    // Here's how you code a press-and-hold button:
    if ( button.isPressed() > 0 ) // Do this as long as the button is pressed
    {
      // Add your press and hold action here:
      LEDState = !LEDState ;
      digitalWrite(pinLED, LEDState );
    }
    if ( updateUI == true) {
      displaybox.update();
      button.update(); 
      delay(100);//delay is needed or else Arduino 101 will be crash
      updateUI = false;
    } 
  }
  else {
    updateUI = true;
  }
}



