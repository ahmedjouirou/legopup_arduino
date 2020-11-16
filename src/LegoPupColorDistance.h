#ifndef LegoPupColorDistance_h
#define LegoPupColorDistance_h

#include    "Arduino.h"
class LegoPupColorDistance {
public:
	LegoPupColorDistance();
	LegoPupColorDistance(int8_t* pSensorColor, int8_t* pSensorDistance);
	void Process(void);

	void SetSensor_Color(int8_t* pDataPointer);
	void SetSensor_Distance(int8_t* pDataPointer);
	
	bool IsConnected(void);

private:

	uint8_t calcChecksum(uint8_t* pData, int pLen);
	void commWaitForHubIdle(void);
	void commSendInitSequence(void);

	int	_connSerialRX_pin;
 	int _connSerialTX_pin;

	int8_t*	_sensorColor;
	int8_t*	_sensorDistance;


	unsigned char _rxBuf[32];
	unsigned char _txBuf[32];
	unsigned long _lastAckTick;
	bool _connected;
};

#endif
