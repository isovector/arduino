// Andee101.h - Arduino 101 Library
// Annikken Pte Ltd
// Author: Muhammad Hasif

//The Annikken Andee101 is an app to enable users to create their own dynamic dashboard on their smartphones and tablets without any Objective C, Swift or Java programming. All they need is the Annikken Andee101 App, an Arduino 101 and this library. All functions will be well documented. For any questions regarding the Annikken Andee101 or even the Annikken Andee, feel free to email us at andee@annikken.com.

extern BLEService Andee101Service;
extern BLECharacteristic Andee101Write;
extern BLECharacteristic Andee101Read;
//These functions are to set the Service and Characteristic UUID for the Arduino 101. The Annikken Andee101 app will only respond to the UUID used in the Cpp file. You can change the UUID if you intend to write your own app.


extern bool AndeeConnected;

extern bool versionAndClear;

extern bool resetBLEFlag;

void printDEC(char*);
//This function prints out buffers into the Serial Terminal with a Decimal format. THis helps users to easily verify if the data they send is correct.


void printHEX(char*);
//This function prints out buffers into the Serial Terminal with a Hexadecimal format. THis helps users to easily verify if the data they send is correct.


void sendToPhone(char*);
//This function transfers data from the Arduino 101 to the connected BLE device. Since the Arduino 101 can only send data in 18 byte packets, this function automatically cuts the data up into 18 byte packets and send them one after the other to the target. 


void convertColor(const char*,char*);
//This function take the values of HEX 00 to FF and shrinks and maps them out to DEC 32 to 232. This is done reduce the amount of bytes used up by colors. This helps speed up data transfer by reducing data sent. However accuracy is sacrificed.


void systemTime(void);
//This function will send a 'T' to the smartphone. This will trigger the app to send back the system time expressed in milliseconds since the Epoch.
//The bytes sent are ASTART,'T',AEND

void blePeripheralConnectHandler(BLEDevice);

void blePeripheralDisconnectHandler(BLEDevice);

void readBLEBuffer(BLEDevice,BLECharacteristic);
//This function deals with replies from the smartphone or tablet. 
//The data being sent from the smartphone to the Arduino 101 will be similar to the data sent from the Arduin0 101 to the smartphone. The data will be broken into 18 byte packets and sent to the Arduino 101. 
//This function contains a logic to piece the packets together to form the data. This part should not be changed, unless there are more efficient ways to carry out the data combination. Once the data has been pieced together, the data can now be parsed.

//The replies will start with an ASTART byte,which is defined in the definitions section. 
//The next byte will be the TYPE byte, which will be used to recognise the function being used. The list of definitions for the TYPE is also listed below. The rest of the bytes depend on which function is being used. 
//The most important part of the packet is the AEND byte, which is located at the end of the packet. By identifying the last byte, we can sift out any other noise and parse the data that is sent to the Arduino 101.
//Some types, like buttons, will share the same buffers.
//The P_SEP byte acts as a comma in between data for some types

void processReply();
//This function processes the replies from the smart device

class Andee101Class
{
	public:
	
	void versionClear(void);
	
	bool isConnected(void);
	
	void resetBLE(void);
	 
	void begin(void);
	//This is needed to start the BLE services
	
	void broadcast(void);
	//This is needed to start the advertisement of the BLE on the Arduino 101
	
	void poll(void);
	//
	
	void setName(const char*);
	//This function allows you to change the BLE name. MAX 8 characters
	
	void clear(void);
	//This function clears the dashboard on the screen of the smartphone/tablet.
	//The bytes sent are ASTART,'K' and AEND		
	
	void getDeviceTime(int*,int*,int*);
	//This function converts the time from systemTime in millis to a HH:MM:SS 24 hour clock
	
	void getDeviceDate(int*,int*,int*);	
	//This function converts the time from systemTime in millis to a DD/MM/YYYY format date
	
	long getDeviceTimeStamp(void);
	//This function simply returns the time from systemTime in millis if this is the preferred data
	
	void sendVersion(void);
	//This function is needed to send the version type to the app
	//The bytes sent are ASTART,'V', the string Andee101Version,AEND
	
    void disconnect(void);
	//This forces the smartphone/tablet to dsconnect its BLE
	//The bytes sent are ASTART, DC, AEND
	
