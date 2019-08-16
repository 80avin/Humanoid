#include <Arduino.h>
#include "ServoController.hpp"
#include <Servo.h>

#include "configs.hpp"

#if DEBUG
#include "Logs.hpp"
#endif

/**
 * @brief 
 * 
 */
servo_data_2t ServoController::data2[SERVO_CHANNELS];

/**
 * @brief 
 * 
 */
const PROGMEM uint8_t ServoController::servoPins[SERVO_CHANNELS] = SERVO_PINS;

/**
 * "bound" array is located in FLASH memory and store information about the maximum
 * and minimum pulse width that could be set for each channel.
 */
const PROGMEM uint16_t
  ServoController::bound[SERVO_CHANNELS][BOUND_SIZE] = SERVO_BOUNDS_US;

/**
 * @brief Construct a new servo data 2t::servo data 2t object
 * 
 */
servo_data_2t::servo_data_2t(){
  currentUs = targetUs = startUs = duration = startTime = 0;
  firstMove = true;
}

void servo_data_2t::set(const uint16_t &_targetUs, const uint32_t &_duration, const uint8_t &_polynomial=2){
  startUs = currentUs;
  targetUs = _targetUs;
  duration = _duration;
  polynomial = _polynomial;
  firstMove = true;
}

/**
 * @brief 
 * 
 */
void servo_data_2t::update(){
  uint32_t currentTime = millis();
  if(firstMove){
    startTime = currentTime;
    currentUs = (duration == 0)?targetUs:startUs;
    firstMove = false;
  }
  else{
    if(duration==0)
      currentUs = targetUs;
    else{
        float fracTime = float(currentTime-startTime)/duration;
        if(fracTime>=1.0){
            duration=0;
            currentUs=targetUs;
        }
        else if(polynomial==2){  // S/2 = u(T/2)+1/2*a(T/2)(T/2) => a=4S/T^2 => s/(tu-su) = {2*(t/D)^2, 1-2*(1-t/D)^2}
            currentUs = (fracTime <= 0.5)?(startUs + ((float(targetUs)-startUs)*2.0*fracTime*fracTime)):
                                 (targetUs - ((float(targetUs)-startUs)*2.0*(1-fracTime)*(1-fracTime)));
        }
        else{
            currentUs = uint16_t(startUs + ((float(targetUs) - startUs)*fracTime));
        }
        // LOG_SERIAL.print(F(" FT: "));
        // LOG_SERIAL.println(fracTime);
      }
  }
  servo.writeMicroseconds(currentUs);
}


/**
 * Initializes class's fields.
 * It programs each pin as an OUTPUT pin.
 * It sends a reset pulse to the 4017
 * It starts Timer1 interrupt.
 */
void ServoController::begin() {
#if DEBUG_I
LOG_I("ServoController::begin", "IN")
#endif
  for(int i=0; i<SERVO_CHANNELS; ++i){
    data2[i].servo.attach( pgm_read_byte(servoPins+i),raw_readMinWidth(i), raw_readMaxWidth(i));
    //data2[i].targetUs = data2[i].startUs = raw_degToUs(i, raw_getDefaultPos(i));
    data2[i].duration = 0;
  }
}

/**
 * Checks if a valid channel is passed.
 *
 * @param _ch channel index.
 * @return false if it's a valid channel, false otherwise.
 */
inline bool ServoController::isValidChannel(const uint8_t &_ch) {
  return (_ch < SERVO_CHANNELS);
}

/**
 * Updates a channel to a new pulse width, the class will continue to pulse the
 * channel with this value for the lifetime of the MPU or until writeWidth or
 * writeAngle is called again to update the value.
 *
 * @param _ch channel index.
 * @param _us pulse width to set.
 * @param _calibration if true it applies the width without any software adjustment.
 *  It's a special mode used for calibration purposes only.
 */
void ServoController::writeWidth(const uint8_t &_ch, uint16_t _us, const bool &_calibration) {
  if(!isValidChannel(_ch)) {
    return;
  }
  if(!_calibration) {
    _us = raw_validWidth(_ch, _us);
  }
  raw_writeWidth(_ch, _us);
}

/**
 * Updates a channel to a new angle, the class will continue to pulse the
 * channel with this value for the lifetime of the MPU or until writeWidth or
 * writeAngle is called again to update the value.
 *
 * @param _ch channel index.
 * @param _deg angle to set.
 */
void ServoController::writeAngle(const uint8_t &_ch, uint16_t _deg) {
  if(!isValidChannel(_ch)) {
    return;
  }
  _deg = raw_validAngle(_ch, _deg);
  raw_writeWidth(_ch, raw_degToUs(_ch, _deg) );
}

/**
 * Reads previously set pulse width for a channel.
 *
 * @param _ch channel index.
 * @return the channel's pulse width.
 */
