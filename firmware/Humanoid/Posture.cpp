#include "Arduino.h"
#include "Posture.hpp"
#include "ServoController.hpp"

#include "configs.hpp"

#if DEBUG
#include "Logs.hpp"
#endif

/**
 * pos array is located in FLASH momery and store information about the minimum,
 * and maximum angle avaible for a bodypart. It also store the default position
 * for each bodypart.
 */
body_pos_t
  Posture::pos[PART_SIZE][POS_SIZE] = SERVO_ANGLE_POS;

/**
 * offset array is located in FLASH momery and store information about the
 * angle offset for each bodypart in order to get all part alligned in case of
 * errors due to 3D printed pieces.
 */
// body_offset_t
//   Posture::offset[PART_SIZE] = SERVO_ANGLE_OFFSET;

/**
 * head and tail array is located in SRAM momery and store respectivley
 * information about the index of next free queue block and the index of the
 * last unfree block.
 */
uint8_t
  Posture::head[PART_SIZE],
  Posture::tail[PART_SIZE];

/**
 * queue array is located in SRAM momery and store information about the next
 * planned movment for each bodypart.
 */
block_t
  Posture::queue[PART_SIZE][BF_SIZE];

/**
 * Initializes class's fields.
 */
void Posture::begin() {
#if DEBUG_I
LOG_I("Posture::begin", "IN")
#endif
  setDefault();
  
  for(uint8_t _idx = 0; _idx < PART_SIZE; _idx++) {
    head[_idx] = tail[_idx] = 0;
  }
}

/**
 * Checks if valid index is passed.
 *
 * @param _idx body part index.
 * @return true if the queue is empty, false otherwise.
 */
inline bool Posture::isValidBodypart(const uint8_t &_idx) {
  return (_idx < PART_SIZE);
}


/**
 * Applies a movment for a bodypart.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 */
void Posture::setPos(uint8_t _idx, uint16_t _angle) {
  if(!isValidBodypart(_idx)) {
    return;
  }
  raw_setPos(_idx, _angle);
}

/**
 * Applies a sweep movement for a bodypart.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 * movment.
 */
void Posture::setSweep(uint8_t _idx, uint16_t _angle,
                            uint16_t _time) {
  if(!isValidBodypart(_idx)) {
    return;
  }
  _angle = raw_validAngle(_idx, _angle);
  raw_setSweep(_idx, _angle, _time);
}

/**
 * Sets all bodypart to their default position.
 */
void Posture::setDefault() {
  uint16_t _deg;
  for(uint8_t _idx = 0; _idx < PART_SIZE; _idx++) {
    _deg = raw_getDefaultPos(_idx);
    raw_setPos(_idx, _deg);
  }
}

/**
 * Sets the servo to the default position.
 *
 * @param _idx body part index.
 */
void Posture::setDefault(uint8_t _idx) {
  if(!isValidBodypart(_idx)) {
    return;
  }
  uint16_t _deg = raw_getDefaultPos(_idx);
  raw_setPos(_idx, _deg);
}


void Posture::sweepDefault(uint16_t _time){
  for(uint8_t _idx =0; _idx < PART_SIZE; ++_idx){
    raw_setSweep(_idx, raw_getDefaultPos(_idx), _time);
  }
}

void Posture::sweepDefault(uint8_t &_idx, const uint16_t &_time){
    if(!isValidBodypart(_idx)) {
      return;
    }
    raw_setSweep(_idx, raw_getDefaultPos(_idx), _time);
}

/**
 * Keeps a bodypart in his position for some time.
 *
 * @param _idx body part index.
 * @param _time duration of the movment.
 */
void Posture::setWait(uint8_t _idx, uint16_t _time) {
  if(!isValidBodypart(_idx)) {
    return;
  }
  raw_setWait(_idx, _time);
}

/**
 * Gets the actual position of a bodypart.
 *
 * @param _idx body part index.
 * @return channel angle.
 */
uint16_t Posture::getPos(uint8_t _idx) {
  if(!isValidBodypart(_idx)) {
    return 0;
  }
  return raw_getPos(_idx);
}

