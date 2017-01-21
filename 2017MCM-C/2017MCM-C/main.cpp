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
#include "assert.h"

//Time
const double DT = 0.1;
const double END_TIME = 1.0;
//Road
const int NUM_LANE = 4;
const int ROAD_LENGTH = 1000;
const int BLOCK_LENGTH = 5;
const int NUM_BLOCKS_PER_LANE = ROAD_LENGTH / BLOCK_LENGTH;
const double LANE_V_LIMIT[5] = {50.0, 40.0, 40.0, 40.0, 40.0};
//Car setting
const int OBSERVABLE_DIST_FORWARD = 100;
const int OBSERVABLE_BLOCKS_FORWARD = OBSERVABLE_DIST_FORWARD / BLOCK_LENGTH + 1;
    //Caution!!! OBSERVABLE_BLOCKS_FORWARD may exceeds the actual observable range!!!
const int OBSERVABLE_DIST_BACKWARD = 50;
const int OBSERVABLE_BLOCKS_BacWARD = OBSERVABLE_DIST_BACKWARD / BLOCK_LENGTH + 1;
const int REACTION_TIME = 15; // 0.1s
const int NO_CAR_DIST = 20;
const int NO_CAR_BLOCK = NO_CAR_DIST / BLOCK_LENGTH;
const int TRIGGER_DIST = 100;
const int TRIGGER_BLOCK = TRIGGER_DIST / BLOCK_LENGTH;
const double COEF[4] = {0.0073, 0.0010, -0.0024, -0.0004};
//

class Car {
private:
    char type;                         //self-driving 's' or human-driving 'h'
    double a;                          //acceleration
    double preA[REACTION_TIME];        //previous acceleration
    double v;                          //velocity
    double preV[REACTION_TIME];        //previous velocity
    double s;                          //position
    double preS[REACTION_TIME];        //previous position
    int lane;
    int preLane[REACTION_TIME];        //previous lane
    int blockPos;
    int preBlockPos[REACTION_TIME];    //previous block
public:

