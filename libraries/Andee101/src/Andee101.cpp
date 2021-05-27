// Andee101.cpp - Arduino 101 Library
// Annikken Pte Ltd
// Author: Muhammad Hasif

#include <CurieBle.h>
#include <CurieTime.h>
#include <Andee101.h>
#include <stdlib.h>

char Andee101Version[5] = {'1','.','2','.','3'};

int nameFlag = 0;
int buttonNumber = 24;
char buttonBuffer[50];

char sensorsBuffer[64];

char readBuffer[128];
char readPartBuffer[64];
char phoneBuffer[64];
char sliderBuffer[MAXSLIDER][20];


char JoystickBufferX [4];
char JoystickBufferY [4];

bool AndeeConnected = false;
bool versionAndClear = false;
bool resetBLEFlag = true;
bool AndeeAlive = false;

bool dataLog = false;

BLEService Andee101Service("516e7d03-c4ea-4103-9bd2-c560221a0c16");
BLECharacteristic Andee101Write ("516e7d04-c4ea-4103-9bd2-c560221a0c16", BLEWrite|BLENotify, 20);
BLECharacteristic Andee101Read ("516e7d05-c4ea-4103-9bd2-c560221a0c16", BLEWrite|BLENotify, 20); 
															   
Andee101Class Andee101;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    //
//                                          General Functions                                         //
//                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////

void convertColor (const char* colorBuffer, char* outputColor)//converting color from 0 to 255 to 32 to 200
{
    int x = 0;
    char tmp[3];
    int colorByte;
    char newColor[5];

    for(x = 0; x < 4; x++ )
    {
        strncpy(tmp,colorBuffer + x*2,2);
        tmp[2] = '\0';
        sscanf(tmp,"%X",&colorByte);
        colorByte = ((colorByte * 200) / 255) + 32;
        newColor[x] = (char)colorByte;
    }
    newColor[4] = '\0';
	strcpy(outputColor,newColor);
    /* if(type == SET_COLOR )
    {
        strcpy(bodyBGBuffer,newColor);
        //printHex("bodyBG",bodyBGBuffer,andeeLog);
    }
    else if(type == SET_TEXT_COLOR)
    {
        strcpy(bodyFontBuffer,newColor);
        //printHex("bodyFont",bodyFontBuffer,andeeLog);
    }
    else if(type == SET_TITLE_COLOR)
    {
        strcpy(titleBGBuffer,newColor);
        //printHex("titleBG",titleBGBuffer,andeeLog);

    }
    else if(type == SET_TITLE_TEXT_COLOR)
    {
        strcpy(titleFontBuffer,newColor);
        //printHex("titleFont",titleFontBuffer,andeeLog);
    }     */
}

void printDEC(char* buffer)
{
	if(dataLog == true)
	{
		Serial.print("DEC:");
		for(unsigned int v = 0; v < (strlen(buffer)) ; v++)
		{
			Serial.print(buffer[v] & 0xFF,DEC);Serial.print(" ");
		}
		Serial.println("");
	}
	
}

void printHEX(char* buffer)
{
	if(dataLog == true)
	{
		Serial.print("HEX:");
		for(unsigned int v = 0; v < (strlen(buffer)) ; v++)
		{
			Serial.print(buffer[v] & 0xFF,HEX);Serial.print(" ");
		}
		Serial.println("");	
	}
	
}

void sendToPhone( char*UI )
{
  char partialUI[18] = {};
  int msgLen = 0;
  msgLen = strlen(UI);
  if(Andee101Write.subscribed() == true)
  {
	  if (msgLen >= 19)
	  {
		for (int i = 0; i <= msgLen;)
		{
		  memset(partialUI,0x00,18);
		  memcpy(partialUI, UI + i, 18);
		  //Serial.print("sendBT:");printHEX(partialUI);		  
		  Andee101Write.setValue((const char*)partialUI);
		  delay(5);
		  
		  i = i + 18;		
			
		}
	  }
	  else
	  {
		  //Serial.print("sendBT:");printHEX(partialUI);
		  Andee101Write.setValue((const char*)UI);
	  }  
  }
  else
  {
	  //Serial.println("Phone not connected");
  }
	  
}

void systemTime(void)
{
	char msgToPhone[18] = {START_TAG_COMMAND,TIMEEPOCH,END_TAG_COMMAND,0x00,0x00,0x00,
						   0x00,0x00,0x00,0x00,0x00,0x00,
						   0x00,0x00,0x00,0x00,0x00,0x00};
	sendToPhone(msgToPhone);
	//delay(5);
	memset(msgToPhone,0x00,18);
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.println("");Serial.print("Andee101 Connected\r\n");
  AndeeConnected = true;
  versionAndClear = false;    
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.println("");Serial.print("Andee101 disconnected\r\n");    
  AndeeConnected = false;
  resetBLEFlag = false;
    
}

