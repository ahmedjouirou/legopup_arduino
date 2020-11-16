

#include "LegoPupColorDistance.h"

LegoPupColorDistance::LegoPupColorDistance(void){
	_sensorColor = NULL;
	_sensorDistance = NULL;

	_connSerialRX_pin = 0;
	_connSerialTX_pin = 1;

	_lastAckTick = 0;
	_connected = false;
}

LegoPupColorDistance::LegoPupColorDistance(int8_t* pSensorTiltX, int8_t* pSensorTiltY){
	_sensorColor = pSensorTiltX;
	_sensorDistance = pSensorTiltY;

	_connSerialRX_pin = 0;
	_connSerialTX_pin = 1;

	_lastAckTick = 0;
	_connected = false;
}

void LegoPupColorDistance::SetSensor_Color(int8_t* pDataPointer){
	_sensorColor = pDataPointer;
}

void LegoPupColorDistance::SetSensor_Distance(int8_t* pDataPointer){
	_sensorDistance = pDataPointer;
}

bool LegoPupColorDistance::IsConnected(void){
	return _connected;
}

uint8_t LegoPupColorDistance::calcChecksum(uint8_t* pData, int pLen){
	uint8_t lRet, i;
	lRet = 0xFF;
	for(i=0; i<pLen; i++){
		lRet ^= pData[i];
	}
	return lRet;
}


