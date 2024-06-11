#include "Initializations.h"

bool slamstarted = false;

void slamAttack() {
  setCycleStartPoints();
  currentState = SlamAttack;
  slamstarted = false;

  int attackSpeed = map(rc_data.slider2, 0, 100, 20, 100);
  attackSpeed = 25;

  float frames = attackSpeed * 0.4;
  for (int i = 0; i < frames; i++) {
    float t = (float)i / frames;
    moveToPos(0, getFootPlacementPathPoint(0, t));
    moveToPos(1, getFootPlacementPathPoint(1, t));
    moveToPos(2, getFootPlacementPathPoint(2, t));
    moveToPos(3, getFootPlacementPathPoint(3, t));
    moveToPos(4, getFootPlacementPathPoint(4, t));
    moveToPos(5, getFootPlacementPathPoint(5, t));
  }
  setCycleStartPoints();

  frames = attackSpeed * 1.2;
  for (int i = 0; i < frames; i++) {
    float t = (float)i / (frames);
    moveToPos(0, getLeapPathPoint(0, t));
    moveToPos(1, getLeapPathPoint(1, t));
    moveToPos(4, getLeapPathPoint(4, t));
    moveToPos(5, getLeapPathPoint(5, t));

    moveToPos(2, getSlamPathPoint(2, t));
    moveToPos(3, getSlamPathPoint(3, t));
    if (t >= 0.5 && slamstarted == false) {
      slamstarted = true;      
    }
  }
  delay(100);
  setCycleStartPoints();
}

Vector3 getFootPlacementPathPoint(int leg, float t) {
  int xOffset = 0;
  int yOffset = 0;
  int zOffset = 0;

  if (leg == 1) {  
    zOffset = -60;
    yOffset = -50;
    xOffset = -70;
  }
  if (leg == 4) {   
    zOffset = -50;
    yOffset = -60;
    xOffset = -70;
  }

  if (leg == 0) {  
    xOffset = 40;
  }
  if (leg == 5) {   
    xOffset = 40;
  }

  float x = cycleStartPoints[leg].x + xOffset;

  ControlPoints[0] = cycleStartPoints[leg];
  ControlPoints[1] = Vector3(x, -50 * strideMultiplier[leg], -50 + zOffset).rotate(55 * rotationMultiplier[leg], Vector2(x, 0)); 
  Vector3 point = GetPointOnBezierCurve(ControlPoints, 2, t);

  return point;
}

Vector3 getLeapPathPoint(int leg, float t) {
  float x = cycleStartPoints[leg].x;
  Vector3 start = cycleStartPoints[leg];
  Vector3 end = Vector3(x - 20, cycleStartPoints[leg].y + (160 * strideMultiplier[leg]), -80).rotate(55 * rotationMultiplier[leg], Vector2(x, 0));
  Vector3 middle = ((start + end) * 0.5) + Vector3(0, 0, -300);

  if (leg == 0 || leg == 5) {
    middle.z += 180;
  }

  ControlPoints[0] = start;
  ControlPoints[1] = middle;
  ControlPoints[2] = end;
  Vector3 point = GetPointOnBezierCurve(ControlPoints, 3, t);
  return point;
}

