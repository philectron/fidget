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

#define SLAVE_ADDRESS           0x04
#define KEYBOARD_STOP           0
#define KEYBOARD_FORWARD        1
#define KEYBOARD_STEER_LEFT     2
#define KEYBOARD_BACKWARD       3
#define KEYBOARD_STEER_RIGHT    4
#define KEYBOARD_TURN_LEFT      5
#define KEYBOARD_TURN_RIGHT     6
#define KEYBOARD_LOOK_LEFT      7
#define KEYBOARD_LOOK_RIGHT     8

// DC motors' pinout on the L294DNE DIP package
const byte LEFT_MOTOR_ENABLE_2 = 3;
const byte LEFT_MOTOR_INPUT_3 = 4;
const byte LEFT_MOTOR_INPUT_4 = 2;
const byte RIGHT_MOTOR_ENABLE_1 = 9;
const byte RIGHT_MOTOR_INPUT_1 = 8;
const byte RIGHT_MOTOR_INPUT_2 = 10;

// global constants
const int BAUD_RATE = 9600;
const byte MOTOR_HIGH_POWER = 255;
const byte MOTOR_LOW_POWER = 127;
const int MOTOR_DURATION = 10;

// global variables
byte from_master = 0;
byte to_master = 0;

void setup() {
    // set modes for pins
    pinMode(LEFT_MOTOR_ENABLE_2, OUTPUT);
    pinMode(LEFT_MOTOR_INPUT_3, OUTPUT);
    pinMode(LEFT_MOTOR_INPUT_4, OUTPUT);
    pinMode(RIGHT_MOTOR_ENABLE_1, OUTPUT);
    pinMode(RIGHT_MOTOR_INPUT_1, OUTPUT);
    pinMode(RIGHT_MOTOR_INPUT_2, OUTPUT);

    // start the serial monitor
    Serial.begin(BAUD_RATE);

    // initialize i2c communication and set Arduino as the slave
    Wire.begin(SLAVE_ADDRESS);

    // define callbacks for i2c communication
    Wire.onReceive(ReceiveI2C);
    Wire.onRequest(SendI2C);
}

void loop() {
    int duration = 0;

    // make the robot move based on the keyboard input
    while (duration < MOTOR_DURATION) {
        Serial.print("    Duration = ");
        Serial.println(duration);
        if (from_master == KEYBOARD_FORWARD) {
            MoveForward();
        } else if (from_master == KEYBOARD_STEER_LEFT) {
            SteerLeft();
        } else if (from_master == KEYBOARD_BACKWARD) {
            MoveBackWard();
        } else if (from_master == KEYBOARD_STEER_RIGHT) {
            SteerRight();
        } else if (from_master == KEYBOARD_TURN_LEFT) {
            TurnLeft();
        } else if (from_master == KEYBOARD_TURN_RIGHT) {
            TurnRight();
        } else if (from_master == KEYBOARD_LOOK_LEFT) {
            LookLeft();
        } else if (from_master == KEYBOARD_LOOK_RIGHT) {
            LookRight();
        } else {
            Stop();
        }
        duration++;
    }
}

/**
 * Callback for receiving data
 */
void ReceiveI2C(int byte_count) {
    // read data when available
    while (Wire.available()) {
        // read from master
        from_master = Wire.read();

        // display the input on the serial monitor
        Serial.print("Received from master: ");
        Serial.println(from_master);
    }
}

/**
 * Callback for sending data
 */
void SendI2C(void) {
    // write to master
    Wire.write(to_master);

    // display the output on the serial monitor
    Serial.print("Sent to master: ");
    Serial.println(to_master);
}

/**
 * Sets both motors to rotate forward with the same power. Moves the robot
 * forward.
 */
void MoveForward(void) {
    digitalWrite(LEFT_MOTOR_INPUT_3, HIGH);
    digitalWrite(LEFT_MOTOR_INPUT_4, LOW);
    analogWrite(LEFT_MOTOR_ENABLE_2, MOTOR_HIGH_POWER);
    digitalWrite(RIGHT_MOTOR_INPUT_1, HIGH);
    digitalWrite(RIGHT_MOTOR_INPUT_2, LOW);
    analogWrite(RIGHT_MOTOR_ENABLE_1, MOTOR_HIGH_POWER);
}

