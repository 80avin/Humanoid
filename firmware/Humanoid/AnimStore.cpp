#include<Arduino.h>
#include "Posture.hpp"

#include "AnimStore.hpp"
#include "configs.hpp"

#if DEBUG
#include "Logs.hpp"
#endif
/**
 * anim struct is located in SRAM momery and store information about the current
 * applied animation, the struct is cleared after each command is successfully executed.
 */
anim_t
  AnimStore::anim;

/**
 * "steps_size" array is located in FLASH memory and store information about the
 * number of steps of each animation.
 */
steps_size_t
  AnimStore::steps_size[ANIM_SIZE][ANIM_STEPS_DIV] = ANIM_STEPS_SIZE;


steps_info_t AnimStore::steps_TEST[ANIM_TEST_SIZE][3] = ANIM_TEST_STEPS;
steps_info_t AnimStore::steps_HELLO[ANIM_HELLO_SIZE][3] = ANIM_HELLO_STEPS;
steps_info_t AnimStore::steps_SIT[ANIM_SIT_SIZE][3] = ANIM_SIT_STEPS;
steps_info_t AnimStore::steps_STAND[ANIM_STAND_SIZE][3] = ANIM_STAND_STEPS;
steps_info_t AnimStore::steps_STRETCH[ANIM_STRETCH_SIZE][3] = ANIM_STRETCH_STEPS;
steps_info_t AnimStore::steps_FWW[ANIM_FWW_SIZE][3] = ANIM_FWW_STEPS;
steps_info_t AnimStore::steps_SWL[ANIM_SWL_SIZE][3] = ANIM_SWL_STEPS;
steps_info_t AnimStore::steps_SWR[ANIM_SWR_SIZE][3] = ANIM_SWR_STEPS;
steps_info_t AnimStore::steps_LTURN[ANIM_LTURN_SIZE][3] = ANIM_LTURN_STEPS;
steps_info_t AnimStore::steps_RTURN[ANIM_RTURN_SIZE][3] = ANIM_RTURN_STEPS;
steps_info_t AnimStore::steps_CLTURN[ANIM_CLTURN_SIZE][3] = ANIM_CLTURN_STEPS;
steps_info_t AnimStore::steps_CRTURN[ANIM_CRTURN_SIZE][3] = ANIM_CRTURN_STEPS;
uint16_t AnimStore::steps_CUSTOM[100][3] = {{0}};

uint8_t AnimStore::animQueue[ANIMQUEUE_SIZE];
uint8_t AnimStore::head, AnimStore::tail;
/**
 * Initializes class's fields.
 */
void AnimStore::begin() {
#if DEBUG_I
LOG_I("AnimStore::begin", "IN")
#endif
  clearAnimation(true);
}
/**
 * Applies a specific animation.
 * 
 * @param _anim animation id.
 * @param _dist distance to travel (for anmations that moves the robot).
 * @param _time the duration of the animation.
 * @param _angle the angle to trvale (for anmations that rotates the robot).
 */
void AnimStore::applyAnimation(uint8_t _anim/*, uint16_t _dist,
                                    uint16_t _time, uint16_t _angle */) {
  anim.activeAnimation = _anim;
  anim.stepAnimation = 0;
  // anim.timeAnimation = _time;
  // anim.distAnimation = _dist;
  // anim.angleAnimation = _angle;

  anim.startAnimation = pgm_read_word_near(&(steps_size[_anim][ANIM_STEPS_START]));
  anim.loopAnimation = pgm_read_word_near(&(steps_size[_anim][ANIM_STEPS_LOOP]));
  anim.endAnimation = pgm_read_word_near(&(steps_size[_anim][ANIM_STEPS_END]));
}

/**
 * Stops an animation.
 *
 * @param _force .
 */
void AnimStore::clearAnimation(bool _force) {
  if(_force) {
    anim.activeAnimation = ANIM_NULL;
    anim.endingAnimation = false;
    anim.stepAnimation = 0;
    // anim.timeAnimation = 0;
    // anim.distAnimation = 0;
    // anim.angleAnimation = 0;
  }
  else {
    anim.endingAnimation = true;
  }
}

/**
 * Processes the current applied animation.
 */
void AnimStore::executeAnimation() {
#if DEBUG_I
LOG_I("AnimStore::executeAnimation", "IN")
#endif
  static uint8_t _idx;
  static uint16_t _angle, _time;
  if(anim.activeAnimation == ANIM_NULL) {
    popQueue();
    return;
  }
  if(!anim.busyAnimation) {
    nextStep(_idx, _angle, _time);
  }
  bool _inserted = Posture::pushQueue(_idx, _angle, _time);
  if(!_inserted) {
    anim.busyAnimation = true;
    return;
  }
  anim.busyAnimation = false;
}

