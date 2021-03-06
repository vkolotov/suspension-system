// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef AutomaticSuspensionSystem_H_
#define AutomaticSuspensionSystem_H_
#include "Arduino.h"
#include <math.h>
#include <avr/eeprom.h>
//add your includes for the project AutomaticSuspensionSystem here

void (*resetArduino)(void) = 0; //declare reset function @ address 0

extern int __bss_end;
extern void *__brkval;

int getFreeMemory() {
  int free_memory;

  if((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}

#include <Wire.h>
#include <ADXL345.h>

#include <Activity.h>

#include <Settings.h>
#include <ComplementaryFilter.h>
#include <Suspension.h>
#include <FrequencySystem.h>
#include <SpeedSystem.h>
#include <CadenceSystem.h>
#include <AccelerometerSystem.h>
#include <UnsprungAccelerometerSystem.h>
#include <SprungAccelerometerSystem.h>

#include <Button.h>
#include <Messages.h>

#include <avr/sleep.h>


#include <State.h>
#include <Automaton.h>
#include <Application.h>

#include <BluetoothSystem.h>

#include <SleepState.h>
#include <CommonState.h>
#include <ManualState.h>
#include <CDTState.h>
#include <AutomaticState.h>
#include <DescentState.h>
#include <TrailState.h>
#include <ClimbState.h>

#include <ActiveState.h>
#include <AbsorbState.h>
#include <BurnOutState.h>
#include <IdleState.h>
#include <PedalState.h>
#include <StandOverState.h>



//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif

void loop();
void setup();

#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project AutomaticSuspensionSystem here

//Do not add code below this line
#endif /* AutomaticSuspensionSystem_H_ */
