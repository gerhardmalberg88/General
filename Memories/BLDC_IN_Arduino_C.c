// IMPORTANT INFORMATION
// Hall sensors can be connected from +5V towards GND as A, B, C or C, B, A. (C, B, A) is used in project.
// Phasepin1 is always on HIGH side and counting starts from there towards bottom.
// Tune: Throttle, Manual start speed, PWM both on Manualmode and Automode,

// PWM WRITE PINS
const int Phase_pin1 = PA6;
const int Phase_pin3 = PA7;
const int Phase_pin5 = PB0;
const int Regen_PWM_pin = PB1;
// DIGITAL WRITE PINS
const int Phase_pin2 = PB12;
const int Phase_pin4 = PB13;
const int Phase_pin6 = PB14;
const int BadPowerLed_pin = 0; // Vehicle MCU
const int Regen_ONOFF_pin = 0; // Vehicle MCU or manually controlled switch
const int TESTLED = PC13;
// ANALOG READ PINS
const int Throttle_Read_pin = PA0; 
const int Battery_Voltage_Read_Pin = 0; // Vehicle MCU
const int Current_Read_pin = 0; // Vehicle MCU
const int Temp_Read_pin = 0; // Vehicle MCU
// DIGITAL READ PINS
const int Regen_Switch_pin = 0; // Vehicle MCU or manually controlled switch
const int Hall_A_pin = PA1; // GREEN
const int Hall_B_pin = PA2; // YELLOW
const int Hall_C_pin = PA3; // BLUE
// SWITCHES
int POWER;
int ALL_PINS;
// Declared DATA
unsigned int Throttle_Value; 
unsigned int Throttle_Threshold = 500; // from 725-1847 // Changing?
unsigned int Battery_Voltage; 
unsigned int Battery_SOC;
unsigned int Current_Value;
unsigned int LOW_Voltage;
unsigned int HIGH_Voltage;
unsigned int Good_Current;
unsigned int Over_Current;
unsigned int Regen_PWM_Value = 100; // TEST USE! CALIBRATE!
unsigned long Timer_Manual = 0;
unsigned int Timer_Manual_Change;
unsigned int Manual_interval = 100; // Total manual control time between every Phase change (Millis)
unsigned int Manual_Timer_state = 0;
unsigned int Phase1 = 1;
unsigned int Phase2 = 2;
unsigned int Phase3 = 3;
unsigned int Phase4 = 4;
unsigned int Phase5 = 5;
unsigned int Phase6 = 6;
unsigned int Phase_state = Phase1;
unsigned int Previous_Phase = Phase6;
unsigned int ON = 1;
unsigned int OFF = 0;
unsigned int state;
unsigned int A_Hall_Sensor_Data;
unsigned int B_Hall_Sensor_Data;
unsigned int C_Hall_Sensor_Data;
unsigned int Hall_Sensor_ON_Value = 1; // This is set to 2V as HIGH input  // 5V input 1V = 51 // 3.3V input 1V = 77
unsigned int Previous_A_Hall_Sensor_Data;
unsigned int Previous_B_Hall_Sensor_Data;
unsigned int Previous_C_Hall_Sensor_Data;
unsigned int PWM_to_AUTO;
unsigned int PWM_to_MANUAL;
unsigned int MANUALSpeed;
unsigned int FULLSPEED_TEST = 250;
unsigned int Counter_Manual;
unsigned int AUTOSWITCH_data;

// TEST
//const int RegenModepin = PC13;
//const int MANUALMODEPIN = PC14;
//const int AUTOPIN = PC15;