	void textToSpeech(const char*,float,float,char);
	//This function activates the Text to Speech(TTS) function on the smartphone/tablet. You can place accents, speed and pitch of speech and also the text to be spoken.
	//accents available are US, GREAT_BRITON, AUSTRALIA, IRELAND, SOUTH_AFRICA 
	//speed is a value from 0.0 to 2.0
	//pitch is a value from 0.0 to 2.00
	//speech is your string that you want to be vocalised
	//The bytes sent are ASTART, TTS, P_SEP, speech, P_SEP, speed, P_SEP, pitch, P_SEP, accent, AEND	
	
	void takePhoto(char, char, char);
	//This function uses the smartphone/tablet camera to take pictures.
	//cameraType is either FRONT, REAR or CAM_DEFAULT
	//autoFocus AUTO, ON or OFF
	//flash AUTO, ON or OFF
	//The bytes sent are ASTART, CAMERA, P_SEP, cameraType, P_SEP, autoFocus, P_SEP, flash, AEND
	
	void gyroInit(int,int);
	//This function activates the gyro on the smartphone/tablet. The device will send gyro data based on the number of iterations and interval
	//interval(time between each sending of gyro data) any integer
	//iteration(how many data to be sent) any integer. Use -1 if you want to send unlimited amount of data
	//You will need to call the function getGyroReading to get the data
	//The bytes sent are ASTART, GYRO, '0', P_SEP, interval, P_SEP, iteration, AEND
	
	void gyroStop();
	//Stop all gyro activity
	//The bytes sent are ASTART, GYRO, '1', AEND
	
	void getGyroReading(float*,float*,float*);
	//Call this function when using gyroInit. The 3 values will be stored to the user's own declared variables
	
	void lacInit(int,int);
	//This function activates the LAC on the smartphone/tablet. The device will send LAC data based on the number of iterations and interval
	//interval(time between each sending of LAC data) any integer
	//iteration(how many data to be sent) any integer. Use -1 if you want to send unlimited amount of data
	//You will need to call the function getLacReading to get the data
	//The bytes sent are ASTART, LAC, '0', P_SEP, interval, P_SEP, iteration, AEND
	
	void lacStop(); 
	//Stop all LAC activity
	//The bytes sent are ASTART, LAC, '1', AEND
	
	void getLacReading (float*,float*,float*);
	//Call this function when using lacInit. The 3 values will be stored to the user's own declared variables
	
	void gravInit(int,int);
	//This function activates the grav on the smartphone/tablet. The device will send grav data based on the number of iterations and interval
	//interval(time between each sending of grav data) any integer
	//iteration(how many data to be sent) any integer. Use -1 if you want to send unlimited amount of data
	//You will need to call the function getGravReading to get the data
	//The bytes sent are ASTART, GYRO, '0', P_SEP, interval, P_SEP, iteration, AEND
	
	void gravStop();
	//Stop all gyro activity
	//The bytes sent are ASTART, GRAV, '1', AEND
	
	void getGravReading(float*,float*,float*);
	//Call this function when using gravInit. The 3 values will be stored to the user's own declared variables
		
	void gpsInit(int,int);
	//This function activates the GPS on the smartphone/tablet. The device will send GPS data based on the number of iterations and interval
	//interval(time between each sending of GPS data) any integer
	//iteration(how many data to be sent) any integer. Use -1 if you want to send unlimited amount of data
	//You will need to call the function getGpsReading to get the data
	//The bytes sent are ASTART, GPS, '0', P_SEP, interval, P_SEP, iteration, AEND
	
	void gpsStop();
	//Stop all gyro activity
	//The bytes sent are ASTART, GPS, '1', AEND
	
	void getGpsReading(float*,float*,float*);	
	//Call this function when using gpsInit. The 3 values will be stored to the user's own declared variables
	
	void changeScreen(int);
	//Call this function to change the Apple Watch screen to a particular screen
	
	//Apple Watch Functionality//
	void showScreen();
	//Call this to show a certain screen
	
	void hideScreen();
	//Call this to hide a certain screen
	
	void textInput();
	//Call this to use the voice to text function of the Apple Watch
	
