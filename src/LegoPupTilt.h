#ifndef LegoPupTilt_h
#define LegoPupTilt_h

#include    "Arduino.h"
class LegoPupTilt {
public:
	LegoPupTilt();
	LegoPupTilt(int8_t* pSensorTiltX, int8_t* pSensorTiltY);
	void Process(void);

	void SetSensor_TiltX(int8_t* pDataPointer);
	void SetSensor_TiltY(int8_t* pDataPointer);
	
	bool IsConnected(void);

private:

	uint8_t calcChecksum(uint8_t* pData, int pLen);
	void commWaitForHubIdle(void);
	void commSendInitSequence(void);

	int	_connSerialRX_pin;
 	int _connSerialTX_pin;

	int8_t*	_sensorTiltX;
	int8_t*	_sensorTiltY;


	unsigned char _rxBuf[32];
	unsigned char _txBuf[32];
	unsigned long _lastAckTick;
	bool _connected;
};

#endif