void setup() {

//// Start serial communication (At least 115200!)
//Serial.begin(4800); 
// Declare DigitalWrite PINS
pinMode(Phase_pin2, OUTPUT);
pinMode(Phase_pin4, OUTPUT);
pinMode(Phase_pin6, OUTPUT);
pinMode(TESTLED, OUTPUT);
// Declare PWMWrite PINS
pinMode(Phase_pin1, OUTPUT);
pinMode(Phase_pin3, OUTPUT);
pinMode(Phase_pin5, OUTPUT);
pinMode(Regen_PWM_pin, OUTPUT);
// Declare DigitalRead PINS
pinMode(Hall_A_pin, INPUT);
pinMode(Hall_B_pin, INPUT);
pinMode(Hall_C_pin, INPUT);
// Declare AnalogRead PINS
pinMode(Throttle_Read_pin, INPUT);
// Write POWER OFF
// Write all pins OFF
// Read Battery 
// Read Current
// Write POWER ON
// Write manual timer state OFF (For manual phase control timer)
Manual_Timer_state = 0;
// Write first manual Phase to 1 (Start with phase 1)
Phase_state = 1;
// Write Previous phase state to 6 (Required for starting)
Previous_Phase = 6;
// All TEST conditions ON+++++++++++

}

void loop() {

// ****************************SYSTEM****************************
// Read Battery -> If SOC = good -> Turn POWER ON
// Read Current -> If Current = overcurrent -> All pins OFF
// While loop (wait till conditions match)
//void Read_Power_State();
// Read Throttle -> If <= Throttle_Threshold -> All pins OFF & Regen ON -> else Manual or Auto Control
Throttle_Value = analogRead(Throttle_Read_pin);
// Map PWM data for AUTO & MANUAL control
PWM_to_AUTO = map(Throttle_Value, Throttle_Threshold, 2500, 10, 150);
PWM_to_MANUAL = map(Throttle_Value, Throttle_Threshold, 2500, 25, 25);
MANUALSpeed = map(Throttle_Value, Throttle_Threshold, 3000, 150000, 60000);
//Serial.print(Throttle_Value);
// Declare previous Hall sensor data
// Read all Hall sensors
// Declare previous Hall sensor data
Previous_A_Hall_Sensor_Data = A_Hall_Sensor_Data;
Previous_B_Hall_Sensor_Data = B_Hall_Sensor_Data;
Previous_C_Hall_Sensor_Data = C_Hall_Sensor_Data;
// Read all hall sensors
A_Hall_Sensor_Data = digitalRead(Hall_A_pin);
B_Hall_Sensor_Data = digitalRead(Hall_B_pin);
C_Hall_Sensor_Data = digitalRead(Hall_C_pin);
//Serial.print(" ");
//Serial.print(A_Hall_Sensor_Data);
//Serial.print(" ");
//Serial.print(B_Hall_Sensor_Data);
//Serial.print(" ");
//Serial.println(C_Hall_Sensor_Data);
if (Throttle_Value < Throttle_Threshold) {
    Regen_State(ON);
    // Write Regen -> If throttle = OFF -> Auto & Manual pins OFF -> Regen ON -> Regen PWM ON
    AUTO_MANUAL_PINS_OFF();
    // Write Manual_Timer_state OFF if regen is switched OFF and manual control ON
    Manual_Timer_state = 0;
    // Read & Write Hall sensor data
    HallSensor_READANDWRITE_PHASE();
    digitalWrite(TESTLED, LOW);
}

if (Throttle_Value >= Throttle_Threshold) {
  // Read Hall Sensor -> Write Auto Mode when state is changed
  Hall_Sensor_READANDDECIDE();
  // Write Regen Mode OFF
  Regen_State(OFF);
  // Read Motor state or NOT
  // Start from zero
  if (Manual_Timer_state == 0){
    // Define Timer
    Timer_Manual = micros();
    // Write Manual_Timer_state ON
    Manual_Timer_state = 1;   
}
  // Calculate change value
  Timer_Manual_Change = micros() - Timer_Manual;
  // Read time between previous change -> If matches -> Write next Phase
  if(Manual_Timer_state == 1 && Timer_Manual_Change >= MANUALSpeed){
      // If motor is not moving -> Write Manual -> AUTO & REGEN pins OFF -> Phase pins ON (interval) -> Manual PWM ON
      AUTO_REGEN_PINS_OFF();
      // Write Next phase
      // Save Current Phase state
      Phase_Control();
      // Read Hall Sensor -> Write Auto Mode when state is changed
      Hall_Sensor_READANDDECIDE();
    // If Manual_Timer_state is not 2 -> Write Manual_Timer_state OFF to start timer again for next phase END PHASE
    if(Manual_Timer_state != 2){
      Manual_Timer_state = 0;
    }
}

// State & previous state
// Write Auto -> Manual & REGEN pins OFF -> Auto PWM ON
if (Manual_Timer_state == 2){
  // Write commands for AUTO MODE
Phase_Control_Hall_Sensor_NOLOGIC();
digitalWrite(TESTLED, HIGH);
  }
  
}

//// Read Temp -> If High -> Turn Blower ON -> All pins OFF for 2sec
//void Temp_Blower_state (int Temp_state){} 

// Regen PWM Value (3-state switch)
}