    Car(char type, double a, double v, double s, int lane, int blockPos) {
        this->type = type;
        this->a = a;
        this->preA = 0;
        this->v = v;
        this->preV = 0;
        this->s = s;
        this->preS = 0;
        this->lane = lane;
        this->preLane = lane;
        this->blockPos = blockPos;
        this->preBlockPos = 0;
    }
    char getType() {
        return type;
    }
    double getA() {
        return a;
    }
    double getPreA(int preTime) {
        return preA[preTime];
    }
    double getV() {
        return v;
    }
    double getPreV(int preTime) {
        return preV[preTime];
    }
    double getS() {
        return s;
    }
    double getPreS(int preTime) {
        return preS[preTime];
    }
    int getLane() {
        return lane;
    }
    int getPreLane(int preTime) {
        return preLane[preTime];
    }
    int getBlockPos() {
        return blockPos;
    }
    int getPreBlockPos(int preTime) {
        return preBlockPos[preTime];
    }
    void setA(double a) {
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preA[preTime] = this->preA[--preTime]; //NOTE!!!
        }
        this->preA[0] = this->a;
        this->a = a;
    }
    void setV(double v) {
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preV[preTime] = this->preV[--preTime]; //NOTE!!!
        }
        this->preV[0] = this->v;
        this->v = v;
    }
    void setS(double s) {
        //after running this method, the pointers road[][] need to be updated
        //TODO
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preS[preTime] = this->preS[--preTime]; //NOTE!!!
        }
        this->preS[0] = this->s;
        this->s = s;
    }
    void setLane(int lane) {
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preLane[preTime] = this->preLane[--preTime]; //NOTE!!!
        }
        this->preLane[0] = this->lane;
        this->lane = lane;
    }
    void setBlockPos(int blockPos) {
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preBlockPos[preTime] = this->preBlockPos[--preTime]; //NOTE!!!
        }
        this->preBlockPos[0] = this->blockPos;
        this->blockPos = blockPos;
    }
    void updVSBlockPos() {//TODO change pre attributes
        //this method should be called only after new acceleration is set
        setV(std::max(std::min(v + a * DT, LANE_V_LIMIT[lane]), 0.0));
        setS(s + ((v + preV[0]) / 2.0) * DT)
        setBlockPos(std::min(((int)s)/BLOCK_LENGTH, NUM_BLOCKS_PER_LANE - 1))
        // printf("a:%f  v:%f  s:%f\n", a, v, s);
    }
    Car *frontCar(Car *road[NUM_LANE][NUM_BLOCKS_PER_LANE]) {
        if (type == 's') {   //self-driving car
            for (int i = 1; blockPos + i < NUM_BLOCKS_PER_LANE; i++) {
                if ((road[lane][blockPos+i] != NULL) &&
                    (road[lane][blockPos+i]->getPreBlockPos(0) == blockPos+i)) {
                    return road[lane][blockPos+i];
                }
            }
            return NULL;
        } else {    //human-driving car
            for (int i = 1; (i < OBSERVABLE_BLOCKS_FORWARD)
                    && (blockPos + i < NUM_BLOCKS_PER_LANE); i++) {
                if ((road[lane][blockPos+i] != NULL) &&
                    (road[lane][blockPos+i]->getPreBlockPos(0) == blockPos+i) &&
                    (road[lane][blockPos+i]->getS()-s
                        <= OBSERVABLE_DIST_FORWARD)) {
                    return road[lane][blockPos+i];
                }
            }
            return NULL;
        }
    }
    Car *backCar(Car *road[NUM_LANE][NUM_BLOCKS_PER_LANE]) {
        if (type == 's') {
            for (int i = 1; blockPos - i >= 0; i++) {
                if ((road[lane][blockPos-i] != NULL) &&
                    (road[lane][blockPos-i]->getBlockPos() == blockPos-i)) {
                    return road[lane][blockPos-i];
                }
            return NULL;
        }
        for (int i = 1; (i < OBSERVABLE_BLOCKS_BACKWARD
                         ) && blockPos - i >= 0; i++) {
            if ((road[lane][blockPos-i] != NULL) &&
                (road[lane][blockPos-i]->getBlockPos() == blockPos-i) &&
                (s-road[lane][blockPos-i]->getS()
                    <= OBSERVABLE_DIST_BACKWARD)) {
                return road[lane][blockPos-i];
            }
        }·
        return NULL;
    }
};
void move(Car *road[][NUM_BLOCKS_PER_LANE], int lane, int blockPos);
void moveSelfCar(Car *road[][NUM_BLOCKS_PER_LANE], int lane, int blockPos);
void moveHumanCar(Car *road[][NUM_BLOCKS_PER_LANE], int lane, int blockPos);
void runDT(Car *road[][NUM_BLOCKS_PER_LANE]);
void printRoad(Car *road[][NUM_BLOCKS_PER_LANE], char parameter);

int main() {

    Car *road[NUM_LANE][NUM_BLOCKS_PER_LANE];
    for (int lane = 0; lane < NUM_LANE; lane++) {
        for (int blockPos = 0; blockPos < NUM_BLOCKS_PER_LANE; blockPos++) {
            road[lane][blockPos] = NULL;
        }
    }
//    for (int lane = 0; lane < NUM_LANE; lane++) {
//        for (int blockPos = 0; blockPos < NUM_BLOCKS_PER_LANE; blockPos++) {
//            assert(road[lane][blockPos] == NULL);
//            printf("lane: %4d blockPos: %4d is NULL", lane, blockPos);
//        }
//    }

//    printRoad(road, 'v');

        for (double t = 0.0; t < END_TIME; t += DT) {
            runDT(road);
        }
    printRoad(road, 's');
    return 0;
}


void move(Car *road[][NUM_BLOCKS_PER_LANE], int lane, int blockPos) {
    if (road[lane][blockPos]->getType == 's') {
        moveSelfCar(road, lane, blockPos);
    } else {
        moveHumanCar(road, lane, blockPos);
    }
}

    double x = 0;
    if (frontCar != NULL) {
        x += COEF[0] * (frontCar->getV() - thisCar->getV());
        x += COEF[1] * (frontCar->getS() - thisCar->getS());
    }
    if (backCar != NULL) {
        x += COEF[2] * (thisCar->getV() - backCar->getV());
        x += COEF[3] * (thisCar->getS() - backCar->getS());
    }

