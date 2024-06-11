#include "Initializations.h"

// Standing Control Points Array
Vector3 SCPA[6][10];

Vector3 standingStartPoints[6];      // the points the legs are at in the beginning of the standing state
Vector3 standingInBetweenPoints[6];  // the middle points of the bezier curves that the legs will follow to smoothly transition to the end points
Vector3 standingEndPoint;

int currentLegs[3] = { -1, -1, -1 };
int standLoops = 0;

void standingState() {
  bool moveAllAtOnce = false;
  bool highLift = false;
  setCycleStartPoints();
  standingEndPoint = Vector3(distanceFromCenter, 0, distanceFromGround + standingDistanceAdjustment);
  standLoops = 2;
  
  if (currentState == Calibrate || currentState == Initialize || currentState == SlamAttack) moveAllAtOnce = true;
  if (currentState == SlamAttack) highLift = true;
  
  if (currentState != Stand) {
    set3HighestLeg();
    standLoops = 0;
    standProgress = 0;
    memcpy(standingStartPoints, currentPoints, sizeof(currentPoints[0]) * 6);
    currentState = Stand;

    // Calculate the inbetween and ending points
    for (int i = 0; i < 6; i++) {
      Vector3 inBetweenPoint = standingStartPoints[i];
      inBetweenPoint.x = (inBetweenPoint.x + standingEndPoint.x) / 2;
      inBetweenPoint.y = (inBetweenPoint.y + standingEndPoint.y) / 2;
      inBetweenPoint.z = ((inBetweenPoint.z + standingEndPoint.z) / 2);
      if (abs(inBetweenPoint.z - standingEndPoint.z) < 50) inBetweenPoint.z += 50;
      if (highLift) inBetweenPoint.z += 150;
      standingInBetweenPoints[i] = inBetweenPoint;
      SCPA[i][0] = standingStartPoints[i];
      SCPA[i][1] = standingInBetweenPoints[i];
      SCPA[i][2] = standingEndPoint;
    }

    for (int i = 0; i < 6; i++) {
      legStates[i] = Standing;
    } 
  }

  // Update distance from ground constantly
  for (int i = 0; i < 6; i++) {
    SCPA[i][2] = standingEndPoint;
  }

  // Readjusting. This takes about a second
  while (standLoops < 2) {
    standProgress += 25;
    if (highLift) {
      standProgress += 40 - 50 * ((float)standProgress / points);
    }

    float t = (float)standProgress / points;
    if (t > 1) {
      t = 1;
    }

    if (moveAllAtOnce) {
      for (int i = 0; i < 6; i++) {
        moveToPos(i, GetPointOnBezierCurve(SCPA[i], 3, t));
      }

      if (standProgress > points) {
        standProgress = 0;
        standLoops = 2;
      }
    } else {
      for (int i = 0; i < 3; i++) {
        if (currentLegs[i] != -1) {
          moveToPos(currentLegs[i], GetPointOnBezierCurve(SCPA[currentLegs[i]], 3, t));
        }
      }

      if (standProgress > points) {
        standProgress = 0;
        standLoops++;
        set3HighestLeg();
      }
    }
  }

  // Constantly move to the standing end position
  for (int i = 0; i < 6; i++) {
    moveToPos(i, GetPointOnBezierCurve(SCPA[i], 3, 1));
  }
  return;
}