	void sendSMS(const char*,const char*);
	//This function will make the app send an SMS to the receipient with a message
	
	void vibrate();
	//This function will cause the smartdevice to vibrate for roughly 1 second
	
	
} ;
extern Andee101Class Andee101;
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////

 class Andee101Helper
{
	private:	
	
	//This class contains buffers that stores the characteristics of each widgets. Each widget will have its own buffers.
	
	public:
	unsigned char id;
	//Each widget will eb assigned an id. This is for identification
	//unsigned char UIUpdateFlag;
	char bleBuffer[256];
	//The main data buffer that is sent to the smartphone/tablet. All widget chracteristics  will be written to this buffer before immediately sending the data. This buffer is usually updated in Andee101Helper::update()
			
	char data1Buffer[2];
	char dataBuffer[64];
	//buffer for data in a widget
	
	char titleBuffer[64];
	//buffer for title in a widget
	
	char unitBuffer[64];
	//buffer for unit in a widget
	
	char xywhBuffer[13];
	//buffer for coordinates in a widget. This determines x and y position and size in width and height
	
	char titleBGBuffer[5];
	//buffer for widget title bar background color
	
	char titleFontBuffer[5];
	//buffer for widget title font color
	
	char bodyBGBuffer[5];
	//buffer for widget background color
	
	char bodyFontBuffer[5];
	//buffer for widget font color
	
	char minBuffer[10];
	//buffer for min value. Only used in some widgets
	
	char maxBuffer[10];
	//buffer for max value. Only used in some widgets	
	
	char inputTypeBuffer;
	//buffer to store input mode of widgets. Used to change between input modes that some widgets have
	
	char subBuffer;
	//buffer to store various data. Also commonly used to store subtype of widgets.
	
	char flashBuffer;
	//buffer to store various data. Commonly used to store value for flash in the camera function
	
	char watchBuffer;
	//buffer to store watch data	
	
	char tempBuffer[12];
		
	void setId(int);
	//This function sets the id for the widget. Also initialises some parts of the widget to a default layout
	
	void setType(char);
	/*
	This function sets the type of widget. The available types for the Andee101 will be DATA_OUT, DATA_OUT_CIRCLE, DATA_OUT_HEADER, 
	BUTTON_IN, CIRCLE_BUTTON,
	ANALOG_DIAL_OUT, 
	KEYBOARD_IN, DATE_IN, TIME_IN, 
	SLIDER_IN, TEXTBOX, TTS, JOYSTICK	
	*/
	void setLocation(char, char, char);
	void setCoord(unsigned int, unsigned int, unsigned int, unsigned int);//(x,y,w,h) respectively
	//This function sets the position and size of the widget. x and y are the x coordinates and y corrdinates respectively. w and h is the width and height of the widget respectively. 
	
	void setInputMode(char);
	//This function is to set an input behaviour, depnding on the widget.
	//For Buttons: ACK for an ackowledged button press 
	//             NO_ACK for a multi press button
	/*For Sliders: 
				   ON_FINGER_UP for a slider that updates the value on lifting the finger from the screen
				   ON_VALUE_CHANGE for a slider that updates the value when the slider is moved
				   NO_FINGER for a slider that works similar to s progress bar, with no interaction needed from the user 
	  For KEYBOARD_IN, TIME_IN and DATE_IN:
					ALPHA_NUMERIC for AlphaNumeric Keyboard
					ALPHA_NUMERIC_PW for AlphaNumeric keyboard that hides the typed character after a few seconds
					NUMERIC for a Numeric Keyboard with symbols
					NUMERIC_PW for a Numeric Keyboard that hides typed characters after a few seconds
	*/
		
	void setSubType(char);
	//*****DEPRECATED. THIS FUNCTION WILL BE USED INTERNALLY. USERS NO LONGER NEED TO DEFINE ARGUMENT OF THIS FUNCTION.****//
	
	
	void setColor(const char*);
	void setColor(char*);
	//This function is to store the background color into the appropriate buffer
	
	void setTitleColor(const char*);
	void setTitleColor(char*);
	//This function is to store the title background color into the appropriate buffer
	
	void setTitleTextColor(const char*);
	void setTitleTextColor(char*);
	//This function is to store the title font color into the appropriate buffer
	
