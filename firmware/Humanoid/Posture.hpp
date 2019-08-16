#ifndef POSTURE_HPP
#define POSTURE_HPP

#include "configs.hpp"

typedef uint16_t angle_t;


#define POS_MIN                 0
#define POS_MED                 1
#define POS_MAX                 2
#define POS_SIZE                3

#if BF_SIZE && !(BF_SIZE & (BF_SIZE - 1))   // If BF_SIZE is a power of 2
  #define modQueue(n) ((n) & (BF_SIZE - 1))
#else
  #define modQueue(n) ((n) % (BF_SIZE))
#endif

#define nextQueue(n) modQueue(n + 1)
#define countQueue(head,tail) modQueue(head - tail + BF_SIZE)
#define emptyQueue(head,tail) (head == tail)
#define fullQueue(head,tail) (nextQueue(head) == tail)

#define INVALID_BODY_POS 65535

struct block_t {
  uint16_t movAngle, movTime;
};

typedef const PROGMEM uint16_t body_pos_t;
typedef const PROGMEM int8_t body_offset_t;

class Posture {
  public:
    static void begin();
    static bool isValidBodypart(const uint8_t &_idx);
    static void setPos(uint8_t _idx, uint16_t _angle);
    static void setSweep(uint8_t _idx, uint16_t _angle,
                         uint16_t _time);
    static void setDefault();
    static void setDefault(uint8_t _idx);
    static void sweepDefault(uint16_t _time);
    static void sweepDefault(uint8_t &_idx, const uint16_t &_time);
    static void setWait(uint8_t _idx, uint16_t _time);
    static uint16_t getPos( uint8_t _idx);
    static uint16_t getMinPos(uint8_t _idx);
    static uint16_t getDefaultPos(uint8_t _idx);
    static uint16_t getMaxPos(uint8_t _idx);
    static bool isMoving(uint8_t _idx);

    static bool pushQueue( uint8_t _idx, uint16_t _angle,
                          uint16_t _time);
    static bool popQueue( uint8_t _idx);
    static bool isQueueFull( uint8_t _idx);
    static bool isQueueEmpty( uint8_t _idx);

    static void movementPlanner();
  private:
    // No-one have to create an instance of this class as we use it as
    // a singleton, so we keep constructor as private.
    Posture();

    static uint16_t raw_validAngle(const uint8_t &_idx,
                                   const uint16_t &_angle);

    static void raw_setPos(const uint8_t &_idx,
                           const uint16_t &_angle);
    static void raw_setSweep(const uint8_t &_idx,
                             const uint16_t &_angle, const uint16_t &_time);
    static void raw_setDefault(const uint8_t &_idx);
    static void raw_setWait(const uint8_t &_idx,
                            const uint16_t &_time);
    static uint16_t raw_getPos(const uint8_t &_idx);
    static uint16_t raw_getMinPos(const uint8_t &_idx);
    static uint16_t raw_getDefaultPos(const uint8_t &_idx);
    static uint16_t raw_getMaxPos(const uint8_t &_idx);
    static bool raw_isMoving(const uint8_t &_idx);

    static void raw_pushQueue(const uint8_t &_idx,
                              const uint16_t &_angle, const uint16_t &_time);
    static void raw_popQueue(const uint8_t &_idx);
    static bool raw_isQueueFull(const uint8_t &_idx);
    static bool raw_isQueueEmpty(const uint8_t &_idx);

    static body_pos_t pos[PART_SIZE][POS_SIZE];
    //static body_offset_t offset[PART_SIZE];
    static uint8_t head[PART_SIZE], tail[PART_SIZE];
    static block_t queue[PART_SIZE][BF_SIZE];
};
#endif