/**
 * Gets the minimum position for a bodypart.
 *
 * @param _idx body part index.
 * @return channel angle.
 */
uint16_t Posture::getMinPos(uint8_t _idx) {
  if(!isValidBodypart(_idx)) {
    return 0;
  }
  return raw_getMinPos(_idx);
}

/**
 * Gets the default position for a bodypart.
 *
 * @param _idx body part index.
 * @return channel angle.
 */
uint16_t Posture::getDefaultPos(uint8_t _idx) {
  if(!isValidBodypart(_idx)) {
    return 0;
  }
  return raw_getDefaultPos(_idx);
}

/**
 * Gets the maximum position for a bodypart.
 *
 * @param _idx body part index.
 * @return channel angle.
 */
uint16_t Posture::getMaxPos(uint8_t _idx) {
  if(!isValidBodypart(_idx)) {
    return 0;
  }
  return raw_getMaxPos(_idx);
}

/**
 * Checks if a bodypart is moving
 *
 * @param right or left body part, body part index.
 * @return true if the bodyport is moving.
 */
bool Posture::isMoving(uint8_t _idx) {
  if(!isValidBodypart(_idx)) {
    return false;
  }
  return raw_isMoving(_idx);
}

/**
 * Inserts a movement into the queue.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of the movment.
 * 
 * @return false if the queue is full or the bodypart is invalid.
 */
bool Posture::pushQueue(uint8_t _idx, uint16_t _angle,
                             uint16_t _time) {
  if(!isValidBodypart(_idx) || raw_isQueueFull(_idx)) {
    return false;
  }
  raw_pushQueue(_idx, _angle, _time);
  return true;
}

/**
 * Executes one movement from the queue.
 *
 * @param _idx body part index.
 * @return false if the queue is empty or the bodypart is invalid.
 */
bool Posture::popQueue(uint8_t _idx) {
  if(!isValidBodypart(_idx) || isQueueEmpty(_idx)) {
    return false;
  }
  raw_popQueue(_idx);
  return true;
}

/**
 * Checks if the queue is full.
 *
 * @param _idx body part index.
 * @return true if the queue is full, false otherwise.
 */
bool Posture::isQueueFull(uint8_t _idx) {
  if(!isValidBodypart(_idx)) {
    return true;
  }
  return raw_isQueueFull(_idx);
}

/**
 * Checks if the queue is empty.
 *
 * @param _idx body part index.
 * @return true if the queue is empty, false otherwise.
 */
bool Posture::isQueueEmpty(uint8_t _idx) {
  if(!isValidBodypart(_idx)) {
    return false;
  }
  return raw_isQueueEmpty(_idx);
}

/**
 * Contrains the angle into his bounds.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @return costrain angle.
 */
inline uint16_t Posture::raw_validAngle(
                                             const uint8_t &_idx,
                                             const uint16_t &_angle) {
  uint16_t _min = raw_getMinPos(_idx);
  uint16_t _max = raw_getMaxPos(_idx);/*
  int8_t _offset = pgm_read_word_near(&(offset[_idx]));
  if(_angle < _min) {
    if(_offset > 0) {
      return _min + _offset;
    }
    return _min;
  }
  if(_angle > _max) {
    if(_offset < 0) {
      return _max + _offset;
    }
    return _max;
  }
  if(_offset > 0) {
    if(_angle + _offset > _max) {
      return _max;
    }
  }
  else {
    if(-_offset > _angle) {
      return _min;
    }
    if(_angle + _offset < _min) {
      return _min;
    }
  }
  return _angle + _offset;*/
  if(_angle < _min){
      return _min;
  }
  else if(_angle > _max){
      return _max;
  }
  else{
      return _angle;
  }
}

/**
 * See setPos.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 */
inline void Posture::raw_setPos(const uint8_t &_idx,
                                     const uint16_t &_angle) {
  uint16_t _anglefix = raw_validAngle(_idx, _angle);
  ServoController::writeAngle(_idx, _anglefix);
}

/**
 * See setSweep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of the movment.
 */
