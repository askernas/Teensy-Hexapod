#ifndef INITIALIZATIONS_H
#define INITIALIZATIONS_H

#include <Adafruit_PWMServoDriver.h>

// Define PCA9685 instances
extern Adafruit_PWMServoDriver pwm1;
extern Adafruit_PWMServoDriver pwm2;

// Right Side - Board 1 (pwm1)
#define RIGHT_FRONT_COXA_PIN 0
#define RIGHT_FRONT_FEMUR_PIN 1
#define RIGHT_FRONT_TIBIA_PIN 2

#define RIGHT_MIDDLE_COXA_PIN 4
#define RIGHT_MIDDLE_FEMUR_PIN 5
#define RIGHT_MIDDLE_TIBIA_PIN 6

#define RIGHT_REAR_COXA_PIN 8
#define RIGHT_REAR_FEMUR_PIN 9
#define RIGHT_REAR_TIBIA_PIN 10

// Left Side - Board 2 (pwm2)
#define LEFT_BACK_COXA_PIN 0
#define LEFT_BACK_FEMUR_PIN 1
#define LEFT_BACK_TIBIA_PIN 2

#define LEFT_MIDDLE_COXA_PIN 4
#define LEFT_MIDDLE_FEMUR_PIN 5
#define LEFT_MIDDLE_TIBIA_PIN 6

#define LEFT_FRONT_COXA_PIN 8
#define LEFT_FRONT_FEMUR_PIN 9
#define LEFT_FRONT_TIBIA_PIN 10

// Vector offsets for each leg
const Vector3 offsets1 = {90, 75, -18};  // Right front leg
const Vector3 offsets2 = {93, 75, -15};  // Right middle leg
const Vector3 offsets3 = {93, 75, -18};  // Right rear leg
const Vector3 offsets4 = {87, 80, -26};  // Left back leg
const Vector3 offsets5 = {85, 89, -16};  // Left middle leg
const Vector3 offsets6 = {93, 85, -24};  // Left front leg
const Vector3 offsets[6] = {offsets1, offsets2, offsets3, offsets4, offsets5, offsets6};

// Leg dimensions
const float a1 = 41;  // Coxa Length
const float a2 = 116; // Femur Length
const float a3 = 183; // Tibia Length   
float legLength = a1 + a2 + a3;

// Other constants and variables
Vector3 currentPoints[6];
Vector3 cycleStartPoints[6];
Vector3 currentRot(180, 0, 180);
Vector3 targetRot(180, 0, 180);
float strideMultiplier[6] = {1, 1, 1, -1, -1, -1};
float rotationMultiplier[6] = {-1, 0, 1, -1, 0, 1};
Vector3 ControlPoints[10];
Vector3 RotateControlPoints[10];
Vector3 AttackControlPoints[10];

// Function to attach servos to PCA9685 boards
void attachServos() {
    // Right Side - Board 1 (pwm1)
    pwm1.setPWM(RIGHT_FRONT_COXA_PIN, 0, pulseWidthToCount(1500));
    pwm1.setPWM(RIGHT_FRONT_FEMUR_PIN, 0, pulseWidthToCount(1500));
    pwm1.setPWM(RIGHT_FRONT_TIBIA_PIN, 0, pulseWidthToCount(1500));

    pwm1.setPWM(RIGHT_MIDDLE_COXA_PIN, 0, pulseWidthToCount(1500));
    pwm1.setPWM(RIGHT_MIDDLE_FEMUR_PIN, 0, pulseWidthToCount(1500));
    pwm1.setPWM(RIGHT_MIDDLE_TIBIA_PIN, 0, pulseWidthToCount(1500));

    pwm1.setPWM(RIGHT_REAR_COXA_PIN, 0, pulseWidthToCount(1500));
    pwm1.setPWM(RIGHT_REAR_FEMUR_PIN, 0, pulseWidthToCount(1500));
    pwm1.setPWM(RIGHT_REAR_TIBIA_PIN, 0, pulseWidthToCount(1500));

    // Left Side - Board 2 (pwm2)
    pwm2.setPWM(LEFT_BACK_COXA_PIN, 0, pulseWidthToCount(1500));
    pwm2.setPWM(LEFT_BACK_FEMUR_PIN, 0, pulseWidthToCount(1500));
    pwm2.setPWM(LEFT_BACK_TIBIA_PIN, 0, pulseWidthToCount(1500));

    pwm2.setPWM(LEFT_MIDDLE_COXA_PIN, 0, pulseWidthToCount(1500));
    pwm2.setPWM(LEFT_MIDDLE_FEMUR_PIN, 0, pulseWidthToCount(1500));
    pwm2.setPWM(LEFT_MIDDLE_TIBIA_PIN, 0, pulseWidthToCount(1500));

    pwm2.setPWM(LEFT_FRONT_COXA_PIN, 0, pulseWidthToCount(1500));
    pwm2.setPWM(LEFT_FRONT_FEMUR_PIN, 0, pulseWidthToCount(1500));
    pwm2.setPWM(LEFT_FRONT_TIBIA_PIN, 0, pulseWidthToCount(1500));
}

// Helper function to convert microseconds to PCA9685 pulse width counts
int pulseWidthToCount(int microseconds) {
    return map(microseconds, 500, 2500, SERVOMIN, SERVOMAX);
}

#endif // INITIALIZATIONS_H
