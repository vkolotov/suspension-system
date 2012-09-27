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
	mock.pinValue = true;
	mock.update(1000);
	assertEquals<uint32_t>(0.0f, mock.getAverageSpeed());
	mock.update(1010);
	assertEquals<uint32_t>(0.0f, mock.getAverageSpeed());
	mock.update(1020);
	assertEquals<uint32_t>(0.0f, mock.getAverageSpeed());
	mock.pinValue = false;
	mock.update(1300);
	mock.pinValue = true;
	mock.update(2000);
	assertEquals<float>(WHEEL_LENGTH / 1000.0f, mock.getAverageSpeed());
	assertEquals<float>(WHEEL_LENGTH * 3.6f / 1000.0f, mock.getAverageSpeedKmH());
	mock.pinValue = false;
	mock.update(2500);
	assertEquals<float>(WHEEL_LENGTH * 3.6f / 1000.0f, mock.getAverageSpeedKmH());
	mock.update(2000 + MAXIMUM_SPEED_TIME);
	assertEquals<int>(int (0.0f * 100), int (mock.getAverageSpeed() * 100));
	Serial.println("Done --");
	delete config;
}

void loop() {

	if (testIndex != 0) {
		return;
	}

	//testConfiguration();
	testSpeedSystem();

}
