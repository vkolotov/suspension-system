// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef AutomaticSuspensionSystem_H_
#define AutomaticSuspensionSystem_H_
#include "Arduino.h"
//add your includes for the project AutomaticSuspensionSystem here
#include <Settings.h>
//#include <AutomaticSuspension.h>
#include <Suspension.h>
#include <FrequencySystem.h>
#include <SpeedSystem.h>
#include <CadenceSystem.h>
#include <ForkAccelerometerSystem.h>

#include <Button.h>

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
void updateMudules();
void updateApplication();
//Do not add code below this line
#endif /* AutomaticSuspensionSystem_H_ */
