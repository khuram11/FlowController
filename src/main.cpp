#include <Arduino.h>
#include "main.h"


void setup()
{
  setupSerial();
  setupPins();
}

void loop()
{
  ImplementPID();
}


void setupPins(void)
{
  pinMode(ActuatorPin, OUTPUT);
  analogReadResolution(8);
}

feedback_t GetFeedback(void)
{
  uint32_t avg=0; 
  for (int i = 0 ; i< 30 ; i++) 
  {
    avg+= (uint8_t)analogRead(FeedbackPin);
    delay(5);
  }
  // Serial.println(avg);
  avg=avg/30;
  // Serial.println(avg);
  return avg;
}

void ActuatorCommand(ActuatorSignal_t act_signal)
{
  analogWrite(ActuatorPin, act_signal);
}

void ImplementPID() {
    if (InputReceived) {
        String inputcommand = "";
        uint8_t Setpoint = 0;
        
        // Read from serial port until no more data is available
        while (Serial.available()) {
            inputcommand += Serial.readString();
        }
        
        // Print the received command
        Serial.print("Command Received: ");
        Serial.println(inputcommand);

        // Check if the command is for setting light
        if (inputcommand.startsWith("SET Light :")) {
            // Extract setpoint from the command
            sscanf(inputcommand.c_str(), "SET Light : %hhu", &Setpoint);

            // Convert setpoint to actuator signal (scaled from percentage to 0-255)
            uint8_t actuator_signal = Setpoint * 255 / 100;
            
            Serial.println("Starting PID");
            PID_controller(Setpoint);
            Serial.println("PID Finished");
            
            // Print feedback
            Serial.print("Feedback: ");
            Serial.println(GetFeedback());
        } else {
            // Invalid command
            Serial.println("Invalid Serial Command for Output, Please use \"SET Light : X%\"");
        }
        
        // Reset input flag
        InputReceived = false;
    }
}


void SerialRXCallback(void)
{
  InputReceived = true ;
}
void setupSerial(void)
{
  Serial.begin(115200);
  Serial.println("Flow Controller Application beganning");
  Serial.onReceive(SerialRXCallback);
}




uint8_t FeedbackToInput(uint8_t value) {
    if (value >= 231)
        return 0;
    else if (value <= 35)
        return 100;
    else if (value == 36)
        return 90;
    else if (value == 37)
        return 80;
    else if (value == 44)
        return 60;
    else if (value == 51)
        return 50;
    else if (value == 57)
        return 40;
    else if (value == 66)
        return 30;
    else if (value == 77)
        return 20;
    else if (value == 94)
        return 10;
    else if (value == 115)
        return 5;
    else if (value == 171)
        return 1;
    else if (value <= 231 && value > 171)
        return (((value - 171) * 100) / (231 - 171));
    else if (value <= 171 && value > 115)
        return (((value - 115) * 100) / (171 - 115));
    else if (value <= 115 && value > 94)
        return (((value - 94) * 100) / (115 - 94));
    else if (value <= 94 && value > 77)
        return (((value - 77) * 100) / (94 - 77));
    else if (value <= 77 && value > 66)
        return (((value - 66) * 100) / (77 - 66));
    else if (value <= 66 && value > 57)
        return (((value - 57) * 100) / (66 - 57));
    else if (value <= 57 && value > 51)
        return (((value - 51) * 100) / (57 - 51));
    else if (value <= 51 && value > 44)
        return (((value - 44) * 100) / (51 - 44));
    else if (value <= 44 && value > 37)
        return (((value - 37) * 100) / (44 - 37));
    else if (value <= 37 && value > 35)
        return (((value - 35) * 100) / (37 - 35));
    else 
        return 0;
}

void PID_controller(uint8_t setpoint)
{
   error= abs((setpoint -FeedbackToInput(GetFeedback())));
   Serial.printf("Setpoint : %d \r\n",setpoint);
   Serial.printf("Feedback : %d \r\n",GetFeedback());
   Serial.printf("Feedback (In input form) : %d \r\n",FeedbackToInput(GetFeedback()));
   Serial.printf("Inital Error : %d \r\n",error);
    integral =0;
    prev_error =0;
    while ( error > Errorthreshold )
    {
      error = abs((setpoint -FeedbackToInput(GetFeedback())));
      integral += error ;
      double derivate = error - prev_error ;
      output =  Kp * error + Ki * integral + Kd * derivate;
      // ActuatorCommand(outputactua
      ActuatorCommand((output * 255 / 100));
      prev_error = error;
      Serial.printf("Error    : %d \n",error);
      // Serial.printf("Integral : %f \n",integral);
      // Serial.printf("derivate : %f \n",derivate);
      Serial.printf("output   : %d \n",output);
      delay(1000);
    }
}