// **************************** MOTOR CONTROL FUNCTIONS (MASTER)****************************
// Read POWER state
void Read_Power_State(){
  // I2C read data
  // Address data to POWER STATE
  // If POWER STATE is good -> Continue -> else while loop till conditions match  
}
// ALL_PINS_OFF
void ALL_PINS_OFF(){// CHECK ALL FUNCTIONS********************************
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, LOW);
  analogWrite(Regen_PWM_pin, 0);
}

// Auto & Manual pins OFF
void AUTO_MANUAL_PINS_OFF(){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, LOW);
}

// Auto & Regen pins OFF
void AUTO_REGEN_PINS_OFF(){
  analogWrite(Regen_PWM_pin, 0);
}

// Manual & Regen pins OFF
void MANUAL_REGEN_PINS_OFF(){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, LOW);
  analogWrite(Regen_PWM_pin, 0);
}

// Battery SOC TEST (100 Ohm Resistor short for amount of time)
void SOC_TEST(){
  
}

// Regen State
void Regen_State(int state){
  if(state == 1){
    // REGEN PWM (3-State Switch)
    analogWrite(Regen_PWM_pin, Regen_PWM_Value);
  }
  if(state == 0){
    analogWrite(Regen_PWM_pin, 0);
  }
}

// Read Temp -> If High -> Turn Blower ON -> All pins OFF for 2sec
void Temp_Blower_state (int Temp_state){
  
}

 // PHASE CONTROL TO OTHER DIRECTION
void Phase_Control (){
  if (Phase_state == 6){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, HIGH);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, PWM_to_MANUAL);
  digitalWrite(Phase_pin6, LOW);
  Previous_Phase = Phase_state;
  Phase_state = 7; // Next Phase
  }
  if (Phase_state == 5){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, HIGH);
  analogWrite(Phase_pin3, PWM_to_MANUAL);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, LOW);
  Previous_Phase = Phase_state;
  Phase_state = 6; // Next Phase
  }
  if (Phase_state == 4){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, PWM_to_MANUAL);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, HIGH);
  Previous_Phase = Phase_state;
  Phase_state = 5; // Next Phase
  } 
  if (Phase_state == Phase3){
  analogWrite(Phase_pin1, PWM_to_MANUAL);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, HIGH);
  Previous_Phase = Phase_state;
  Phase_state = Phase4; // Next Phase
  }
  if (Phase_state == 2){
  analogWrite(Phase_pin1, PWM_to_MANUAL);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, HIGH);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, LOW);
  Previous_Phase = Phase_state;
  Phase_state = Phase3; // Next Phase
  }
  if (Phase_state == 1){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, HIGH);
  analogWrite(Phase_pin5, PWM_to_MANUAL);
  digitalWrite(Phase_pin6, LOW);
  Previous_Phase = Phase_state;
  Phase_state = 2; // Next Phase
  }
  if(Phase_state == 7){
    Phase_state = 1;
  }
}

