/**
 * @file     fidget.ino
 * @author   Phi Luu
 * @date     July 13, 2017
 *
 * @brief    Robot Collection - Project 02: Fidget
 *
 * @section  DESCRIPTION
 *
 * This robot is a computer keyboard controlled robot which can run, turn
 * camera, and avoid obstacles. This robot is inspired by and has the same
 * chassis as the GoPiGo robot of Dexter Industries.
 *
 * This program is the "slave" end of the I2C connection between a Raspberry
 * Pi and an Arduino UNO. It receives the I2C signals and control the motors
 * in order to make the robot move. It also utilizes the sonic sensor to help
 * the robot avoid obstacles.
 */

#include <Servo.h>
#include <Wire.h>

#define SLAVE_ADDRESS    0x04
#define KEYBOARD_STOP    0
#define KEYBOARD_W       1
#define KEYBOARD_A       2
#define KEYBOARD_S       3
#define KEYBOARD_D       4
#define KEYBOARD_Q       5
#define KEYBOARD_E       6

// Pinout
const byte SONIC_TRIG  = 7;
const byte SONIC_ECHO  = 8;
const byte SERVO       = 9;
const byte RIGHT_MOTOR = 10;
const byte LEFT_MOTOR  = 11;

// constants
const unsigned int BAUD_RATE = 9600;
const byte POWER             = 191;
const unsigned int WAIT_TIME = 100;

// global variables
byte input_int  = 0;
byte output_int = 0;

void setup() {
    // set modes for pins
    pinMode(SONIC_TRIG,  OUTPUT);
    pinMode(SONIC_ECHO,  INPUT);
    pinMode(SERVO,       OUTPUT);
    pinMode(RIGHT_MOTOR, OUTPUT);
    pinMode(LEFT_MOTOR,  OUTPUT);

    // start the serial monitor
    Serial.begin(BAUD_RATE);

    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);

    // define callbacks for i2c communication
    Wire.onReceive(ReceiveData);
    Wire.onRequest(SendData);

    Serial.println("Slave is ready.");
}

void loop() {
    if (input_int == KEYBOARD_STOP) {
        Stop();
        Serial.println("Motors are stopped.");
        output_int = KEYBOARD_STOP;
    } else if (input_int == KEYBOARD_W) {
        MoveForward();
        Serial.println("Moving forward.");
        output_int = KEYBOARD_W;
    } else if (input_int == KEYBOARD_A) {
        TurnLeft();
        Serial.println("Turning left.");
        output_int = KEYBOARD_A;
    } else if (input_int == KEYBOARD_S) {
        MoveBackward();
        Serial.println("Moving backward.");
        output_int = KEYBOARD_S;
    } else if (input_int == KEYBOARD_D) {
        TurnRight();
        Serial.println("Turning right.");
        output_int = KEYBOARD_D;
    }
}

// callback for received data
void ReceiveData(int num_bytes) {
    num_bytes = 0;

    while (Wire.available()) {
        input_int = Wire.read();
        Serial.print("Received from master: ");
        Serial.println(input_int);
        num_bytes++;
    }
}

// callback for sending data
void SendData(void) {
    Wire.write(output_int);
}

void Stop(void) {
    analogWrite(RIGHT_MOTOR, 0);
    analogWrite(LEFT_MOTOR,  0);
}

void MoveForward(void) {
    Stop();
    delay(WAIT_TIME);
    analogWrite(RIGHT_MOTOR, POWER);
    analogWrite(LEFT_MOTOR,  POWER);
}

void MoveBackward(void) {
    Stop();
    delay(WAIT_TIME);
    analogWrite(RIGHT_MOTOR, -POWER);
    analogWrite(LEFT_MOTOR,  -POWER);
}

void TurnLeft(void) {
    Stop();
    delay(WAIT_TIME);
    analogWrite(RIGHT_MOTOR, POWER);
    analogWrite(LEFT_MOTOR,  -POWER);
}

void TurnRight(void) {
    Stop();
    delay(WAIT_TIME);
    analogWrite(RIGHT_MOTOR, -POWER);
    analogWrite(LEFT_MOTOR,  POWER);
}
