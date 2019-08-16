#ifndef ANIMSTORE_HPP
#define ANIMSTORE_HPP

#include "configs.hpp"


#if ANIMQUEUE_SIZE && !(ANIMQUEUE_SIZE & (ANIMQUEUE_SIZE - 1))   // If ANIMQUEUE_SIZE is a power of 2
  #define modAnimQueue(n) ((n) & (ANIMQUEUE_SIZE - 1))
#else
  #define modAnimQueue(n) ((n) % (ANIMQUEUE_SIZE))
#endif

#define nextAnimQueue(n) modAnimQueue(n + 1)
#define countAnimQueue(head,tail) modAnimQueue(head - tail + ANIMQUEUE_SIZE)
#define emptyAnimQueue(head,tail) (head == tail)
#define fullAnimQueue(head,tail) (nextQueue(head) == tail)



#define ANIM_NULL                   255

#define ANIM_STEPS_START            0
#define ANIM_STEPS_LOOP             1
#define ANIM_STEPS_END              2
#define ANIM_STEPS_DIV              3

#define ANIM_STEPS_PART             0
#define ANIM_STEPS_POS              1
#define ANIM_STEPS_TIME             2
#define ANIM_STEPS_INFO             3


struct anim_t {
  bool busyAnimation, endingAnimation;
  uint8_t activeAnimation, stepAnimation,
          startAnimation, loopAnimation, endAnimation;
  // uint16_t distAnimation, timeAnimation, angleAnimation;
};

typedef const PROGMEM uint8_t steps_size_t;
typedef const PROGMEM uint16_t steps_info_t;

// struct Frame_t{
//     angle_t joints[PART_SIZE];
//     uint16_t timeBefore;
// };

// struct Animation_t{
//     uint8_t numberOfFrames;
//     Frame_t* frames;
// };

class AnimStore{
    public:
        static void begin();

        static void applyAnimation(uint8_t _anim/* , uint16_t _dist, uint16_t _time, angle_t _angle = 0 */);
        static void clearAnimation(bool _force = false);
        static void executeAnimation();
// animation queue management
        static bool pushQueue(const uint8_t &_anim);
        static bool popQueue();
        static bool isQueueEmpty();
        static bool isQueueFull();
        static void overWriteQueue(const uint8_t &_anim);

        static void clearAll();
    private:
        AnimStore();

        static void nextStep(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        
        static void nextStepHELLO(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepTEST(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepSIT(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepSTAND(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepSTRETCH(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepFWW(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepSWL(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepSWR(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepLTURN(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepRTURN(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepCLTURN(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);
        static void nextStepCRTURN(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);

        static void nextStepCUSTOM(uint8_t &_idx, uint16_t &_angle, uint16_t &_time);

        static anim_t anim;
        static steps_size_t steps_size[ANIM_SIZE][ANIM_STEPS_DIV];
        static steps_info_t steps_TEST[ANIM_TEST_SIZE][3];
        static steps_info_t steps_HELLO[ANIM_HELLO_SIZE][3];
        static steps_info_t steps_SIT[ANIM_SIT_SIZE][3];
        static steps_info_t steps_STAND[ANIM_STAND_SIZE][3];
        static steps_info_t steps_STRETCH[ANIM_STRETCH_SIZE][3];
        static steps_info_t steps_FWW[ANIM_FWW_SIZE][3];
        static steps_info_t steps_SWL[ANIM_SWL_SIZE][3];
        static steps_info_t steps_SWR[ANIM_SWR_SIZE][3];
        static steps_info_t steps_LTURN[ANIM_LTURN_SIZE][3];
        static steps_info_t steps_RTURN[ANIM_RTURN_SIZE][3];
        static steps_info_t steps_CLTURN[ANIM_CLTURN_SIZE][3];
        static steps_info_t steps_CRTURN[ANIM_CRTURN_SIZE][3];

        static uint16_t steps_CUSTOM[100][3];

        static bool isValidAnimation(const uint8_t &_anim);
        static void clearQueue();
        static void raw_pushQueue(const uint8_t &_anim);
        static void raw_popQueue();

        static uint8_t animQueue[ANIMQUEUE_SIZE];
        static uint8_t head, tail;
};



#endif