void moveSelfCar(Car *road[][NUM_BLOCKS_PER_LANE], int lane, int blockPos) {
    Car *thisCar = road[lane][blockPos];
    Car *frontCar = thisCar->frontCar(road);
    Car *backCar = thisCar->backCar(road);
}
void moveHumanCar(Car *road[][NUM_BLOCKS_PER_LANE], int lane, int blockPos) {
    Car *thisCar = road[lane][blockPos];
    Car *frontCar = thisCar->frontCar(road);
    Car *backCar = thisCar->backCar(road);

    double x = 0;
    if (frontCar != NULL) {
        x += COEF[0] * (frontCar->getPreV() - thisCar->getV());
        x += COEF[1] * (frontCar->getPreS() - thisCar->getS());
    }
    if (backCar != NULL) {
        x += COEF[2] * (thisCar->getV() - backCar->getV());
        x += COEF[3] * (thisCar->getS() - backCar->getS());
    }
    double a = 6.0 / (1+exp(-4.0*pow(x, 7.0))) - 3.0;       //new acceleration

    thisCar->setA(a);
    thisCar->updVSBlockPos();//TODO when to delete car

}

void runDT(Car *road[][NUM_BLOCKS_PER_LANE]) {
    for (int blockPos = NUM_BLOCKS_PER_LANE - 1; blockPos >= 0; blockPos--) {
        for (int lane = 0; lane < NUM_LANE; lane++) {
            if (road[lane][blockPos] != NULL) {
                move(road, lane, blockPos);
            }
        }
    }
    for (int lane = 0; lane < NUM_LANE; lane++) {
        int nearestCarBlock = TRIGGER_BLOCK;
        for (int blockPos = TRIGGER_BLOCK - 1; blockPos >= 0; blockPos--) {
            if (road[lane][blockPos] != NULL) {
                nearestCarBlock = blockPos;
            }
        }
        double probStartACar = (double(nearestCarBlock - NO_CAR_BLOCK))/(TRIGGER_DIST - NO_CAR_BLOCK);
        probStartACar = 0.5;
        if (rand() < RAND_MAX * probStartACar) {
                Car *newCar = new Car('h', 0, 25.0 + (rand() % 10), 0.1, lane, 0);
                road[lane][0] = newCar;
            }
    }
}

void printRoad(Car *road[][NUM_BLOCKS_PER_LANE], char parameter) {
    switch (parameter) {
        case 't': {
            for (int blockPos = 0; blockPos < NUM_BLOCKS_PER_LANE; blockPos++) {
                printf("%5d ", blockPos);
                for (int lane = 0; lane < NUM_LANE; lane++) {
                    if (road[lane][blockPos] == NULL) {
                        printf("  ");
                    } else {
                        printf("%c ", road[lane][blockPos]->getType());
                    }
                }
                printf("\n");
            }
        }
            break;
        case 'a': {
            for (int blockPos = 0; blockPos < NUM_BLOCKS_PER_LANE; blockPos++) {
                printf("%5d ", blockPos);
                for (int lane = 0; lane < NUM_LANE; lane++) {
                    if (road[lane][blockPos] == NULL) {
                        printf("     ");
                    } else {
                        printf("%5.1f ", road[lane][blockPos]->getA());
                    }
                }
                printf("\n");
            }
        }
            break;
        case 'v': {
            for (int blockPos = 0; blockPos < NUM_BLOCKS_PER_LANE; blockPos++) {
                printf("%5d ", blockPos);
                for (int lane = 0; lane < NUM_LANE; lane++) {
                    if (road[lane][blockPos] == NULL) {
                        printf("     ");
                    } else {
                        printf("%5.1f ", road[lane][blockPos]->getV());
                    }
                }
                printf("\n");
            }
        }
            break;
        case 's': {
            for (int blockPos = 0; blockPos < NUM_BLOCKS_PER_LANE; blockPos++) {
                printf("%5d ", blockPos);
                for (int lane = 0; lane < NUM_LANE; lane++) {
                    if (road[lane][blockPos] == NULL) {
                        printf("     ");
                    } else {
                        printf("%5.1f ", road[lane][blockPos]->getS());
                    }
                }
                printf("\n");
            }
        }
            break;

    }
    for (int lane = 0; lane < NUM_LANE; lane++) {
        for (int blockPos = 0; blockPos < NUM_BLOCKS_PER_LANE; blockPos++) {

        }
    }
}
