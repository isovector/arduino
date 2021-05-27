/************************************************************
  [Annikken Andee 101]
  ================
  Lesson 01d
  Displaying Raw Analog Input Information

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee   
************************************************************/

// Works with any analog sensor. You can use a Light Dependent Resistor(LDR) to try out

// Always include these libraries. 
#include <CurieBLE.h>
#include <Andee101.h>

// 1 Widget will be created to display the analog input
Andee101Helper analogDisplay;

// We will use Analog Input Pin A0 to read our analog input.
// Change the pin number if you want to use another pin.
const int analogInputPin = A0;

void setup() 
{
  Andee101.begin();  // Setup the Arduino 101 to start broadcasting as an Annikken Andee101 peripheral   
  setInitialData(); // Define widget types and their appearance  
}

void setInitialData()
{
  // Only one display box this time
  analogDisplay.setId(0);  // Each widget must have a unique ID number
  analogDisplay.setType(DATA_OUT);  // This defines your widget as a display box
  analogDisplay.setCoord(0, 10, 100, 20); // Sets the location and size of your widget
  analogDisplay.setTitle("Analog Reading");
  analogDisplay.setData(""); // We'll update it with new analog data later.
}

void loop() 
{
  Andee101.poll();//required in every Andee101 sketch
  if(Andee101.isConnected() == true)//Use this to tell the Arduino what to do when the Annikken Andee101 app has connected to it
  {
    int reading = analogRead(analogInputPin); 
    analogDisplay.setData(reading); // Set the display box with new data value
    analogDisplay.update(); // Update the display to show the new value
    delay(100);//delay is needed or else Arduino 101 will be crash
  }

}
