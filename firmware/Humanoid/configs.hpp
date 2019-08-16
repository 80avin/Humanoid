#ifndef CONFIGS_HPP
#define CONFIGS_HPP

// general 
#define DEBUG_COMMAND   0
#define DEBUG_ANGLES    1
#define DEBUG_FREQUENCY 0
#define DEBUG_I 0
#define DEBUG ( DEBUG_COMMAND || DEBUG_ANGLES || DEBUG_FREQUENCY || DEBUG_I )
// configs for AnimStore

#define ANIMQUEUE_SIZE 8
// indices of the animations
#define ANIM_TEST                   0
#define ANIM_HELLO                  1
#define ANIM_SIT                    2
#define ANIM_STAND                  3
#define ANIM_STRETCH                4
#define ANIM_FWW                    5                       // ForWard Walk
#define ANIM_SWL                    6                       // Side Walk Left
#define ANIM_SWR                    7                       // Side Walk Right
#define ANIM_LTURN                  8                       // Left TURN
#define ANIM_RTURN                  9                       // Right TURN
#define ANIM_CLTURN                 10                       // Curved Left TURN
#define ANIM_CRTURN                 11                      // Curved Right TURN
#define ANIM_CUSTOM                 255

#define ANIM_SIZE                   12                      // Number of Animations

// ANIM_STEPS_{START, LOOP, END}
#define ANIM_STEPS_SIZE {         \
  {0, 32, 0},                     \
  {0, 0, 0},                     \
  {19, 0, 4},                      \
  {0, 0, 0},                      \
  {0, 0, 0},                      \
  {40, 0, 0},                      \
  {0, 0, 0},                      \
  {0, 0, 0},                      \
  {19, 0, 4},                      \
  {56, 0, 0},                     \
  {5, 2, 0},                      \
  {8, 9, 0}                      \
}

#define ANIM_TEST_SIZE                   32                            // TEST
#define ANIM_HELLO_SIZE                  6                            //  
#define ANIM_SIT_SIZE                    23                          // TODO: make sit animation
#define ANIM_STAND_SIZE                  1                            //
#define ANIM_STRETCH_SIZE                1                            //
#define ANIM_FWW_SIZE                    40                            // ForWard Walk
#define ANIM_SWR_SIZE                    1                            // Side Walk Right
#define ANIM_SWL_SIZE                    1                            // Side Walk Left
#define ANIM_LTURN_SIZE                  1                            // Left TURN
#define ANIM_RTURN_SIZE                  1                            // Right TURN
#define ANIM_CLTURN_SIZE                 1                            // Curved Left TURN
#define ANIM_CRTURN_SIZE                 1                            // Curved Right TURN