	void setTextColor(const char*);
	void setTextColor(char*);
	//This function is to store the font color into the appropriate buffer
	
	void setData(const char*);
	//This function stores the string into the dataBuffer
	void setData(int);	
	//This function has the same function as the above function but handles an integer instead
	void setData(float, char);
	//This function has the same function as the above function but handles a float instead
	void setData(double, char);
	//This function has the same function as the above function but handles a double instead
	
	void setTitle(const char*);
	//This function stores the string into the titleBuffer
	void setTitle(int);	
	//This function has the same function as the above function but handles an integer instead
	void setTitle(float, char);
	//This function has the same function as the above function but handles a float instead
	void setTitle(double, char);
	//This function has the same function as the above function but handles a double instead
	
	void setUnit(const char*);
	//This function stores the string into the unitBuffer
	void setUnit(int);	
	//This function has the same function as the above function but handles an integer instead
	void setUnit(float, char);
	//This function has the same function as the above function but handles a float instead
	void setUnit(double, char);
	//This function has the same function as the above function but handles a double instead
	
	void setMinMax(int,int);
	//This function stores the min and max integer values into the minBuffer and maxBuffer respectively
	void setMinMax(float,float,char);
	//This function works in a similar way as the above function but handles a float instead
	void setMinMax(double,double,char);
	//This function works in a similar way as the above function but handles a double instead
		
	//void setKeyboardType(char);
	void getKeyboardMessage(char*);
	//This function returns the sentence or phrase typesd into the smartphone/tablet in the KYBOARD_IN funtion
	
	void setDefaultDate(int,int,int);
	//When using the type DATE_IN, you can set the default date on the date picker on the smartphone/tablet
	
	void getDateInput(int*,int*,int*);
	//When using the type DATE_IN, this function can retrieve the date input from the smartphone/tablet
	
	void setDefaultTime(int,int,int);
	//When using the type TIME_IN, this function can set the default date on the smartphone/tablet
	
	void getTimeInput(int*,int*,int*);
	//When using the type TIME_IN, this function can retrieve the time input from the smartphone/tablet
		
	void setSliderInitialValue(int);
	//This function sets the initial value the slider will be at. Value is an integer
	void setSliderInitialValue(float, char);	
	//This function works in a similar way as the above function but handles a float instead
	void setSliderInitialValue(double,char);
	//This function works in a similar way as the above function but handles a double instead
	
	void setSliderNumIntervals(int);
	//This functions sets the number of intervals the slider has	
	
	void getSliderValue(int*);
	//This function returns the value the slider is at for integers
	void getSliderValue(float*);	
	//This function works in a similar way as the above function but handles a float instead
	void getSliderValue(double*);
	//This function works in a similar way as the above function but handles a double instead

	void getJoystick(int*,int*);
	//This function saves the value of the hoystick in x and y where x is the x coordinate and y is the y coordinate
	
	int isPressed(void);
	//This function is used to check if a particular button is pressed. This function returns a true if a button is pressed and a false if no button is pressed
	
	void ack(void);
	//This function returns an acknowledgement to the smartphone/tablet that it has received the button press. This is to allow the button widget to refresh
	
	int pressCounter(void);
	//This function serves as a counter for the number of times a button has been pressed. This function returns an integer
		
	void update(void);
	//This function is very important. If this function is not called, the widget will not appear. This function does the combining of all the widget attributes e.g.title, data, unit, colors into 1 data buffer. This buffer will then be sent to the smartphone/tablet. Each type will have a different data configuration. Look at the sprintf functions of each type to see what attributes are being used for a widget
    
	void remove();	
	//This function is used to remove the widget. This is useful in situations where only 1 or more widgets needs to be changed. No arguments needed.
	//Data sent is ASTART, 'D', P_SEP, id, AEND
} ;

///////////////////////////////
/////////Definitions///////////
///////////////////////////////

#define C_HLIMIT 100
#define C_LLIMIT 0

const char SET_COLOR = '0';
const char SET_TEXT_COLOR = '1';
const char SET_TITLE_COLOR = '2';
const char SET_TITLE_TEXT_COLOR = '3';

