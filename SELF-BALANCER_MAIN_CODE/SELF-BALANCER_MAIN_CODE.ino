#include <Wire.h>
#include <AccelStepper.h>
#include <MPU6050_tockn.h>
#include <SoftwareSerial.h>

#define RX_PIN 10  // Define the RX pin for Bluetooth module
#define TX_PIN 11  // Define the TX pin for Bluetooth module

SoftwareSerial btSerial(RX_PIN, TX_PIN);  // Create a SoftwareSerial object for Bluetooth communication

int gyroAddr = 0x68;
int accCalibrationValue = -8110;  // Enter the accelerometer calibration value

// Various settings
float pidPGain = 18;
float pidIGain = 1;
float pidDGain = 12;
float turningSpeed = 400;  // Turning speed (900)
float maxTargetSpeed = 1400;  // Max target speed (1500)

byte startFlag, receivedByte, lowBatteryFlag;

int leftMotor, throttleLeftMotor, throttleCounterLeftMotor, throttleLeftMotorMemory;
int rightMotor, throttleRightMotor, throttleCounterRightMotor, throttleRightMotorMemory;
int batteryVoltage;
int receiveCounter;
int gyroPitchDataRaw, gyroYawDataRaw, accelerometerDataRaw;

long gyroYawCalibrationValue, gyroPitchCalibrationValue;

unsigned long loopTimer;

float angleGyro, angleAcc, angle, selfBalancePidSetpoint;
float pidErrorTemp, pidIMem, pidSetpoint, gyroInput, pidOutput, pidLastDError;
float pidOutputLeft, pidOutputRight;
int speedM = 1000;  // max 2500
float pickup = 0.009;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  TWBR = 12;

  btSerial.begin(9600);  // Initialize Bluetooth module

  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 |= (1 << OCIE2A);
  TCCR2B |= (1 << CS21);
  OCR2A = 39;
  TCCR2A |= (1 << WGM21);

  // By default the MPU-6050 sleeps. So we have to wake it up.
  Wire.beginTransmission(gyroAddr);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(gyroAddr);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(gyroAddr);
  Wire.write(0x1C);
  Wire.write(0x08);
  Wire.endTransmission();

  // Set some filtering to improve the raw data.
  Wire.beginTransmission(gyroAddr);
  Wire.write(0x1A);
  Wire.write(0x03);
  Wire.endTransmission();

  // Internally CNC shield pins are connected to 2,3,5,6 for step and dir
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  for (receiveCounter = 0; receiveCounter < 500; receiveCounter++) {
    if (receiveCounter % 15 == 0)
      digitalWrite(13, !digitalRead(13));

    Wire.beginTransmission(gyroAddr);
    Wire.write(0x43);
    Wire.endTransmission();
    Wire.requestFrom(gyroAddr, 4);
    gyroYawCalibrationValue += Wire.read() << 8 | Wire.read();
    gyroPitchCalibrationValue += Wire.read() << 8 | Wire.read();
    delayMicroseconds(3700);
  }
  gyroPitchCalibrationValue /= 500;
  gyroYawCalibrationValue /= 500;

  loopTimer = micros() + 4000;
}

