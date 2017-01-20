//
//  main.cpp
//  2017MCM-C
//
//  Created by Shi Yue on 20/1/2017.
//  Copyright © 2017 Shi Yue. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <cmath>

const double DT = 0.1;
const int NUM_LANE = 4;
const int ROAD_LENGTH = 1000;
const int NUM_BLOCKS_PER_LANE = ROAD_LENGTH / NUM_LANE;
const double LANE_V_LIMIT[5] = {70.0, 60.0, 60.0, 60.0, 60.0};

class Car {
private:
    char type;      //self-driving or human-driving
    double a;       //acceleration
    double v;       //velocity
    double s;       //position
    int lane;
public:

    Car(char type, double a, double v, double s, int lane) {
        this->type = type;
        this->a = a;
        this->v = v;
        this->a = a;
        this->lane = lane;
    }
    char getType() {
        return type;
    }
    double getA() {
        return a;
    }
    double getV() {
        return v;
    }
    double getS() {
        return s;
    }
    int getLane() {
        return lane;
    }
    void setA(double a) {
        this->a = a;
    }
    void setV(double v) {
        this->v = v;
    }
    void setS(double s) {
        this->s = s;
    }
    void setLane(int lane) {
        this->lane = lane;
    }
    void updS() {
        //this method should be called only after new acceleration is set
        double vv = v;
        v =  std::max(std::min(v + a * DT, LANE_V_LIMIT[lane]), 0.0);
        s += (vv + v) / 2;
    }
};

int main() {
    Car *road[NUM_LANE][NUM_BLOCKS_PER_LANE];

    return 0;
}
