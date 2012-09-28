// Do not remove the include below
#include "UnitTests.h"

uint16_t testIndex = 0;
void assertTrue(bool value, String message = "") {
	Serial.print(testIndex++);
	Serial.print(" T ");
	if (!value) {
		Serial.print("FAIL ");
		Serial.println(message);
	} else {
		Serial.println("OK");
	}
}

template <typename Type>
void assertEquals(Type reference, Type value, String message = "") {
	Serial.print(testIndex++);
	Serial.print(" E ");
	if (reference != value) {
		Serial.print("FAIL ");
		Serial.print("Expected: ");
		Serial.print(reference);
		Serial.print(" Was: ");
		Serial.print(value);
		Serial.print(" ");
		Serial.println(message);
	} else {
		Serial.println("OK");
	}
}

void setup() {
	Serial.begin(38400);
	//delay(2000);
	Serial.println("Starting...");
	initConfiguration();
}


void testConfiguration() {
	Configuration* config = resetConfiguration();
	Serial.println("Conf test --");
	assertEquals<uint8_t>(2, config->system.mode);
	assertTrue(config->semiautomaticStateConfig.averageDegreeMeasuringPeriod == 2000);

	config->system.mode = 1;
	config->semiautomaticStateConfig.averageDegreeMeasuringPeriod = 1000;

	saveConfiguration(config);
	delete config;
	config = loadConfiguration();

	assertEquals<uint8_t>(1, config->system.mode);
	assertTrue(config->semiautomaticStateConfig.averageDegreeMeasuringPeriod == 1000);

	delete config;
	config = resetConfiguration();
	assertEquals<uint8_t>(2, config->system.mode);
	assertTrue(config->semiautomaticStateConfig.averageDegreeMeasuringPeriod == 2000);

	Serial.println("Done --");
	delete config;
}


void testSpeedSystem() {
	Serial.println("Speed test --");
	Configuration* config = resetConfiguration();
	SpeedSystemMock mock(&(config->speed));
	mock.update(500);
	assertEquals<uint32_t>(0.0f, mock.getAverageSpeed());
	assertTrue(!mock.isProcessing());
	mock.pinValue = true;
	mock.update(1000);
	assertEquals<uint32_t>(0.0f, mock.getAverageSpeed());
	assertTrue(mock.isProcessing());
	mock.update(1010);
	assertEquals<uint32_t>(0.0f, mock.getAverageSpeed());
	assertTrue(mock.isProcessing());
	mock.update(1020);
	assertEquals<uint32_t>(0.0f, mock.getAverageSpeed());
	assertTrue(mock.isProcessing());
	mock.pinValue = false;
	mock.update(1300);
	mock.pinValue = true;
	mock.update(2000);
	assertEquals<float>(WHEEL_LENGTH / 1000.0f, mock.getAverageSpeed());
	assertEquals<float>(WHEEL_LENGTH * 3.6f / 1000.0f, mock.getAverageSpeedKmH());
	assertTrue(mock.isProcessing());
	mock.pinValue = false;
	mock.update(2500);
	assertEquals<float>(WHEEL_LENGTH * 3.6f / 1000.0f, mock.getAverageSpeedKmH());
	assertTrue(mock.isProcessing());
	mock.update(2000 + MAXIMUM_SPEED_TIME);
	assertEquals<int>(int (0.0f * 100), int (mock.getAverageSpeed() * 100));
	assertTrue(!mock.isProcessing());
	Serial.println("Done --");
	delete config;
}

void testCadenceSystem() {
	Serial.println("Cadence test --");
	Configuration* config = resetConfiguration();
	CadenceSystemMock mock(&(config->cadence));
	assertTrue(!mock.isProcessing());
	mock.update(500);
	assertEquals<uint16_t>(0, mock.getCadence());
	assertTrue(!mock.isProcessing());
	mock.pinValue = true;
	mock.update(1000);
	assertEquals<uint16_t>(0, mock.getCadence());
	assertTrue(mock.isProcessing());
	mock.update(1010);
	assertEquals<uint16_t>(0, mock.getCadence());
	assertTrue(mock.isProcessing());
	mock.update(1020);
	assertEquals<uint16_t>(0, mock.getCadence());
	assertTrue(mock.isProcessing());
	mock.pinValue = false;
	mock.update(1000 + config->cadence.frequencySystemConfig.minTime);
	assertTrue(mock.isProcessing());
	mock.pinValue = true;
	mock.update(2000);
	assertTrue(mock.isProcessing());
	assertEquals<uint16_t>(60, mock.getCadence());
	mock.pinValue = false;
	mock.update(2400);
	assertEquals<uint16_t>(60, mock.getCadence());
	assertTrue(mock.isProcessing());

	mock.pinValue = true;
	mock.update(2500);
	assertTrue(mock.isProcessing());
	assertEquals<uint16_t>(80, mock.getCadence());

	mock.update(2500 + 750 + config->cadence.timeoutCorrection);
	assertTrue(mock.isProcessing());
	assertEquals<uint16_t>(80, mock.getCadence());

	mock.update(2500 + 750 + config->cadence.timeoutCorrection + 1);
	assertTrue(!mock.isProcessing());
	assertEquals<uint16_t>(0, mock.getCadence());

	Serial.println("Done --");
	delete config;
}

void testButtons() {
	Serial.println("Buttons test --");

	ButtonMock mock(FRONT_BUTTON_PIN, false, BUTTON_DEBOUNCE_DURATION);
	assertTrue(!mock.isPushed());
	assertTrue(!mock.isPushed(1000));
	mock.update(500);
	assertTrue(!mock.isPushed());
	assertTrue(!mock.isPushed(1000));
	mock.pinValue = true;
	mock.update(1000);
	assertTrue(!mock.isPushed());
	assertTrue(!mock.isPushed(1000));
	mock.update(1010);
	assertTrue(!mock.isPushed());
	assertTrue(!mock.isPushed(1000));

	mock.pinValue = false;
	mock.update(1000 + BUTTON_DEBOUNCE_DURATION - 1);
	assertTrue(!mock.isPushed());
	assertTrue(!mock.isPushed(1000));

	mock.update(1000 + BUTTON_DEBOUNCE_DURATION);
	assertTrue(mock.isPushed());
	assertTrue(!mock.isPushed(1000));

	mock.pinValue = true;
	mock.update(2000);
	assertTrue(!mock.isPushed());
	assertTrue(!mock.isPushed(1000));
	mock.pinValue = false;
	mock.update(3000);
	assertTrue(mock.isPushed());
	assertTrue(mock.isPushed(1000));
	assertTrue(!mock.isPushed(1000));
	assertTrue(!mock.isPushed());

	Serial.println("Done --");

}

void loop() {

	if (testIndex != 0) {
		return;
	}

	//testConfiguration();
	//testSpeedSystem();
	//testCadenceSystem();
	testButtons();

}
