#include "AutomaticSuspensionSystem.h"

AutomaticSuspension application;
vector<ThreadListener*> threadListeners;

void setup() {
	Serial.begin(9600);
	application.init();
	threadListeners = application.getThreadListeners();

}

void loop() {
	int priority;
	int priorityIndex = 0;
	for (vector<ThreadListener*>::iterator it = threadListeners.begin(); it < threadListeners.end(); it++) {
		ThreadListener* current = *it;
		priority = current->getPriority();
		if (priority <= priorityIndex) {
			current->update();
		}
	}
	if (priorityIndex >= 2) {
		priorityIndex = 0;
	} else {
		priorityIndex++;
	}
}