//void Phase_Control (){
//  if (Phase_state == 6){
//  analogWrite(Phase_pin1, 0);
//  digitalWrite(Phase_pin2, LOW);
//  analogWrite(Phase_pin3, 0);
//  digitalWrite(Phase_pin4, HIGH);
//  analogWrite(Phase_pin5, PWM_to_MANUAL);
//  digitalWrite(Phase_pin6, LOW);
//  Previous_Phase = Phase_state;
//  Phase_state = 7; // Next Phase
//  }
//  if (Phase_state == 5){
//  analogWrite(Phase_pin1, PWM_to_MANUAL);
//  digitalWrite(Phase_pin2, LOW);
//  analogWrite(Phase_pin3, 0);
//  digitalWrite(Phase_pin4, HIGH);
//  analogWrite(Phase_pin5, 0);
//  digitalWrite(Phase_pin6, LOW);
//  Previous_Phase = Phase_state;
//  Phase_state = 6; // Next Phase
//  }
//  if (Phase_state == 4){
//  analogWrite(Phase_pin1, 0);
//  digitalWrite(Phase_pin2, LOW);
//  analogWrite(Phase_pin3, PWM_to_MANUAL);
//  digitalWrite(Phase_pin4, LOW);
//  analogWrite(Phase_pin5, 0);
//  digitalWrite(Phase_pin6, HIGH);
//  Previous_Phase = Phase_state;
//  Phase_state = 5; // Next Phase
//  } 
//  if (Phase_state == Phase3){
//  analogWrite(Phase_pin1, PWM_to_MANUAL);
//  digitalWrite(Phase_pin2, LOW);
//  analogWrite(Phase_pin3, 0);
//  digitalWrite(Phase_pin4, LOW);
//  analogWrite(Phase_pin5, 0);
//  digitalWrite(Phase_pin6, HIGH);
//  Previous_Phase = Phase_state;
//  Phase_state = Phase4; // Next Phase
//  }
//  if (Phase_state == 2){
//  analogWrite(Phase_pin1, 0);
//  digitalWrite(Phase_pin2, HIGH);
//  analogWrite(Phase_pin3, PWM_to_MANUAL);
//  digitalWrite(Phase_pin4, LOW);
//  analogWrite(Phase_pin5, 0);
//  digitalWrite(Phase_pin6, LOW);
//  Previous_Phase = Phase_state;
//  Phase_state = Phase3; // Next Phase
//  }
//  if (Phase_state == 1){
//  analogWrite(Phase_pin1, 0);
//  digitalWrite(Phase_pin2, HIGH);
//  analogWrite(Phase_pin3, 0);
//  digitalWrite(Phase_pin4, LOW);
//  analogWrite(Phase_pin5, PWM_to_MANUAL);
//  digitalWrite(Phase_pin6, LOW);
//  Previous_Phase = Phase_state;
//  Phase_state = 2; // Next Phase
//  }
//  if(Phase_state == 7){
//    Phase_state = 1;
//  }
//}

//void Phase_Control_Hall_Sensor(){
//  // Read all hall sensors
//A_Hall_Sensor_Data = digitalRead(Hall_A_pin);
//B_Hall_Sensor_Data = digitalRead(Hall_B_pin);
//C_Hall_Sensor_Data = digitalRead(Hall_C_pin);

