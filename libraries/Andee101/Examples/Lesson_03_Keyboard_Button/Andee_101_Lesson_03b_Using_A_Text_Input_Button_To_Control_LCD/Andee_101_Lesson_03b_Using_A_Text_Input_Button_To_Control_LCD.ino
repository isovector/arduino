
/************************************************************
  [ Andee - with Arduino/Genuino 101]
  ================
  Lesson 03b
  Using the Text Input Button to Control LCD Display

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Always include these libraries. Annikken Andee101 needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>
#include <LiquidCrystal.h>

// We'll need two textInput buttons to control each row of our
// 16x2 LCD Display. We'll also have two display boxes for feedback
Andee101Helper displayboxA;
Andee101Helper displayboxB;
Andee101Helper textInputButtonA;
Andee101Helper textInputButtonB;
char userInputA[17]; // Always declare (required characters + 1)
char userInputB[17];

/* Initialise LCD Display. Here's the pin connection that we're using:
   LCD RS pin to digital pin 8
   LCD Enable pin to digital pin 9
   LCD D4 pin to digital pin 4
   LCD D5 pin to digital pin 5
   LCD D6 pin to digital pin 6
   LCD D7 pin to digital pin 7 */
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup()
{
  Andee101.begin();  // Setup communication between Annikken Andee101 and Arduino

  // Tell Arduino how many rows and characters are available in the LCD Display
  lcd.begin(16, 2); // 16 characters by 2 rows
  lcd.clear(); // Clear screen

  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  displayboxA.setId(0);
  displayboxA.setType(DATA_OUT);
  displayboxA.setCoord(0, 0, 100, 25);
  displayboxA.setTitle("LCD Line 1");

  displayboxB.setId(1);
  displayboxB.setType(DATA_OUT);
  displayboxB.setCoord(0, 25, 100, 25);
  displayboxB.setTitle("LCD Line 2");

  textInputButtonA.setId(2);
  textInputButtonA.setType(KEYBOARD_IN); // Sets object as a text input button
  textInputButtonA.setCoord(0, 50, 100, 25);
  textInputButtonA.setTitle("Change Line 1");
  textInputButtonA.setInputMode(ALPHA_NUMERIC);
  textInputButtonA.setColor(THEME_DEFAULT_DARK);

  textInputButtonB.setId(3);
  textInputButtonB.setType(KEYBOARD_IN); // Sets object as a text input button
  textInputButtonB.setCoord(0, 75, 100, 25);
  textInputButtonB.setTitle("Change Line 2");
  textInputButtonB.setInputMode(ALPHA_NUMERIC);
  textInputButtonB.setColor(THEME_DEFAULT_DARK);
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  Andee101.poll();//required in every Andee101 sketch
  if (Andee101.isConnected() == true) {
    if ( textInputButtonA.isPressed() )
    {
      memset(userInputA, 0x00, 17); // Empty the contents of the string before receiving user input
      textInputButtonA.ack(); // Acknowledge button press or else phone will be left waiting
      textInputButtonA.getKeyboardMessage(userInputA); // Display keyboard and store input into userInput
      displayboxA.setData(userInputA); // Display user input on the display box

      // We'll need to clear the LCD display to get rid of stray characters from the
      // previous display
      lcd.clear(); // Clear screen
      // Print first row
      lcd.setCursor(0, 0); // Go to left-most character, top row
      lcd.print(userInputA);
      // Print second row
      lcd.setCursor(0, 1); // Go to left-most character, second row
      lcd.print(userInputB);
    }

    if ( textInputButtonB.isPressed() )
    {
      memset(userInputB, 0x00, 17); // Empty the contents of the string before receiving user input
      textInputButtonB.ack(); // Acknowledge button press or else phone will be left waiting
      textInputButtonB.getKeyboardMessage(userInputB); // Display keyboard and store input into userInput
      displayboxB.setData(userInputB); // Display user input on the display box

      lcd.clear(); // Clear screen
      lcd.setCursor(0, 0); // Go to left-most character, top row
      lcd.print(userInputA);
      lcd.setCursor(0, 1); // Go to left-most character, second row
      lcd.print(userInputB);
    }

    displayboxA.update(); // Always remember to update so that new content will be displayed
    displayboxB.update();
    textInputButtonA.update();
    textInputButtonB.update();

    delay(500); // Always leave a short delay for Bluetooth communication
  }
}



