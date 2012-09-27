// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef UnitTests_H_
#define UnitTests_H_
#include "Arduino.h"
//add your includes for the project UnitTests here

#include <math.h>
#include <avr/eeprom.h>
//add your includes for the project AutomaticSuspensionSystem here

#include <Wire.h>
#include <ADXL345.h>

#include <Activity.h>

#include <Settings.h>
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

#include <CalibrationState.h>
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


#include <SpeedSystemMock.h>

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project UnitTests here




//Do not add code below this line
#endif /* UnitTests_H_ */
