#ifndef SERCOCONTROLLER_HPP
#define SERVOCONTROLLER_HPP

#include<Servo.h>
#include "configs.hpp"

#define MIN_SERVO_ANGLE             0
#define MAX_SERVO_ANGLE          1800     //180*10

#define BOUND_MIN 0
#define BOUND_MAX 1
#define BOUND_SIZE 2

struct servo_data_t {
  bool pulseReached;
  float eachusTicks;
  uint16_t lastUpdate;
  int32_t actionTicks;
  volatile bool updateDisabled;
  volatile int16_t deltaTicks;
  volatile float incrementTicks;
  volatile uint16_t pulseTicks;
};

class servo_data_2t {
  public:
    Servo servo;
    bool firstMove;
    uint16_t currentUs;
    uint16_t targetUs;
    uint16_t startUs;
    uint32_t duration;
    uint32_t startTime;
    uint8_t polynomial;
    servo_data_2t();
    void update();
    void set(const uint16_t &_targetUs, const uint32_t &_duration, const uint8_t &_polynomial=2);
    void clear();
};

class ServoController{
  public:
    static void begin();

    static bool     isValidChannel(const uint8_t &_ch);
    static void     writeWidth(const uint8_t &_ch, uint16_t _us, const bool &_calibration = false);
    static void     writeAngle(const uint8_t &_ch, uint16_t _deg);
    static uint16_t readWidth(const uint8_t &_ch);
    static uint16_t readAngle(const uint8_t &_ch);
    static void     sweepWidth(const uint8_t &_ch, uint16_t _us, const uint16_t &_time, const bool &_calibration=false);
    static void     sweepAngle(const uint8_t &_ch, uint16_t _deg, const uint16_t &_time);
    static void     wait(const uint8_t &_ch, const uint16_t &_time);
    static uint16_t readMinWidth(const uint8_t &_ch);
    static uint16_t readMaxWidth(const uint8_t &_ch);
    static bool     isMoving(const uint8_t &_ch);
    static bool     detach(const uint8_t &_ch);
    static void     detachAll();
    static bool     attach(const uint8_t &_ch);
    static void     attachAll();
    
    static void     servoRoutine();
    
  private:
    // No-one have to create an instance of this class as we use it as
    // a singleton, so we keep constructor as private.
    ServoController();

    // All raw_ function declared here do not check the validity of the passed
    // arguments for speed reasons. They are supposed to be used only internally.
    
    static uint16_t raw_degToUs(const uint8_t &_ch, const uint16_t &_deg);
    static uint16_t raw_usToDeg(const uint8_t &_ch, const uint16_t &_us);
    static uint16_t raw_validWidth(const uint8_t &_ch, const uint16_t &_us);
    static uint16_t raw_validAngle(const uint8_t &_ch, const uint16_t &_deg);
    
    static void     raw_writeWidth(const uint8_t &_ch, const uint16_t &_us);
    static uint16_t raw_readWidth(const uint8_t &_ch);
    static void     raw_sweepWidth(const uint8_t &_ch, const uint16_t &_us,
                               const uint32_t &_time);
    static void     raw_wait(const uint8_t &_ch, const uint32_t &_time);
    static uint16_t raw_readMinWidth(const uint8_t &_ch);
    static uint16_t raw_readMaxWidth(const uint8_t &_ch);
    static void     raw_attach(const uint8_t &_ch);
    static void     raw_detach(const uint8_t &_ch);
    
    static servo_data_2t data2[SERVO_CHANNELS];
    static const PROGMEM uint8_t servoPins[SERVO_CHANNELS];
    static const PROGMEM uint16_t bound[SERVO_CHANNELS][BOUND_SIZE];
};


#endif