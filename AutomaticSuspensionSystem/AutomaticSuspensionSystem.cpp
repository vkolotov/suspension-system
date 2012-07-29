#include "AutomaticSuspensionSystem.h"

#include <StandardCplusplus.h>
#include <vector>

using namespace std;

AutomaticSuspension application;
vector<ThreadListener*> threadListeners;
unsigned long last;

void setup() {

	Serial.begin(9600);
	last = millis();
	application.init();
	threadListeners = application.getThreadListeners();

}

void loop() {

	for (int i = 0; i < threadListeners.size(); i++) {
		threadListeners[i]->update();
	}
	long currentTime = millis();
	if (currentTime - last > 5000) {
		last = currentTime;
		Serial.println("w");
	}

}