/**
 * Sets both motors to rotate backward with the same power. Moves the robot
 * backward.
 */
void MoveBackWard(void) {
    digitalWrite(LEFT_MOTOR_INPUT_3, LOW);
    digitalWrite(LEFT_MOTOR_INPUT_4, HIGH);
    analogWrite(LEFT_MOTOR_ENABLE_2, MOTOR_HIGH_POWER);
    digitalWrite(RIGHT_MOTOR_INPUT_1, LOW);
    digitalWrite(RIGHT_MOTOR_INPUT_2, HIGH);
    analogWrite(RIGHT_MOTOR_ENABLE_1, MOTOR_HIGH_POWER);
}

/**
 * Sets both motors to rotate forward, but provides the left motor with less
 * power than the right motor. Steers the robot to the left.
 */
void SteerLeft(void) {
    digitalWrite(LEFT_MOTOR_INPUT_3, HIGH);
    digitalWrite(LEFT_MOTOR_INPUT_4, LOW);
    analogWrite(LEFT_MOTOR_ENABLE_2, MOTOR_LOW_POWER);
    digitalWrite(RIGHT_MOTOR_INPUT_1, HIGH);
    digitalWrite(RIGHT_MOTOR_INPUT_2, LOW);
    analogWrite(RIGHT_MOTOR_ENABLE_1, MOTOR_HIGH_POWER);
}

/**
 * Sets both motors to rotate forward, but provides the right motor with less
 * power than the left motor. Steers the robot to the right.
 */
void SteerRight(void) {
    digitalWrite(LEFT_MOTOR_INPUT_3, HIGH);
    digitalWrite(LEFT_MOTOR_INPUT_4, LOW);
    analogWrite(LEFT_MOTOR_ENABLE_2, MOTOR_HIGH_POWER);
    digitalWrite(RIGHT_MOTOR_INPUT_1, HIGH);
    digitalWrite(RIGHT_MOTOR_INPUT_2, LOW);
    analogWrite(RIGHT_MOTOR_ENABLE_1, MOTOR_LOW_POWER);
}

/**
 * Sets left motor to rotate backward and right motor to rotate forward with the
 * same power. Turns the robot to the left.
 */
void TurnLeft(void) {
    digitalWrite(LEFT_MOTOR_INPUT_3, LOW);
    digitalWrite(LEFT_MOTOR_INPUT_4, HIGH);
    analogWrite(LEFT_MOTOR_ENABLE_2, MOTOR_HIGH_POWER);
    digitalWrite(RIGHT_MOTOR_INPUT_1, HIGH);
    digitalWrite(RIGHT_MOTOR_INPUT_2, LOW);
    analogWrite(RIGHT_MOTOR_ENABLE_1, MOTOR_HIGH_POWER);
}

/**
 * Sets left motor to rotate forward and right motor to rotate backward with the
 * same power. Turns the robot to the right.
 */
void TurnRight(void) {
    digitalWrite(LEFT_MOTOR_INPUT_3, HIGH);
    digitalWrite(LEFT_MOTOR_INPUT_4, LOW);
    analogWrite(LEFT_MOTOR_ENABLE_2, MOTOR_HIGH_POWER);
    digitalWrite(RIGHT_MOTOR_INPUT_1, LOW);
    digitalWrite(RIGHT_MOTOR_INPUT_2, HIGH);
    analogWrite(RIGHT_MOTOR_ENABLE_1, MOTOR_HIGH_POWER);
}

/**
 * Rotates the servo a numer of degrees to the left.
 */
void LookLeft(void) {}

/**
 * Rotates the servo a number of degrees to the right.
 */
void LookRight(void) {}

void Stop(void) {
    analogWrite(LEFT_MOTOR_ENABLE_2, 0);
    analogWrite(RIGHT_MOTOR_ENABLE_1, 0);
}