uint16_t ServoController::readWidth(const uint8_t &_ch) {
  if(!isValidChannel(_ch)) {
    return 0;
  }
  uint16_t _us = raw_readWidth(_ch);
  return _us;
}

/**
 * Reads previously set angle for a channel.
 *
 * @param _ch channel index.
 * @return the channel's angle.
 */
uint16_t ServoController::readAngle(const uint8_t &_ch) {
  if(!isValidChannel(_ch)) {
    return (uint16_t)-1.0;
  }
  uint16_t _us = raw_readWidth(_ch);
  return raw_usToDeg(_ch, _us);
}

/**
 * Sets a certain pulse width smoothly for a channel in a predeterminated number
 * of millisecond.
 *
 * @param _ch channel index.
 * @param _us pulse width to set.
 * @param _time time to sweep.
 * @return none.
 */
void ServoController::sweepWidth(const uint8_t &_ch, uint16_t _us, const uint16_t &_time, const bool &_calibration) {
  if(!isValidChannel(_ch)) {
    return;
  }
  if(!_calibration) {
    _us = raw_validWidth(_ch, _us);
  }
  raw_sweepWidth(_ch, _us, _time);
}

/**
 * Sets an angle smoothly for a channel in a predeterminated number of
 * millisecond.
 *
 * @param _ch channel index.
 * @param _deg angle to set.
 * @param _time time to sweep.
 */
void ServoController::sweepAngle(const uint8_t &_ch, uint16_t _deg, const uint16_t &_time) {
  if(!isValidChannel(_ch)) {
    return;
  }
  _deg = raw_validAngle(_ch, _deg);
  raw_sweepWidth(_ch, raw_degToUs(_ch, _deg), _time);
}

/**
 * Keeps a channel in the actual position for a predeterminated number
 * of millisecond.
 *
 * @param _ch channel index.
 * @param _time time to sweep.
 */
void ServoController::wait(const uint8_t &_ch, const uint16_t &_time) {
  if(!isValidChannel(_ch)) {
    return;
  }
  raw_wait(_ch, _time);
}

/**
 * Gets the minimum pulse width that could be set to a channel.
 *
 * @param _ch channel index.
 * @return min pulse width.
 */
uint16_t ServoController::readMinWidth(const uint8_t &_ch) {
  if(!isValidChannel(_ch)) {
    return 0;
  }
  return raw_readMinWidth(_ch);
}

/**
 * Gets the maximum pulse width that could be set to a channel.
 *
 * @param _ch channel index.
 * @return max pulse width.
 */
uint16_t ServoController::readMaxWidth(const uint8_t &_ch) {
  if(!isValidChannel(_ch)) {
    return 0;
  }
  return raw_readMaxWidth(_ch);
}

/**
 * Checks if a specific channel has reached the planned position.
 *
 * @param _ch channel index.
 * @return false if the position has been reached, false otherwise.
 */
bool ServoController::isMoving(const uint8_t &_ch) {
  if(!isValidChannel(_ch)) {
    return false;
  }
  return (data2[_ch].currentUs != data2[_ch].targetUs);
}

/**
 * @brief 
 * 
 */
bool ServoController::detach(const uint8_t &_ch){
  if(!isValidChannel(_ch) && data2[_ch].servo.attached()){
    return false;
  }
  raw_detach(_ch);
  return true;
}

/**
 * @brief 
 * 
 */
void ServoController::detachAll(){
  for(int i=0; i< PART_SIZE; i++){
    if(data2[i].servo.attached()){
      raw_detach(i);
    }
  }
}

/**
 * @brief 
 * 
 */
bool ServoController::attach(const uint8_t &_ch){
  if(!isValidChannel(_ch) && !data2[_ch].servo.attached()){
    return false;
  }
  raw_attach(_ch);
  return true;
}

/**
 * @brief 
 * 
 */
void ServoController::attachAll(){
  for(int i=0; i<PART_SIZE; ++i){
    if(!data2[i].servo.attached()){
      raw_attach(i);
    }
  }
}
 
/**
 * Converts deg to us.
 *
 * @param _ch channel index.
 * @param _deg angle to set.
 * @return microseconds.
 */
inline uint16_t ServoController::raw_degToUs(const uint8_t &_ch,
                                         const uint16_t &_deg) {
  uint16_t _minUs = raw_readMinWidth(_ch);
  return _minUs + 
          (float(_deg - MIN_SERVO_ANGLE))*
            (float(raw_readMaxWidth(_ch) - _minUs)/
              (MAX_SERVO_ANGLE - MIN_SERVO_ANGLE));
}

/**
 * Convert us to deg.
 *
 * @param _ch channel index.
 * @param _us pulse width to set.
 * @return angle.
 */
