/*
Author: Jae An
*/

#ifndef ArduinoBlue_h
#define ArduinoBlue_h

#include "FunctionType.h"
#include <Arduino.h>

#define PATH_TRANSMISSION 244
#define PATH_TRANSMISSION_CONFIRMATION 245
#define LOCATION_TRANSMISSION_START 246
#define DELIMETER 247
#define TEXT_SEND_TRANSMISSION 248
#define CONNECTION_CHECK 249
#define TRANSMISSION_END 250
#define DRIVE_TRANSMISSION 251
#define BUTTON_TRANSMISSION 252
#define SLIDER_TRANSMISSION 253
#define TEXT_TRANSMISSION 254

#define DEFAULT_VALUE 255

#define TEXT_TRANSMISSION_TIMEOUT 5000 // ms
#define SHORT_TRANSMISSION_TIMEOUT 500
#define PATH_TRANSMISSION_TIMEOUT 10000

#define PATH_OVERFLOW_VALUE 1000000

const uint8_t DEFAULT_STEERING = 49;
const uint8_t DEFAULT_THROTTLE = 49;
const uint8_t MAX_SHORT_SIGNAL_LENGTH = 3;


class ArduinoBlue
{
public:
    ArduinoBlue(Stream &output);
    int getButton();
    int getSliderId();
    int getSliderVal();
    int getThrottle();
    int getSteering();
    float * getPathArrayX();
    float * getPathArrayY();
    float getPathY(float);
    int getPathLength();
    bool checkBluetooth();
    bool isConnected();
    bool isPathAvailable();
    void sendText(String msg);
    void sendMessage(String msg);
    void sendLocation(float, float, float, float, float);
    static float bytesToFloat(uint8_t u1, uint8_t u2, uint8_t u3, uint8_t u4);
    String getText();
    void setInterruptToggle(functiontype attach, functiontype detach);
private:
    Stream & _bluetooth;
    uint8_t _signal[MAX_SHORT_SIGNAL_LENGTH];
    uint8_t _signalLength = 0;
    uint8_t _throttle = DEFAULT_STEERING;
    uint8_t _steering = DEFAULT_THROTTLE;
    uint8_t _sliderVal = DEFAULT_VALUE;
    uint8_t _sliderId = DEFAULT_VALUE;
    uint8_t _button = DEFAULT_VALUE;
    bool _pathAvailable = false;
    String _text;
    float * _pathX;
    float * _pathY;
    float _prevReturnXx;
    int _pathLength;
    void clearSignalArray();
    void pushToSignalArray(uint8_t elem);
    void storeShortTransmission();
    bool storePathTransmission();
    void processDriveTransmission();
    void processButtonTransmission();
    void processSliderTransmission();
    void processTextTransmission();
    void processPathTransmission();
    void sendFloatAsBytes(float);
    void attachInterrupts();
    void detachInterrupts();
    String readString();
    functiontype _attachInterrupts = nullptr;
    functiontype _detachInterrupts = nullptr;
};

#endif
