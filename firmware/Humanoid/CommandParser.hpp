#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include "configs.hpp"

#define CMD_TYPE_NULL 0
#define CMD_INDEX_NULL 255
#define CMD_PARAMS_NULL 65535

class cmd_t{
        public:
        char commandType;
        uint8_t commandIndex;
        uint16_t params[3];
        cmd_t();
        cmd_t(const char &_cT, const uint8_t &_cI, const uint16_t &_p1, const uint16_t &_p2, const uint16_t &_p3);
        void reset();
        void set(const char &_cT, const uint8_t &_cI, const uint16_t &_p1, const uint16_t &_p2, const uint16_t &_p3);
};
/* 
class cmd_anim_t{
    public:
    char 
} */

class CommandParser{
    public:
        static void begin();
        static void parse();
        CommandParser();
    private:
    static cmd_t cmd;
    static uint8_t nextToRead;
    static bool readingBinary;
    static uint16_t customAnimSize;
    static uint8_t customAnimCol;
    //static SerialCommands serialCommands

    static void parseChar(const char &_ch);
    static void parseTypeMArgs(const char &_ch);
    static void parseTypePArgs(const char &_ch);
    static void parseTypeQArgs(const char &_ch);
    static void parseTypeUArgs(const char &_ch);
    static void parseTypeAArgs(const char &_ch);

    static void executeCmd();
};

#endif