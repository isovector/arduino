/************************************************************
  [ Andee101 - with Arduino/Genuino 101 ]
  ================
  Lesson 2e
  Using Buttons to Control Buzzer

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Always include these libraries. Annikken Andee 101 needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>

// We'll create two buttons. One to summon Darth Vader
// and the other to release an annoying beep
Andee101Helper btnPlayMelody;
Andee101Helper btnBeep;

const int pinBuzzer = 12; // Buzzer is connected to Pin 12

// Each item in this array contains the frequency of each note.
int melody[] = { 440, 440, 440, 349, 523, 440, 349, 523, 440, 659, 
                659, 659, 698, 523, 415, 349, 523, 440 };
                
// Duration is specified in ms
int duration[] = { 500, 500, 500, 350, 150, 500, 350, 150, 1000, 
                  500, 500, 500, 350, 150, 500, 350, 150, 1000 }; 
int numNotes = 18; // Number of notes in the array

int state; // Variable to check whether the state of the toggle button

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee101.begin();  // Setup communication between Annikken Andee101 and Arduino
  setInitialData(); // Define object types and their appearance
  
  pinMode(pinBuzzer, OUTPUT); // Set pin to output mode
  state = 0;
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  btnPlayMelody.setId(0);
  btnPlayMelody.setType(BUTTON_IN);
  btnPlayMelody.setCoord(0,0,100,25);
  btnPlayMelody.setTitle("Lord Vader... RISE!");
  
  btnBeep.setId(1);
  btnBeep.setType(BUTTON_IN);
  btnBeep.setCoord(0,30,100,25);
  btnBeep.setTitle("Annoy your friends");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  // You can use buttons to start or stop a buzzer.
  // We're using a single-press button to prevent accidental
  // multiple presses.
  Andee101.poll();//required in every Andee101 sketch
  if(Andee101.isConnected() == true)
  {
    if( btnBeep.isPressed() )
    {
      btnBeep.ack();
      
      if(state == 0) // If ON
      {
        tone(pinBuzzer, 440); // Play buzzer
        state = 1;
        btnBeep.setTitle("Stop annoying people!");
        btnBeep.update();
      }
      else // If OFF
      {
        noTone(pinBuzzer); // stop buzzer
        state = 0;
        btnBeep.setTitle("Annoy your friends");
        btnBeep.update();      
      }
    }
    
    // Or you can use buttons to initiate a melody
    if( btnPlayMelody.isPressed() )
    {
      btnPlayMelody.ack();
      btnPlayMelody.setTitle("Playing...");
      btnPlayMelody.update();
      
      for(int i = 0; i < numNotes; i++)
      {
        tone(pinBuzzer, melody[i]); // Play note
        delay(duration[i]); // Hold for a specified duration
        noTone(pinBuzzer); // Stop buzzer
        delay(20); // Short pause in between notes so that the notes can be clearly heard.
      }
      btnPlayMelody.setTitle("Lord Vader... RISE!");
      btnPlayMelody.update();
    }
    
    btnPlayMelody.update();
    btnBeep.update();  
    delay(500); // Always leave a short delay for Bluetooth communication
  }
}



