#include <Arduino.h>

#include "configs.hpp"

#include "Humanoid.hpp"
#include "ServoController.hpp"
#include "CommandParser.hpp"
#include "Posture.hpp"
#include "AnimStore.hpp"

#if DEBUG_FREQUENCY
uint32_t prevTime =0, curTime =0;
uint32_t prevMicros =0, curMicros =0;
#endif

#if DEBUG
#include "Logs.hpp"
#endif

void Humanoid::init(){
    
#if DEBUG
    Logs::begin();
#endif
    ServoController::begin();
    Posture::begin();
    AnimStore::begin();
    CommandParser::begin();
}

void Humanoid::loop(){
#if DEBUG_I
LOG_I("Humanoid::loop", "IN")
#endif
#if DEBUG_FREQUENCY
    curTime=millis();
    curMicros=micros();
#endif

    ServoController::servoRoutine();
    Posture::movementPlanner();
    AnimStore::executeAnimation();
    CommandParser::parse();

#if DEBUG_FREQUENCY
    float delMilli = float(curTime)-prevTime;
    LOG_SERIAL.print(" Freq: ");
    if(delMilli>=0 && delMilli <=20){
        LOG_SERIAL.print(1000.0/(float(curMicros)-prevMicros));
        LOG_SERIAL.println("kHz");
    }
    else{
        LOG_SERIAL.print(1000.0/(delMilli));
        LOG_SERIAL.println("Hz");
    }
    prevTime=curTime;
    prevMicros=curMicros;
#endif
}
