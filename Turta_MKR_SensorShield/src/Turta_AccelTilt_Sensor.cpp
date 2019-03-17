/*
  Turta_AccelTilt_Sensor.h - MMA8491Q Sensor Library for Turta MKR Sensor Shield.
  Created by Turta, February 28, 2019.
  Released under the MIT Licence.
  www.turta.io
*/

#include <Arduino.h>
#include <Wire.h>
#include "Turta_AccelTilt_Sensor.h"

#define EN_PIN 6
#define INTX_PIN A3
#define INTY_PIN A2
#define INTZ_PIN A1
#define SCL_PIN 12
#define SDA_PIN 11
#define BUS_SPEED 400000

Turta_AccelTilt_Sensor::Turta_AccelTilt_Sensor() {}

void Turta_AccelTilt_Sensor::begin() {
  Wire.begin();
  pinMode(INTX_PIN, INPUT);
  pinMode(INTY_PIN, INPUT);
  pinMode(INTZ_PIN, INPUT);
  pinMode(EN_PIN, OUTPUT);
}

double Turta_AccelTilt_Sensor::convertToG(int analog_data) {
  if ((analog_data & 0x2000) == 0x2000) // Zero or negative G
    return (0x3FFF - analog_data) / -1024.0;
  else // Positive G
    return analog_data / 1024.0;
}

double Turta_AccelTilt_Sensor::readXAxis() {
  digitalWrite(EN_PIN, HIGH);
  delay(1);

  while ((Turta_AccelTilt_Sensor::i2CReadOneByte(MMA8491Q_STATUS) & 0x01) != 0x01)
    delay(1);
  int tempData = Turta_AccelTilt_Sensor::i2CReadTwoBytesAsIntRS2B(MMA8491Q_OUT_X_MSB);
  digitalWrite(EN_PIN, LOW);

  return Turta_AccelTilt_Sensor::convertToG(tempData);
}

double Turta_AccelTilt_Sensor::readYAxis() {
  digitalWrite(EN_PIN, HIGH);
  delay(1);

  while ((Turta_AccelTilt_Sensor::i2CReadOneByte(MMA8491Q_STATUS) & 0x02) != 0x02)
    delay(1);
  int tempData = Turta_AccelTilt_Sensor::i2CReadTwoBytesAsIntRS2B(MMA8491Q_OUT_Y_MSB);
  digitalWrite(EN_PIN, LOW);

  return Turta_AccelTilt_Sensor::convertToG(tempData);
}

double Turta_AccelTilt_Sensor::readZAxis() {
  digitalWrite(EN_PIN, HIGH);
  delay(1);

  while ((Turta_AccelTilt_Sensor::i2CReadOneByte(MMA8491Q_STATUS) & 0x04) != 0x04)
    delay(1);
  int tempData = Turta_AccelTilt_Sensor::i2CReadTwoBytesAsIntRS2B(MMA8491Q_OUT_Z_MSB);
  digitalWrite(EN_PIN, LOW);

  return Turta_AccelTilt_Sensor::convertToG(tempData);
}

void Turta_AccelTilt_Sensor::readXYZAxis(double & x, double & y, double & z) {
  byte bfr[6] = {0};

  digitalWrite(EN_PIN, HIGH);
  delay(1);
  while ((Turta_AccelTilt_Sensor::i2CReadOneByte(MMA8491Q_STATUS) & 0x08) != 0x08)
    delay(1);

  Turta_AccelTilt_Sensor::i2CReadMultipleBytes(MMA8491Q_OUT_X_MSB, 6, bfr);
  digitalWrite(EN_PIN, LOW);

  x = Turta_AccelTilt_Sensor::convertToG((bfr[0] << 6) + (bfr[1] >> 2));
  y = Turta_AccelTilt_Sensor::convertToG((bfr[2] << 6) + (bfr[3] >> 2));
  z = Turta_AccelTilt_Sensor::convertToG((bfr[4] << 6) + (bfr[5] >> 2));
}

void Turta_AccelTilt_Sensor::readTiltState(bool & x, bool & y, bool & z) {
  digitalWrite(EN_PIN, HIGH);
  delay(1);

  while ((Turta_AccelTilt_Sensor::i2CReadOneByte(MMA8491Q_STATUS) & 0x08) != 0x08)
    delay(1);
  x = (digitalRead(INTX_PIN) == HIGH) ? false : true;
  y = (digitalRead(INTY_PIN) == HIGH) ? false : true;
  z = (digitalRead(INTZ_PIN) == HIGH) ? false : true;
  digitalWrite(EN_PIN, LOW);
}

// I2C Communication
void Turta_AccelTilt_Sensor::i2CWriteOneByte(byte reg, byte data) {
  Wire.beginTransmission((uint8_t)MMA8491Q_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.write(data);
  Wire.endTransmission();
}

byte Turta_AccelTilt_Sensor::i2CReadOneByte(byte reg) {
  byte data;
  Wire.beginTransmission((uint8_t)MMA8491Q_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MMA8491Q_I2C_ADDRESS, (uint8_t)1);
  data = Wire.read();
  Wire.endTransmission();
  return data;
}

int Turta_AccelTilt_Sensor::i2CReadTwoBytesAsIntRS2B(byte reg) {
  int i = 0;
  byte data[2] = {0};
  Wire.beginTransmission((uint8_t)MMA8491Q_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MMA8491Q_I2C_ADDRESS, (uint8_t)2);
  while (Wire.available())
    data[i++] = Wire.read();
  Wire.endTransmission();
  return (data[0] << 6) + (data[1] >> 2);
}

void Turta_AccelTilt_Sensor::i2CReadMultipleBytes(byte reg, short len, byte *data) {
  short i = 0;
  Wire.beginTransmission((uint8_t)MMA8491Q_I2C_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MMA8491Q_I2C_ADDRESS, (uint8_t)8);
  while (Wire.available())
    data[i++] = Wire.read();
  Wire.endTransmission();
}