void loop() {
  if (Serial.available()) {
    receivedByte = Serial.read();
  }

  // Angle calculations

  Wire.beginTransmission(gyroAddr);
  Wire.write(0x3F);
  Wire.endTransmission();
  Wire.requestFrom(gyroAddr, 2);
  accelerometerDataRaw = Wire.read() << 8 | Wire.read();
  accelerometerDataRaw += accCalibrationValue;

  if (accelerometerDataRaw > 8200) {
    accelerometerDataRaw = 8200;
  }
  if (accelerometerDataRaw < -8200) {
    accelerometerDataRaw = -8200;
  }

  angleAcc = asin((float)accelerometerDataRaw / 8200.0) * 57.296;

  if (startFlag == 0 && angleAcc > -0.5 && angleAcc < 0.5) {
    angleGyro = angleAcc;
    startFlag = 1;
  }

  Wire.beginTransmission(gyroAddr);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(gyroAddr, 4);
  gyroYawDataRaw = Wire.read() << 8 | Wire.read();
  gyroPitchDataRaw = Wire.read() << 8 | Wire.read();

  gyroPitchDataRaw -= gyroPitchCalibrationValue;
  angleGyro += gyroPitchDataRaw * 0.000031;

  gyroYawDataRaw -= gyroYawCalibrationValue;
  angleGyro = angleGyro * 0.9996 + angleAcc * 0.0004;
  pidErrorTemp = angleGyro - selfBalancePidSetpoint - pidSetpoint;

  if (pidOutput > 10 || pidOutput < -10) {
    pidErrorTemp += pidOutput * 0.015 ;
  }

  pidIMem += pidIGain * pidErrorTemp;

  if (pidIMem > speedM) {
    pidIMem = speedM;
  } else if (pidIMem < -speedM) {
    pidIMem = -speedM;
  }

  pidOutput = pidPGain * pidErrorTemp + pidIMem + pidDGain * (pidErrorTemp - pidLastDError);

  if (pidOutput > speedM) {
    pidOutput = speedM;
  } else if (pidOutput < -speedM) {
    pidOutput = -speedM;
  }

  pidLastDError = pidErrorTemp;

  if (pidOutput < 5 && pidOutput > -5) {
    pidOutput = 0;
  }

  if (angleGyro > 40 || angleGyro < -40 || startFlag == 0 || lowBatteryFlag == 1) {
    pidOutput = 0;
    pidIMem = 0;
    startFlag = 0;
    selfBalancePidSetpoint = 0;
  }

  pidOutputLeft = pidOutput;
  pidOutputRight = pidOutput;

  if (receivedByte == 'R') {
    pidOutputLeft += turningSpeed;
    pidOutputRight -= turningSpeed;
  }
  if (receivedByte == 'L') {
    pidOutputLeft -= turningSpeed;
    pidOutputRight += turningSpeed;
  }

  if (receivedByte == 'F') {
    pidSetpoint -= pickup;
  }
  if (receivedByte == 'B') {
    pidSetpoint += pickup;
  }

  if (receivedByte == 'S') {
    pidSetpoint = 0;
  }

  if (pidSetpoint == 0) {
    if (pidOutput < 0) {
      selfBalancePidSetpoint += 0.0015;
    }
    if (pidOutput > 0) {
      selfBalancePidSetpoint -= 0.0015;
    }
  }

  if (pidOutputLeft > 0) {
    pidOutputLeft = speedM - (1 / (pidOutputLeft + 9)) * 5500;
  } else if (pidOutputLeft < 0) {
    pidOutputLeft = -speedM - (1 / (pidOutputLeft - 9)) * 5500;
  }

  if (pidOutputRight > 0) {
    pidOutputRight = speedM - (1 / (pidOutputRight + 9)) * 5500;
  } else if (pidOutputRight < 0) {
    pidOutputRight = -speedM - (1 / (pidOutputRight - 9)) * 5500;
  }

  // Calculate the needed pulse time for the left and right stepper motor controllers
  if (pidOutputLeft > 0) {
    leftMotor = speedM - pidOutputLeft;
  } else if (pidOutputLeft < 0) {
    leftMotor = -speedM - pidOutputLeft;
  } else {
    leftMotor = 0;
  }

  if (pidOutputRight > 0) {
    rightMotor = speedM - pidOutputRight;
  } else if (pidOutputRight < 0) {
    rightMotor = -speedM - pidOutputRight;
  } else {
    rightMotor = 0;
  }

  throttleLeftMotor = leftMotor;
  throttleRightMotor = rightMotor;

  while (loopTimer > micros());
  loopTimer += 4000;
}

ISR(TIMER2_COMPA_vect) {
  // Left motor pulse calculations
  throttleCounterLeftMotor++;
  if (throttleCounterLeftMotor > throttleLeftMotorMemory) {
    throttleCounterLeftMotor = 0;
    throttleLeftMotorMemory = throttleLeftMotor;
    if (throttleLeftMotorMemory < 0) {
      PORTD &= 0b10111111;
      throttleLeftMotorMemory *= -1;
    } else {
      PORTD |= 0b01000000;
    }
  } else if (throttleCounterLeftMotor == 1) {
    PORTD |= 0b00001000;
  } else if (throttleCounterLeftMotor == 2) {
    PORTD &= 0b11110111;
  }

  // Right motor pulse calculations
  throttleCounterRightMotor++;
  if (throttleCounterRightMotor > throttleRightMotorMemory) {
    throttleCounterRightMotor = 0;
    throttleRightMotorMemory = throttleRightMotor;
    if (throttleRightMotorMemory < 0) {
      PORTD |= 0b00100000;
      throttleRightMotorMemory *= -1;
    } else {
      PORTD &= 0b11011111;
    }
  } else if (throttleCounterRightMotor == 1) {
    PORTD |= 0b00000100;
  } else if (throttleCounterRightMotor == 2) {
    PORTD &= 0b11111011;
  }
}
