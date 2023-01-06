#include "Arduino.h"
#include "rubik_solver.h"

void rubik_solver::solution(std::string command){
    rubik_solver::command = command;
}
void rubik_solver::print_command(){
    int len_string = rubik_solver::command.size();
    rubik_solver::read_string();
    for (int i = 0; i < 1000; i++){
        if (rubik_solver::command_array[i].compare("") > 0){
            Serial.printf("%s", rubik_solver::command_array[i].c_str());
            Serial.print(" ");
        } else {
            //
        }
    }
}
void rubik_solver::read_string(){
    int len_string = rubik_solver::command.size();
    int i = 0;
    int j = 0;
    while (i < len_string){
        if (int(rubik_solver::command[i + 1]) > 64 &&
            int(rubik_solver::command[i + 1]) < 91){
            rubik_solver::command_array[j] = rubik_solver::command[i];
            i += 1;
            j += 1;
        } else{
            std::string pre(1, rubik_solver::command[i]);
            std::string suf(1, rubik_solver::command[i + 1]);
            rubik_solver::command_array[j] = pre + suf;
            i += 2;
            j += 1;
        }
    }
}
void rubik_solver::recgn_Yellow(){
    rubik_solver::Backward("Y");
    rubik_solver::Flip("Y", 90);
    rubik_solver::Forward("Y");
    rubik_solver::Backward("X");

    Serial.println("Recognize Yellow face ...");
}
void rubik_solver::recgn_Blue(){
    rubik_solver::Flip("Y", 90);
    rubik_solver::Forward("X");
    rubik_solver::Backward("Y");
    rubik_solver::Flip("X", 90);

    Serial.println("Recognize Blue face ...");
}
void rubik_solver::recgn_Red(){
    rubik_solver::Forward("Y");
    rubik_solver::Backward("X");
    rubik_solver::Flip("X", 90);
    rubik_solver::Forward("X");
    rubik_solver::Backward("Y");
    rubik_solver::Flip("X", -90);

    Serial.println("Recognize Red face ...");
}
void rubik_solver::recgn_Green(){
    rubik_solver::Forward("Y");
    rubik_solver::Backward("X");
    rubik_solver::Flip("X", 90);
    rubik_solver::Forward("X");
    rubik_solver::Backward("Y");
    rubik_solver::Flip("X", -90);

    Serial.println("Recognize Green face ...");
}
void rubik_solver::recgn_Orange(){
    rubik_solver::Forward("Y");
    rubik_solver::Backward("X");
    rubik_solver::Flip("X", 90);
    rubik_solver::Forward("X");
    rubik_solver::Backward("Y");
    rubik_solver::Flip("X", -90);

    Serial.println("Recognize Orange face ...");
}
void rubik_solver::recgn_White(){
    rubik_solver::Flip("X", 180);
    rubik_solver::Forward("Y");
    rubik_solver::Backward("X");
    rubik_solver::Flip("X", 90);
    rubik_solver::Flip("Y", 90);

    Serial.println("Recognize White face ...");
}
void rubik_solver::returnFace(){
    rubik_solver::Flip("Y", 180);
    rubik_solver::Forward("X");
    rubik_solver::Backward("Y");
    rubik_solver::Flip("Y", 90);
    rubik_solver::Forward("Y");
}
void rubik_solver::SOLVE(){
    int len_string = rubik_solver::command.size();
    rubik_solver::read_string();
    for (int i = 0; i < 1000; i++){
        if (rubik_solver::command_array[i].compare("") > 0){
            Serial.printf("running: %s\t...\n", rubik_solver::command_array[i].c_str());
            int sym = rubik_solver::command_array[i][0];
            int value = rubik_solver::command_array[i][1];
            switch (sym){
            case 85:
                if (value == 50){rubik_solver::u(-180);}
                else if (value == 39){rubik_solver::u(90);}
                else {rubik_solver::u(-90);}
                break;
            case 68:
                if (value == 50){rubik_solver::d(-180);}
                else if (value == 39){rubik_solver::d(90);}
                else {rubik_solver::d(-90);}
                break;
            case 76:
                if (value == 50){rubik_solver::l(-180);}
                else if (value == 39){rubik_solver::l(90);}
                else {rubik_solver::l(-90);}
                break;
            case 82:
                if (value == 50){rubik_solver::r(-180);}
                else if (value == 39){rubik_solver::r(90);}
                else {rubik_solver::r(-90);}
                break;
            case 70:
                if (value == 50){rubik_solver::f(-180);}
                else if (value == 39){rubik_solver::f(90);}
                else {rubik_solver::f(-90);}
                break;
            case 66:
                if (value == 50){rubik_solver::b(-180);}
                else if (value == 39){rubik_solver::b(90);}
                else {rubik_solver::b(-90);}
                break;
            }
        } else {
            //
        }
    }
    rubik_solver::bw();
}