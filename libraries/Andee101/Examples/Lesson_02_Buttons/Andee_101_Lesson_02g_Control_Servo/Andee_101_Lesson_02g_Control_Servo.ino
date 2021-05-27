/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================
  Lesson 02g
  Control Servo with Button

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Always include these libraries. Annikken Andee101 needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>
#include <Servo.h>

// We'll create 3 buttons. One to make it slowly turn left, the other
// to make it slowly turn right. And the third one to quickly turn to
// a user-specified position. We'll also create one display box to tell
// us about the servo's current position
Andee101Helper btnTurnLeft;
Andee101Helper btnTurnRight;
Andee101Helper btnCustomPosition;
Andee101Helper displaybox;

Servo theServo; // Declare a servo

char userPos[4];
int currentPosition; // Stores current position of servo (from 0 to 180)
int newPosition;
int turnResolution = 10; // Servo turns by 20 every time left/right turn button is pressed
const int servoPin = 2; // Servo connected to pin 2

// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup()
{
  Andee101.begin();  // Setup communication between Annikken Andee101 and Arduino
  currentPosition = 0; // Initialise position to 0
  theServo.attach(servoPin); // Tell Arduino which pin the servo is connected to
  theServo.write(currentPosition); // Set servo to position 0
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  btnTurnLeft.setId(0);
  btnTurnLeft.setType(BUTTON_IN);
  btnTurnLeft.setCoord(0, 0, 50, 25);
  btnTurnLeft.setTitle("Turn Left");
  btnTurnLeft.setInputMode(NO_ACK); // You need this line to allow for multiple button presses

  btnTurnRight.setId(1);
  btnTurnRight.setType(BUTTON_IN);
  btnTurnRight.setCoord(50, 0, 50, 25);
  btnTurnRight.setTitle("Turn Right");
  btnTurnRight.setInputMode(NO_ACK); // You need this line to allow for multiple button presses

  btnCustomPosition.setId(2);
  btnCustomPosition.setType(KEYBOARD_IN); // Sets object as a text input button
  btnCustomPosition.setCoord(0, 25, 100, 25);
  btnCustomPosition.setTitle("Quickly Go to Custom Position (0 - 180)");
  btnCustomPosition.setInputMode(NUMERIC);

  displaybox.setId(3);
  displaybox.setType(DATA_OUT); // Sets object as a text input button
  displaybox.setCoord(0, 50, 100, 25);
  displaybox.setTitle("Current Position");
  displaybox.setData(0);
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  if (Andee101.isConnected() == true) {
    if ( btnCustomPosition.isPressed() > 0 )
    {
      memset(userPos, 0x00, 4); // Empty the contents of the string before receiving user input
      btnCustomPosition.ack(); // Acknowledge button press or else phone will be left waiting
      btnCustomPosition.getKeyboardMessage(userPos); // Display keyboard and store input into userInput

      newPosition = atoi(userPos); // Convert string value to integer value

      // Tell Arduino x Andee101 what to do if user keys in ridiculous values
      if (newPosition < 0) newPosition = 0;
      if (newPosition > 180) newPosition = 180;

      currentPosition = newPosition;
      // This is how you do a quick turn
      theServo.write(currentPosition); // Turn servo to new position
      displaybox.setData(currentPosition); // Update new position
    }

    int btnLeft = btnTurnLeft.isPressed();//Get button press count
    if ( btnLeft > 0 ) // As long as left button is pressed
    {
      newPosition = currentPosition - turnResolution * btnLeft; // Set new position
      if (newPosition < 0) newPosition = 0; // Set to 0 if new position goes below 0

      // This is how you do a slow turn:
      for (currentPosition; currentPosition > newPosition; currentPosition--)
      {
        theServo.write(currentPosition);
        delay(15); // You can change the delay value. Larger value means slower turns
        // Do not set your delay to 0 when you're doing this. You run the possibility of
        // damaging the servo. Or you might just make it become more cranky.
        displaybox.setData(currentPosition); // Update servo position on screen as it turns
        displaybox.update();
        
      }
    }

    int btnRight = btnTurnRight.isPressed();//Get button press count
    if ( btnRight > 0 ) // As long as right button is pressed
    {
      newPosition = currentPosition + turnResolution * btnRight; // Set new position
      if (newPosition > 180) newPosition = 180; // Set to 180 if new position goes above 180
      for (currentPosition; currentPosition < newPosition; currentPosition++)
      {
        theServo.write(currentPosition);
        delay(15); // You can change the delay value. Larger value means slower turns
        // Do not set your delay to 0 when you're doing this. You run the possibility of
        // damaging the servo. Or you might just make it become more cranky.
        displaybox.setData(currentPosition); // Update servo position on screen as it turns
        displaybox.update();
      }
    }
    delay(200);
    btnTurnLeft.update(); // Always remember to update so that new content will be displayed
    btnTurnRight.update();
    btnCustomPosition.update();
    displaybox.update();
    Serial.println("loop end");
    delay(200); // Always leave a short delay for Bluetooth communication
  }
}



