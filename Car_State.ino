#include "Initializations.h"

// Variables
float forwardAmount;
float turnAmount;
float tArray[6];
int ControlPointsAmount = 0;
int RotateControlPointsAmount = 0;
float pushFraction = 3.0 / 6.0;
float speedMultiplier = 0.5;
float strideLengthMultiplier = 1.5;
float liftHeightMultiplier = 1.0;
float maxStrideLength = 200;
float maxSpeed = 100;
float legPlacementAngle = 56;

int leftSlider = 50;
float globalSpeedMultiplier = 0.55;
float globalRotationMultiplier = 0.55;

void carState() {
  leftSlider = (int)rc_data.slider2;
  globalSpeedMultiplier = (leftSlider + 10.0) * 0.01;
  globalRotationMultiplier = map(rc_data.slider2, 0, 100, 40, 130) * 0.01;

  if (currentState != Car || previousGait != currentGait) {
    currentState = Car;

    // Initialize Leg States
    for (int i = 0; i < 6; i++) {
      legStates[i] = Reset;
    }   

    switch (currentGait) {
      case Tri:
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 2);
        cycleProgress[2] = 0;
        cycleProgress[3] = (points / 2);
        cycleProgress[4] = 0;
        cycleProgress[5] = (points / 2);

        pushFraction = 3.1 / 6.0;
        speedMultiplier = 1;
        strideLengthMultiplier = 1.2;
        liftHeightMultiplier = 1.1;
        maxStrideLength = 240;
        maxSpeed = 200;
        break;

      case Wave:
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 6);
        cycleProgress[2] = (points / 6) * 2;
        cycleProgress[3] = (points / 6) * 5;
        cycleProgress[4] = (points / 6) * 4;
        cycleProgress[5] = (points / 6) * 3;

        pushFraction = 5.0 / 6.0;
        speedMultiplier = 0.40;
        strideLengthMultiplier = 2;
        liftHeightMultiplier = 1.3;
        maxStrideLength = 150;
        maxSpeed = 160;
        break;

      case Ripple:
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 6) * 4;
        cycleProgress[2] = (points / 6) * 2;
        cycleProgress[3] = (points / 6) * 5;
        cycleProgress[4] = (points / 6);
        cycleProgress[5] = (points / 6) * 3;

        pushFraction = 3.2 / 6.0;
        speedMultiplier = 1;
        strideLengthMultiplier = 1.3;
        liftHeightMultiplier = 1;
        maxStrideLength = 220;
        maxSpeed = 200;
        break;

      case Bi:
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 3);
        cycleProgress[2] = (points / 3) * 2;
        cycleProgress[3] = 0;
        cycleProgress[4] = (points / 3);
        cycleProgress[5] = (points / 3) * 2;

        pushFraction = 2.1 / 6.0;
        speedMultiplier = 4;        
        strideLengthMultiplier = 1;
        liftHeightMultiplier = 1.8;
        maxStrideLength = 230;
        maxSpeed = 130;
        break;

      case Quad:
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 3);
        cycleProgress[2] = (points / 3) * 2;
        cycleProgress[3] = 0;
        cycleProgress[4] = (points / 3);
        cycleProgress[5] = (points / 3) * 2;

        pushFraction = 4.1 / 6.0;
        speedMultiplier = 1;        
        strideLengthMultiplier = 1.2;
        liftHeightMultiplier = 1.8;
        maxStrideLength = 220;
        maxSpeed = 200;
        break;

      case Hop:
        cycleProgress[0] = 0;
        cycleProgress[1] = 0;
        cycleProgress[2] = 0;
        cycleProgress[3] = 0;
        cycleProgress[4] = 0;
        cycleProgress[5] = 0;

        pushFraction = 3 / 6.0;
        speedMultiplier = 1;
        strideLengthMultiplier = 1.6;
        liftHeightMultiplier = 2.5;
        maxStrideLength = 240;
        maxSpeed = 200;
        break;
    }      
  }

  for (int i = 0; i < 6; i++) {
    tArray[i] = (float)cycleProgress[i] / points;    
  }  

  forwardAmount = joy1CurrentMagnitude;
  turnAmount = joy2CurrentVector.x;

  moveToPos(0, getGaitPoint(0, pushFraction));
  moveToPos(1, getGaitPoint(1, pushFraction));
  moveToPos(2, getGaitPoint(2, pushFraction));
  moveToPos(3, getGaitPoint(3, pushFraction));
  moveToPos(4, getGaitPoint(4, pushFraction));
  moveToPos(5, getGaitPoint(5, pushFraction));
  
  float progressChangeAmount = (max(abs(forwardAmount), abs(turnAmount)) * speedMultiplier) * globalSpeedMultiplier;
  
  progressChangeAmount = constrain(progressChangeAmount, 0, maxSpeed * globalSpeedMultiplier);

  for (int i = 0; i < 6; i++) {
    cycleProgress[i] += progressChangeAmount;

    if (cycleProgress[i] >= points) {
      cycleProgress[i] = cycleProgress[i] - points;
    }
  } 
}