/**
 * Computes the next step for the current applied animation.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStep(uint8_t &_idx, uint16_t &_angle,
                              uint16_t &_time) {
  if(!anim.endingAnimation && anim.loopAnimation == 0 && anim.stepAnimation == anim.startAnimation) {
    return;
  }
  else if(!anim.endingAnimation && anim.stepAnimation == anim.startAnimation + anim.loopAnimation) {
    anim.stepAnimation = anim.startAnimation;
  }
  else if(anim.stepAnimation == anim.startAnimation + anim.loopAnimation + anim.endAnimation) {
    clearAnimation(true);
    return;
  }

  switch(anim.activeAnimation) {
    case ANIM_TEST: nextStepTEST(_idx, _angle, _time); break;
    case ANIM_HELLO: nextStepHELLO(_idx, _angle, _time); break;
    case ANIM_SIT: nextStepSIT(_idx, _angle, _time); break;
    case ANIM_STAND: nextStepSTAND(_idx, _angle, _time); break;
    case ANIM_STRETCH: nextStepSTRETCH(_idx, _angle, _time); break;
    case ANIM_FWW: nextStepFWW(_idx, _angle, _time); break;
    case ANIM_SWL: nextStepSWL(_idx, _angle, _time); break;
    case ANIM_SWR: nextStepSWR(_idx, _angle, _time); break;
    case ANIM_LTURN: nextStepLTURN(_idx, _angle, _time); break;
    case ANIM_RTURN: nextStepRTURN(_idx, _angle, _time); break;
    case ANIM_CLTURN: nextStepCLTURN(_idx, _angle, _time); break;
    case ANIM_CRTURN: nextStepCRTURN(_idx, _angle, _time); break;
  }
  anim.stepAnimation++;
}

// To create the steps code, make a HTML textarea (t) and paste the code of nextStepFWW in it
// then, run following javascript
// txt=t.value
// t.value=''
// for(i=0;i<motions.length;i++){
//  t.value+='\n'
//  t.value += txt.replace(/FWW/g,motions[i])
// }



/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepTEST(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_TEST[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_TEST[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_TEST[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepHELLO(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_HELLO[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_HELLO[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_HELLO[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepSIT(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_SIT[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_SIT[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_SIT[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepSTAND(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_STAND[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_STAND[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_STAND[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepSTRETCH(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_STRETCH[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_STRETCH[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_STRETCH[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepFWW(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_FWW[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_FWW[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_FWW[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepSWL(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_SWL[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_SWL[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_SWL[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepSWR(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_SWR[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_SWR[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_SWR[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepLTURN(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_LTURN[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_LTURN[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_LTURN[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepRTURN(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_RTURN[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_RTURN[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_RTURN[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepCLTURN(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_CLTURN[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_CLTURN[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_CLTURN[anim.stepAnimation][ANIM_STEPS_TIME]));
}

/**
 * See nextStep.
 *
 * @param _idx body part index.
 * @param _angle angle*10 to set.
 * @param _time duration of
 */
void AnimStore::nextStepCRTURN(uint8_t &_idx, uint16_t &_angle,
                                 uint16_t &_time) {
  _idx = pgm_read_word_near(&(steps_CRTURN[anim.stepAnimation][ANIM_STEPS_PART]));
  _angle = pgm_read_word_near(&(steps_CRTURN[anim.stepAnimation][ANIM_STEPS_POS]));
  _time = pgm_read_word_near(&(steps_CRTURN[anim.stepAnimation][ANIM_STEPS_TIME]));
}


/**
 * @brief 
 * 
 * @param _idx 
 * @return true 
 * @return false 
 */
bool AnimStore::pushQueue(const uint8_t &_anim){
  if(!isValidAnimation(_anim) || isQueueFull())
    return false;
  raw_pushQueue(_anim);
  return true;
}

bool AnimStore::popQueue(){
  if(isQueueEmpty())
    return false;
  raw_popQueue();
  return true;
}

inline bool AnimStore::isQueueEmpty(){
  return emptyAnimQueue(head, tail);
}

inline bool AnimStore::isQueueFull(){
  return fullAnimQueue(head, tail);
}

inline void AnimStore::overWriteQueue(const uint8_t &_anim){
  applyAnimation(_anim);
  clearQueue();
}

void AnimStore::clearAll(){
  clearQueue();
  clearAnimation(true);
}

/**
 * @brief 
 * 
 * @param _anim Animation index
 * @return true If animation is valid
 */
inline bool AnimStore::isValidAnimation(const uint8_t &_anim){
  return _anim < ANIM_SIZE;
}

inline void AnimStore::clearQueue(){
  head = tail;
}

inline void AnimStore::raw_pushQueue(const uint8_t &_anim){
  animQueue[head]=_anim;
  head = nextAnimQueue(head);
}

inline void AnimStore::raw_popQueue(){
  applyAnimation(animQueue[tail]);
  tail = nextAnimQueue(tail);
}