inline void Posture::raw_setSweep(const uint8_t &_idx,
                                       const uint16_t &_angle,
                                       const uint16_t &_time) {
  ServoController::sweepAngle(_idx, _angle, _time);
}

/**
 * See setDefault.
 *
 * @param _idx body part index.
 */
inline void Posture::raw_setDefault(
                                         const uint8_t &_idx) {
  raw_setPos(_idx, raw_getDefaultPos(_idx));
}

/**
 * See setWait.
 *
 * @param _idx body part index.
 * @param _time duration of the movment.
 */
inline void Posture::raw_setWait(const uint8_t &_idx,
                                      const uint16_t &_time) {
  ServoController::wait(_idx, _time);
}

/**
 * See getPos
 *
 * @param _idx body part index.
 * @return channel angle.
 */
inline uint16_t Posture::raw_getPos(
                                         const uint8_t &_idx) {
  return ServoController::readAngle(_idx);
}

/**
 * See getMinPos
 *
 * @param _idx body part index.
 * @return channel angle.
 */
inline uint16_t Posture::raw_getMinPos(const uint8_t &_idx) {
  return pgm_read_word_near(&(pos[_idx][POS_MIN]));
}

/**
 * See getDefaultPos
 *
 * @param _idx body part index.
 * @return channel angle.
 */
inline uint16_t Posture::raw_getDefaultPos(const uint8_t &_idx) {
  return pgm_read_word_near(&(pos[_idx][POS_MED]));
}

/**
 * See getMaxPos
 *
 * @param _idx body part index.
 * @return channel angle.
 */
inline uint16_t Posture::raw_getMaxPos(const uint8_t &_idx) {
  return pgm_read_word_near(&(pos[_idx][POS_MAX]));
}

/**
 * See isMoving
 *
 * @param _idx body part index.
 * @return true if the bodyport is moving.
 */
inline bool Posture::raw_isMoving(const uint8_t &_idx) {
  return ServoController::isMoving(_idx);
}
 
/**
 * See pushQueue.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of the movment.
 */
inline void Posture::raw_pushQueue(const uint8_t &_idx,
                                        const uint16_t &_angle,
                                        const uint16_t &_time) {
  queue[_idx][head[_idx]].movAngle = _angle;
  queue[_idx][head[_idx]].movTime = _time;
  head[_idx] = nextQueue(head[_idx]);
}

/**
 *  See popQueue.
 *
 * @param _idx body part index.
 */
inline void Posture::raw_popQueue(const uint8_t &_idx) {
  if(queue[_idx][tail[_idx]].movAngle == INVALID_BODY_POS) {
    raw_setWait(_idx, queue[_idx][tail[_idx]].movTime);
  }
  else {
    raw_setSweep(_idx, queue[_idx][tail[_idx]].movAngle, 
                 queue[_idx][tail[_idx]].movTime);
  }
  tail[_idx] = nextQueue(tail[_idx]);
}

/**
 * See isQueueFull.
 *
 * @param _idx body part index.
 * @return true if the queue is full, false otherwise.
 */
inline bool Posture::raw_isQueueFull(const uint8_t &_idx) {
  return fullQueue(nextQueue(head[_idx]), tail[_idx]);
}

/**
 * See isQueueEmpty.
 *
 * @param _idx body part index.
 * @return true if the queue is empty, false otherwise.
 */
inline bool Posture::raw_isQueueEmpty(const uint8_t &_idx) {
  return emptyQueue(head[_idx], tail[_idx]);
}

/**
 * This routine is called by the loop and flush the movement queue.
 */
void Posture::movementPlanner() {
#if DEBUG_I
LOG_I("Posture::movementPlanner", "IN")
#endif
    // _idx will be initialized only once.
  static uint8_t _idx = 0;
  if(!isMoving(_idx) && !raw_isQueueEmpty(_idx)) {
    raw_popQueue(_idx);
  }
  if(++_idx >= PART_SIZE) {
    _idx = 0;
  }
  // just running it twice.
  if(!isMoving(_idx) && !raw_isQueueEmpty(_idx)) {
    raw_popQueue(_idx);
  }
  if(++_idx >= PART_SIZE) {
    _idx = 0;
  }
}
