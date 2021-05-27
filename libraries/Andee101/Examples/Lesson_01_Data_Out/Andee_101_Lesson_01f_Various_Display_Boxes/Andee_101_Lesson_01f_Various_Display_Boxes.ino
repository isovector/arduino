/************************************************************
  [ Andee - with Arduino/Genuino 101]
  ================
  Lesson 01f
  Displaying the various Display Box Subtypes

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/
// Always include these libraries. Annikken Andee101 needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>

// Every object that appears on your smartphone's screen
// needs to be declared like this:
Andee101Helper display1;
Andee101Helper display2;
Andee101Helper display3;
// We're creating 3 objects


void setup() {
  Andee101.begin();// Setup communication between Annikken Andee101 and Arduino
  setInitialData();// Define object types and their appearance 
}

void setInitialData()
{
  display1.setId(0); // Each object must have a unique ID number
  display1.setType(DATA_OUT);// Defines object as a display box
  display1.setCoord(1,1,98,32); // Sets the location and size of your object
  display1.setTitle("Title Here");// Sets the title of the object
  display1.setData("Data Here");//Leave this empty since this will be filled up with data later on
  display1.setUnit("Units Here");
  display1.setColor(LIGHT_BLUE);//Sets the colour of the UI object

  display2.setId(1); // Each object must have a unique ID number
  display2.setType(DATA_OUT_CIRCLE);// Defines object as a circular display box
  display2.setCoord(1,34,98,32); // Sets the location and size of your object
  display2.setTitle("Title Here");// Sets the title of the object
  display2.setData("Data Here");//Leave this empty since this will be filled up with data later on
  display2.setUnit("Units Here");
  display2.setColor(LIGHT_BLUE);//Sets the colour of the UI object

  display3.setId(2); // Each object must have a unique ID number
  display3.setType(DATA_OUT_HEADER);// Defines object as a Title Header type
  display3.setCoord(1,67,98,32); // Sets the location and size of your object
  display3.setTitle("Title Here");// Sets the title of the object
  display3.setData("");//Since this is a HEADER subtype, Data and Units Field will not be used for HEADER subtype
  display3.setUnit("");
  display3.setColor(LIGHT_BLUE);//Sets the colour of the UI object
}

void loop() {
  Andee101.poll();//required in every Andee101 sketch
  // put your main code here, to run repeatedly:
  display1.update();
  display2.update();
  display3.update();
  delay(200);//delay is needed or else Arduino 101 will be crash
}
