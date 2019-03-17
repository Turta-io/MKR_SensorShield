/*
  Turta_AccelTilt_Sensor.h - MMA8491Q Sensor Library for Turta MKR Sensor Shield.
  Created by Turta, February 28, 2019.
  Released under the MIT Licence.
  www.turta.io
*/

#ifndef Turta_AccelTilt_Sensor_h
#define Turta_AccelTilt_Sensor_h

#include <Arduino.h>
#include <Wire.h>

// Device Registers
const byte MMA8491Q_I2C_ADDRESS = 0x55;
const byte MMA8491Q_STATUS = 0x00;
const byte MMA8491Q_OUT_X_MSB = 0x01;
const byte MMA8491Q_OUT_Y_MSB = 0x03;
const byte MMA8491Q_OUT_Z_MSB = 0x05;

class Turta_AccelTilt_Sensor
{
  public:
    Turta_AccelTilt_Sensor();
    void begin();
    double readXAxis();
    double readYAxis();
    double readZAxis();
    void readXYZAxis(double & x, double & y, double & z);
    void readTiltState(bool & x, bool & y, bool & z);
  private:
    double convertToG(int analog_data);
    void i2CWriteOneByte(byte reg, byte data);
    byte i2CReadOneByte(byte reg);
    int i2CReadTwoBytesAsIntRS2B(byte reg);
    void i2CReadMultipleBytes(byte reg, short len, byte *data);
};

#endif