void readBLEBuffer(BLEDevice central, BLECharacteristic characteristic)
{  
  unsigned int mLen;
  unsigned int rLen;
  char buffer[20];
  
    
  if(Andee101Read.value())
  {
	memset(buffer,0x00,20);
	
    mLen = strlen( (const char*) Andee101Read.value() );
	
	memcpy(buffer, (char*) Andee101Read.value() , mLen);
    
	buffer[mLen] = '\0';    
	
	if(readPartBuffer[0] != 0x00)
	{
		memcpy(readPartBuffer+(strlen(readPartBuffer)),buffer,mLen);
	}
	else
    {
		memset(readPartBuffer,0x00,64);				
		memcpy(readPartBuffer,buffer,mLen);			
	}
	
	printHEX(readPartBuffer);
	rLen = strlen(readPartBuffer);
	
	for(unsigned int b = 0; b < rLen ;b++)
	{
        
		if(readPartBuffer[b] == END_TAG_REPLY)
		{
            
			readPartBuffer[b] = '\0';
			memset(readBuffer,0x00,128);
			memcpy(readBuffer,readPartBuffer,rLen);
			memset(readPartBuffer,0x00,64);
		}
	}
  }
	
	//printHEX(readBuffer);	
	 
	
  
  //return;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//            				   Processing Replies from Smart Device      					 //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////

void processReply()
{
	unsigned char pressBuffer = 0;	
	
	if (	readBuffer[0] == BUTTON_IN || readBuffer[0] == KEYBOARD_IN|| 
			readBuffer[0] == TIME_IN || readBuffer[0] == DATE_IN || readBuffer[0] == CIRCLE_BUTTON)		
	{
		buttonBuffer[0] = 'B';
		buttonBuffer[1] = 'U';
		 for(int i = 0;i< (buttonNumber+1);i++)
		{				
		
				
			if(buttonBuffer[((i*2)+2)] == readBuffer[2])
			{
				pressBuffer = buttonBuffer[((i*2)+3)];				
				pressBuffer++;
				
				sprintf(buttonBuffer + ((i*2)+3),"%c",pressBuffer);
				break;
			}
			else if(buttonBuffer[((i*2)+2)] == 0x00)
			{
				sprintf(buttonBuffer + ((i*2)+2), "%c%c",readBuffer[2],'1');				
				break;
			}
		}
		
		if(readBuffer[0] == KEYBOARD_IN || readBuffer[0] == TIME_IN || readBuffer[0] == DATE_IN)
		{
			memset(phoneBuffer,0x00,64);
			int buffLen = strlen(readBuffer);			
			memcpy(phoneBuffer,readBuffer + 4,(buffLen - 4));
			Serial.println(phoneBuffer);
		}
		memset(readBuffer,0x00,128);
		return;
	}
	else if(readBuffer[0] == SLIDER_IN)
	{
		char value[18];
		char iid;
		unsigned int i = 0;
		
		iid = readBuffer[2];
		
		for(unsigned int i = 0; i < strlen(readBuffer);i++)
		{
			if(readBuffer[i+4] != SEPARATOR)
			{
				value[i] = readBuffer[i+4];
			}
			else 
			{
				value[i]='\0';
				break;
			}				
		}
		
		for(i = 0;i<MAXSLIDER;i++)
		{
			if(sliderBuffer[i][0] == 0x00)
			{
				sliderBuffer[i][0] = iid;
				strcpy(sliderBuffer[i]+1,value);
				break;
			}
			else if(sliderBuffer[i][0] == iid)
			{
				memset(sliderBuffer[i],0x00,20);
				sliderBuffer[i][0] = iid;
				strcpy(sliderBuffer[i]+1,value);
				break;
			}
		}
    
		memset(readBuffer,0x00,128);
	}
	else if(readBuffer[0] == TIMEEPOCH)
	{
		char buffer[11];
		char gmtBuffer[6];		
		unsigned long sysTime;
		long gmt,dst;
		char *ptr,*dls;
		int pLen,dLen;
		
		ptr = strchr(readBuffer+2,SEPARATOR);
		ptr++;
				
		dls = strchr(ptr,SEPARATOR);
		dls++;
		if(ptr != NULL)
		{
			pLen = strlen(ptr);
			dLen = strlen(dls);
			strncpy(gmtBuffer,ptr,pLen-dLen);
			gmtBuffer[pLen - 1] = 0x00;
			gmt = atol(gmtBuffer);
		}
		else
		{
			gmt = 0.00;
		}		
		if(dls !=NULL)
		{
			dst = atol(dls);
		}
		else
		{
			dst = 0.00;
		}
		memcpy(buffer,readBuffer+2,10);
		sysTime = atol(buffer) + (gmt*3600) + (dst*3600) ;
		sprintf(phoneBuffer,"%ld",sysTime);
		setTime(sysTime);
		memset(readBuffer,0x00,128);
	}
	else if(readBuffer[0] == JOYSTICK)
	{
		memset(phoneBuffer,0x00,64);
		memcpy(phoneBuffer,readBuffer,12);
		phoneBuffer[13] = '\0';
		memset(readBuffer,0x00,128);		
	}
	else if(readBuffer[0] == GYRO)
	{
		memset(sensorsBuffer,0x00,64);		
		memcpy(sensorsBuffer,readBuffer+2,strlen(readBuffer)-2);
		memset(readBuffer,0x00,128);
	}
	else if(readBuffer[0] == LAC)
	{
		memset(sensorsBuffer,0x00,64);		
		memcpy(sensorsBuffer,readBuffer+2,strlen(readBuffer)-2);
		memset(readBuffer,0x00,128);
	}
	else if(readBuffer[0] == GRAV)
	{
		memset(sensorsBuffer,0x00,64);		
		memcpy(sensorsBuffer,readBuffer+2,strlen(readBuffer)-2);
		memset(readBuffer,0x00,128);
	}
	else if(readBuffer[0] == GPS)
	{
		memset(sensorsBuffer,0x00,64);		
		memcpy(sensorsBuffer,readBuffer+2,strlen(readBuffer)-2);
		memset(readBuffer,0x00,128);
	}
	
	else if(readBuffer[0] == WATCH)
	{
		if (readBuffer[4] == 'T')
		{
			buttonBuffer[0] = 'B';
			buttonBuffer[1] = 'U';
			for(int i = 0; i < (buttonNumber+1); i++)
			{
					
				if(buttonBuffer[((i*2)+2)] == (readBuffer[2] + 32))
				{
					pressBuffer = buttonBuffer[((i*2)+3)];				
					pressBuffer++;
					sprintf(buttonBuffer + ((i*2)+3),"%c",pressBuffer);
					break;
				}		
				else if(buttonBuffer[((i*2)+2)] == 0x00)
				{
					sprintf(buttonBuffer + ((i*2)+2), "%c%c",(readBuffer[2] + 32),'1');
                    
					break;
				}				
			}
			if (readBuffer[2] == 76)
			{
				memset(phoneBuffer,0x00,64);
				int buffLen = 0;
				
				buffLen = strlen(readBuffer);
				readBuffer[buffLen-3] = '\0';			
				memcpy(phoneBuffer,readBuffer + 4,(buffLen - 1));			
				
			}
		}
		memset(readBuffer,0x00,128);
	}
	 else if(readBuffer[0] == 0x88)
	{
		AndeeAlive= true;
		memset(readBuffer,0x00,128);
		Serial.println("testing receivers");	
		return;
	}
	else if(readBuffer[0] == 0x00)
	{
		//ignore this state
	}
	else
	{
		memset(readBuffer,0x00,128);
		Serial.println("Command not recognised");
	} 
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    //
//                                        Annikken Andee101Class                                         //
//                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////

//

void Andee101Class::versionClear()
{
	if(versionAndClear == false)
	{
		delay(1800);
		Andee101.sendVersion();	
		Andee101.clear();
		versionAndClear = true;
	}
}

bool Andee101Class::isConnected()
{
	/* if(AndeeAlive == true)
	{
		Andee101.sendVersion();
		AndeeAlive = false;
	}
	return AndeeConnected; */
	
	
	return AndeeConnected;    
}

void Andee101Class::resetBLE()
{
	if(resetBLEFlag == false)
	{
		
		Andee101.broadcast();
		memset(buttonBuffer,0x00,50);
		memset(phoneBuffer,0x00,64);
		memset(readBuffer,0x00,128);
		memset(readPartBuffer,0x00,64);
		memset(sliderBuffer,0x00,64);
		resetBLEFlag = true;
	}
}

void Andee101Class::begin()
{
	
	if (nameFlag == 0)
	{
		BLE.setDeviceName("Andee101");
		BLE.setLocalName("Andee101");
	}
	BLE.begin();
	BLE.setAppearance(384);
	BLE.setAdvertisedService(Andee101Service);
	
	Andee101Service.addCharacteristic(Andee101Read);
	Andee101Service.addCharacteristic(Andee101Write);
	
	BLE.addService(Andee101Service);
	
	BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
	BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
	Andee101Read.setEventHandler(BLEWritten, readBLEBuffer);	
	
	
	BLE.advertise();
	
	memset(buttonBuffer,0x00,50);
	memset(phoneBuffer,0x00,64);
	memset(readBuffer,0x00,128);
	memset(readPartBuffer,0x00,64);
	memset(sliderBuffer,0x00,64);
}



void Andee101Class::poll()
{
	BLE.poll();
	
	processReply();
}

void Andee101Class::setName(const char* name)
{
	BLE.setDeviceName(name);
	BLE.setLocalName(name);
	nameFlag = 1;
	delay(5);
}

void Andee101Class::clear()
{
	char msgToPhone[18] = {START_TAG_COMMAND, CLEAR, END_TAG_COMMAND, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 	
	sendToPhone(msgToPhone);
	delay(5);
	memset(msgToPhone,0x00,18);
}

void Andee101Class::getDeviceTime(int* h,int* m, int* s)
{
	systemTime();
	delay(50);
	
	*h = hour();
	*m = minute();
	*s = second();
}

void Andee101Class::getDeviceDate(int* d, int* m, int* y )
{
	systemTime();
	delay(50);
	
	*d = day();
	*m = month();
	*y = year();
}

long Andee101Class::getDeviceTimeStamp(void)
{
	long buffer;
	systemTime();
	delay(50);
	buffer = atol(phoneBuffer);
	return buffer;
}

void Andee101Class::sendVersion(void)
{
	char buffer[10];
	memset(buffer,0x00,10);
	buffer[0] = START_TAG_VERSION;
	buffer[1] = '1';
	buffer[2] = SEPARATOR;
	buffer[3] = Andee101Version[0];
	buffer[4] = Andee101Version[1];
	buffer[5] = Andee101Version[2];
	buffer[6] = Andee101Version[3];
	buffer[7] = Andee101Version[4];
	buffer[8] = END_TAG_VERSION;
	buffer[9] = '\0';
	
	sendToPhone(buffer);
	delay(5);	
}

void Andee101Class::disconnect(void){
    char buffer[4];
    memset(buffer,0x00,4);
    buffer[0] = START_TAG_COMMAND;
    buffer[1] = DC;
    buffer[2] = END_TAG_COMMAND;
    buffer[3] = '\0';
    
    sendToPhone(buffer);
    delay(5);
}
/////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                         //
//                                      Text To Speech                                     //
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

void Andee101Class::textToSpeech(const char* speech, float speed, float pitch, char accent)
{
	char buffer[128];
	sprintf(buffer, "%c%c%c%s%c%.01f%c%.01f%c%c%c", START_TAG_COMMAND, TTS, SEPARATOR, speech, SEPARATOR, speed, SEPARATOR, pitch, SEPARATOR, accent, END_TAG_COMMAND);
	sendToPhone(buffer);	
}

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                               Control for Smartphone Camera                              //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

void Andee101Class::takePhoto(char cameraType, char autoFocus, char flash)
{
	char buffer[11];
	
	sprintf(buffer, "%c%c%c%c%c%c%c%c%c", START_TAG_COMMAND, CAMERA, SEPARATOR, cameraType, SEPARATOR,	autoFocus, SEPARATOR, flash, END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(2);
}
///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//            Gyroscope, Linear Accelerometer, Gravity and GPS data from Smartphone          //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////

void Andee101Class::gyroInit(int interval, int iteration)
{
	char buffer[64];	
	sprintf(buffer,"%c%c%c%c%d%c%d%c",START_TAG_COMMAND,GYRO,'0',SEPARATOR,interval,SEPARATOR,iteration,END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(50);
}

void Andee101Class::gyroStop()
{
	char buffer[6];	
	sprintf(buffer,"%c%c%c%c",START_TAG_COMMAND,GYRO,'1',END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(2);
}

void Andee101Class::getGyroReading(float* x,float* y,float* z)
{
	char* pEnd;	
	
	
	*x = strtod (sensorsBuffer, &pEnd);
	*y = strtod (pEnd, &pEnd);
	*z = strtod (pEnd, NULL);
	
	return;	
}

void Andee101Class::lacInit(int interval, int iteration)
{
	char buffer[64];	
	sprintf(buffer,"%c%c%c%c%d%c%d%c",START_TAG_COMMAND,LAC,'0',SEPARATOR,interval,SEPARATOR,iteration,END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(50);
}

void Andee101Class::lacStop()
{
	char buffer[6];	
	sprintf(buffer,"%c%c%c%c",START_TAG_COMMAND,LAC,'1',END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(2);
}

void Andee101Class::getLacReading(float* x,float* y,float* z)
{
	char* pEnd;	
	
	
	*x = strtod (sensorsBuffer, &pEnd);
	*y = strtod (pEnd, &pEnd);
	*z = strtod (pEnd, NULL);
	
	return;	
}

void Andee101Class::gravInit(int interval, int iteration)
{
	char buffer[64];	
	sprintf(buffer,"%c%c%c%c%d%c%d%c",START_TAG_COMMAND,GRAV,'0',SEPARATOR,interval,SEPARATOR,iteration,END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(50);
}

void Andee101Class::gravStop()
{
	char buffer[6];	
	sprintf(buffer,"%c%c%c%c",START_TAG_COMMAND,GRAV,'1',END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(2);
}

void Andee101Class::getGravReading(float* x,float* y,float* z)
{
	char* pEnd;	
	
	
	*x = strtod (sensorsBuffer, &pEnd);
	*y = strtod (pEnd, &pEnd);
	*z = strtod (pEnd, NULL);
	
	return;	
}

void Andee101Class::gpsInit(int interval, int iteration)
{
	char buffer[64];
	sprintf(buffer,"%c%c%c%c%d%c%d%c",START_TAG_COMMAND,GPS,'0',SEPARATOR,interval,SEPARATOR,iteration,END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(50);
}

void Andee101Class::gpsStop()
{
	char buffer[6];	
	sprintf(buffer,"%c%c%c%c",START_TAG_COMMAND,GPS,'1',END_TAG_COMMAND);	
	sendToPhone(buffer);
	delay(2);
}

void Andee101Class::getGpsReading(float* x,float* y,float* z)
{
	char* pEnd;	
	
	
	*x = strtod (sensorsBuffer, &pEnd);
	*y = strtod (pEnd, &pEnd);
	*z = strtod (pEnd, NULL);
	
	return;	
}

void Andee101Class::changeScreen(int screen)
{	
	char buffer[8];
	sprintf(buffer,"%c%c%c%c%c%i%c",START_TAG_UIXYWH,WATCH,SEPARATOR,'C',SEPARATOR,screen,END_TAG_UIXYWH);	
	buffer[7] = '\0';
	sendToPhone(buffer);
	delay(2);
}

void Andee101Class::showScreen()
{
	char buffer[6];
	sprintf(buffer,"%c%c%c%c%c",START_TAG_UIXYWH,WATCH,SEPARATOR,'S',END_TAG_UIXYWH);
	buffer[5] = '\0';
	sendToPhone(buffer);
	delay(2);
}

void Andee101Class::hideScreen()
{
	char buffer[6] = {START_TAG_UIXYWH,WATCH,SEPARATOR,'H',END_TAG_UIXYWH};
	buffer[5] = '\0';
	sendToPhone(buffer);
	delay(2);
}

void Andee101Class::textInput()
{
	char buffer[6] = {START_TAG_UIXYWH,WATCH,SEPARATOR,'X',END_TAG_UIXYWH};
	buffer[5] = '\0';
	sendToPhone(buffer);
	delay(2);
}

void Andee101Class::sendSMS(const char* number,const char* message)
{
	char buffer[64];
	sprintf(buffer,"%c%c%c%s%c%s%c",START_TAG_COMMAND,SMS,SEPARATOR,number,SEPARATOR,message,END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(2);
}

void Andee101Class::vibrate()
{
	char buffer[5];
	sprintf(buffer,"%c%c%c",START_TAG_COMMAND,VIBRATE,END_TAG_COMMAND);
	sendToPhone(buffer);
	delay(2);
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//                              Annikken Andee101Helper Functions                              //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////


void Andee101Helper::setId(int value)
{
	id = value + 32;
	
	sprintf(dataBuffer,"%c%c%c%c",0x20,0x20,0x20,0x20);
	sprintf(titleBuffer,"%c%c%c%c",0x20,0x20,0x20,0x20);
	sprintf(unitBuffer,"%c%c%c%c",0x20,0x20,0x20,0x20);
	
	convertColor("FF00FF00",titleBGBuffer);
	convertColor("FFFF0000",titleFontBuffer);
	convertColor("FF0000FF",bodyFontBuffer);
	convertColor("FF83A4C8",bodyBGBuffer);	
	
	sprintf(maxBuffer,"%s","   ");
	sprintf(minBuffer,"%s","   ");
	inputTypeBuffer = '0';
	subBuffer = '0';
	flashBuffer = '0';		
	
	if (value == WATCH_TITLE)
	{
		watchBuffer = '0';
	}	
	else if(value == WATCH_BUTTON1)
	{
		watchBuffer = '1';
	}
	else if(value == WATCH_BUTTON2)
	{
		watchBuffer = '2';
	}
	else if(value == WATCH_BUTTON3)
	{
		watchBuffer = '3';
	}
	else if(value == WATCH_BUTTON4)
	{
		watchBuffer = '4';
	}
}

void Andee101Helper::setType(char type)
{
	if(type == DATA_OUT)
	{
		sprintf(bleBuffer , "%c%c", (START_TAG_UIXYWH), (DATA_OUT));
	}
	else if(type == DATA_OUT_CIRCLE)
	{		
		sprintf(bleBuffer, "%c%c",START_TAG_UIXYWH,DATA_OUT_CIRCLE);
	}
	else if(type == DATA_OUT_HEADER)
	{		
		sprintf(bleBuffer, "%c%c",START_TAG_UIXYWH,DATA_OUT_HEADER);
	}
	
	else if(type == BUTTON_IN)
	{		
		sprintf(bleBuffer, "%c%c",START_TAG_UIXYWH,BUTTON_IN);
	}
	else if(type == CIRCLE_BUTTON)
	{		
		sprintf(bleBuffer, "%c%c",START_TAG_UIXYWH,CIRCLE_BUTTON);
	}
	
	else if(type == ANALOG_DIAL_OUT)
	{
		sprintf(bleBuffer,"%c%c", START_TAG_UIXYWH, ANALOG_DIAL_OUT);
	}
	
	else if(type == KEYBOARD_IN)
	{		
		sprintf(bleBuffer, "%c%c", START_TAG_UIXYWH, KEYBOARD_IN);
	}
	
	else if(type == DATE_IN)
	{
		sprintf(bleBuffer, "%c%c", START_TAG_UIXYWH, DATE_IN);
	}
	
	else if(type == TIME_IN)
	{
		sprintf(bleBuffer,"%c%c", START_TAG_UIXYWH, TIME_IN);
	}
	
	else if(type == SLIDER_IN)
	{
		sprintf(bleBuffer, "%c%c", START_TAG_UIXYWH,SLIDER_IN);
	}				
	
	else if (type == JOYSTICK)
	{
		sprintf(bleBuffer,"%c%c", START_TAG_UIXYWH,JOYSTICK);
	}
	
	else if (type == WATCH)
	{
		sprintf(bleBuffer,"%c%c", START_TAG_UIXYWH,WATCH);
	}

	else
	{
		
	}
}

void Andee101Helper::setLocation(char row, char order, char span){	
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int w = 0;
	unsigned int h = 20;	
	
	if(row == 0)
	{
		y = 4;
	}
	else if(row == 1)
	{
		y = 28;
	}
	else if(row == 2)
	{
		y = 52;
	}
	else if(row == 3)
	{
		y = 76;
	}
	
	switch(span){
		case ONE_THIRD:
			if(order == 0)
			{
				x = 4;
				w = 30;
			}
			else if(order == 1)
			{
				x = 35;
				w = 30;
			}
			else if(order == 2)
			{
				x = 66;
				w = 30;
			}
			break;
		case TWO_THIRD:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 35;
			}
			else if(order == 2)
			{
				x = 66;
			}
			w = 61;
			break;
		case FULL:
			x = 4;
			w = 92;
			break;
		case ONE_QUART:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 28;
			}
			else if(order == 2)
			{
				x = 52;
			}
			else if(order == 3)
			{
				x = 76;
			}
			w = 20;
			break;
		case THREE_QUART:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 28;
			}
			else if(order == 2)
			{
				x = 52;
			}
			w = 68;
			break;
		case HALF:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 52;
			}
			w =44;
			break;	
		default:
			//Serial.println("Andee UI Span ERROR");
			break;
	}
	
	memset(xywhBuffer,0x00,5);
	xywhBuffer[0] = x+32;
	xywhBuffer[1] = y+32;
	xywhBuffer[2] = w+32;
	xywhBuffer[3] = h+32;
}

void Andee101Helper::setCoord(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
	if(x > C_HLIMIT)
	{
		x = C_HLIMIT;
	}        
    else if(x < C_LLIMIT)
	{
		 x = C_LLIMIT;
	}
	
	
	if(y > C_HLIMIT)
	{
		y = C_HLIMIT;
	}        
    else if(y < C_LLIMIT)
	{
		 y = C_LLIMIT;
	}
	
	
	if(w > C_HLIMIT)
	{
		w = C_HLIMIT;
	}        
    else if(w < C_LLIMIT)
	{
		 w = C_LLIMIT;
	}
	
	
	if(h > C_HLIMIT)
	{
		h = C_HLIMIT;
	}        
    else if(h < C_LLIMIT)
	{
		 h = C_LLIMIT;
	}
		
	memset(xywhBuffer,0x00,5);
	xywhBuffer[0] = x+32;
	xywhBuffer[1] = y+32;
	xywhBuffer[2] = w+32;
	xywhBuffer[3] = h+32;
	//sprintf(xywhBuffer, "%02u%02u%02u%03u", x, y, w, h);
}

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                               Input Mode and Sub Type                              //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////

void Andee101Helper::setInputMode(char type)
{
	if(type < 32)
		inputTypeBuffer = type + 48;
	
	else
		inputTypeBuffer = type;
	
}

void Andee101Helper::setSubType(char type)
{
	if(type < 32)
		subBuffer = type + 48;
	
	else
		subBuffer = type;
	
	
}

/////////////////////////////////////////////////////////////////////
//                                                                 //
//                           Set Colours                           //
//                                                                 //
/////////////////////////////////////////////////////////////////////



void Andee101Helper::setTitleColor(const char* color)
{
	memset(titleBGBuffer,0x00,5);
	convertColor(color,titleBGBuffer);	
}
void Andee101Helper::setTitleColor(char* color)
{
	memset(titleBGBuffer,0x00,5);
	convertColor(color,titleBGBuffer);	
}



void Andee101Helper::setTitleTextColor(const char* color)
{
	memset(titleFontBuffer,0x00,5);
	convertColor(color,titleFontBuffer);
}
void Andee101Helper::setTitleTextColor(char* color)
{
	memset(titleFontBuffer,0x00,5);
	convertColor(color,titleFontBuffer);
}


void Andee101Helper::setColor(const char* color)
{
	memset(bodyBGBuffer,0x00,5);
	convertColor(color,bodyBGBuffer);
} 
void Andee101Helper::setColor(char* color)
{
	memset(bodyBGBuffer,0x00,5);
	convertColor(color,bodyBGBuffer);
} 


void Andee101Helper::setTextColor(const char* color)
{
	memset(bodyFontBuffer,0x00,5);
	convertColor(color,bodyFontBuffer);
} 
void Andee101Helper::setTextColor(char* color)
{
	memset(bodyFontBuffer,0x00,5);
	convertColor(color,bodyFontBuffer);
} 

//////////////////////////////////////////////////////////////////////
//                                                                  //
//                  SetData, Unit, Title & MinMax                   //
//                                                                  //
//////////////////////////////////////////////////////////////////////

void Andee101Helper::setData(const char* data)
{
	memset(dataBuffer,0x00,64);		
	if(strlen(data) <= 0)
	{
		data = "     ";
	}
    sprintf(dataBuffer, "%s", data);
}

void Andee101Helper::setData(int data)
{		
	memset(dataBuffer,0x00,64);			
	sprintf(dataBuffer, "%d", data);
}

void Andee101Helper::setData(float data,char decPlace)
{		
	memset(dataBuffer,0x00,64);		
	dtostrf(data,3,decPlace,dataBuffer);
}

void Andee101Helper::setData(double data,char decPlace)
{		
	memset(dataBuffer,0x00,64);	
	dtostrf(data,3,decPlace,dataBuffer);
}



void Andee101Helper::setTitle(const char* title)
{
	memset(titleBuffer,0x00,64);	
	if(strlen(title) <= 0)
	{
		title = "     ";
	}
	sprintf(titleBuffer, "%s", title);
}

void Andee101Helper::setTitle(int title)
{
	memset(titleBuffer,0x00,64);	
	sprintf(titleBuffer, "%d", title);
}

void Andee101Helper::setTitle(float title, char decPlace)
{
	memset(titleBuffer,0x00,64);	
	dtostrf(title,3,decPlace,titleBuffer);
}

void Andee101Helper::setTitle(double title, char decPlace)
{
	memset(titleBuffer,0x00,64);	
	dtostrf(title,3,decPlace,titleBuffer);
}



void Andee101Helper::setUnit(const char* unit)
{
	memset(unitBuffer,0x00,64);    
	if(strlen(unit) <= 0)
	{
		unit = "     ";
	}
	sprintf(unitBuffer, "%s", unit);
}

void Andee101Helper::setUnit(int unit)
{
	memset(unitBuffer,0x00,64);	
	sprintf(unitBuffer, "%d", unit);
}

void Andee101Helper::setUnit(float unit, char decPlace)
{
	memset(unitBuffer,0x00,64);	
	dtostrf(unit,3,decPlace,unitBuffer);
}

void Andee101Helper::setUnit(double unit, char decPlace)
{
	memset(unitBuffer,0x00,64);	
	dtostrf(unit,3,decPlace,unitBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////
//                                                                                  //
//                         Miscellaneous Data Functions                             //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////

void Andee101Helper::setMinMax(int min, int max)
{
	memset(minBuffer,0x00,5);	
	memset(maxBuffer,0x00,5);	
	sprintf(minBuffer,"%d", min);
	sprintf(maxBuffer,"%d", max);
}

void Andee101Helper::setMinMax(float min,float max,char decPlace)
{
	memset(minBuffer,0x00,5);	
	memset(maxBuffer,0x00,5);
	dtostrf(min,3,decPlace,minBuffer);
	dtostrf(max,3,decPlace,maxBuffer);
}

void Andee101Helper::setMinMax(double min,double max,char decPlace)
{
	memset(minBuffer,0x00,5);	
	memset(maxBuffer,0x00,5);
	dtostrf(min,3,decPlace,minBuffer);
	dtostrf(max,3,decPlace,maxBuffer);
}

/* void Andee101Helper::setKeyboardType(char type)
{
	inputTypeBuffer = type;
} */

void Andee101Helper::getKeyboardMessage(char* message)
{	
	sprintf(message,"%s", phoneBuffer);
	return;	
}

void Andee101Helper::setDefaultDate(int date, int month, int year)
{
	sprintf(dataBuffer,"%02i%02i%04i", date%31, month%12, year);
}

void Andee101Helper::getDateInput(int* d,int* m, int* y)
{
	*y = atoi(phoneBuffer+4);
	phoneBuffer[4] = 0x00;
	*m = atoi(phoneBuffer+2);
	phoneBuffer[2] = 0x00;
	*d = atoi(phoneBuffer);
	return;
}

void Andee101Helper::setDefaultTime(int hour,int min, int sec)
{
	sprintf(dataBuffer,"%02i%02i%02i", hour%24, min%60, sec%60);
}

void Andee101Helper::getTimeInput(int* h,int* m,int* s)
{
	*s = atoi(phoneBuffer+4);
	phoneBuffer[4] = 0x00;
	*m = atoi(phoneBuffer+2);
	phoneBuffer[2] = 0x00;
	*h = atoi(phoneBuffer);
	
	return;
}


/////////////////////////////////////////////////////////////////////
//                                                                 //
//                            BUTTONS                              //
//                                                                 //
/////////////////////////////////////////////////////////////////////

int Andee101Helper::isPressed(void)
{
	
	for(int i = 0; i<buttonNumber;i++)
	{
		int pos = (i*2)+2;
		if(id == buttonBuffer[pos])
		{
			if(buttonBuffer[pos+1] > '0')
			{
				flashBuffer = buttonBuffer[pos+1];
				buttonBuffer[pos+1] = 0x30;				
				
				if(inputTypeBuffer == 48)
				{
					return '1';
				}
				else
				{
					return flashBuffer - '0';
				}				
			}
		}
	}
	flashBuffer = 0x00;
	return 0;
}
 void Andee101Helper::ack(void)
{	 
	if(bleBuffer[1] == BUTTON_IN)
	{
		if(inputTypeBuffer == '0')
		{
			char* buffer = new char[18];
			buffer[0] = END_TAG_UIXYWH;
			buffer[1] = START_TAG_UIXYWH;
			buffer[2] = ACKN;
			buffer[3] = SEPARATOR;
			buffer[4] = id;
			buffer[5] = END_TAG_UIXYWH;
			for(int l = 6; l < 18; l++)
			{
				buffer[l] = 0x00;
			}
			sendToPhone(buffer);
		}

	}
	else
	{
		char* buffer = new char[18];
			buffer[0] = END_TAG_UIXYWH;
			buffer[1] = START_TAG_UIXYWH;
			buffer[2] = ACKN;
			buffer[3] = SEPARATOR;
			buffer[4] = id;
			buffer[5] = END_TAG_UIXYWH;
			for(int l = 6; l < 18; l++)
			{
				buffer[l] = 0x00;
			}
			
		 
			sendToPhone(buffer);
	}
	
}

int Andee101Helper::pressCounter()
{		
	return(int)(flashBuffer-48);
}

////////////////////////////////////////////////////////////////////////////////////
//                                                                                //
//                                   SLIDERS                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void Andee101Helper::setSliderInitialValue(int value)
{
	sprintf(dataBuffer,"%d",value);
	sprintf(tempBuffer,"%d",value);
	flashBuffer = '1';
}

void Andee101Helper::setSliderInitialValue(float value,char decPlace)
{
	dtostrf(value,3,decPlace,dataBuffer);
	dtostrf(value,3,decPlace,tempBuffer);
	flashBuffer = '1';
}
void Andee101Helper::setSliderInitialValue(double value,char decPlace)
{
	dtostrf(value,3,decPlace,dataBuffer);
	dtostrf(value,3,decPlace,tempBuffer);
	flashBuffer = '1';
}

void Andee101Helper::setSliderNumIntervals(int numInterval)
{	
	if(numInterval <= 223 && numInterval >= 0)
	{
		subBuffer = numInterval + 32;
	}
	else if(numInterval > 223)
	{
		subBuffer = 255;
	}
	else if (numInterval <= 0)
	{
		subBuffer = 0;
	}
}

void Andee101Helper::getSliderValue(int* x)
{
	char buffer[20];
	unsigned int i = 0;
	memset(buffer,0x00,20);
	
	for(i = 0; i < MAXSLIDER; i++)
	{
		if(sliderBuffer[i][0] == id)
		{
			strcpy(buffer,sliderBuffer[i] + 1);			
		}
	}
	
	if(buffer[0] == 0x00)
	{
		*x = atoi(tempBuffer);
	}		
	else
	{
		*x = atoi(buffer);
	}	
	setSliderInitialValue(*x);	
}

void Andee101Helper::getSliderValue(float* f)
{
	char buffer[20];
	unsigned int i = 0;
	memset(buffer,0x00,20);
	for(i = 0; i < MAXSLIDER; i++)
	{
		if(sliderBuffer[i][0] == id)
		{
			strcpy(buffer,sliderBuffer[i] + 1);
		}
	}
	if(buffer[0] == 0x00)
	{
		*f = atof(tempBuffer);
	}		
	else
	{
		*f = atof(buffer);
	}	
	setSliderInitialValue(*f);
}

void Andee101Helper::getSliderValue(double* d)
{
	char buffer[20];
	unsigned int i = 0;
	memset(buffer,0x00,20);
	for(i = 0; i < MAXSLIDER; i++)
	{
		if(sliderBuffer[i][0] == id)
		{
			strcpy(buffer,sliderBuffer[i] + 1);
		}
	}
	if(buffer[0] == 0x00)
	{
		*d = strtod(tempBuffer,NULL);
	}		
	else
	{
		*d = strtod(buffer,NULL);
	}	
	setSliderInitialValue(*d);
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                                    Joystick Control                                  //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

void Andee101Helper::getJoystick(int* x,int* y)
{
	char bufferX[5];
	char bufferY[5];
	
	for(int i = 0;i<4;i++)
	{
		bufferX[i] = phoneBuffer[i+4];
	}
	bufferX[4] = '\0';
	
	for(int j = 0;j<4;j++)
	{
		bufferY[j] = phoneBuffer[j+8];
	}
	bufferY[4] = '\0';
	 
	
	if(id == phoneBuffer[2])
	{
		*x = atoi(bufferX); //- (int)100;
		*y = atoi(bufferY); //- (int)100;		
	}	
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void Andee101Helper::update(void)
{
	Andee101.versionClear();
	//Andee101.isConnected();
	
	if(bleBuffer[1] == DATA_OUT)	
	{
		sprintf(bleBuffer , "%c%c%c%s%c%c%s%s%s%s%c%s%c%s%c%s%c", (START_TAG_UIXYWH),(DATA_OUT),(id), xywhBuffer,'0',SEPARATOR,titleBGBuffer,titleFontBuffer,bodyBGBuffer,bodyFontBuffer,SEPARATOR,	titleBuffer,SEPARATOR,unitBuffer,SEPARATOR,dataBuffer,(END_TAG_UIXYWH));
	}
	else if(bleBuffer[1] == DATA_OUT_CIRCLE)	
	{
		sprintf(bleBuffer , "%c%c%c%s%c%c%s%s%s%c%s%c%s%c%s%c", (START_TAG_UIXYWH),(DATA_OUT_CIRCLE),(id), xywhBuffer,'0',SEPARATOR,titleFontBuffer,bodyBGBuffer,bodyFontBuffer,SEPARATOR,titleBuffer,SEPARATOR,unitBuffer,SEPARATOR,dataBuffer,(END_TAG_UIXYWH));
	}
	else if(bleBuffer[1] == DATA_OUT_HEADER)	
	{
		sprintf(bleBuffer , "%c%c%c%s%c%c%s%s%c%s%c", (START_TAG_UIXYWH),(DATA_OUT_HEADER),(id), xywhBuffer,'0',SEPARATOR,bodyBGBuffer,bodyFontBuffer,SEPARATOR,titleBuffer,(END_TAG_UIXYWH));
	}
	
	
	else if(bleBuffer[1] == BUTTON_IN)
	{
		memcpy(titleBGBuffer,bodyBGBuffer,5);
		memcpy(titleFontBuffer,bodyFontBuffer,5);
		sprintf(bleBuffer, "%c%c%c%s%c%c%s%s%c%s%c",START_TAG_UIXYWH,BUTTON_IN, id,xywhBuffer,inputTypeBuffer,SEPARATOR,titleBGBuffer,titleFontBuffer,SEPARATOR,titleBuffer,END_TAG_UIXYWH);
	}
	else if(bleBuffer[1] == CIRCLE_BUTTON)
	{
		sprintf(bleBuffer, "%c%c%c%s%c%c%s%s%c%s%c",START_TAG_UIXYWH,CIRCLE_BUTTON, id,xywhBuffer,inputTypeBuffer,SEPARATOR,titleBGBuffer,titleFontBuffer,SEPARATOR,titleBuffer,END_TAG_UIXYWH);
	}
	
	
	
	else if(bleBuffer[1] == ANALOG_DIAL_OUT)
	{
		if (strcmp (maxBuffer," ") == 0)
		{
			sprintf(maxBuffer,"%s","255");
		}
		if (strcmp (minBuffer," ") == 0)
		{
			sprintf(minBuffer,"%c",'0');
		}
		sprintf(bleBuffer,"%c%c%c%s%c%c%s%s%c%s%c%s%c%s%c%s%c%s%c", START_TAG_UIXYWH, ANALOG_DIAL_OUT, id, xywhBuffer,'0', SEPARATOR, titleBGBuffer,bodyBGBuffer, SEPARATOR, titleBuffer, SEPARATOR, unitBuffer,SEPARATOR, dataBuffer, SEPARATOR, maxBuffer, SEPARATOR, minBuffer, END_TAG_UIXYWH);
	}	
	
	
	else if(bleBuffer[1] == KEYBOARD_IN)
	{
		sprintf(bleBuffer, "%c%c%c%s%c%c%s%c%s%c%s%c", START_TAG_UIXYWH, KEYBOARD_IN, id, xywhBuffer, inputTypeBuffer,SEPARATOR, titleFontBuffer, SEPARATOR, titleBuffer, SEPARATOR, dataBuffer, END_TAG_UIXYWH);
	}
	
	else if(bleBuffer[1] == DATE_IN)
	{
		sprintf(bleBuffer, "%c%c%c%s%c%c%s%c%s%c%s%c", START_TAG_UIXYWH, DATE_IN,  id,xywhBuffer, inputTypeBuffer,SEPARATOR, titleFontBuffer, SEPARATOR, titleBuffer, SEPARATOR, dataBuffer, END_TAG_UIXYWH);
	}
	
	else if(bleBuffer[1] == TIME_IN)
	{
		sprintf(bleBuffer,"%c%c%c%s%c%c%s%c%s%c%s%c", START_TAG_UIXYWH, TIME_IN,  id,xywhBuffer, inputTypeBuffer,SEPARATOR, titleFontBuffer, SEPARATOR, titleBuffer, SEPARATOR, dataBuffer, END_TAG_UIXYWH);
	}
	
	else if(bleBuffer[1] == SLIDER_IN)
	{
		sprintf(bleBuffer, "%c%c%c%s%c%c%s%s%c%s%c%s%c%s%c%s%c%c%c%c%c", START_TAG_UIXYWH,SLIDER_IN, id,xywhBuffer,inputTypeBuffer,SEPARATOR,titleBGBuffer,bodyBGBuffer,SEPARATOR,titleBuffer,SEPARATOR,dataBuffer,SEPARATOR,maxBuffer,SEPARATOR,minBuffer,SEPARATOR,subBuffer,SEPARATOR,flashBuffer,END_TAG_UIXYWH);
	}	
	
	else if(bleBuffer[1] == JOYSTICK)
	{
		sprintf(bleBuffer,"%c%c%c%s%c%c%s%s%c%s%c%s%c%s%c", START_TAG_UIXYWH,JOYSTICK, id,xywhBuffer,inputTypeBuffer,SEPARATOR,titleBGBuffer,bodyBGBuffer,SEPARATOR,titleBuffer,SEPARATOR, unitBuffer,SEPARATOR,dataBuffer,END_TAG_UIXYWH);
	}
	
	else if(bleBuffer[1] == WATCH)
	{
		sprintf(bleBuffer,"%c%c%c%c%c%s%s%c%s%c", START_TAG_UIXYWH,WATCH,SEPARATOR,watchBuffer,SEPARATOR,titleBGBuffer,titleFontBuffer,SEPARATOR,titleBuffer,END_TAG_UIXYWH);
	}
	
	//printHEX(bleBuffer);
	sendToPhone(bleBuffer);
	delay(15);
}


void Andee101Helper::remove()
{
	char removeUI[6] = {START_TAG_UIXYWH, REMOVE, SEPARATOR, (char)id, END_TAG_UIXYWH, 0x00};
	
	sendToPhone(removeUI);
	delay(2);
}