Vector3 getSlamPathPoint(int leg, float t) {
  float slamPercentage = 0.70;
  float landPercentage = 0.95;

  // Leg Raise
  if (t < slamPercentage) {
    ControlPoints[0] = cycleStartPoints[leg];
    ControlPoints[1] = Vector3(200, 0, 200).rotate(-40 * rotationMultiplier[leg], Vector2(0, 0)); 
    ControlPoints[2] = Vector3(0, 0, 300).rotate(-35 * rotationMultiplier[leg], Vector2(0, 0)); 
    Vector3 point = GetPointOnBezierCurve(ControlPoints, 3, mapFloat(t, 0, slamPercentage, 0, 1));
    return point;
  }

  // Leg Slam
  if (t >= slamPercentage && t < landPercentage) {
    ControlPoints[0] = Vector3(0, 0, 300).rotate(-35 * rotationMultiplier[leg], Vector2(0, 0));
    ControlPoints[1] = Vector3(300, 0, 300).rotate(-35 * rotationMultiplier[leg], Vector2(0, 0)); 
    ControlPoints[2] = Vector3(325, 0, 50).rotate(-35 * rotationMultiplier[leg], Vector2(0, 0)); 
    ControlPoints[3] = Vector3(250, 0, 0).rotate(-35 * rotationMultiplier[leg], Vector2(0, 0)); 
    Vector3 point = GetPointOnBezierCurve(ControlPoints, 4, mapFloat(t, slamPercentage, landPercentage, 0, 1));
    return point;
  }

  if (t >= landPercentage) {
    return Vector3(250, 0, 0).rotate(-35 * rotationMultiplier[leg], Vector2(0, 0));
  }
  return Vector3(0, 0, 0); // Default return to avoid compiler warning
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

// bool slamstarted = false;
// void slamAttack(){
//   setCycleStartPoints();
//   currentState = SlamAttack;
//   slamstarted = false;

//   int attackSpeed = map(rc_data.slider2,0,100,20,100);
//   attackSpeed = 25;

//   float frames = attackSpeed*0.4;
//   for(int i = 0; i < frames; i++){
//     float t = (float)i/frames;
//     moveToPos(0, getFootPlacementPathPoint(0,t));
//     moveToPos(1, getFootPlacementPathPoint(1,t));
//     moveToPos(2, getFootPlacementPathPoint(2,t));
//     moveToPos(3, getFootPlacementPathPoint(3,t));
//     moveToPos(4, getFootPlacementPathPoint(4,t));
//     moveToPos(5, getFootPlacementPathPoint(5,t));
//   }
//   setCycleStartPoints();

//   frames = attackSpeed*1.2;
//   for(int i = 0; i < frames; i++){
//     float t = (float)i/(frames);
//     moveToPos(0, getLeapPathPoint(0,t));
//     moveToPos(1, getLeapPathPoint(1,t));
//     moveToPos(4, getLeapPathPoint(4,t));
//     moveToPos(5, getLeapPathPoint(5,t));

//     moveToPos(2, getSlamPathPoint(2,t));
//     moveToPos(3, getSlamPathPoint(3,t));
//     if(t >= 0.5 && slamstarted == false) {
//       slamstarted = true;      
//     }
//   }
//   delay(100);
//   setCycleStartPoints();
// }

// Vector3 getFootPlacementPathPoint(int leg, float t){
//   int xOffset = 0;
//   int yOffset = 0;
//   int zOffset = 0;

//   if(leg == 1){  
//     zOffset = -60;
//     yOffset = -50;
//     xOffset = -70;
//   }
//   if(leg == 4){   
//     zOffset = -50;
//     yOffset = -60;
//     xOffset = -70;
//   }

//   if(leg == 0){  
//     xOffset = 40;
//   }
//   if(leg == 5){   
//     xOffset = 40;
//   }

//   float x = cycleStartPoints[leg].x + xOffset;

//   ControlPoints[0] = cycleStartPoints[leg];
//   ControlPoints[1] = Vector3(x, -50 * strideMultiplier[leg], -50 + zOffset).rotate(55 * rotationMultiplier[leg], Vector2(x,0)); 
//   Vector3 point = GetPointOnBezierCurve(ControlPoints, 2, t);

//   return point;
// }


// Vector3 getLeapPathPoint(int leg, float t){
//   float x = cycleStartPoints[leg].x;
//   Vector3 start = cycleStartPoints[leg];
//   Vector3 end = Vector3(x-20, cycleStartPoints[leg].y + (160 * strideMultiplier[leg]), -80).rotate(55 * rotationMultiplier[leg], Vector2(x,0));
//   Vector3 middle = ((start + end)*0.5) + Vector3(0,0,-300);

//   if(leg == 0 || leg == 5){
//     middle.z += 180;
//   }

//   ControlPoints[0] = start;
//   ControlPoints[1] = middle;
//   ControlPoints[2] = end;
//   Vector3 point = GetPointOnBezierCurve(ControlPoints, 3, t);
//   return point;
// }


// Vector3 getSlamPathPoint(int leg, float t){

//   float slamPercentage = 0.70;
//   float landPercentage = 0.95;
//   //Leg Raise
//   if(t < slamPercentage){
//     ControlPoints[0] = cycleStartPoints[leg];
//     ControlPoints[1] = Vector3(200, 0, 200).rotate(-40 * rotationMultiplier[leg], Vector2(0,0)); 
//     ControlPoints[2] = Vector3(0, 0, 300).rotate(-35 * rotationMultiplier[leg], Vector2(0,0)); 
//     Vector3 point = GetPointOnBezierCurve(ControlPoints, 3, mapFloat(t,0,slamPercentage,0,1));
//     return point;
//   }

//   //Leg Slam
//   if(t >= slamPercentage && t < landPercentage){
//     ControlPoints[0] = Vector3(0, 0, 300).rotate(-35 * rotationMultiplier[leg], Vector2(0,0));
//     ControlPoints[1] = Vector3(300, 0, 300).rotate(-35 * rotationMultiplier[leg], Vector2(0,0)); 
//     ControlPoints[2] = Vector3(325, 0, 50).rotate(-35 * rotationMultiplier[leg], Vector2(0,0)); 
//     ControlPoints[3] = Vector3(250, 0, 0).rotate(-35 * rotationMultiplier[leg], Vector2(0,0)); 
//     Vector3 point = GetPointOnBezierCurve(ControlPoints, 4, mapFloat(t,slamPercentage,landPercentage,0,1));
//     return point;
//   }

//   if(t >= landPercentage){
//     return Vector3(250, 0, 0).rotate(-35 * rotationMultiplier[leg], Vector2(0,0));
//   }  
// }