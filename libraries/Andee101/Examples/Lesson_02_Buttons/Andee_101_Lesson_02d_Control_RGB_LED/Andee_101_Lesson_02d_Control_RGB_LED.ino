/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Lesson 02d
  Use Button to Control RGB LED

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/ 

// Always include these libraries. Annikken Andee101 needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>

// We'll need 6 buttons to increase/decrease the red, green
// and blue channels of our RGB LED
Andee101Helper displayCurrentColour;
Andee101Helper buttonRup; // Buttons to adjust Red
Andee101Helper buttonRdown;
Andee101Helper buttonGup; // Buttons to adjust Green
Andee101Helper buttonGdown;
Andee101Helper buttonBup; // Buttons to adjust Blue
Andee101Helper buttonBdown;

// To get various colours from the RGB LED, we'll need to connect 
// the LED to pins capable of analog output. On the Arduino Uno,
// pins capable of analog output are Pins 3, 5, 6, 9, 10.
// Do not use Pins 8, 11, 12, 13 (Arduino Uno layout) as Andee101 
// is using them. Strange things will happen if you do.
const int pinR = 6; // Red is connected to pin 6
const int pinG = 5; // Green is connected to pin 5
const int pinB = 3; // Blue is connected to pin 3

// We'll need to store the intensity levels of each channel
// Note: Analog output is only capable of producing a range from 0 to 255
int r = 0; // Red channel
int g = 0; // Green channel
int b = 0; // Blue channel

int resolution = 30; // Set the amount to increase/decrease for each button press
char colour[9] = "00000000"; // The initial colour of our display

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee101.begin();  // Setup communication between Annikken Andee101 and Arduino
  setInitialData(); // Define object types and their appearance
  pinMode(pinR, OUTPUT); // Set pins to output mode
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  displayCurrentColour.setId(0); // Our display box
  displayCurrentColour.setType(DATA_OUT);
  displayCurrentColour.setCoord(0,0,100,25);
  displayCurrentColour.setTitle("Current Colour");
  displayCurrentColour.setTitleColor(WHITE); // Set title to white
  displayCurrentColour.setTitleTextColor(DARK_GRAY); // Set title font to black
  displayCurrentColour.setColor(WHITE);
  displayCurrentColour.setTextColor(DARK_GRAY);
  displayCurrentColour.setData(colour); // Show the RGB colour code.

  buttonRup.setId(1); // Button to increase red level
  buttonRup.setType(BUTTON_IN);
  buttonRup.setCoord(0,25,33,20);
  buttonRup.setTitle("R +");
  buttonRup.setColor(RED); // Set button to red
  buttonRup.setTextColor(WHITE);
  buttonRup.setInputMode(NO_ACK); // You need this line to allow for multiple button presses

  buttonRdown.setId(2); // Button to decrease red level
  buttonRdown.setType(BUTTON_IN);
  buttonRdown.setCoord(0,50,33,20);
  buttonRdown.setTitle("R -");
  buttonRdown.setColor(RED); // Set button to red
  buttonRdown.setTextColor(WHITE);
  buttonRdown.setInputMode(NO_ACK); // You need this line to allow for multiple button presses

  buttonGup.setId(3); // Button to increase green level
  buttonGup.setType(BUTTON_IN);
  buttonGup.setCoord(33,25,34,20);
  buttonGup.setTitle("G +");
  buttonGup.setColor(GREEN); // Set button to green
  buttonGup.setTextColor(WHITE);
  buttonGup.setInputMode(NO_ACK); // You need this line to allow for multiple button presses

  buttonGdown.setId(4); // Button to decrease green level
  buttonGdown.setType(BUTTON_IN);
  buttonGdown.setCoord(33,50,34,20);
  buttonGdown.setTitle("G -");
  buttonGdown.setColor(GREEN); // Set button to green
  buttonGdown.setTextColor(WHITE);
  buttonGdown.setInputMode(NO_ACK); // You need this line to allow for multiple button presses

  buttonBup.setId(5); // Button to increase blue level
  buttonBup.setType(BUTTON_IN);
  buttonBup.setCoord(67,25,33,20);
  buttonBup.setTitle("B +");
  buttonBup.setColor(BLUE); // Set button to blue
  buttonBup.setTextColor(WHITE);
  buttonBup.setInputMode(NO_ACK); // You need this line to allow for multiple button presses

  buttonBdown.setId(6); // Button to decrease blue level
  buttonBdown.setType(BUTTON_IN);
  buttonBdown.setCoord(67,50,33,20);
  buttonBdown.setTitle("B -");
  buttonBdown.setColor(BLUE);  // Set button to blue 
  buttonBdown.setTextColor(WHITE);
  buttonBdown.setInputMode(NO_ACK); // You need this line to allow for multiple button presses
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{   
  Andee101.poll();//required in every Andee101 sketch
  if(Andee101.isConnected() == true)
  {
    Serial.println("start loop");
    // This is the most important chunk of code. Each analogWrite() statement sets the intensity
    // of each colour channel.
    analogWrite(pinR, r); 
    analogWrite(pinG, g); 
    analogWrite(pinB, b); 
  
    // Buttons used to adjust the colour channel intensity. We will employ
    // the press-and-hold button method to control the RGB LEDs
    if( buttonRup.isPressed() > 0 ) // Red button up will increase red intensity
    {      
      int k = buttonRup.pressCounter();//Get the number of presses on the button
      r = r+(resolution*k); // Increase the intensity by a fixed amount (specified above)
      if(r>255) r = 255; // 255 is the max value. If it goes beyond that, set the value to 255.  
    }  
    if( buttonRdown.isPressed() > 0 ) // Red button down will decrease red intensity
    {
      int k = buttonRdown.pressCounter();//Get the number of presses on the button
      r = r-resolution*k; // Decrease the intensity by a fixed amount (specified above)
      if(r<1) r = 0; // 0 is the min value. If it goes under that, set the value to 0. 
    }
    if( buttonGup.isPressed() > 0 ) // Green button up will increase green intensity
    {
      int k = buttonGup.pressCounter();
      g = g+resolution*k;
      if(g>255) g = 255;   
    }  
    if( buttonGdown.isPressed() > 0 ) // Green button down will decrease green intensity
    {
      int k = buttonGdown.pressCounter();
      g = g-resolution*k;
      if(g<1) g = 0;      
    }
    if( buttonBup.isPressed() > 0 ) // Blue button up will increase blue intensity
    {
      int k = buttonBup.pressCounter();
      b = b+resolution*k;
      if(b>255) b = 255;
    }
    if( buttonBdown.isPressed() > 0 ) // Blue button down will decrease blue intensity
    {
      int k = buttonBdown.pressCounter();
      b = b-resolution*k;
      if(b<1) b = 0;
    }  

    Serial.print("value R:");Serial.println(r);
    Serial.print("value G:");Serial.println(g);
    Serial.print("value B:");Serial.println(b);
    
    sprintf(colour, "FF%02X%02X%02X",r,g,b); // Convert to an ARGB colour code string
    
    displayCurrentColour.setData(colour); // Show user the ARGB colour code on the smartphone
    displayCurrentColour.setColor(colour);
    displayCurrentColour.setTitleColor(colour);
    displayCurrentColour.update(); // Update screen
    
    buttonRup.update();
    buttonRdown.update();
    buttonGup.update();
    buttonGdown.update();
    buttonBup.update();
    buttonBdown.update();   
    
    delay(500); // Always leave a short delay for Bluetooth communication
  }
}



