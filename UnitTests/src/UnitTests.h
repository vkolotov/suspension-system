// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef UnitTests_H_
#define UnitTests_H_
#include "Arduino.h"
//add your includes for the project UnitTests here


//add your includes for the project AutomaticSuspensionSystem here

#include <AutomaticSuspensionSystem.h>

#include <SpeedSystemMock.h>
#include <CadenceSystemMock.h>
#include <ButtonMock.h>
#include <UnsprungAccelerometerSysteMock.h>

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
