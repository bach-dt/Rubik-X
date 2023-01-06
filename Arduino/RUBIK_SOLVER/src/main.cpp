#include <Arduino.h>
#include "rubik_solver.h"
#include "Wire.h"

String cstr;
std::string str;
rubik_solver RUBIK(13, 12, 27, 14, 25, 26, 32, 33, 18, 5, 23, 19, 1000, 800, 12);
void setup() {
  Serial.begin(9600);
  RUBIK.OFF();
  delay(2000);
}

void loop() {
  str = "";
  cstr = Serial.readString();
  if (cstr == "recgn_Yellow"){
    RUBIK.recgn_Yellow();
    Serial.println("Recognized Yellow ...");
  }
  if (cstr == "recgn_Blue"){
    RUBIK.recgn_Blue();
    Serial.println("Recognized Blue ...");
  }
  if (cstr == "recgn_Red"){
    RUBIK.recgn_Red();
    Serial.println("Recognized Red ...");
  }
  if (cstr == "recgn_Green"){
    RUBIK.recgn_Green();
    Serial.println("Recognized Green ...");
  }
  if (cstr == "recgn_Orange"){
    RUBIK.recgn_Orange();
    Serial.println("Recognized Orange ...");
  }
  if (cstr == "recgn_White"){
    RUBIK.recgn_White();
    Serial.println("Recognized White ...");
  }
  if (cstr == "retn_Face"){
    RUBIK.returnFace();
  }
  if (cstr[0] == 'S'){
    for (int i = 1; i < cstr.length(); i++){
        str += cstr[i];
    }
    RUBIK.solution(str);
    RUBIK.SOLVE();
  }
  
}