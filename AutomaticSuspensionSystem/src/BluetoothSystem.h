/*
 * BluetoothSystem.h
 *
 *  Created on: 05.09.2012
 *      Author: Vlad
 */

#ifndef BLUETOOTHSYSTEM_H_
#define BLUETOOTHSYSTEM_H_

struct Test {
	uint8_t a;
	uint8_t b;
	uint8_t c;
} test = {7, 8, 9};

const static char* COMMANDS[6] = {"AT", "AT+RESET", "AT+ROLE=0", "AT+NAME=Bike", "AT+INIT", "AT+INQ"};

class BluetoothSystem : public Activity {
public:
	BluetoothSystem(Application* app) : Activity(), app(app), lastSent(0) {};
	~BluetoothSystem() {};

	void init() {

		String result = "";
		//Serial.begin(38400);

		delay(3000);
		for (int i = 0; i < 6; i++) {
		  result = sendBlueToothCommand(COMMANDS[i]);
		  if (result.indexOf("OK") < 0) {
			  Serial.print("Bluetooth error ");
			  Serial.println(result);
			  return;
		  }
		  delay(1000);
		}
	}


	void update(unsigned long time) {

		if (Serial.available()) {
			if (Serial.available() == 1) {
				uint8_t requestId = Serial.read();

				if (requestId == 0) {
					// send back status
					Serial.write((uint8_t)0);
				} else if (requestId == 1) {
					sendTelemetry();
				} else if (requestId == 2) {
					receiveMessage();
				}


			} else {
				// something strange there, let's skip it
				while (Serial.available()) {
					Serial.read();
				}
			}
//			if (time - lastSent > 2000) {
//				lastSent = time;
				//Serial.println("Send");

			//}
		}

	}


private:

	void sendTelemetry() {

		TelemetryMessage msg = {app->speedSystem.getAverageSpeed(), app->cadenceSystem.getCadence(),
				random(32000, 32600), random(60, 120)};
		Serial.write((uint8_t*)&msg, sizeof(msg));
	}

	void receiveMessage() {
		// send back status
		Serial.write((uint8_t)0);
		Serial.flush();
		Serial.setTimeout(10000);
		TelemetryMessage msg = {-1, -1, -1, -1, -1.0, -1.0};
		printBites(msg);
		Serial.readBytes((char*)&msg, sizeof(msg));
//		if (msg.speed == -1) {
//			Serial.println("Timeout.");
//		} else {
//			Serial.print(msg.speed);
//			Serial.print(" ");
//			Serial.print(msg.cadence);
//			Serial.print(" ");
//			Serial.print(msg.distance);
//			Serial.print(" ");
//			Serial.print(msg.heartRate);
//			Serial.print(" ");
//			Serial.print(msg.floatField);
//			Serial.print(" ");
//			Serial.println(msg.doubleField);
			printBites(msg);
			Serial.print(0);
			Serial.flush();
		//}
	}

	void printBites(TelemetryMessage msg) {
		uint8_t* p = (uint8_t*)&msg;
		for (int i = 0; i < sizeof(msg); i++) {
			for (int j = 0; j < 8; j++) {
				Serial.print(bitRead(*p, j));
			}
			p++;
			Serial.print(" ");
		}
		Serial.println();
	}


	String sendBlueToothCommand(const char command[])	{
	    String result = "";

	    Serial.print(command);

	    Serial.println();

	    Serial.flush();
	    delay(500);
	    if (Serial.available()) {
			while (Serial.available()) {
				char inByte = (char) Serial.read();
				result += inByte;
			}
	    } else {
	    	result = "No response";
	    }
	    return result;
	}

private:

	Application* app;
	unsigned long lastSent;
};

#endif /* BLUETOOTHSYSTEM_H_ */
