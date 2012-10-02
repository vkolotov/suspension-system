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
	assertEquals<uint16_t>(2000, config->semiautomaticStateConfig.averageDegreeMeasuringPeriod);
	assertEquals<uint16_t>(WHEEL_BASE, config->system.wheelBase);
	assertEquals<uint16_t>(4000, config->system.maxUnlockTimeout);

	config->system.mode = 1;
	config->semiautomaticStateConfig.averageDegreeMeasuringPeriod = 1000;

	saveConfiguration(config);
	delete config;
	config = loadConfiguration();

	assertEquals<uint8_t>(1, config->system.mode);
	assertEquals<uint16_t>(1000, config->semiautomaticStateConfig.averageDegreeMeasuringPeriod);

	delete config;
	config = resetConfiguration();
	assertEquals<uint8_t>(2, config->system.mode);
	assertEquals<uint16_t>(2000, config->semiautomaticStateConfig.averageDegreeMeasuringPeriod);

	assertEquals<uint16_t>(WHEEL_BASE, config->system.wheelBase);

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

void testUnsprungAccelerometerSystem() {
	Serial.println("UnsprungAccelerometerSystem test --");
	Configuration* config = resetConfiguration();
	SpeedSystemMock speed(&(config->speed));
	UnsprungAccelerometerSysteMock accel(config, &(config->unsprungAccelerometerSystem), &speed);
	speed.update(500);
	accel.update(500);

	assertEquals<uint32_t>(0.0f, speed.getAverageSpeed());
	assertTrue(!accel.isActive());

	uint16_t threshold = config->unsprungAccelerometerSystem.accelerometerSystemConfig.severityThreshold;
	uint16_t idleValue = 100;

	accel.setAccelerationX(idleValue);
	accel.calibrate();
	accel.update(600);
	assertTrue(!accel.isActive());
	assertEquals<uint16_t>(idleValue, accel.getAccelerationX());

	accel.setAccelerationX(idleValue + threshold - 1);
	accel.update(700);
	assertTrue(!accel.isActive());
	assertEquals<uint16_t>(0, accel.getTimeout());


	accel.setAccelerationX(idleValue + threshold);
	accel.update(800);
	assertTrue(accel.isActive());

	assertTrue(speed.getAverageSpeed() < 0.5f);

	assertEquals<uint16_t>(0, accel.getTimeout());
	accel.setAccelerationX(0);
	accel.update(801);
	assertEquals<uint16_t>(0, accel.getTimeout());
	assertTrue(!accel.isActive());

	accel.update(10000);
	assertTrue(!accel.isActive());
	assertEquals<uint16_t>(0, accel.getNumberOfBumps());

	speed.speed = 2.0f;
	accel.setAccelerationX(idleValue + threshold);
	accel.update(11000);
	accel.setAccelerationX(0);
	assertTrue(accel.isActive());
	assertEquals<uint16_t>(1, accel.getNumberOfBumps());
	assertEquals<uint16_t>(config->system.wheelBase / speed.speed, accel.getTimeout());
	uint32_t timeout = accel.getTimeout();
	accel.update(11000 + timeout);
	assertTrue(accel.isActive());

	accel.update(11000 + timeout + 1);
	assertTrue(!accel.isActive());

	accel.update(20000);
	assertTrue(!accel.isActive());
	assertEquals<uint16_t>(0, accel.getNumberOfBumps());

	uint32_t updateTime = 20000;
	for (uint8_t i = 1; i <= 20; i++) {
		accel.setAccelerationX(0);
		accel.update(updateTime);
		accel.setAccelerationX(idleValue + threshold);
		accel.update(updateTime + 50);
		updateTime += 100;
		assertEquals<uint16_t>(i, accel.getNumberOfBumps());
	}
	assertTrue(accel.isActive());
	assertEquals<uint16_t>(20, accel.getNumberOfBumps());
	assertEquals<uint16_t>(config->system.maxUnlockTimeout, accel.getTimeout());

	accel.setAccelerationX(0);
	accel.update(30000);
	assertTrue(!accel.isActive());

	Serial.println("Done --");
	delete config;
}
void testFrequencyQueue() {
	FrequencyQueue<20> timing(4000);

	uint16_t segment = 4000 / 20;

	assertEquals<unsigned long>(0, timing.getSum());
	assertEquals<unsigned long>(0, timing.calculateSum());
	assertEquals<unsigned long>(segment, timing.getSegmentDuration());

	timing.update(1000);
	assertEquals<unsigned long>(1000, timing.getSegmentTail());
	assertEquals<unsigned long>(1000 / segment, timing.size());
	timing.event();
	assertEquals<unsigned long>(1, timing.getSum());
	assertEquals<unsigned long>(1, timing.calculateSum());
	timing.update(2000);
	timing.event();
	assertEquals<unsigned long>(2, timing.getSum());
	assertEquals<unsigned long>(2, timing.calculateSum());
	assertEquals<unsigned long>(2000 / segment, timing.size());
	assertEquals<unsigned long>(2000, timing.getSegmentTail());

	timing.update(3000);
	assertEquals<unsigned long>(2, timing.getSum());
	assertEquals<unsigned long>(2, timing.calculateSum());
	assertEquals<unsigned long>(3000, timing.getSegmentTail());
	assertEquals<unsigned long>(3000 / segment, timing.size());
	timing.update(4000);
	assertEquals<unsigned long>(2, timing.getSum());
	assertEquals<unsigned long>(4000, timing.getSegmentTail());
	assertEquals<unsigned long>(4000 / segment, timing.size());
	assertEquals<unsigned long>(2, timing.calculateSum());

	timing.update(5000);
	assertEquals<unsigned long>(1, timing.getSum());
	assertEquals<unsigned long>(1, timing.calculateSum());
	assertEquals<unsigned long>(5000, timing.getSegmentTail());
	assertEquals<unsigned long>(20, timing.size());

	timing.update(6000);
	assertEquals<unsigned long>(0, timing.getSum());
	assertEquals<unsigned long>(0, timing.calculateSum());
	assertEquals<unsigned long>(20, timing.size());


}

void testBasicQueue() {
	BasicQueue<2, uint16_t> queue;
	assertEquals<uint16_t>(0, queue.size());

	queue.push(1);
	assertEquals<uint16_t>(1, queue.size());
	assertEquals<uint16_t>(1, queue.last());

	queue.push(2);
	assertEquals<uint16_t>(2, queue.size());
	assertEquals<uint16_t>(2, queue.last());

	assertEquals<uint16_t>(1, queue.push(3));
	assertEquals<uint16_t>(2, queue.size());
	assertEquals<uint16_t>(3, queue.last());

	assertEquals<uint16_t>(2, queue.push(4));
	assertEquals<uint16_t>(2, queue.size());
	assertEquals<uint16_t>(4, queue.last());

	queue.clear();
	assertEquals<uint16_t>(0, queue.size());

}

void loop() {

	if (testIndex != 0) {
		return;
	}

	//testConfiguration();
	//testSpeedSystem();
	//testCadenceSystem();
	//testButtons();
	testUnsprungAccelerometerSystem();
	//testFrequencyQueue();
	//testBasicQueue();

}