void LegoPupColorDistance::commWaitForHubIdle(void){
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


void LegoPupColorDistance::commSendInitSequence(void){
	// Initialize uart
	Serial.begin(2400);

	Serial.write("\x00",1);
	Serial.flush();
	delay(10);
	Serial.write("\x40\x25\x9A",3);
	Serial.write("\x51\x07\x07\x0A\x07\xA3",6);
	Serial.write("\x52\x00\xC2\x01\x00\x6E",6);
	Serial.write("\x5F\x00\x00\x00\x10\x00\x00\x00\x10\xA0",10);
	Serial.flush();
	delay(10);
	Serial.write("\x9A\x20\x43\x41\x4C\x49\x42\x00\x00\x00\x00",11);
	Serial.write("\x9A\x21\x00\x00\x00\x00\x00\xFF\x7F\x47\x83",11);
	Serial.write("\x9A\x22\x00\x00\x00\x00\x00\x00\xC8\x42\xCD",11);
	Serial.write("\x9A\x23\x00\x00\x00\x00\x00\xFF\x7F\x47\x81",11);
	Serial.write("\x92\x24\x4E\x2F\x41\x00\x69",7);
	Serial.write("\x8A\x25\x10\x00\x40",5);
	Serial.write("\x92\xA0\x08\x01\x05\x00\xC1",7);
	Serial.flush();
	delay(10);
	Serial.write("\x99\x20\x44\x45\x42\x55\x47\x00\x00\x00\x17",11);
	Serial.write("\x99\x21\x00\x00\x00\x00\x00\xC0\x7F\x44\xBC",11);
	Serial.write("\x99\x22\x00\x00\x00\x00\x00\x00\xC8\x42\xCE",11);
	Serial.write("\x99\x23\x00\x00\x00\x00\x00\x00\x20\x41\x24",11);
	Serial.write("\x91\x24\x4E\x2F\x41\x00\x6A",7);
	Serial.write("\x89\x25\x10\x00\x43",5);
	Serial.write("\x91\xA0\x02\x01\x05\x00\xC8",7);
	Serial.flush();
	delay(10);
	Serial.write("\x98\x20\x53\x50\x45\x43\x20\x31\x00\x00\x53",11);
	Serial.write("\x98\x21\x00\x00\x00\x00\x00\x00\x7F\x43\x7A",11);
	Serial.write("\x98\x22\x00\x00\x00\x00\x00\x00\xC8\x42\xCF",11);
	Serial.write("\x98\x23\x00\x00\x00\x00\x00\x00\x7F\x43\x78",11);
	Serial.write("\x90\x24\x4E\x2F\x41\x00\x6B",7);
	Serial.write("\x88\x25\x00\x00\x52",5);
	Serial.write("\x90\xA0\x04\x00\x03\x00\xC8",7);
	Serial.flush();
	delay(10);
	Serial.write("\x9F\x00\x49\x52\x20\x54\x78\x00\x00\x00\x77",11);
	Serial.write("\x9F\x01\x00\x00\x00\x00\x00\xFF\x7F\x47\xA6",11);
	Serial.write("\x9F\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xE8",11);
	Serial.write("\x9F\x03\x00\x00\x00\x00\x00\xFF\x7F\x47\xA4",11);
	Serial.write("\x97\x04\x4E\x2F\x41\x00\x4C",7);
	Serial.write("\x8F\x05\x00\x04\x71",5);
	Serial.write("\x97\x80\x01\x01\x05\x00\xED",7);
	Serial.flush();
	delay(10);
	Serial.write("\x9E\x00\x52\x47\x42\x20\x49\x00\x00\x00\x5F",11);
	Serial.write("\x9E\x01\x00\x00\x00\x00\x00\xC0\x7F\x44\x9B",11);
	Serial.write("\x9E\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xE9",11);
	Serial.write("\x9E\x03\x00\x00\x00\x00\x00\xc0\x7F\x44\x99",11);
	Serial.write("\x96\x04\x52\x41\x57\x00\x29",7);
	Serial.write("\x8E\x05\x10\x00\x64",5);
	Serial.write("\x96\x80\x03\x01\x05\x00\xEE",7);
	Serial.flush();
	delay(10);
	Serial.write("\x9D\x00\x43\x4F\x4C\x20\x4F\x00\x00\x00\x4D",11);
	Serial.write("\x9D\x01\x00\x00\x00\x00\x00\x00\x20\x41\x02",11);
	Serial.write("\x9D\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xEA",11);
	Serial.write("\x9D\x03\x00\x00\x00\x00\x00\x00\x20\x41\x00",11);
	Serial.write("\x95\x04\x49\x44\x58\x00\x3B",7);
	Serial.write("\x8D\x05\x00\x04\x73",5);
	Serial.write("\x95\x80\x01\x00\x03\x00\xE8",7);
	Serial.flush();
	delay(10);
	Serial.write("\x94\x00\x41\x4D\x42\x49\x6C",7);
	Serial.write("\x9C\x01\x00\x00\x00\x00\x00\x00\xC8\x42\xE8",11);
	Serial.write("\x9C\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xEB",11);
	Serial.write("\x9C\x03\x00\x00\x00\x00\x00\x00\xC8\x42\xEA",11);
	Serial.write("\x94\x04\x50\x43\x54\x00\x28",7);
	Serial.write("\x8C\x05\x10\x00\x66",5);
	Serial.write("\x94\x80\x01\x00\x03\x00\xE9",7);
	Serial.flush();
	delay(10);
	Serial.write("\x9B\x00\x52\x45\x46\x4C\x54\x00\x00\x00\x2D",11);
	Serial.write("\x9B\x01\x00\x00\x00\x00\x00\x00\xC8\x42\xEF",11);
	Serial.write("\x9B\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xEC",11);
	Serial.write("\x9B\x03\x00\x00\x00\x00\x00\x00\xC8\x42\xED",11);
	Serial.write("\x93\x04\x50\x43\x54\x00\x2F",7);
	Serial.write("\x8B\x05\x10\x00\x61",5);
	Serial.write("\x93\x80\x01\x00\x03\x00\xEE",7);
	Serial.flush();
	delay(10);
	Serial.write("\x9A\x00\x43\x4F\x55\x4E\x54\x00\x00\x00\x26",11);
	Serial.write("\x9A\x01\x00\x00\x00\x00\x00\x00\xC8\x42\xEE",11);
	Serial.write("\x9A\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xED",11);
	Serial.write("\x9A\x03\x00\x00\x00\x00\x00\x00\xC8\x42\xEC",11);
	Serial.write("\x92\x04\x43\x4E\x54\x00\x30",7);
	Serial.write("\x8A\x05\x08\x00\x78",5);
	Serial.write("\x92\x80\x01\x02\x04\x00\xEA",7);
	Serial.flush();
	delay(10);
	Serial.write("\x91\x00\x50\x52\x4F\x58\x7B",7);
	Serial.write("\x99\x01\x00\x00\x00\x00\x00\x00\x20\x41\x06",11);
	Serial.write("\x99\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xEE",11);
	Serial.write("\x99\x03\x00\x00\x00\x00\x00\x00\x20\x41\x04",11);
	Serial.write("\x91\x04\x44\x49\x53\x00\x34",7);
	Serial.write("\x89\x05\x50\x00\x23",5);
	Serial.write("\x91\x80\x01\x00\x03\x00\xEC",7);
	Serial.flush();
	delay(10);
	Serial.write("\x98\x00\x43\x4F\x4C\x4F\x52\x00\x00\x00\x3A",11);
	Serial.write("\x98\x01\x00\x00\x00\x00\x00\x00\x20\x41\x07",11);
	Serial.write("\x98\x02\x00\x00\x00\x00\x00\x00\xC8\x42\xEF",11);
	Serial.write("\x98\x03\x00\x00\x00\x00\x00\x00\x20\x41\x05",11);
	Serial.write("\x90\x04\x49\x44\x58\x00\x3E",7);
	Serial.write("\x88\x05\xC4\x00\xB6",5);
	Serial.write("\x90\x80\x01\x00\x03\x00\xED",7);
	Serial.write("\x88\x06\x4F\x00\x3E",5);
	Serial.flush();
	delay(10);
	Serial.write("\x04",1);
	Serial.flush();
	delay(5);
}


void LegoPupColorDistance::Process(void){

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

				// Send data needed by the Hub
				_txBuf[0] = 0x46;					// header
				_txBuf[1] = 0x08;					// value
				_txBuf[2] = calcChecksum(_txBuf,2);	// checksum
				Serial.write((char*)_txBuf, 3);
				Serial.flush();

				//Send dummy data
				_txBuf[0] = 0xD0;					// header
				_txBuf[1] = *_sensorColor;			// color    [0..10]
				_txBuf[2] = *_sensorDistance;		// distance [0..10]
				_txBuf[3] = 0xFF;					// value3
				_txBuf[4] = 0x03;					// value4
				_txBuf[5] = calcChecksum(_txBuf,5);	// checksum
				Serial.write((char*)_txBuf, 6);
				Serial.flush();
				delay(50);
			}
			else if(dat == 0x46){
				/*
				// TODO: Future work to handle commands coming from the Hub
				//DbgSerial.println("Received CMD from the Hub");
				int lcnt = Serial.readBytes(_rxBuf, 16);
				for(int i=0; i<lcnt; i++){ printHex(_rxBuf[i]);}DbgSerial.println();
				//46 08 (B1) | D0 00 00 05 00 (2A)
				//                      co 
				uint8_t extMode = _rxBuf[0];
				uint8_t checksum1 = _rxBuf[1];
				uint8_t col = _rxBuf[5]; 
				uint8_t checksum2 = _rxBuf[7];
				*/
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