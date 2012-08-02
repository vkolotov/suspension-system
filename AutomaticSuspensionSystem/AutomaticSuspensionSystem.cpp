#include "AutomaticSuspensionSystem.h"

AutomaticSuspension application;
unsigned long time = 0;

void setup() {

	Serial.begin(9600);
	application.init();

}

void loop() {
	time = millis();
	application.frontSuspension->update(time);
	application.rearSuspension->update(time);
	application.cadenceSystem->update(time);
	application.forkAccelerometerSystem->update(time);
	application.frontButton->update(time);
	application.modeButton->update(time);
	application.rearButton->update(time);
	application.update(time);

}
