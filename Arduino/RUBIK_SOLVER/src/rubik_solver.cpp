#include "Arduino.h"
#include "rubik_solver.h"

rubik_solver::rubik_solver(uint8_t dir_1, uint8_t stp_1, uint8_t dir_2, uint8_t stp_2,
                           uint8_t dir_3, uint8_t stp_3, uint8_t dir_4, uint8_t stp_4, 
                           uint8_t dir_5, uint8_t stp_5, uint8_t dir_6, uint8_t stp_6, int delayTime, int step_per_rev, uint8_t distance){
        rubik_solver::rubik_pin.DIR_1 = dir_1;
        rubik_solver::rubik_pin.STP_1 = stp_1;
        rubik_solver::rubik_pin.DIR_2 = dir_2;
        rubik_solver::rubik_pin.STP_2 = stp_2;
        rubik_solver::rubik_pin.DIR_3 = dir_3;
        rubik_solver::rubik_pin.STP_3 = stp_3;
        rubik_solver::rubik_pin.DIR_4 = dir_4;
        rubik_solver::rubik_pin.STP_4 = stp_4;
        rubik_solver::rubik_pin.DIR_5 = dir_5;
        rubik_solver::rubik_pin.STP_5 = stp_5;
        rubik_solver::rubik_pin.DIR_6 = dir_6;
        rubik_solver::rubik_pin.STP_6 = stp_6;
        rubik_solver::delayTime = delayTime;
        rubik_solver::step_per_rev = step_per_rev;
        rubik_solver::distance = distance;

        Serial.begin(9600);
        pinMode(rubik_solver::rubik_pin.DIR_1, OUTPUT);
        pinMode(rubik_solver::rubik_pin.STP_1, OUTPUT);
        pinMode(rubik_solver::rubik_pin.DIR_2, OUTPUT);
        pinMode(rubik_solver::rubik_pin.STP_2, OUTPUT);
        pinMode(rubik_solver::rubik_pin.DIR_3, OUTPUT);
        pinMode(rubik_solver::rubik_pin.STP_3, OUTPUT);
        pinMode(rubik_solver::rubik_pin.DIR_4, OUTPUT);
        pinMode(rubik_solver::rubik_pin.STP_4, OUTPUT);
        pinMode(rubik_solver::rubik_pin.DIR_5, OUTPUT);
        pinMode(rubik_solver::rubik_pin.STP_5, OUTPUT);
        pinMode(rubik_solver::rubik_pin.DIR_6, OUTPUT);
        pinMode(rubik_solver::rubik_pin.STP_6, OUTPUT);

        Serial.println("Hello Noer, have a nice day!");
}

void rubik_solver::OFF(){
        digitalWrite(rubik_solver::rubik_pin.DIR_1, 0);
        digitalWrite(rubik_solver::rubik_pin.STP_1, 0);
        digitalWrite(rubik_solver::rubik_pin.DIR_2, 0);
        digitalWrite(rubik_solver::rubik_pin.STP_2, 0);
        digitalWrite(rubik_solver::rubik_pin.DIR_3, 0);
        digitalWrite(rubik_solver::rubik_pin.STP_3, 0);
        digitalWrite(rubik_solver::rubik_pin.DIR_4, 0);
        digitalWrite(rubik_solver::rubik_pin.STP_4, 0);
        digitalWrite(rubik_solver::rubik_pin.DIR_5, 0);
        digitalWrite(rubik_solver::rubik_pin.STP_5, 0);
        digitalWrite(rubik_solver::rubik_pin.DIR_6, 0);
        digitalWrite(rubik_solver::rubik_pin.STP_6, 0);
}

void rubik_solver::Dir_Step_of_Stepper(char* stepper){
        if (strcmp(stepper, "XL") == 0){
                rubik_solver::DIR = rubik_solver::rubik_pin.DIR_1;
                rubik_solver::STP = rubik_solver::rubik_pin.STP_1;
        }else if (strcmp(stepper, "XR") == 0){
                rubik_solver::DIR = rubik_solver::rubik_pin.DIR_2;
                rubik_solver::STP = rubik_solver::rubik_pin.STP_2;
        }else if (strcmp(stepper, "YL") == 0){
                rubik_solver::DIR = rubik_solver::rubik_pin.DIR_3;
                rubik_solver::STP = rubik_solver::rubik_pin.STP_3;
        }else if (strcmp(stepper, "YR") == 0){
                rubik_solver::DIR = rubik_solver::rubik_pin.DIR_4;
                rubik_solver::STP = rubik_solver::rubik_pin.STP_4;
        }
};

