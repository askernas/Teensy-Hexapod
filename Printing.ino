#include "Initializations.h"

void print_value(String name, float value, bool newLine) {
  Serial.print(name + ": ");
  if (newLine) {
    Serial.println(value);
  } else {
    Serial.print(value);
  }
}

void print_value(String name, String value, bool newLine) {
  Serial.print(name + ": ");
  if (newLine) {
    Serial.println(value);
  } else {
    Serial.print(value);
  }
}

void print_value(String name, Vector3 value, bool newLine) {
  Serial.print(name + ": ");
  if (newLine) {
    Serial.println(value.toString());
  } else {
    Serial.print(value.toString());
  }
}

void print_value(String name, int value, bool newLine) {
  Serial.print(name + ": ");
  if (newLine) {
    Serial.println(value);
  } else {
    Serial.print(value);
  }
}

void print_value(String name, bool value, bool newLine) {
  Serial.print(name + ": ");
  if (newLine) {
    Serial.println(value ? "true" : "false");
  } else {
    Serial.print(value ? "true" : "false");
  }
}

// void print_value(String name, float value, bool newLine){
//   Serial.print(name + ": ");

//   if(newLine)Serial.println(value);
//   else Serial.print(value);
  
// }

// void print_value(String name, String value, bool newLine){
//   Serial.print(name + ": ");
//   if(newLine)Serial.println(value);
//   else Serial.print(value);
// }

// void print_value(String name, Vector3 value, bool newLine){
//   Serial.print(name + ": ");
//   if(newLine)Serial.println(value.toString());
//   else Serial.print(value.toString());
}