void Phase_Control_Hall_Sensor_NOLOGIC(){
    // Read all hall sensors
A_Hall_Sensor_Data = digitalRead(Hall_A_pin);
B_Hall_Sensor_Data = digitalRead(Hall_B_pin);
C_Hall_Sensor_Data = digitalRead(Hall_C_pin);
    // Write Phase depending on sensor data
  // Phase 1
  if(A_Hall_Sensor_Data == Hall_Sensor_ON_Value && B_Hall_Sensor_Data != Hall_Sensor_ON_Value && C_Hall_Sensor_Data != Hall_Sensor_ON_Value){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, HIGH);
  analogWrite(Phase_pin5, PWM_to_AUTO);
  digitalWrite(Phase_pin6, LOW);
  Previous_Phase = Phase_state;
  Phase_state = Phase2; // Next Phase
  }
  // Phase 2
  if(A_Hall_Sensor_Data == Hall_Sensor_ON_Value && B_Hall_Sensor_Data == Hall_Sensor_ON_Value && C_Hall_Sensor_Data != Hall_Sensor_ON_Value){
  analogWrite(Phase_pin1, PWM_to_AUTO);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, HIGH);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, LOW);
  Previous_Phase = Phase_state;
  Phase_state = Phase3; // Next Phase
  }
  // Phase 3
  if(A_Hall_Sensor_Data != Hall_Sensor_ON_Value && B_Hall_Sensor_Data == Hall_Sensor_ON_Value && C_Hall_Sensor_Data != Hall_Sensor_ON_Value){
  analogWrite(Phase_pin1, PWM_to_AUTO);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, HIGH);
  Previous_Phase = Phase_state;
  Phase_state = Phase4; // Next Phase
  }
  // Phase 4
  if(A_Hall_Sensor_Data != Hall_Sensor_ON_Value && B_Hall_Sensor_Data == Hall_Sensor_ON_Value && C_Hall_Sensor_Data == Hall_Sensor_ON_Value){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, LOW);
  analogWrite(Phase_pin3, PWM_to_AUTO);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, HIGH);
  Previous_Phase = Phase_state;
  Phase_state = Phase5; // Next Phase
  }
  // Phase 5
  if(A_Hall_Sensor_Data != Hall_Sensor_ON_Value && B_Hall_Sensor_Data != Hall_Sensor_ON_Value && C_Hall_Sensor_Data == Hall_Sensor_ON_Value){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, HIGH);
  analogWrite(Phase_pin3, PWM_to_AUTO);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, 0);
  digitalWrite(Phase_pin6, LOW);
  Previous_Phase = Phase_state;
  Phase_state = Phase6; // Next Phase
  }
  // Phase 6
  if(A_Hall_Sensor_Data == Hall_Sensor_ON_Value && B_Hall_Sensor_Data != Hall_Sensor_ON_Value && C_Hall_Sensor_Data == Hall_Sensor_ON_Value){
  analogWrite(Phase_pin1, 0);
  digitalWrite(Phase_pin2, HIGH);
  analogWrite(Phase_pin3, 0);
  digitalWrite(Phase_pin4, LOW);
  analogWrite(Phase_pin5, PWM_to_AUTO);
  digitalWrite(Phase_pin6, LOW);
  Previous_Phase = Phase_state;
  Phase_state = Phase1; // Next Phase
//  // Write Manual_Timer_state to 2 for exiting manual control
//  Manual_Timer_state = 2;
  }
}

void HallSensor_READANDWRITE_PHASE(){
   if(A_Hall_Sensor_Data == Hall_Sensor_ON_Value && B_Hall_Sensor_Data != Hall_Sensor_ON_Value && C_Hall_Sensor_Data != Hall_Sensor_ON_Value){
    Phase_state = 1;
   }
   if(A_Hall_Sensor_Data == Hall_Sensor_ON_Value && B_Hall_Sensor_Data == Hall_Sensor_ON_Value && C_Hall_Sensor_Data != Hall_Sensor_ON_Value){
    Phase_state = 2;
   }
   if(A_Hall_Sensor_Data != Hall_Sensor_ON_Value && B_Hall_Sensor_Data == Hall_Sensor_ON_Value && C_Hall_Sensor_Data != Hall_Sensor_ON_Value){
    Phase_state = 3;
   }
   if(A_Hall_Sensor_Data != Hall_Sensor_ON_Value && B_Hall_Sensor_Data == Hall_Sensor_ON_Value && C_Hall_Sensor_Data == Hall_Sensor_ON_Value){
    Phase_state = 4;
   }
   if(A_Hall_Sensor_Data != Hall_Sensor_ON_Value && B_Hall_Sensor_Data != Hall_Sensor_ON_Value && C_Hall_Sensor_Data == Hall_Sensor_ON_Value){
    Phase_state = 5;
   }
   if(A_Hall_Sensor_Data == Hall_Sensor_ON_Value && B_Hall_Sensor_Data != Hall_Sensor_ON_Value && C_Hall_Sensor_Data == Hall_Sensor_ON_Value){
    Phase_state = 6;
   }
   
}