void rubik_solver::Rotate(int degrees){
        int step = abs(int(float(degrees) / float(360) * float(rubik_solver::step_per_rev)));
        if (degrees > 0){
                digitalWrite(rubik_solver::DIR, HIGH);
                for(int i = 0; i < step; i++){
                        digitalWrite(rubik_solver::STP, HIGH);
                        delayMicroseconds(rubik_solver::delayTime);
                        digitalWrite(rubik_solver::STP, LOW);
                        delayMicroseconds(rubik_solver::delayTime);
                }
        } else {
                digitalWrite(rubik_solver::DIR, LOW);
                for(int i = 0; i < step; i++){
                        digitalWrite(rubik_solver::STP, HIGH);
                        delayMicroseconds(rubik_solver::delayTime);
                        digitalWrite(rubik_solver::STP, LOW);
                        delayMicroseconds(rubik_solver::delayTime);
                } 
        }
        delay(1000); 
}

void rubik_solver::ClockWise(char* stepper, int degrees){
        rubik_solver::Dir_Step_of_Stepper(stepper);
        rubik_solver::Rotate(degrees);
};

void rubik_solver::Counter_ClockWise(char* stepper, int degrees){
        rubik_solver::Dir_Step_of_Stepper(stepper);
        rubik_solver::Rotate(degrees);
};

void rubik_solver::Forward(char* axis){
        if (strcmp(axis, "X") == 0){
                rubik_solver::DIR = rubik_solver::rubik_pin.DIR_5;
                rubik_solver::STP = rubik_solver::rubik_pin.STP_5;
        } else if (strcmp(axis, "Y") == 0){
                rubik_solver::DIR = rubik_solver::rubik_pin.DIR_6;
                rubik_solver::STP = rubik_solver::rubik_pin.STP_6;
        }
        int degrees = int(float(rubik_solver::distance) / float(16*2) * float(360));
        rubik_solver::Rotate(degrees);
};

void rubik_solver::Backward(char* axis){
        if (strcmp(axis, "X") == 0){
                rubik_solver::DIR = rubik_solver::rubik_pin.DIR_5;
                rubik_solver::STP = rubik_solver::rubik_pin.STP_5;
        } else if (strcmp(axis, "Y") == 0){
                rubik_solver::DIR = rubik_solver::rubik_pin.DIR_6;
                rubik_solver::STP = rubik_solver::rubik_pin.STP_6;
        }
        int degrees = int(float(rubik_solver::distance) / float(16*2) * float(360));
        rubik_solver::Rotate(-degrees);
};

void rubik_solver::u(int degrees){
        rubik_solver::Backward("Y");
        rubik_solver::Flip("X", 90);
        rubik_solver::Forward("Y");
        rubik_solver::Backward("X");
        rubik_solver::Flip("X", 90);
        rubik_solver::Forward("X");
        rubik_solver::ClockWise("YL", degrees);
        rubik_solver::Backward("Y");
        if (degrees % 180 != 0){
                rubik_solver::CorrectOrient("YL");
        }
        rubik_solver::Flip("X", -90);
        rubik_solver::Forward("Y");
        rubik_solver::Backward("X");
        rubik_solver::Flip("X", 90);
        rubik_solver::Forward("X");
}

void rubik_solver::d(int degrees){
        rubik_solver::Backward("Y");
        rubik_solver::Flip("X", 90);
        rubik_solver::Forward("Y");
        rubik_solver::Backward("X");
        rubik_solver::Flip("X", 90);
        rubik_solver::Forward("X");
        rubik_solver::ClockWise("YR", degrees);
        rubik_solver::Backward("Y");
        if (degrees % 180 != 0){
                rubik_solver::CorrectOrient("YR");
        }
        rubik_solver::Flip("X", -90);
        rubik_solver::Forward("Y");
        rubik_solver::Backward("X");
        rubik_solver::Flip("X", 90);
        rubik_solver::Forward("X");
}

void rubik_solver::l(int degrees){
        rubik_solver::ClockWise("YR", degrees);
        if (degrees % 180 != 0){
                rubik_solver::Backward("Y");
                rubik_solver::CorrectOrient("YR");
                rubik_solver::Forward("Y");
        }
}

void rubik_solver::r(int degrees){
        rubik_solver::ClockWise("YL", degrees);
        if (degrees % 180 != 0){
                rubik_solver::Backward("Y");
                rubik_solver::CorrectOrient("YL");
                rubik_solver::Forward("Y");
        }
}

void rubik_solver::f(int degrees){
        rubik_solver::ClockWise("XL", degrees);
        if (degrees % 180 != 0){
                rubik_solver::Backward("X");
                rubik_solver::CorrectOrient("XL");
                rubik_solver::Forward("X");
        }
}

void rubik_solver::b(int degrees){
        rubik_solver::ClockWise("XR", degrees);
        if (degrees % 180 != 0){
                rubik_solver::Backward("X");
                rubik_solver::CorrectOrient("XR");
                rubik_solver::Forward("X");
        }
}