inline uint16_t ServoController::raw_usToDeg(const uint8_t &_ch,
                                         const uint16_t &_us) {
  uint16_t _minUs = raw_readMinWidth(_ch);
  return MIN_SERVO_ANGLE +
          (float(_us - _minUs)) * 
            (float(MAX_SERVO_ANGLE - MIN_SERVO_ANGLE) / 
              (raw_readMaxWidth(_ch) - _minUs));
}

/**
 * Constrain a pulse width into our limit.
 *
 * @param _ch channel index.
 * @param _us pulse width to set.
 * @return constrained microseconds.
 */
inline uint16_t ServoController::raw_validWidth(const uint8_t &_ch,
                                            const uint16_t &_us) {
  uint8_t _readus = raw_readMinWidth(_ch);
  if(_us < _readus) {
    return _readus;
  }
  _readus = raw_readMaxWidth(_ch);
  if(_us > _readus) {
    return _readus;
  }
  return _us;
}

/**
 * Constrain an angle into our limit.
 *
 * @param _ch channel index.
 * @param _deg angle to set.
 * @return constrained angle.
 */
inline uint16_t ServoController::raw_validAngle(const uint8_t &_ch,
                                            const uint16_t &_deg) {
  if(_deg < MIN_SERVO_ANGLE) {
    return MIN_SERVO_ANGLE;
  }
  if(_deg > MAX_SERVO_ANGLE) {
    return MAX_SERVO_ANGLE;
  }
  return _deg;
}

/**
 * See writeWidth
 *
 * @param _ch channel index.
 * @param _us pulse width to set.
 * @return none.
 */
inline void ServoController::raw_writeWidth(const uint8_t &_ch,
                                        const uint16_t &_us) {
  data2[_ch].set(_us, 0);
}

/**
 * See readWidth
 *
 * @param _ch channel index.
 * @return channel pulse width.
 */
inline uint16_t ServoController::raw_readWidth(const uint8_t &_ch) {
  return data2[_ch].currentUs;
}

//static uint32_t start[20];

/**
 * See sweepWidth
 *
 * @param _ch channel index.
 * @param _us pulse width to set.
 * @param _time time to sweep.
 */
inline void ServoController::raw_sweepWidth(const uint8_t &_ch, const uint16_t &_us,
                                        const uint32_t &_time) {
  data2[_ch].set(_us, _time);
  //Serial.println(' ');
  //Serial.println(_time);
  //start[_ch] = millis();
}

/**
 * See wait
 *
 * @param _ch channel index.
 * @param _time time to sweep.
 */
inline void ServoController::raw_wait(const uint8_t &_ch, const uint32_t &_time) {
  data2[_ch].set(data2[_ch].currentUs, _time);
}


/**
 * See readMinWidth.
 *
 * @param _ch channel index.
 * @return min pulse width.
 */
inline uint16_t ServoController::raw_readMinWidth(const uint8_t &_ch) {
  return pgm_read_word_near(&(bound[_ch][BOUND_MIN]));
}

/**
 * See readMaxWidth.
 *
 * @param _ch channel index.
 * @return max pulse width.
 */
inline uint16_t ServoController::raw_readMaxWidth(const uint8_t &_ch) {
  return pgm_read_word_near(&(bound[_ch][BOUND_MAX]));
}

inline void ServoController::raw_detach(const uint8_t &_ch){
  data2[_ch].servo.detach();
}

inline void ServoController::raw_attach(const uint8_t &_ch){
  data2[_ch].servo.attach( pgm_read_byte(servoPins+_ch),raw_readMinWidth(_ch), raw_readMaxWidth(_ch));
}

/**
 * See raw_interrupt.
 */
 
void ServoController::servoRoutine() {
#if DEBUG_I
  LOG_I("ServoController::servoRoutine", "IN")
#endif
  for(int i=0; i<SERVO_CHANNELS; ++i){
    data2[i].update();
    #if DEBUG_ANGLES
    LOG_SERIAL.print(" : ");
    LOG_SERIAL.print(raw_usToDeg(i, data2[i].currentUs));
    #endif
  }
  // LOG_SERIAL.print("\n minu = ");
  // LOG_SERIAL.print(raw_readMinWidth(0));
  // LOG_SERIAL.print(", maxu=");
  // LOG_SERIAL.print(raw_readMaxWidth(0));
  // LOG_SERIAL.print(" : 90d=");
  // LOG_SERIAL.print(raw_degToUs(0, 900));
  // LOG_SERIAL.print("u =");
  // LOG_SERIAL.print(raw_usToDeg(0, raw_degToUs(0, 900)));
  // LOG_SERIAL.print('d');
#if DEBUG_ANGLES
  LOG_SERIAL.print('\n');
#endif
}