#define ANIM_TEST_STEPS {               \
  {0,0,4000},                           \
  {1,0,4000},                           \
  {2,0,4000},                           \
  {3,0,4000},                           \
  {4,0,4000},                           \
  {5,0,4000},                           \
  {6,0,4000},                           \
  {7,0,4000},                           \
  {8,0,4000},                           \
  {9,0,4000},                           \
  {10,0,4000},                           \
  {11,0,4000},                           \
  {12,0,4000},                           \
  {13,0,4000},                           \
  {14,0,4000},                           \
  {15,0,4000},                           \
                                        \
  {0,1800,4000},                           \
  {1,1800,4000},                           \
  {2,1800,4000},                           \
  {3,1800,4000},                           \
  {4,1800,4000},                           \
  {5,1800,4000},                           \
  {6,1800,4000},                           \
  {7,1800,4000},                           \
  {8,1800,4000},                           \
  {9,1800,4000},                           \
  {10,1800,4000},                           \
  {11,1800,4000},                           \
  {12,1800,4000},                           \
  {13,1800,4000},                           \
  {14,1800,4000},                           \
  {15,1800,4000}                            \
}
#define ANIM_HELLO_STEPS {              \
  {PART_TO_CH(Rarm1), 1600, 1000},                  \
  {PART_TO_CH(Rarm2), 400, 1000},                   \
  {PART_TO_CH(Rarm3), 900, 1000},                   \
  {PART_TO_CH(Rarm2), 500, 1000},                   \
  {PART_TO_CH(Rarm2), INVALID_BODY_POS, 1000},      \
  {PART_TO_CH(Rarm2), 1300, 1000}                  \
}
#define ANIM_SIT_STEPS {                    \
    {PART_TO_CH(Rleg1), 520, 1000},         \
    {PART_TO_CH(Rleg2),1150,1000},          \
    {PART_TO_CH(Rleg3),900,1000},           \
    {PART_TO_CH(Rleg4),1120,1000},          \
                                            \
    {PART_TO_CH(Lleg1), 1270, 1000},        \
    {PART_TO_CH(Lleg2),650,1000},           \
    {PART_TO_CH(Lleg3), 900,1000},          \
    {PART_TO_CH(Lleg4),670,1000},           \
                                            \
    {PART_TO_CH(Rleg2),1150-250,1000},      \
    {PART_TO_CH(Rleg3),900-250,1000},       \
    {PART_TO_CH(Lleg2),650+250,1000},       \
    {PART_TO_CH(Lleg3), 900+250,1000}       \
}
#define ANIM_STAND_STEPS {              \
  {PART_SIZE, INVALID_BODY_POS, 0}      \
}
#define ANIM_STRETCH_STEPS {            \
  {PART_SIZE, INVALID_BODY_POS, 0}      \
}
#define ANIM_FWW_STEPS {                \
/* move to default legs */ \
{PART_TO_CH(Rleg1), 520, 1000},\
{PART_TO_CH(Rleg2),1150,1000},\
{PART_TO_CH(Rleg3),900,1000},\
{PART_TO_CH(Rleg4),1120,1000},\
{PART_TO_CH(Lleg1), 1270, 1000},\
{PART_TO_CH(Lleg2),650,1000},\
{PART_TO_CH(Lleg3), 900,1000},\
{PART_TO_CH(Lleg4),670,1000},\
/* tilt right 15deg */\
{PART_TO_CH(Rleg1), 520-150, 500},\
{PART_TO_CH(Rleg2),INVALID_BODY_POS,500},\
{PART_TO_CH(Rleg3),INVALID_BODY_POS,500},\
{PART_TO_CH(Rleg4),1120-150,500},\
{PART_TO_CH(Lleg1), 1270-150, 500},\
{PART_TO_CH(Lleg2),INVALID_BODY_POS,600},/*500 for this & 100 for next.*/\
{PART_TO_CH(Lleg3), INVALID_BODY_POS,600},\
{PART_TO_CH(Lleg4),670-150,500},\
/* raise/front left by 20deg */ \
{PART_TO_CH(Rleg1), INVALID_BODY_POS, 500},\
{PART_TO_CH(Rleg2),1150-200,500},\
{PART_TO_CH(Rleg3),900+200,500},\
{PART_TO_CH(Rleg4),INVALID_BODY_POS,500},\
{PART_TO_CH(Lleg1),INVALID_BODY_POS, 500},\
{PART_TO_CH(Lleg2),INVALID_BODY_POS,500},\
{PART_TO_CH(Lleg3),INVALID_BODY_POS,500},\
{PART_TO_CH(Lleg4),INVALID_BODY_POS,500},\
/* start of loop*/ \
/* tilt left by 30deg */ \
{PART_TO_CH(Rleg1), 520+150, 500},\
{PART_TO_CH(Rleg2),INVALID_BODY_POS,500},\
{PART_TO_CH(Rleg3),INVALID_BODY_POS,500},\
{PART_TO_CH(Rleg4),1120+150,500},\
{PART_TO_CH(Lleg1),1270+150, 500},\
{PART_TO_CH(Lleg2),INVALID_BODY_POS,500},\
{PART_TO_CH(Lleg3),INVALID_BODY_POS,500},\
{PART_TO_CH(Lleg4),670+150,500},\
/* raise/front right & back left by 40deg */ \
{PART_TO_CH(Rleg1), INVALID_BODY_POS, 500},\
{PART_TO_CH(Rleg2),1150+200,500},\
{PART_TO_CH(Rleg3),900-200,500},\
{PART_TO_CH(Rleg4),INVALID_BODY_POS,500},\
{PART_TO_CH(Lleg1), INVALID_BODY_POS, 500},\
{PART_TO_CH(Lleg2),650+200,500},\
{PART_TO_CH(Lleg3), 900-200,500},\
{PART_TO_CH(Lleg4),INVALID_BODY_POS,500}\
}
#define ANIM_SWR_STEPS {                \
  {PART_SIZE, INVALID_BODY_POS, 0}      \
}
#define ANIM_SWL_STEPS {                \
  {PART_SIZE, INVALID_BODY_POS, 0}      \
}
#define ANIM_LTURN_STEPS {              \
  {PART_SIZE, INVALID_BODY_POS, 0}      \
}
#define ANIM_RTURN_STEPS {              \
  {PART_SIZE, INVALID_BODY_POS, 0}      \
}
#define ANIM_CLTURN_STEPS {             \
  {PART_SIZE, INVALID_BODY_POS, 0}      \
}
#define ANIM_CRTURN_STEPS {             \
  {PART_SIZE, INVALID_BODY_POS, 0}      \
}


