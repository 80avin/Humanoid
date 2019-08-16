#include "Arduino.h"

#include "ServoController.hpp"
#include "AnimStore.hpp"
#include "CommandParser.hpp"
#include "Posture.hpp"

#include "configs.hpp"

#if DEBUG
#include "Logs.hpp"
#endif
cmd_t CommandParser::cmd = cmd_t();
uint8_t CommandParser::nextToRead = 1;
bool CommandParser::readingBinary = false;
uint16_t CommandParser::customAnimSize = 0;
uint8_t CommandParser::customAnimCol = 0;

cmd_t::cmd_t(){
    this->reset();
}
cmd_t::cmd_t(const char &_cT, const uint8_t &_cI, const uint16_t &_p1, const uint16_t &_p2, const uint16_t &_p3){
    this->set(_cT, _cI, _p1, _p2, _p3);
}

void cmd_t::reset(){
    this->set(CMD_TYPE_NULL, ~0, ~0, ~0, ~0);
}

void cmd_t::set(const char &_cT, const uint8_t &_cI, const uint16_t &_p1, const uint16_t &_p2, const uint16_t &_p3){
    this->commandType=_cT;
    this->commandIndex=_cI;
    this->params[0]=_p1;
    this->params[1]=_p2;
    this->params[2]=_p3;
}

void CommandParser::begin(){
#if DEBUG_I
LOG_I("CommandParser::begin", "IN")
#endif
    COMMAND_SERIAL.begin(COMMAND_SERIAL_BAUD);
    COMMAND_SERIAL.println("Accepting Command");
    cmd = cmd_t();
    nextToRead = 1;
}

/**
 * @brief 
 * 
 */
void CommandParser::parse(){
#if DEBUG_I
LOG_I("CommandParser::parse", "IN")
#endif
    char _ch;
    
    while(COMMAND_SERIAL.available()){
        _ch=COMMAND_SERIAL.read();
        if(readingBinary || ( _ch != '\n' && _ch != '\r' && _ch != ';')){
            parseChar(_ch);
        }
        else{
            nextToRead=1;
            if(cmd.commandIndex != CMD_TYPE_NULL)
                executeCmd();
            cmd.reset();
        }
    }
}

/**
 * @brief 
 * 
 * @param _ch 
 */
void CommandParser::parseChar(const char &_ch){
    switch(nextToRead){
        case 1:     // commandType - alphabet only
            if(isalnum(_ch)){
                cmd=cmd_t();
                cmd.commandType = _ch;
                ++nextToRead;
            }
            break;
        case 2:     // commandIndex - uint8_t only, <255
            if(_ch==' ' && cmd.commandIndex != 255){
                ++nextToRead;
            }
            else if(_ch>='0' && _ch<='9'){
                cmd.commandIndex = (cmd.commandIndex==255)?(_ch-'0'):(cmd.commandIndex*10+_ch-'0');
            }
            break;
        default:
            if(cmd.commandType=='M'){
                parseTypeMArgs(_ch);
            }
            else if(cmd.commandType=='P'){
                parseTypePArgs(_ch);
            }
            else if(cmd.commandType=='Q'){
                parseTypeQArgs(_ch);
            }
            else if(cmd.commandType=='U'){
                parseTypeUArgs(_ch);
            }
    }
}

/**
 * @brief 
 * 
 * @param _ch 
 */
void CommandParser::parseTypeMArgs(const char &_ch){
    switch(nextToRead){
    case 3:
        if(_ch>='0' && _ch<='9'){
            cmd.params[0]=(cmd.params[0]==65535)?(_ch-'0'):(cmd.params[0]*10+_ch-'0');
        }
        else if(_ch==' '){
            ++nextToRead;
        }
        break;
    case 4:
        if(_ch>='0' && _ch<='9'){
            cmd.params[1]=(cmd.params[1]==65535)?(_ch-'0'):(cmd.params[1]*10+_ch-'0');
        }
        else if(_ch==' '){
            ++nextToRead;
        }
        break;
    case 5:
        if(_ch>='0' && _ch<='9'){
            cmd.params[2]=(cmd.params[2]==65535)?(_ch-'0'):(cmd.params[2]*10+_ch-'0');
        }
        else if(_ch==' '){
            ++nextToRead;
        }
        break;
    default:
        // TODO add default behaviour
        break;
    }
}

/**
 * @brief 
 * 
 * @param _ch 
 */
void CommandParser::parseTypePArgs(const char &_ch){
    switch(nextToRead){
    case 3:
        if(_ch>='0' && _ch<='9'){
            cmd.params[0]=(cmd.params[0]==65535)?(_ch-'0'):(cmd.params[0]*10+_ch-'0');
        }
        else if(_ch==' '){
            ++nextToRead;
        }
        break;
    default:
        // TODO add default behaviour
        break;
    }
}
/**
 * @brief 
 * 
 * @param _ch 
 */

void CommandParser::parseTypeQArgs(const char &_ch){
    switch(nextToRead){
    case 3:
        if(_ch>='0' && _ch<='9'){
            cmd.params[0]=(cmd.params[0]==65535)?(_ch-'0'):(cmd.params[0]*10+_ch-'0');
        }
        else if(_ch==' '){
            ++nextToRead;
        }
        break;
    default:
        // TODO add default behaviour
        break;
    }
}
/**
 * @brief 
 * 
 * @param _ch 
 */