Vector3 getGaitPoint(int leg, float pushFraction) {  
  float rotateStrideLength = joy2CurrentVector.x * globalRotationMultiplier;
  Vector2 v = joy1CurrentVector * Vector2(1, strideLengthMultiplier);
  v.y = constrain(v.y, -maxStrideLength / 2, maxStrideLength / 2);
  v = v * globalSpeedMultiplier;

  float weightSum = abs(forwardAmount) + abs(turnAmount);
  float t = tArray[leg];

  // Propelling
  if (t < pushFraction) { 
    if (legStates[leg] != Propelling) setCycleStartPoints(leg);
    legStates[leg] = Propelling;

    ControlPoints[0] = cycleStartPoints[leg];
    ControlPoints[1] = Vector3(v.x * strideMultiplier[leg] + distanceFromCenter, -v.y * strideMultiplier[leg], distanceFromGround).rotate(legPlacementAngle * rotationMultiplier[leg], Vector2(distanceFromCenter, 0));
    ControlPointsAmount = 2;    
    Vector3 straightPoint = GetPointOnBezierCurve(ControlPoints, ControlPointsAmount, mapFloat(t, 0, pushFraction, 0, 1));

    RotateControlPoints[0] = cycleStartPoints[leg];
    RotateControlPoints[1] = { distanceFromCenter + 40, 0, distanceFromGround };
    RotateControlPoints[2] = { distanceFromCenter, rotateStrideLength, distanceFromGround };
    RotateControlPointsAmount = 3;    
    Vector3 rotatePoint = GetPointOnBezierCurve(RotateControlPoints, RotateControlPointsAmount, mapFloat(t, 0, pushFraction, 0, 1));

    return (straightPoint * abs(forwardAmount) + rotatePoint * abs(turnAmount)) / weightSum;
  }

  // Lifting
  else {
    if (legStates[leg] != Lifting) setCycleStartPoints(leg);
    legStates[leg] = Lifting;

    ControlPoints[0] = cycleStartPoints[leg];
    ControlPoints[1] = cycleStartPoints[leg] + Vector3(0, 0, liftHeight * liftHeightMultiplier);
    ControlPoints[2] = Vector3(-v.x * strideMultiplier[leg] + distanceFromCenter, (v.y + strideOvershoot) * strideMultiplier[leg], distanceFromGround + landHeight).rotate(legPlacementAngle * rotationMultiplier[leg], Vector2(distanceFromCenter, 0));
    ControlPoints[3] = Vector3(-v.x * strideMultiplier[leg] + distanceFromCenter, v.y * strideMultiplier[leg], distanceFromGround).rotate(legPlacementAngle * rotationMultiplier[leg], Vector2(distanceFromCenter, 0));
    ControlPointsAmount = 4;
    Vector3 straightPoint = GetPointOnBezierCurve(ControlPoints, ControlPointsAmount, mapFloat(t, pushFraction, 1, 0, 1));

    RotateControlPoints[0] = cycleStartPoints[leg];
    RotateControlPoints[1] = cycleStartPoints[leg] + Vector3(0, 0, liftHeight * liftHeightMultiplier);
    RotateControlPoints[2] = { distanceFromCenter + 40, 0, distanceFromGround + liftHeight * liftHeightMultiplier };
    RotateControlPoints[3] = { distanceFromCenter, -(rotateStrideLength + strideOvershoot), distanceFromGround + landHeight };
    RotateControlPoints[4] = { distanceFromCenter, -rotateStrideLength, distanceFromGround };
    RotateControlPointsAmount = 5;
    Vector3 rotatePoint = GetPointOnBezierCurve(RotateControlPoints, RotateControlPointsAmount, mapFloat(t, pushFraction, 1, 0, 1));

    return (straightPoint * abs(forwardAmount) + rotatePoint * abs(turnAmount)) / weightSum;
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


// float forwardAmount;
// float turnAmount;
// float  tArray[6];
// int ControlPointsAmount = 0;
// int RotateControlPointsAmount = 0;
// float pushFraction = 3.0/6.0;
// float speedMultiplier = 0.5;
// float strideLengthMultiplier = 1.5;
// float liftHeightMultiplier = 1.0;
// float maxStrideLength = 200;
// float maxSpeed = 100;
// float legPlacementAngle = 56;

// int leftSlider = 50;
// float globalSpeedMultiplier = 0.55;
// float globalRotationMultiplier = 0.55;

// void carState() {
//   leftSlider = (int)rc_data.slider2;
//   globalSpeedMultiplier = (leftSlider + 10.0)*0.01;
//   globalRotationMultiplier = map(rc_data.slider2,0,100,40,130) * 0.01;

//   if (currentState != Car || previousGait != currentGait) {
//     currentState = Car;

//     //Initialize Leg States
//     for(int i = 0; i < 6; i++){
//       legStates[i] = Reset;
//     }   

//     switch (currentGait) {
//       case Tri:
//         cycleProgress[0] = 0;
//         cycleProgress[1] = (points / 2);
//         cycleProgress[2] = 0;
//         cycleProgress[3] = (points / 2);
//         cycleProgress[4] = 0;
//         cycleProgress[5] = (points / 2);

//         pushFraction = 3.1/6.0;
//         speedMultiplier = 1;
//         strideLengthMultiplier = 1.2;
//         liftHeightMultiplier = 1.1;
//         maxStrideLength = 240;
//         maxSpeed = 200;
//         break;

//       case Wave:
//         //Offsets
//         cycleProgress[0] = 0;
//         cycleProgress[1] = (points / 6);
//         cycleProgress[2] = (points / 6)*2;
//         cycleProgress[3] = (points / 6)*5;
//         cycleProgress[4] = (points / 6)*4;
//         cycleProgress[5] = (points / 6)*3;

//         //Percentage Time On Ground
//         pushFraction = 5.0/6.0; 

//         speedMultiplier = 0.40;
//         strideLengthMultiplier = 2;
//         liftHeightMultiplier = 1.3;
//         maxStrideLength = 150;
//         maxSpeed = 160;
//         break;

//       case Ripple:
//         //Offsets
//         cycleProgress[0] = 0;
//         cycleProgress[1] = (points / 6)*4;
//         cycleProgress[2] = (points / 6)*2;
//         cycleProgress[3] = (points / 6)*5;
//         cycleProgress[4] = (points / 6);
//         cycleProgress[5] = (points / 6)*3;

//         //Percentage Time On Ground
//         pushFraction = 3.2/6.0;


//         speedMultiplier = 1;
//         strideLengthMultiplier = 1.3;
//         liftHeightMultiplier = 1;
//         maxStrideLength = 220;
//         maxSpeed = 200;
//         break;

//       case Bi:
//         //Offsets
//         cycleProgress[0] = 0;
//         cycleProgress[1] = (points / 3);
//         cycleProgress[2] = (points / 3)*2;
//         cycleProgress[3] = 0;
//         cycleProgress[4] = (points / 3);
//         cycleProgress[5] = (points / 3)*2;

//         //Percentage Time On Ground
//         pushFraction = 2.1/6.0;

        
//         speedMultiplier = 4;        
//         strideLengthMultiplier = 1;
//         liftHeightMultiplier = 1.8;
//         maxStrideLength = 230;
//         maxSpeed = 130;
//         break;

//       case Quad:
//         //Offsets
//         cycleProgress[0] = 0;
//         cycleProgress[1] = (points / 3);
//         cycleProgress[2] = (points / 3)*2;
//         cycleProgress[3] = 0;
//         cycleProgress[4] = (points / 3);
//         cycleProgress[5] = (points / 3)*2;

//         //Percentage Time On Ground
//         pushFraction = 4.1/6.0;

        
//         speedMultiplier = 1;        
//         strideLengthMultiplier = 1.2;
//         liftHeightMultiplier = 1.8;
//         maxStrideLength = 220;
//         maxSpeed = 200;
//         break;

//       case Hop:
//         //Offsets
//         cycleProgress[0] = 0;
//         cycleProgress[1] = 0;
//         cycleProgress[2] = 0;
//         cycleProgress[3] = 0;
//         cycleProgress[4] = 0;
//         cycleProgress[5] = 0;

//         //Percentage Time On Ground        
//         pushFraction = 3/6.0;

//         speedMultiplier = 1;
//         strideLengthMultiplier = 1.6;
//         liftHeightMultiplier = 2.5;
//         maxStrideLength = 240;
//         maxSpeed = 200;
//         break;
//     }      
//   }

  
  
//   for(int i = 0; i < 6; i++){
//     tArray[i] = (float)cycleProgress[i] / points;    
//   }  

//   forwardAmount = joy1CurrentMagnitude;
//   turnAmount = joy2CurrentVector.x;

//   moveToPos(0, getGaitPoint(0, pushFraction));
//   moveToPos(1, getGaitPoint(1, pushFraction));
//   moveToPos(2, getGaitPoint(2, pushFraction));
//   moveToPos(3, getGaitPoint(3, pushFraction));
//   moveToPos(4, getGaitPoint(4, pushFraction));
//   moveToPos(5, getGaitPoint(5, pushFraction));
  
  

//   float progressChangeAmount = (max(abs(forwardAmount),abs(turnAmount))* speedMultiplier)*globalSpeedMultiplier ;

  
//   progressChangeAmount = constrain(progressChangeAmount,0,maxSpeed*globalSpeedMultiplier);

//   for(int i = 0; i < 6; i++){
//     cycleProgress[i] += progressChangeAmount;

//     if(cycleProgress[i] >= points){
//       cycleProgress[i] = cycleProgress[i] - points;
//     }
//   } 
// }



// Vector3 getGaitPoint(int leg, float pushFraction){  
 

//   float rotateStrideLength = joy2CurrentVector.x * globalRotationMultiplier;
//   Vector2 v = joy1CurrentVector * Vector2(1,strideLengthMultiplier);
//   v.y = constrain(v.y,-maxStrideLength/2, maxStrideLength/2);
//   v = v * globalSpeedMultiplier;

//   float weightSum = abs(forwardAmount) + abs(turnAmount);

//   float t = tArray[leg];

//   //if(leg == 0)print_value("cycleProgress[leg]",cycleProgress[leg]);
  
  
//   //Propelling
//   if(t < pushFraction){ 
//     if(legStates[leg] != Propelling)setCycleStartPoints(leg);
//     legStates[leg] = Propelling;

//     ControlPoints[0] = cycleStartPoints[leg];
//     ControlPoints[1] = Vector3(v.x * strideMultiplier[leg] + distanceFromCenter, -v.y * strideMultiplier[leg], distanceFromGround).rotate(legPlacementAngle * rotationMultiplier[leg], Vector2(distanceFromCenter,0));
//     ControlPointsAmount = 2;    
//     Vector3 straightPoint = GetPointOnBezierCurve(ControlPoints, ControlPointsAmount, mapFloat(t,0,pushFraction,0,1));

//     RotateControlPoints[0] = cycleStartPoints[leg];
//     RotateControlPoints[1] = { distanceFromCenter + 40, 0, distanceFromGround };
//     RotateControlPoints[2] = { distanceFromCenter, rotateStrideLength, distanceFromGround };
//     RotateControlPointsAmount = 3;    
//     Vector3 rotatePoint = GetPointOnBezierCurve(RotateControlPoints, RotateControlPointsAmount, mapFloat(t,0,pushFraction,0,1));

//     //if(leg == 0)print_value("pushing point",(straightPoint*abs(forwardAmount) + rotatePoint*abs(turnAmount))/ weightSum);

//     return (straightPoint*abs(forwardAmount) + rotatePoint*abs(turnAmount))/ weightSum;
//   }

//   //Lifting
//   else{
//     if(legStates[leg] != Lifting)setCycleStartPoints(leg);
//     legStates[leg] = Lifting;

//     ControlPoints[0] = cycleStartPoints[leg];
//     ControlPoints[1] = cycleStartPoints[leg] + Vector3(0,0,liftHeight * liftHeightMultiplier);
//     ControlPoints[2] = Vector3(-v.x * strideMultiplier[leg] + distanceFromCenter, (v.y + strideOvershoot) * strideMultiplier[leg], distanceFromGround + landHeight).rotate(legPlacementAngle * rotationMultiplier[leg], Vector2(distanceFromCenter,0));
//     ControlPoints[3] = Vector3(-v.x * strideMultiplier[leg] + distanceFromCenter, v.y * strideMultiplier[leg], distanceFromGround).rotate(legPlacementAngle * rotationMultiplier[leg], Vector2(distanceFromCenter,0));
//     ControlPointsAmount = 4;
//     Vector3 straightPoint = GetPointOnBezierCurve(ControlPoints, ControlPointsAmount, mapFloat(t,pushFraction,1,0,1));

//     RotateControlPoints[0] = cycleStartPoints[leg];
//     RotateControlPoints[1] = cycleStartPoints[leg] + Vector3(0,0,liftHeight * liftHeightMultiplier);
//     RotateControlPoints[2] = { distanceFromCenter + 40, 0, distanceFromGround + liftHeight * liftHeightMultiplier};
//     RotateControlPoints[3] = { distanceFromCenter, -(rotateStrideLength + strideOvershoot), distanceFromGround + landHeight};
//     RotateControlPoints[4] = { distanceFromCenter, -rotateStrideLength, distanceFromGround};
//     RotateControlPointsAmount = 5;
//     Vector3 rotatePoint =  GetPointOnBezierCurve(RotateControlPoints, RotateControlPointsAmount, mapFloat(t,pushFraction,1,0,1));

//     //if(leg == 0)print_value("lifting point",(straightPoint*abs(forwardAmount) + rotatePoint*abs(turnAmount))/ weightSum);

//     return (straightPoint*abs(forwardAmount) + rotatePoint*abs(turnAmount))/ weightSum;
//   }  
// }