const char SEPARATOR = 251;//0xFB;
const char START_TAG_VERSION = 123;//0x7B
const char END_TAG_VERSION = 125;//0x7D
const char START_TAG_COMMAND = 10;//0x0A
const char END_TAG_COMMAND = 11;//0x0B
const char START_TAG_UIXYWH = 4;//0x04
const char END_TAG_UIXYWH = 5;//0x05
const char END_TAG_REPLY = 93;//0x5D


const char DC= 149;

const char CLEAR = 'L';//
const char TIMEEPOCH = 'T';//

const char CAMERA = 'M';//
const char TTS = 'P';//
const char VIBRATE = 'I';//

const char SMS = 'Z';//
const char NOTIFICATION = 'N';//

const char GYRO = 'O';//
const char LAC = 'F';//
const char GRAV = 'Y';//
const char GPS = 'S';//

const char DATA_OUT = 'C';//
const char DATA_OUT_CIRCLE = 'G';//
const char DATA_OUT_HEADER = 'H';//

const char BUTTON_IN = 'B';//
const char CIRCLE_BUTTON = 'J';//
const char ACKN = 'A';//

const char KEYBOARD_IN = 'K';//
const char DATE_IN = 'D';//
const char TIME_IN = 'X';//

const char SLIDER_IN = 'Q';//

const char ANALOG_DIAL_OUT = 'R';//

const char JOYSTICK = 'U';//

const char WATCH = 'W';//

const char REMOVE = 'E';//

///////////////////Button Type Macros//////////////////////
//TIME_IN, DATE_IN and KEYBOARD_IN are buttons as well so these macros work for them as well
#define ACK '0'
#define NOACK NO_ACK
#define NO_ACK '1'

/*************************************************************
 **************      LOCATION MACROS      ********************
 ************************************************************/ 
#define ONE_THIRD 1
#define TWO_THIRD 2
#define FULL 3
#define ONE_QUART 4
#define HALF 5
#define THREE_QUART 6

///////////Keyboard Definitions////////////////

#define ALPHA_NUMERIC '0'
#define ALPHA_NUMERIC_PW '1'
#define NUMERIC '2'
#define NUMERIC_PW '3'

/////////////////////Slider Macros//////////////////////

#define setActiveColor setTitleColor///Used for AnalogDial as well
#define setBaseColor setColor///Used for AnalogDial as well

#define setSliderMinMax setMinMax
#define setSliderReportMode setInputMode
#define moveSliderToValue setSliderInitialValue

#define INT 0
#define FLOAT (float) 0.0
#define NO_FINGER '2'
#define ON_VALUE_CHANGE '1'
#define ON_FINGER_UP '0'
#define ON '1'
#define OFF '0'
#define MAXSLIDER 10

/////////////////////Joystick Macros//////////////////////

#define setXAxisMax setUnit
#define setYAxisMax setData

/////////////////////////Text to Speech Macros//////////////////////

#define US '0'
#define GREAT_BRITON '1'
#define AUSTRALIA '2'
#define IRELAND '3'
#define SOUTH_AFRICA '4'

///////////////////////////Camera Macros/////////////////////////////////////

#define AUTO '0'
#define CAM_DEFAULT '0'
#define FRONT '1'
#define REAR '2'
#define On '1'
#define Off '2'

////////////////////////////////////Apple Watch Macros////////////////////////////////////

#define setWatchColor setTitleColor
#define setWatchTextColor setTitleTextColor

#define WATCH_TEXT 76
#define WATCH_TITLE 77
#define WATCH_BUTTON1 78
#define WATCH_BUTTON2 79
#define WATCH_BUTTON3 80
#define WATCH_BUTTON4 81

#define WATCH_1BUTTON_A	0
#define WATCH_2BUTTON_A	1
#define WATCH_3BUTTON_A	2
#define WATCH_4BUTTON_A	3
#define WATCH_1BUTTON_B	4
#define WATCH_2BUTTON_B	5
#define WATCH_3BUTTON_B	6
#define WATCH_4BUTTON_B	7
#define WATCH_GAMEPAD	8