void CommandParser::parseTypeUArgs(const char &_ch){
    switch(nextToRead){
    case 3:
        if(_ch>='0' && _ch<='9'){
            cmd.params[0]=(cmd.params[0]==65535)?(_ch-'0'):(cmd.params[0]*10+_ch-'0');
        }
        else if(_ch==' '){
            ++nextToRead;
        }
        break;
    case 4:
        if(_ch>='0' && _ch<='9'){
            cmd.params[1]=(cmd.params[1]==65535)?(_ch-'0'):(cmd.params[1]*10+_ch-'0');
        }
        else if(_ch==' '){
            ++nextToRead;
        }
        break;
    case 5:
        if(_ch>='0' && _ch<='9'){
            cmd.params[2]=(cmd.params[2]==65535)?(_ch-'0'):(cmd.params[2]*10+_ch-'0');
        }
        else if(_ch==' '){
            ++nextToRead;
        }
        break;
    default:
        // TODO add default behaviour
        break;
    }
}

void CommandParser::parseTypeAArgs(const char &_ch){
    switch (nextToRead)
    {
    case 3:
        readingBinary=true;
        if(customAnimCol%3 == 0){

        }
        break;
    
    default:
        break;
    }
}

/**
 * @brief 
 * 
 */

void CommandParser::executeCmd(){
    if(cmd.commandType == 'M'){
        switch(cmd.commandIndex){
            case 1:     // Move this servo to default
                AnimStore::clearAll();
                if(cmd.params[0] != CMD_PARAMS_NULL)
                    Posture::setDefault(PART_TO_CH(cmd.params[0]));
                else
                    Posture::setDefault();
                break;
            case 2:
                AnimStore::clearAll();
                if(cmd.params[0] != CMD_PARAMS_NULL && cmd.params[1] != CMD_PARAMS_NULL)
                    Posture::setPos(PART_TO_CH(cmd.params[0]), cmd.params[1]);
                break;
            case 3:
                AnimStore::clearAll();
                if(cmd.params[0] != CMD_PARAMS_NULL && cmd.params[1] != CMD_PARAMS_NULL && cmd.params[2] != CMD_PARAMS_NULL)
                    Posture::setSweep(PART_TO_CH(cmd.params[0]), cmd.params[1], cmd.params[2]);
                break;
        }
    }
    else if(cmd.commandType == 'P'){
        switch(cmd.commandIndex){
            case 1:
                AnimStore::clearAll();
                if(cmd.params[0] != CMD_PARAMS_NULL)
                    AnimStore::applyAnimation(cmd.params[0]);
                else
                    Posture::sweepDefault(3000);
                break;
            case 2:
                break;
            case 3:
                break;
        }
    }
    else if(cmd.commandType == 'Q'){
        switch(cmd.commandIndex){
            case 1:
                AnimStore::clearAll();
                if(cmd.params[0] != CMD_PARAMS_NULL)
                    AnimStore::pushQueue(cmd.params[0]);
                break;
            case 2:
                break;
            case 3:
                break;
        }
    }
    else if(cmd.commandType == 'U'){
        switch(cmd.commandIndex){
            case 1:     // Move this servo to default
                AnimStore::clearAll();
                if(cmd.params[0] != CMD_PARAMS_NULL)
                    //Posture::setDefault(PART_TO_CH(cmd.params[0]));
                    ServoController::writeWidth(PART_TO_CH(cmd.params[0]), 1500, true);
                else
                    for(uint8_t _ch=0; _ch<PART_SIZE; ++_ch)
                        ServoController::writeWidth(_ch, 1500, true);
                break;
            case 2:
                AnimStore::clearAll();
                if(cmd.params[0] != CMD_PARAMS_NULL && cmd.params[1] != CMD_PARAMS_NULL)
                    //Posture::setPos(PART_TO_CH(cmd.params[0]), cmd.params[1]);
                    ServoController::writeWidth(PART_TO_CH(cmd.params[0]), cmd.params[1], true);
                break;
            case 3:
                AnimStore::clearAll();
                if(cmd.params[0] != CMD_PARAMS_NULL && cmd.params[1] != CMD_PARAMS_NULL && cmd.params[2] != CMD_PARAMS_NULL)
                    //Posture::setSweep(PART_TO_CH(cmd.params[0]), cmd.params[1], cmd.params[2]);
                    ServoController::sweepWidth(PART_TO_CH(cmd.params[0]), cmd.params[1], cmd.params[2], true);
                break;
        }
    }
#if DEBUG_COMMAND
    LOG_SERIAL.print(cmd.commandType);
    LOG_SERIAL.print(" ::: ");
    LOG_SERIAL.print(cmd.commandIndex);
    LOG_SERIAL.print("\t:: ");
    LOG_SERIAL.print(cmd.params[0]);
    LOG_SERIAL.print("\t:: ");
    LOG_SERIAL.print(cmd.params[1]);
    LOG_SERIAL.print("\t:: ");
    LOG_SERIAL.println(cmd.params[2]);
#endif
}

