
#include <LegoPupTilt.h>
#include <SoftwareSerial.h>

SoftwareSerial  DbgSerial(2, 8); // RX:2 TX:8

// Test sensing pin(s)
const int AnalogPinSens = A5;

int8_t SensorX;
int8_t SensorY;

LegoPupTilt           lpup(&SensorX, &SensorY);

void setup() {

  DbgSerial.begin(9600);
  DbgSerial.println("Hello !");
  while (!DbgSerial) {
  
  }
}

void loop() {

  lpup.Process();

  if(lpup.IsConnected()){
    //DbgSerial.println("Connected !");
  }
  else{  
    //DbgSerial.println("Not Connected");
  }
  
  int sensorValue = analogRead(AnalogPinSens);
  SensorX = map(sensorValue, 0, 1023, -45, 45);
  SensorY = map(sensorValue, 0, 1023, -45, 45);
}