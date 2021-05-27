/************************************************************
  [ Andee - with Arduino 101]
  ================
  Lesson 02h
  Displaying the various Button Subtypes

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/
// Always include these libraries. Annikken Andee101 needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>

Andee101Helper displayBtn; 
Andee101Helper btn1;
Andee101Helper btn2;
Andee101Helper btn3;
Andee101Helper btn4;

bool UpdateFlag = false;

void setup() {
  Andee101.begin();// Setup communication between Annikken Andee101 and Arduino
  setInitialData();// Define object types and their appearance
}

void setInitialData()
{
  displayBtn.setId(1); // Each object must have a unique ID number
  displayBtn.setType(DATA_OUT);// Defines object as a display box
  displayBtn.setCoord(3,3,94,42); // Sets the location and size of your object
  displayBtn.setTitle("");// Sets the title of the object
  displayBtn.setData("");  
  displayBtn.setColor("FF00B2B2");//Sets the colour of the UI object
  displayBtn.setTitleColor("FF00D2D2");
  displayBtn.setTextColor(WHITE);
  displayBtn.setTitleTextColor(WHITE);

  btn1.setId(2);
  btn1.setType(BUTTON_IN);
  btn1.setCoord(2,48,48,24);
  btn1.setTitle("Press Me!");
  btn1.setColor("000000FF");

  btn2.setId(3);
  btn2.setType(BUTTON_IN);
  btn2.setCoord(51,48,48,24);
  btn2.setTitle("Press Me!");
  btn2.setColor(RED);
  
  btn3.setId(4);
  btn3.setType(CIRCLE_BUTTON);
  btn3.setCoord(3,74,48,24);
  btn3.setTitle("Press Me!");
  btn3.setColor("0000FF00");

  btn4.setId(5);
  btn4.setType(CIRCLE_BUTTON);
  btn4.setCoord(50,74,48,24);
  btn4.setTitle("Press Me!");
  btn4.setColor(ORANGE); 
}

void loop() {
  Andee101.poll();//required in every Andee101 sketch
  // put your main code here, to run repeatedly:
  if(Andee101.isConnected() == true)
  {
    if(UpdateFlag == false)
    {
      for(int i = 0; i < 2; i++)
      { 
        displayBtn.update();
        btn1.update();
        btn2.update();
        btn3.update();
        btn4.update();
        delay(200);//delay is needed or else Arduino 101 will be crash
      }      
      UpdateFlag = true;
    }
    if(btn1.isPressed())
    {
      btn1.ack();
      displayBtn.setTitle("This is a");
      displayBtn.setData("Rounded Rect with"); 
      displayBtn.setUnit("No Background Color");
      displayBtn.update();
      delay(100);
    }
    if(btn2.isPressed())
    {
      btn2.ack();
      displayBtn.setTitle("This is a");
      displayBtn.setData("Rounded Rect with");
      displayBtn.setUnit("Background Color");
      displayBtn.update();
      delay(100);
    }
    if(btn3.isPressed())
    {
      btn3.ack();
      displayBtn.setTitle("This is a");
      displayBtn.setData("Circle with");
      displayBtn.setUnit("No Background Color");
      displayBtn.update();
      delay(100);
    }
    if(btn4.isPressed())
    {
      btn4.ack();
      displayBtn.setTitle("This is a");
      displayBtn.setData("Circle with");
      displayBtn.setUnit("Background Color");
      displayBtn.update();
      delay(100);
    }    
  }
  else{
    UpdateFlag = false; 
  }
}
