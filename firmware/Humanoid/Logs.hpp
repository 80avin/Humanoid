#ifndef LOGS_HPP
#define LOGS_HPP

#define LOG_I(tag, msg) LOG_SERIAL.print(F("I: "));delay(1); LOG_SERIAL.print(tag); LOG_SERIAL.print(F(": ")); LOG_SERIAL.println(msg);
#define LOG_E(tag, msg) LOG_SERIAL.print(F("E: ")); LOG_SERIAL.print(tag); LOG_SERIAL.print(F(": ")); LOG_SERIAL.println(msg);
#define LOG_D(tag, var, val) LOG_SERIAL.print(F("D: "));delay(1); LOG_SERIAL.print(tag); LOG_SERIAL.print(F(": ")); LOG_SERIAL.print(var); LOG_SERIAL.print(F(": ")); LOG_SERIAL.println(val);
#include "configs.hpp"

namespace Logs{
      void begin();
}

#endif