//////////SET COLOURS//////////
#define PINK "FFFFC0CB"
#define LIGHT_PINK "FFFFB6C1"
#define HOT_PINK "FFFF69B4"
#define DEEP_PINK "FFFF1493"
#define PALE_VIOLET_RED "FFDB7093"
#define MEDIUM_VIOLET_RED "FFC71585"
#define LIGHT_SALMON "FFFFA07A"
#define SALMON "FFFA8072"
#define DARK_SALMON "FFE9967A"
#define LIGHT_CORAL "FFF08080"
#define INDIAN_RED "FFCD5C5C"
#define CRIMSON "FFDC143C"
#define FIRE_BRICK "FFB22222"
#define DARK_RED "FF8B0000"
#define RED "FFFF0000"
#define ORANGE_RED "FFFF4500"
#define TOMATO "FFFF6347"
#define CORAL "FFFF7F50"
#define DARK_ORANGE "FFFF8C00"
#define ORANGE "FFFFA500"
#define GOLD "FFFFD700"
#define YELLOW "FFFFFF00"
#define LIGHT_YELLOW "FFFFFFE0"
#define LEMON_CHIFFON "FFFFFACD"
#define LIGHT_GOLDENROD_YELLOW "FFFAFAD2"
#define PAPAYA_WHIP "FFFFEFD5"
#define MOCCASIN "FFFFE4B5"
#define PEACH_PUFF "FFFFDAB9"
#define PALE_GOLDENROD "FFEEE8AA"
#define KHAKI "FFF0E68C"
#define DARK_KHAKI "FFBDB76B"
#define CORNSILK "FFFFF8DC"
#define BLANCHED_ALMOND "FFFFEBCD"
#define BISQUE "FFFFE4C4"
#define NAVAJO_WHITE "FFFFDEAD"
#define WHEAT "FFF5DEB3"
#define BURLYWOOD "FFDEB887"
#define TAN "FFD2B48C"
#define ROSY_BROWN "FFBC8F8F"
#define SANDY_BROWN "FFF4A460"
#define GOLDENROD "FFDAA520"
#define DARK_GOLDENROD "FFDAA520"
#define PERU "FFCD853F"
#define CHOCOLATE "FFD2691E"
#define SADDLE_BROWN "FF8B4513"
#define SIENNA "FFA0522D"
#define BROWN "FFA52A2A"
#define MAROON "FF800000"
#define DARK_OLIVE_GREEN "FF556B2F"
#define OLIVE "FF808000"
#define OLIVE_DRAB "FF6B8E23"
#define YELLOW_GREEN "FF9ACD32"
#define LIME_GREEN "FF32CD32"
#define LIME "FF00FF00"
#define LAWN_GREEN "FF7CFC00"
#define CHARTREUSE "FF7FFF00"
#define GREEN_YELLOW "FFADFF2F"
#define SPRING_GREEN "FF000FF7F"
#define MEDIUM_SPRING_GREEN "FF00FA9A"
#define LIGHT_GREEN "FF90EE90"
#define PALE_GREEN "FF98FB98"
#define DARK_SEA_GREEN "FF8FBC8F"
#define MEDIUM_SEA_GREEN "FF3CB371"
#define SEA_GREEN "FF2E8B57"
#define FORREST_GREEN "FF228B22"
#define GREEN "FF008000"
#define DARK_GREEN "FF006400"
#define MEDIUM_AQUAMARINE "FF66CDAA"
#define AQUA "FF00FFFF"
#define CYAN "FF00FFFF"
#define LIGHT_CYAN "FFE0FFFF"
#define PALE_TURQUOISE "FFAFEEEE"
#define AQUAMARINE "FF7FFFD4"
#define TURQUOISE "FF40E0D0"
#define MEDIUM_TURQUOISE "FF48D1CC"
#define DARK_TURQUOISE "FF00CED1"
#define LIGHT_SEA_GREEN "FF20B2AA"
#define CADET_BLUE "FF5F9EA0"
#define DARK_CYAN "FF008B8B"
#define TEAL "FF008080"
#define LIGHT_STEEL_BLUE "FFB0C4DE"
#define POWDER_BLUE "FFB0E0E6"
#define LIGHT_BLUE "FFADD8E6"
#define SKY_BLUE "FF87CEEB"
#define LIGHT_SKY_BLUE "FF87CEFA"
#define DEEP_SKY_BLUE "FF00BFFF"
#define DODGER_BLUE "FF1E90FF"
#define CORNFLOWER_BLUE "FF6495ED"
#define STEEL_BLUE "FF4682B4"
#define ROYAL_BLUE "FF4169E1"
#define BLUE "FF0000FF"
#define MEDIUM_BLUE "FF0000CD"
#define DARK_BLUE "FF00008B"
#define NAVY "FF000080"
#define MIDNIGHT_BLUE "FF191970"
#define LAVENDER "FFE6E6FA"
#define THISTLE "FFD8BFD8"
#define PLUM "FFDDA0DD"
#define VIOLET "FFEE82EE"
#define ORCHID "FFDA70D6"
#define FUCHSIA "FFFF00FF"
#define MAGENTA "FFFF00FF"
#define MEDIUM_ORCHID "FFBA55D3"
#define MEDIUM_PURPLE "FF9370DB"
#define BLUE_VIOLET "FF8A2BE2"
#define DARK_VIOLET "FF9400D3"
#define DARK_ORCHID "FF9400D3"
#define DARK_MAGENTA "FF8B008B"
#define PURPLE "FF800080"
#define INDIGO "FF4B0082"
#define DARK_SLATE_BLUE "FF483D8B"
#define SLATE_BLUE "FF6A5ACD"
#define MEDIUM_SLATE_BLUE "FF7B68EE"
#define WHITE "FFFFFFFF"
#define SNOW "FFFFFAFA"
#define HONEYDEW "FFF0FFF0"
#define MINT_CREAM "FFF5FFFA"
#define AZURE "FFF0FFFF"
#define ALICE_BLUE "FFF0F8FF"
#define GHOST_WHITE "FFF8F8FF"
#define WHITE_SMOKE "FFF5F5F5"
#define SEASHELL "FFFFF5EE"
#define BEIGE "FFF5F5DC"
#define OLD_LACE "FFFDF5E6"
#define FLORAL_WHITE "FFFFFAF0"
#define IVORY "FFFFFFF0"
#define ANTIQUE_WHITE "FFFAEBD7"
#define LINEN "FFFAF0E6"
#define LAVENDER_BLUSH "FFFFF0F5"
#define MISTY_ROSE "FFFFE4E1"
#define GAINSBORO "FFDCDCDC"
#define LIGHTGRAY "FFD3D3D3"
#define SILVER "FFC0C0C0"
#define DARK_GRAY "FFA9A9A9"
#define GRAY "FF808080"
#define DIM_GRAY "FF696969"
#define LIGHT_SLATE_GRAY "FF778899"
#define SLATE_GRAY "FF708090"
#define DARK_SLATE_GRAY "FF2F4F4F"
#define BLACK "FF000000"
#define TRANSPARENT "00FFFFFF"
#define DEFAULT_COLOR "FF83A4C8"
#define DEFAULT_TITLE_COLOR "FF2E5B82"
    // Legacy colour codes
