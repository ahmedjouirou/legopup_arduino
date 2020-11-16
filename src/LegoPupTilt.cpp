

#include "LegoPupTilt.h"

LegoPupTilt::LegoPupTilt(void){
	_sensorTiltX = NULL;
	_sensorTiltY = NULL;

	_connSerialRX_pin = 0;
	_connSerialTX_pin = 1;

	_lastAckTick = 0;
	_connected = false;
}

LegoPupTilt::LegoPupTilt(int8_t* pSensorTiltX, int8_t* pSensorTiltY){
	_sensorTiltX = pSensorTiltX;
	_sensorTiltY = pSensorTiltY;

	_connSerialRX_pin = 0;
	_connSerialTX_pin = 1;

	_lastAckTick = 0;
	_connected = false;
}

void LegoPupTilt::SetSensor_TiltX(int8_t* pDataPointer){
	_sensorTiltX = pDataPointer;
}

void LegoPupTilt::SetSensor_TiltY(int8_t* pDataPointer){
	_sensorTiltY = pDataPointer;
}

bool LegoPupTilt::IsConnected(void){
	return _connected;
}

uint8_t LegoPupTilt::calcChecksum(uint8_t* pData, int pLen){
	uint8_t lRet, i;
	lRet = 0xFF;
	for(i=0; i<pLen; i++){
		lRet ^= pData[i];
	}
	return lRet;
}


void LegoPupTilt::commWaitForHubIdle(void){
	// disable uart , need to control directly TX and RX
	Serial.end();

	unsigned long idletick;

	pinMode(_connSerialTX_pin, OUTPUT);
	digitalWrite(_connSerialTX_pin, LOW);

	pinMode(_connSerialRX_pin, INPUT);
	idletick = millis();
	while(1){
		if(digitalRead(_connSerialRX_pin) == LOW){
			idletick = millis();
		}
		if(millis()-idletick > 100){
			break;
		}
	}

	digitalWrite(_connSerialTX_pin, HIGH);
	delay(100);
	digitalWrite(_connSerialTX_pin, LOW);
	delay(100);
}


void LegoPupTilt::commSendInitSequence(void){
	// Initialize uart
	Serial.begin(2400);

	Serial.write("\x00",1);
	Serial.flush();
	delay(10);
	Serial.write("\x40\x22\x9D\x49\x03\x02\xB7\x52\x00\xC2\x01\x00\x6E\x5F\x00",15);Serial.flush();
	Serial.write("\x00\x00\x10\x00\x00\x00\x10\xA0\x9B\x00\x4C\x50\x46\x32\x2D\x43", 16);Serial.flush();
	Serial.write("\x41\x4C\x6F\x9B\x01\x00\x00\x34\xC2\x00\x00\x34\x42\xE5\x9B\x02", 16);Serial.flush();
	Serial.write("\x00\x00\xC8\xC2\x00\x00\xC8\x42\xE6\x9B\x03\x00\x00\x34\xC2\x00", 16);Serial.flush();
	Serial.write("\x00\x34\x42\xE7\x93\x04\x43\x41\x4C\x00\x26\x8B\x05\x10\x00\x61", 16);Serial.flush();
	Serial.write("\x93\x80\x03\x00\x03\x00\xEC\xA2\x00\x4C\x50\x46\x32\x2D\x43\x52", 16);Serial.flush();
	Serial.write("\x41\x53\x48\x00\x00\x00\x00\x00\x00\x53\x9A\x01\x00\x00\x00\x00", 16);Serial.flush();
	Serial.write("\x00\x00\xC8\x42\xEE\x9A\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xED", 16);Serial.flush();
	Serial.write("\x9A\x03\x00\x00\x00\x00\x00\x00\xC8\x42\xEC\x92\x04\x43\x4E\x54", 16);Serial.flush();
	Serial.write("\x00\x30\x8A\x05\x10\x00\x60\x92\x80\x03\x00\x03\x00\xED\xA1\x00", 16);Serial.flush();
	Serial.write("\x4C\x50\x46\x32\x2D\x54\x49\x4C\x54\x00\x00\x00\x00\x00\x00\x00", 16);Serial.flush();
	Serial.write("\x1E\x99\x01\x00\x00\x00\x00\x00\x00\x20\x41\x06\x99\x02\x00\x00", 16);Serial.flush();
	Serial.write("\x00\x00\x00\x00\xC8\x42\xEE\x99\x03\x00\x00\x00\x00\x00\x00\x20", 16);Serial.flush();
	Serial.write("\x41\x04\x91\x04\x44\x49\x52\x00\x35\x89\x05\x04\x00\x77\x91\x80", 16);Serial.flush();
	Serial.write("\x01\x00\x02\x00\xED\xA0\x00\x4C\x50\x46\x32\x2D\x41\x4E\x47\x4C", 16);Serial.flush();
	Serial.write("\x45\x00\x00\x00\x00\x00\x00\x5B\x98\x01\x00\x00\x34\xC2\x00\x00", 16);Serial.flush();
	Serial.write("\x34\x42\xE6\x98\x02\x00\x00\xC8\xC2\x00\x00\xC8\x42\xE5\x98\x03", 16);Serial.flush();
	Serial.write("\x00\x00\x34\xC2\x00\x00\x34\x42\xE4\x90\x04\x44\x45\x47\x00\x2D", 16);Serial.flush();
	Serial.write("\x88\x05\x10\x00\x62\x90\x80\x02\x00\x03\x00\xEE\x04\xF0\x00\x40", 16);Serial.flush();
	Serial.write("\x22\x9D\x49\x03\x02\xB7\x52\x00\xC2\x01\x00\x6E\x5F\x00\x00\x00", 16);Serial.flush();
	Serial.write("\x10\x00\x00\x00\x10\xA0\x9B\x00\x4C\x50\x46\x32\x2D\x43\x41\x4C", 16);Serial.flush();
	Serial.write("\x6F\x9B\x01\x00\x00\x34\xC2\x00\x00\x34\x42\xE5\x9B\x02\x00\x00", 16);Serial.flush();
	Serial.write("\xC8\xC2\x00\x00\xC8\x42\xE6\x9B\x03\x00\x00\x34\xC2\x00\x00\x34", 16);Serial.flush();
	Serial.write("\x42\xE7\x93\x04\x43\x41\x4C\x00\x26\x8B\x05\x10\x00\x61\x93\x80", 16);Serial.flush();
	Serial.write("\x03\x00\x03\x00\xEC\xA2\x00\x4C\x50\x46\x32\x2D\x43\x52\x41\x53", 16);Serial.flush();
	Serial.write("\x48\x00\x00\x00\x00\x00\x00\x53\x9A\x01\x00\x00\x00\x00\x00\x00", 16);Serial.flush();
	Serial.write("\xC8\x42\xEE\x9A\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xED\x9A\x03", 16);Serial.flush();
	Serial.write("\x00\x00\x00\x00\x00\x00\xC8\x42\xEC\x92\x04\x43\x4E\x54\x00\x30", 16);Serial.flush();
	Serial.write("\x8A\x05\x10\x00\x60\x92\x80\x03\x00\x03\x00\xED\xA1\x00\x4C\x50", 16);Serial.flush();
	Serial.write("\x46\x32\x2D\x54\x49\x4C\x54\x00\x00\x00\x00\x00\x00\x00\x1E\x99", 16);Serial.flush();
	Serial.write("\x01\x00\x00\x00\x00\x00\x00\x20\x41\x06\x99\x02\x00\x00\x00\x00", 16);Serial.flush();
	Serial.write("\x00\x00\xC8\x42\xEE\x99\x03\x00\x00\x00\x00\x00\x00\x20\x41\x04", 16);Serial.flush();
	Serial.write("\x91\x04\x44\x49\x52\x00\x35\x89\x05\x04\x00\x77\x91\x80\x01\x00", 16);Serial.flush();
	Serial.write("\x02\x00\xED\xA0\x00\x4C\x50\x46\x32\x2D\x41\x4E\x47\x4C\x45\x00", 16);Serial.flush();
	Serial.write("\x00\x00\x00\x00\x00\x5B\x98\x01\x00\x00\x34\xC2\x00\x00\x34\x42", 16);Serial.flush();
	Serial.write("\xE6\x98\x02\x00\x00\xC8\xC2\x00\x00\xC8\x42\xE5\x98\x03\x00\x00", 16);Serial.flush();
	Serial.write("\x34\xC2\x00\x00\x34\x42\xE4\x90\x04\x44\x45\x47\x00\x2D\x88\x05", 16);Serial.flush();
	Serial.write("\x10\x00\x62\x90\x80\x02\x00\x03\x00\xEE\x04",11);Serial.flush();
	delay(5);
}