void rubik_solver::Flip(char* axis, int degrees){
        int step = abs(int(float(degrees) / float(360) * float(rubik_solver::step_per_rev)));
        if (strcmp(axis, "X") == 0){
                if (degrees > 0){
                        digitalWrite(rubik_solver::rubik_pin.DIR_1, LOW);
                        digitalWrite(rubik_solver::rubik_pin.DIR_2, HIGH);
                        for(int i = 0; i < step; i++){
                                digitalWrite(rubik_solver::rubik_pin.STP_1, HIGH);
                                digitalWrite(rubik_solver::rubik_pin.STP_2, HIGH);
                                delayMicroseconds(rubik_solver::delayTime);
                                digitalWrite(rubik_solver::rubik_pin.STP_1, LOW);
                                digitalWrite(rubik_solver::rubik_pin.STP_2, LOW);
                                delayMicroseconds(rubik_solver::delayTime);
                        }
                } else{
                        digitalWrite(rubik_solver::rubik_pin.DIR_1, HIGH);
                        digitalWrite(rubik_solver::rubik_pin.DIR_2, LOW);
                        for(int i = 0; i < step; i++){
                                digitalWrite(rubik_solver::rubik_pin.STP_1, HIGH);
                                digitalWrite(rubik_solver::rubik_pin.STP_2, HIGH);
                                delayMicroseconds(rubik_solver::delayTime);
                                digitalWrite(rubik_solver::rubik_pin.STP_1, LOW);
                                digitalWrite(rubik_solver::rubik_pin.STP_2, LOW);
                                delayMicroseconds(rubik_solver::delayTime);
                        }
                }
        } else if (strcmp(axis, "Y") == 0){
                if (degrees > 0){
                        digitalWrite(rubik_solver::rubik_pin.DIR_3, LOW);
                        digitalWrite(rubik_solver::rubik_pin.DIR_4, HIGH);
                        for(int i = 0; i < step; i++){
                                digitalWrite(rubik_solver::rubik_pin.STP_3, HIGH);
                                digitalWrite(rubik_solver::rubik_pin.STP_4, HIGH);
                                delayMicroseconds(rubik_solver::delayTime);
                                digitalWrite(rubik_solver::rubik_pin.STP_3, LOW);
                                digitalWrite(rubik_solver::rubik_pin.STP_4, LOW);
                                delayMicroseconds(rubik_solver::delayTime);
                        }   
                } else {
                        digitalWrite(rubik_solver::rubik_pin.DIR_3, LOW);
                        digitalWrite(rubik_solver::rubik_pin.DIR_4, HIGH);
                        for(int i = 0; i < step; i++){
                                digitalWrite(rubik_solver::rubik_pin.STP_3, HIGH);
                                digitalWrite(rubik_solver::rubik_pin.STP_4, HIGH);
                                delayMicroseconds(rubik_solver::delayTime);
                                digitalWrite(rubik_solver::rubik_pin.STP_3, LOW);
                                digitalWrite(rubik_solver::rubik_pin.STP_4, LOW);
                                delayMicroseconds(rubik_solver::delayTime);
                        }   
                }
        }
}

void rubik_solver::CorrectOrient(char* stepper){
        rubik_solver::Dir_Step_of_Stepper(stepper);
        rubik_solver::Rotate(90);
}

void rubik_solver::fw(){
        digitalWrite(rubik_solver::rubik_pin.DIR_5, HIGH);
        digitalWrite(rubik_solver::rubik_pin.DIR_6, HIGH);
        int degrees = int(float(rubik_solver::distance) / float(16*2) * float(360));
        int step = abs(int(float(degrees) / float(360) * float(rubik_solver::step_per_rev)));
        for(int i = 0; i < step; i++){
                digitalWrite(rubik_solver::rubik_pin.STP_5, HIGH);
                digitalWrite(rubik_solver::rubik_pin.STP_6, HIGH);
                delayMicroseconds(rubik_solver::delayTime);
                digitalWrite(rubik_solver::rubik_pin.STP_5, LOW);
                digitalWrite(rubik_solver::rubik_pin.STP_6, LOW);
                delayMicroseconds(rubik_solver::delayTime);
        }
}

void rubik_solver::bw(){
        digitalWrite(rubik_solver::rubik_pin.DIR_5, LOW);
        digitalWrite(rubik_solver::rubik_pin.DIR_6, LOW);
        int degrees = int(float(rubik_solver::distance) / float(16*2) * float(360));
        int step = abs(int(float(degrees) / float(360) * float(rubik_solver::step_per_rev)));
        for(int i = 0; i < step; i++){
                digitalWrite(rubik_solver::rubik_pin.STP_5, HIGH);
                digitalWrite(rubik_solver::rubik_pin.STP_6, HIGH);
                delayMicroseconds(rubik_solver::delayTime);
                digitalWrite(rubik_solver::rubik_pin.STP_5, LOW);
                digitalWrite(rubik_solver::rubik_pin.STP_6, LOW);
                delayMicroseconds(rubik_solver::delayTime);
        }
}


