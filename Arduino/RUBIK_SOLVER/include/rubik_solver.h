#include "vector"
#include "iostream"
#ifndef RUBIK_SOLVER_H
#define RUBIK_SOLVER_H
class rubik_solver
{
private:
    struct PIN{
        uint8_t DIR_1, STP_1, DIR_2, STP_2, DIR_3, STP_3, 
                DIR_4,STP_4, DIR_5, STP_5, DIR_6, STP_6;
    } rubik_pin;
    uint8_t DIR, STP;
    int delayTime;
    int step_per_rev;
    uint8_t distance;
    // std::vector<int8_t> U{0, 0, +1};
    // std::vector<int8_t> D{0, 0, -1};
    // std::vector<int8_t> L{0, -1, 0};
    // std::vector<int8_t> R{0, +1, 0};
    // std::vector<int8_t> F{+1, 0, 0};
    // std::vector<int8_t> B{-1, 0, 0};
    std::string command;
    std::string command_array[1000]; 


public:
    rubik_solver(uint8_t dir_1, uint8_t stp_1, uint8_t dir_2, uint8_t stp_2,
                uint8_t dir_3, uint8_t stp_3, uint8_t dir_4, uint8_t stp_4, 
                uint8_t dir_5, uint8_t stp_5, uint8_t dir_6, uint8_t stp_6, int delayTime, int step_per_rev, uint8_t distance);
    void solution(std::string command);
    void SOLVE();
    void bw();
    void fw();
    void recgn_Yellow();
    void recgn_Blue();
    void recgn_Red();
    void recgn_Green();
    void recgn_Orange();
    void recgn_White();
    void returnFace();
    void OFF();
    void u(int degrees);
    void d(int degrees);
    void l(int degrees);
    void r(int degrees);
    void f(int degrees);
    void b(int degrees);
    void CorrectOrient(char* stepper);
    void Dir_Step_of_Stepper(char* stepper);
    void Rotate(int degrees);
    void ClockWise(char* stepper, int degrees);
    void Counter_ClockWise(char* stepper, int degrees);
    void Forward(char* axis);
    void Backward(char* axis);
    void Flip(char* axis, int degrees);
    void print_command();
    void read_string();
}; 
#endif