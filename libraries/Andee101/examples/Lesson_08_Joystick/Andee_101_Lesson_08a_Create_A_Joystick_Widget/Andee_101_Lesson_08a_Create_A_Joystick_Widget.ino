/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Lesson 08a
  Creating a Joystick Widget

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
Andee101Helper titleDisplay;
Andee101Helper joystick;
// We're creating two objects

int counter = 0;
int xAxis,yAxis; //Need 2 INT numbers to store the joystick values

// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup()
{
  Andee101.setName("Andee101"); // Max 8 characters only
  Andee101.begin();  // Setup the Arduino 101 to start broadcasting as an Annikken Andee101 peripheral 
  setInitialData();  // Define the UI objects and customise their appearance
  Serial.begin(115200);  
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{ 
  
  // The First Data_Out Widget
  titleDisplay.setId(0);           // Each object must have a unique ID number
  titleDisplay.setType(DATA_OUT_HEADER);  // This defines your object as a display box
  titleDisplay.setCoord(0, 0, 100, 20);
  titleDisplay.setTitle("Joystick Example");
  titleDisplay.setTextColor(WHITE);
  titleDisplay.setColor(DARK_BLUE);

  // A joystick widget
  joystick.setId(1);            
  joystick.setType(JOYSTICK);  //This defines your object as a joystick widget
  joystick.setCoord(25, 30, 50, 50);
  joystick.setYAxisMax(100);
  joystick.setXAxisMax(100);
  joystick.setActiveColor(GREEN);// set the color of the "thumbstick"
  joystick.setBaseColor(RED);// set the color of the joystick base
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  if (Andee101.isConnected() == true)
  {
    joystick.getJoystick(&xAxis,&yAxis);// this function stores the value of the joystick 

    Serial.print("xAxis:");Serial.println(xAxis);
    Serial.print("yAxis:");Serial.println(yAxis);
    
    titleDisplay.update();
    joystick.update();
    delay(100);//delay is needed or else Arduino 101 will be crash
  }
}



