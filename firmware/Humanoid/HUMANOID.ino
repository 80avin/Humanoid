/* Humanoid
* Move individual servos by degress*10
*   M1 i       // i = joint_number
*   M2 i a    // a = angle in degrees/10
*   M3 i a t     // t = animation time, in millis
*
* Play animation
*   P1 i       // i = animation number
*
*  Queue animation
*   Q1 i       // i = animation number
*
*  Move servo by us
*   U1 i        // 
*   U2 i a
*   U3 i a t
*
*
* TODO - Receive an animation 
* A0 <bytes> <data>; // bytes = length of data, if there's no semicolon after given number of bytes, the command is discarded
* format of <data> : the data will come as array of {joint, target, time, joint, target, time,...}
* joint is 1byte, target is 2byte, time is 2byte
*/