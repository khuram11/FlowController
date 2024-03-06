#ifndef __MAIN_H__
#define __MAIN_H__
/******************************************************************/ 
#include "Typedef.h"
#include "Defines.h"
/******************************************************************/ 








/******************************************************************/ 
bool InputReceived = false;
/******************************************************************/ 


/******************************************************************/ 
void setupPins(void);
feedback_t GetFeedback(void);
void ActuatorCommand(ActuatorSignal_t act_signal);
void ImplementPID(void);
void SerialRXCallback(void);
void setupSerial(void);
uint8_t FeedbackToInput(int value);
void PID_controller(uint8_t setpoint);
/******************************************************************/ 




/******************************************************************/ 
double Kp=2, Ki=1, Kd=1 , integral, prev_error;
uint8_t error;
uint8_t Errorthreshold = 1;
uint8_t output;
/******************************************************************/ 


#endif //__MAIN_H__