/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Lesson 01e
  Displaying TMP36 Sensor

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/ 

/* TMP36 Wiring:

Vcc Pin - Connect it to both +3.3V source and AREF Pin of your Arduino
Vout Pin - Connect it to your analog input pin
GND Pin - Connect it to GND */

// Always include these libraries. 
#include <CurieBLE.h>
#include <Andee101.h>

// We'll create two display boxes to display temperature in different units
Andee101Helper tempDisplayC; // Temp in deg C
Andee101Helper tempDisplayF; // Temp in deg F

// We'll use Analog Input Pin A0 to read our analog input.
// Change the pin number if you are using another pin.
const int tempInputPin = A0;

// To store the temperature reading
float degC;
float degF;

void setup() {  
  Andee101.begin();  // Setup the Arduino 101 to start broadcasting as an Annikken Andee101 peripheral 
  setInitialData(); // Define widget types and their appearance
}

void setInitialData()
{ 
  tempDisplayC.setId(0);  // Each widget must have a unique ID number
  tempDisplayC.setType(DATA_OUT);  // This defines your widget as a display box
  tempDisplayC.setCoord(0, 0, 100, 20); // Sets the location and size of your widget
  tempDisplayC.setTitle("Temperature");
  tempDisplayC.setData(""); // We'll update it with new analog data later.
  tempDisplayC.setUnit("deg C");
  
  tempDisplayF.setId(1);  // Each widget must have a unique ID number
  tempDisplayF.setType(DATA_OUT);  // This defines your widget as a display box
  tempDisplayF.setCoord(0, 25, 100, 20); // Sets the location and size of your widget
  tempDisplayF.setTitle("Temperature");
  tempDisplayF.setData(""); // We'll update it with new analog data later.
  tempDisplayF.setUnit("deg F");  
}

void loop() {
  Andee101.poll();//required in every Andee101 sketch
  if(Andee101.isConnected() == true)//Use this to tell the Arduino what to do when the Annikken Andee101 app has connected to it
  {
    int reading = analogRead(tempInputPin); 
  
    degC = ( (reading * 3.3 / 1024.0) - 0.5 ) * 100; // Multiplying by 3.3V 
    degF = ( degC * 9.0 / 5.0 ) + 32.0;
    
    
    // Update displays with new readings
    tempDisplayC.setData(degC); 
    tempDisplayF.setData(degF); 
    tempDisplayC.update(); 
    tempDisplayF.update(); 
    delay(200);//delay is needed or else Arduino 101 will be crash
  }
}