void LegoPupTilt::Process(void){

	if(_connected==false){
		//DbgSerial.println("INIT SENSOR");

		//------------------------------------------------------
		// Wait for HUB to idle it's TX pin (idle = High)
		//------------------------------------------------------
		commWaitForHubIdle();
		//------------------------------------------------------


		//------------------------------------------------------
		// Starting initialization sequence
		//------------------------------------------------------
		unsigned long starSequence = millis();
		commSendInitSequence();
		unsigned long starttime = millis();

		//DbgSerial.println(starttime-starSequence); // time requested for initialization sequence
		//------------------------------------------------------


		//------------------------------------------------------
		// Check if the hub send a NACK
		//------------------------------------------------------
		unsigned long currenttime = starttime;
		while((currenttime-starttime) < 2000){
			if (Serial.available() > 0) {
				// read the incoming byte
				unsigned char dat = Serial.read();
				if (dat == 0x04){
					//DbgSerial.println("Connection Espablished !");
					Serial.begin(115200);
					_connected = true;
					_lastAckTick = millis();
					break;
				}
			}
			currenttime = millis();
			delay(10);
		}
		//------------------------------------------------------

	}
	else{
		//------------------------------------------------------
		// Connection established
		//------------------------------------------------------
		if (Serial.available() > 0) {
			unsigned char dat;
			dat = Serial.read();
			if(dat == 0x02){
				//DbgSerial.println("Received NACK from the Hub");
				_lastAckTick = millis();

				//Send dummy data
				_txBuf[0] = 0xC8;					// header
				_txBuf[1] = *_sensorTiltX;			// X [-45..45]
				_txBuf[2] = *_sensorTiltY;			// Y [-45..45]
				_txBuf[3] = calcChecksum(_txBuf,3);	// checksum
				Serial.write((char*)_txBuf, 4);
				delay(50);
			}

		}
		//------------------------------------------------------

		// Check for disconnection from the Hub
		if(millis()- _lastAckTick > 200){
			//DbgSerial.println("Disconnect, Hub didnt send NACK");
			_connected = false;
		}
	}

}