// CommandParser configs

#define COMMAND_SERIAL        Serial1
#define COMMAND_SERIAL_BAUD   115200

// Humanoid configs

// Logs configs

#define LOG_SERIAL            Serial
#define LOG_SERIAL_BAUD       115200


// Posture configs

#define Larm1  1    // left hand
#define Larm2  2
#define Larm3  3    // left shoulder
#define Lleg1  4
#define Lleg2  5
#define Lleg3  6
#define Lleg4  7
#define Lleg5  8    // left foot
#define Rarm1  9   // right hand
#define Rarm2  10
#define Rarm3  11   // right shoulder
#define Rleg1  12
#define Rleg2  13
#define Rleg3  14
#define Rleg4  15
#define Rleg5  16
#define Head   17
#define Body   18
#define PART_SIZE 16   // number of joints


#define PART_TO_CH(_p) (_p-1) //head = part 0
#define BF_SIZE                 4       // size of Posture queue for each part

// {min, default, max}_deg * 10
#define SERVO_ANGLE_POS {																		                              \
    								{0,900,1350},{0,0,1200},{0,800,1600},													        \
       {450,1000,1000},{0,650,1550},{0,0,1400},{250,1400,1800},{500,900,1100},						\
                                                                                          \
    								{450,900,1800},{600,1800,1800},{200,1000,1800},												\
       {1000,1000,1550},{250,1150,1800},{400,1800,1800},{0,400,1550},{700,900,1300},			\
                    }
// jt10 value = {600,1800,1800}
/*
 if axes are inverted. x-> front, y-> left, z-> up
 true => joint axis is +ve of coordinate system, i.e joint moves the next link acc. to +ve coordinate axis
 {
   True, True , False,
   False, False, True, True, True, 
   True, True, True, 
   False, True, False, False, True
 }
*/

// ServoController configs
// number of servo to control
#define SERVO_CHANNELS      PART_SIZE                                 // Number of channels.

#define SERVO_PINS {											        \
									        53, 51, 49,							        \
                47, 45, 43, 41, 39,								    \
                  37, 35, 33,									    \
												31, 29, 27, 25, 23	  \
							}

// {0deg, 180deg}*10 i.e {0, 1800}
#define SERVO_BOUNDS_US{																				                \
								{544,2400},{544,2400},{544,2400},																\
       {544,2400},{544,2400},{544,2400},{544,2400},{544,2400},								  \
                                                                                \
		            {544,2400},{544,2400},{544,2400},								                \
			 {544,2400},{544,2400},{544,2400},{544,2400},{544,2400}	                  \
                    }

/*
  raw data for Poses
  * Sit Pose: {900, 0, 800, 1000, 900, 1400, 250, 900, 900, 1800, 1000, 1000, 900, 400, 1550, 900}
 */

#endif