void Hall_Sensor_READANDDECIDE(){
      // Read Hall sensor data -> if state changes Write Manual_Timer_state = 2
      if(Previous_A_Hall_Sensor_Data != A_Hall_Sensor_Data || Previous_B_Hall_Sensor_Data != B_Hall_Sensor_Data || Previous_C_Hall_Sensor_Data != C_Hall_Sensor_Data){
        Manual_Timer_state = 2;
      }
}


// ****************************VEHICLE MCU FUNCTIONS (SLAVE)****************************
// POWER Function
void POWER_state(){
// While loop (till conditions matched)
while(Battery_Voltage >= LOW_Voltage && Current_Value <= Over_Current){
    // Read Battery 
    Battery_Voltage = analogRead(Battery_Voltage_Read_Pin);
    // Read Current
    Current_Value = analogRead(Current_Read_pin);
    // STATE HYSTERESIS!!!!!
    if (Battery_Voltage <= LOW_Voltage || Current_Value >= Over_Current){
        POWER = 0;
}
    if (Battery_Voltage >= LOW_Voltage && Current_Value <= Over_Current){
        POWER = 1;
}

    if(POWER == 0){
        // Write All pins OFF
        void ALL_PINS_OFF();
        // Write LEDS
        digitalWrite(BadPowerLed_pin, HIGH);
        // I2C send to turn OFF every pin +++++++++++++++++++++++++++++
}
}
if(POWER == 1){
    // Write LEDS
    digitalWrite(BadPowerLed_pin, LOW);
    // I2C send to conditions match +++++++++++++++++++++++++++++
}
}











/*
Ignore:

    // Configure LED pin
    
pinMode(analogInPin, INPUT_ANALOG);
pinMode(pwmOutPin, PWM);
pinMode(digitalOut, OUTPUT);

    // read the analog in value:
    sensorValue = analogRead(analogInPin);
    // map it to the range of the analog out:
    outputValue = map(sensorValue, 0, 1023, 0, 65535);
    // change the analog out value:
    pwmWrite(pwmOutPin, outputValue);

delay() 
delayMicroseconds() 
micros() 
millis() 

//digitalWrite(PWMPIN, HIGH);
//delay(1000);
//digitalWrite(PWMPIN, LOW);
//delay(1000);

//analogWrite(PWMPIN, pwmvalue);
//delay(1000);
//analogWrite(PWMPIN, pwmvalue2);
//delay(1000);

int PWMPIN = PB15;
int inputpin;
int readpin;

long pwmvalue = 65500;
long pwmvalue2 = 8500;


SYSTEM FLOW:

Manual Mode
- PWM pin
- Phase pin

Auto Mode
- PWM pin

Regenerate Mode
- Regenerate ON
- Regenerate PWM

Overcurrent Protection
Overheat Protection
Battery Read (SOC)


Pins: 
1 = x = Manual Mode PWM
2 = x = Auto Mode PWM
3 = x = Regen 1 / Auto 0
4 = x = Regen PWM

Pin declaration in STM32 (example)
analogpin = PB1;
digitalpin = PA2;

IMPORTANT INFO:
Throttle Read data varies between 1069-3185

*/