void set3HighestLeg() {
  currentLegs[0] = -1;
  currentLegs[1] = -1;
  currentLegs[2] = -1;
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 6; i++) {  // go through the legs
      if (currentLegs[0] == i || currentLegs[1] == i || currentLegs[2] == i) continue;
      if (currentPoints[i] == standingEndPoint) continue;
      if (currentLegs[j] == -1 || currentPoints[i].z > currentPoints[currentLegs[j]].z) {
        currentLegs[j] = i;
      }
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

// //Standing Control Points Array
// Vector3 SCPA[6][10];

// Vector3 standingStartPoints[6];      //the points the legs are at in the beginning of the standing state
// Vector3 standingInBetweenPoints[6];  //the middle points of the bezier curves that the legs will follow to smoothly transition to the end points
// Vector3 standingEndPoint;

// int currentLegs[3] = { -1, -1, -1 };
// int standLoops = 0;

// void standingState() {
//   bool moveAllAtOnce = false;
//   bool highLift = false;
//   setCycleStartPoints();
//   standingEndPoint = Vector3(distanceFromCenter, 0, distanceFromGround + standingDistanceAdjustment);
//   standLoops = 2;
//   // We only set the starting, inbetween, and ending points one time, which is when we enter the standing state.
//   if (currentState == Calibrate || currentState == Initialize || currentState == SlamAttack) moveAllAtOnce = true;
//   if (currentState == SlamAttack) highLift = true;
//   if (currentState != Stand) {
    
//     set3HighestLeg();
//     standLoops = 0;
//     standProgress = 0;
//     memcpy(standingStartPoints, currentPoints, sizeof(currentPoints[0]) * 6);
//     currentState = Stand;

//     // Calculate the inbetween and ending points
//     for (int i = 0; i < 6; i++) {
//       Vector3 inBetweenPoint = standingStartPoints[i];
//       inBetweenPoint.x = (inBetweenPoint.x + standingEndPoint.x) / 2;
//       inBetweenPoint.y = (inBetweenPoint.y + standingEndPoint.y) / 2;

//       inBetweenPoint.z = ((inBetweenPoint.z + standingEndPoint.z) / 2);
//       if(abs(inBetweenPoint.z - standingEndPoint.z) < 50 )inBetweenPoint.z += 50;
//       if(highLift)inBetweenPoint.z += 150;

//       standingInBetweenPoints[i] = inBetweenPoint;

//       SCPA[i][0] = standingStartPoints[i];
//       SCPA[i][1] = standingInBetweenPoints[i];
//       SCPA[i][2] = standingEndPoint;
//     }

//     for(int i = 0; i < 6; i++){
//       legStates[i] = Standing;
//     } 
//   }

//   //update distance from ground constantly
//   for (int i = 0; i < 6; i++) {
//     SCPA[i][2] = standingEndPoint;
//   }

//   //readjusting. This takes about a second
//   while(standLoops < 2){
//     standProgress += 25;
//     if(highLift){
//       standProgress += 40 - 50 * ((float)standProgress / points);
//     }

//     float t = (float)standProgress / points;
//     if (t > 1) {
//       t = 1;
//     }

//     if(moveAllAtOnce){
//       for (int i = 0; i < 6; i++) {
//         moveToPos(i, GetPointOnBezierCurve(SCPA[i], 3, t));
//       }

//       if (standProgress > points) {
//         standProgress = 0;
//         standLoops = 2;
//       }
//     }

//     else{
//       for (int i = 0; i < 3; i++) {
//         if (currentLegs[i] != -1) {
//           moveToPos(currentLegs[i], GetPointOnBezierCurve(SCPA[currentLegs[i]], 3, t));
//         }
//       }

//       if (standProgress > points) {
//         standProgress = 0;
//         standLoops ++;
//         set3HighestLeg();
//       }
//     }
//   }


//   //constantly move to the standing end position
//   for (int i = 0; i < 6; i++) {
//     moveToPos(i, GetPointOnBezierCurve(SCPA[i], 3, 1));
//   }
//   return;
// }

// void set3HighestLeg() {

//   currentLegs[0] = -1;
//   currentLegs[1] = -1;
//   currentLegs[2] = -1;
//   for (int j = 0; j < 3; j++) {
//     for (int i = 0; i < 6; i++) {  //go through the legs
//       //if the leg is already on the list of current legs, skip it
//       if (currentLegs[0] == i || currentLegs[1] == i || currentLegs[2] == i) continue;

//       //if the leg is already in position, dont add it
//       if (currentPoints[i] == standingEndPoint) continue;

//       //if the legs z is greater than the leg already there, add it
//       if (currentLegs[j] == -1 || currentPoints[i].z > currentPoints[currentLegs[j]].z) {
//         currentLegs[j] = i;
//       }
//     }
//   }
// }