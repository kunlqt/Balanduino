#ifndef _balanduino_h_
#define _balanduino_h_

#include <stdint.h> // Needed for uint8_t

char stringBuf[30];
char convBuf[10];

bool sendData;
bool sendPIDValues;

const uint16_t PWM_FREQUENCY = 20000; // The motor driver can handle a pwm frequency up to 20kHz
const uint16_t PWMVALUE = F_CPU/PWM_FREQUENCY/2; // The frequency is given by F_CPU/(2*N*ICR) - where N is the prescaler, we use no prescaling so the frequency is given by F_CPU/(2*ICR) - ICR = F_CPU/PWM_FREQUENCY/2

/* Used to make commands more readable */
uint8_t lastCommand; // This is used set a new targetPosition
enum Command {
  updatePS3,
  updateWii,
  updateXbox,
  stop,
  forward,
  backward,
  left,
  right,
  imu,
  joystick,
};

// These are used to read and write to the port registers - see http://www.arduino.cc/en/Reference/PortManipulation
// I do this to save processing power - see this page for more information: http://www.billporter.info/ready-set-oscillate-the-fastest-way-to-change-arduino-pins/
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define pwmPortDirection DDRD

/* Left motor */
#define leftPort PORTC
#define leftPortDirection DDRC

#define leftA PINC6 // PC6 - M1A - pin 23
#define leftB PINC7 // PC7 - M1B - pin 24
#define leftPWM PIND5 // PD5 - PWM1A (OC1A) - pin 18

/* Right motor */
#define rightPort PORTB
#define rightPortDirection DDRB

#define rightA PINB0 // PB0 - M2A - pin 25
#define rightB PINB1 // PB1 - M2B - pin 26
#define rightPWM PIND4 // PD4 - PWM1B (OC1B) - pin 17

/* Pins connected to the motor drivers enable pins */
const uint8_t leftEnable = 21;
const uint8_t rightEnable = 22;

/* Encoders */
const uint8_t leftEncoder1 = 15;
const uint8_t leftEncoder2 = 30;
const uint8_t rightEncoder1 = 16;
const uint8_t rightEncoder2 = 31;

volatile int32_t leftCounter = 0;
volatile int32_t rightCounter = 0;

const uint8_t buzzer = 5; // Buzzer used for feedback, it can be disconected using the jumper

const uint8_t IMUAddress = 0x68; // AD0 is logic low on the PCB
const uint16_t I2C_TIMEOUT = 1000; // Used to check for errors in I2C communication

/* IMU Data */
int16_t accY;
int16_t accZ;
int16_t gyroX;

// Results
double accAngle;
double gyroRate;
double gyroAngle;
double pitch;

/* PID variables */
double Kp = 11;
double Ki = 1;
double Kd = 3; // 5
double targetAngle = 180;
double lastRestAngle = targetAngle; // Used to limit the new restAngle if it's much larger than the previous one

double lastError; // Store last angle error
double integratedError; // Store integrated error

/* Used for timing */
uint32_t kalmanTimer; // Timer used for the Kalman filter
uint32_t pidTimer; // Timer used for the PID loop
uint32_t encoderTimer; // Timer used used to determine when to update the encoder values
uint32_t dataTimer; // This is used so it doesn't send data to often

/* Direction set by the controllers or SPP library */
bool steerForward;
bool steerBackward;
bool steerStop = true; // Stop by default
bool steerLeft;
bool steerRight;

bool stopped; // This is used to set a new target position after braking

bool layingDown = true; // Use to indicate if the robot is laying down

double targetOffset = 0; // Offset for going forward and backward
double turningOffset = 0; // Offset for turning left and right

// Data send via SPP
double sppData1 = 0;
double sppData2 = 0;

bool commandSent = false; // This is used so multiple controller can be used at once

int32_t wheelPosition; // Wheel position based on encoder readings
int32_t lastWheelPosition; // Used to calculate the wheel velocity
int32_t wheelVelocity; // Wheel velocity based on encoder readings
int32_t targetPosition; // The encoder position the robot should be at

const uint16_t zoneA = 4000*2;
const uint16_t zoneB = 2000*2;
const double positionScaleA = 250*2; // One resolution is 928 pulses
const double positionScaleB = 500*2; 
const double positionScaleC = 1000*2;
const double velocityScaleMove = 35*2;
const double velocityScaleStop = 30*2;
const double velocityScaleTurning = 35*2;
#endif