#define DKGRAY "FF444444"
#define LTGRAY "FFCCCCCC"
//#define LIGHT_BLUE "FF33B5E5"
    //THEME COLORS
#define THEME_TURQUOISE "FF1ABC9C"
#define THEME_TURQUOISE_DARK "FF16A085"
#define THEME_GREEN "FF2ECC71"
#define THEME_GREEN_DARK "FF27AE60"
#define THEME_BLUE "FF3498DB"
#define THEME_BLUE_DARK "FF2980B9"
#define THEME_PURPLE "FF9B59B6"
#define THEME_PURPLE_DARK "FF8E44AD"
#define THEME_MIDNIGHT "FF34495E"
#define THEME_MIDNIGHT_DARK "FF2C3E50"
#define THEME_YELLOW "FFF1C40F"
#define THEME_YELLOW_DARK "FFF39C12"
#define THEME_ORANGE "FFE67E22"
#define THEME_ORANGE_DARK "FFD35400"
#define THEME_RED "FFE74C3C"
#define THEME_RED_DARK "FFC0392B"
#define THEME_GREY "FF95A5A6"
#define THEME_GREY_DARK "FF7F8C8D"
#define THEME_DEFAULT DEFAULT_COLOR // Please set the Andee Blue Colour
#define THEME_DEFAULT_DARK DEFAULT_TITLE_COLOR // Please set  the Andee Dark Blue Colour





