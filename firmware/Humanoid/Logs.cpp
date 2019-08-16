#include<Arduino.h>
#include "Logs.hpp"
#include "configs.hpp"

void Logs::begin(){
    LOG_SERIAL.begin(LOG_SERIAL_BAUD);
}
