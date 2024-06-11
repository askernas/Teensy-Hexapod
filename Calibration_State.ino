#include "Initializations.h"

// Constants
Vector3 targetCalibration = Vector3(224, 0, 116);
int inBetweenZ = -20;

void calibrationState() {
  currentState = Calibrate;

  bool legsUp = true;

  for (int i = 0; i < 6; i++) {
    if (currentPoints[i].z < inBetweenZ) legsUp = false;
  }

  if (!legsUp) {
    for (int i = 0; i < 6; i++) {
      float nextZ = lerp(currentPoints[i].z, inBetweenZ + 2, 0.02);
      moveToPos(i, Vector3(currentPoints[i].x, currentPoints[i].y, nextZ));  
    }
  } else {
    for (int i = 0; i < 6; i++) {
      float nextX = min(currentPoints[i].x + 0.5, targetCalibration.x);
      float nextY = min(currentPoints[i].y + 0.5, targetCalibration.y);
      float nextZ = min(currentPoints[i].z + 0.5, targetCalibration.z);
      moveToPos(i, Vector3(nextX, nextY, nextZ));
    }
  }
}

void moveToPos(int leg, Vector3 targetPos) {
  // Calculate angles for each servo
  float coxaAngle = calculateCoxaAngle(targetPos);
  float femurAngle = calculateFemurAngle(targetPos);
  float tibiaAngle = calculateTibiaAngle(targetPos);

  // Set servo positions using PCA9685
  switch (leg) {
    case 0:
      setServoPosition(pwm1, coxa1Channel, coxaAngle);
      setServoPosition(pwm1, femur1Channel, femurAngle);
      setServoPosition(pwm1, tibia1Channel, tibiaAngle);
      break;
    case 1:
      setServoPosition(pwm1, coxa2Channel, coxaAngle);
      setServoPosition(pwm1, femur2Channel, femurAngle);
      setServoPosition(pwm1, tibia2Channel, tibiaAngle);
      break;
    case 2:
      setServoPosition(pwm1, coxa3Channel, coxaAngle);
      setServoPosition(pwm1, femur3Channel, femurAngle);
      setServoPosition(pwm1, tibia3Channel, tibiaAngle);
      break;
    case 3:
      setServoPosition(pwm1, coxa4Channel, coxaAngle);
      setServoPosition(pwm1, femur4Channel, femurAngle);
      setServoPosition(pwm1, tibia4Channel, tibiaAngle);
      break;
    case 4:
      setServoPosition(pwm1, coxa5Channel, coxaAngle);
      setServoPosition(pwm1, femur5Channel, femurAngle);
      setServoPosition(pwm1, tibia5Channel, tibiaAngle);
      break;
    case 5:
      setServoPosition(pwm1, coxa6Channel, coxaAngle);
      setServoPosition(pwm1, femur6Channel, femurAngle);
      setServoPosition(pwm1, tibia6Channel, tibiaAngle);
      break;
  }
}

float calculateCoxaAngle(Vector3 targetPos) {
  // Placeholder function to calculate coxa angle
  return map(targetPos.x, -90, 90, 0, 180);
}

float calculateFemurAngle(Vector3 targetPos) {
  // Placeholder function to calculate femur angle
  return map(targetPos.y, -90, 90, 0, 180);
}

float calculateTibiaAngle(Vector3 targetPos) {
  // Placeholder function to calculate tibia angle
  return map(targetPos.z, -90, 90, 0, 180);
}

// Vector3 targetCalibration = Vector3(224,0,116);
// int inBetweenZ = -20;

// void calibrationState(){
//   currentState = Calibrate;

//   bool legsUp = true;

//   for (int i = 0; i < 6; i++){
//     if(currentPoints[i].z < inBetweenZ) legsUp = false;
//   }

//   if(!legsUp){
//     for (int i = 0; i < 6; i++){
//       float nextZ = lerp(currentPoints[i].z, inBetweenZ + 2, 0.02);
//       moveToPos(i, Vector3(currentPoints[i].x,currentPoints[i].y,nextZ));  
//     }
//   }
//   else{
//     for (int i = 0; i < 6; i++){
//       float nextX = min(currentPoints[i].x + 0.5, targetCalibration.x);
//       float nextY = min(currentPoints[i].y + 0.5, targetCalibration.y);
//       float nextZ = min(currentPoints[i].z + 0.5, targetCalibration.z);
//       moveToPos(i, Vector3(nextX,nextY,nextZ));
//     }
//   }
// }