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
const int OBSERVABLE_DIST_HUMAN_DRI = 100;
const int OBSERVABLE_BLOCKS_HUMAN_DRI = OBSERVABLE_DIST_HUMAN_DRI / BLOCK_LENGTH + 1;
    //Caution!!! OBSERVABLE_BLOCKS_HUMAN_DRI may exceeds the actual observable range!!!
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
        this->a = a;
    }
    void setPreA(double preA) {
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preA[preTime] = this->preA[--preTime]; //NOTE!!!
        }
        this->preA[0] = preA;
    }
    void setV(double v) {
        this->v = v;
    }
    void setPreV(double preV) {
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preV[preTime] = this->preV[--preTime]; //NOTE!!!
        }
        this->preV[0] = preV;
    }
    void setS(double s) {
        //after running this method, the pointers road[][] need to be updated
        //TODO
        this->s = s;
    }
    void setPreS(double preS) {
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preS[preTime] = this->preS[--preTime]; //NOTE!!!
        }
        this->preS[0] = preS;
    }
    void setLane(int lane) {
        this->lane = lane;
    }
    void setPreLane(int preLane) {
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preLane[preTime] = this->preLane[--preTime]; //NOTE!!!
        }
        this->preLane[0] = preLane;
    }
    void setBlockPos(int blockPos) {
        this->blockPos = blockPos;
    }
    void setPreBlockPos(int preBlockPos) {
        for (int preTime = REACTION_TIME - 1; preTime >= 1 ;) {
            this->preBlockPos[preTime] = this->preBlockPos[--preTime]; //NOTE!!!
        }
        this->preBlockPos[0] = preBlockPos;
    }
    void updS() {//TODO change pre attributes
        //this method should be called only after new acceleration is set
        double vv = v;
        v =  std::max(std::min(v + a * DT, LANE_V_LIMIT[lane]), 0.0);
//        printf("v = %f  vv = %f\n", v, vv);
        s += ((vv + v) / 2.0) * DT;
        printf("a:%f  v:%f  s:%f\n", a, v, s);
    }
    Car *frontCar(Car *road[NUM_LANE][NUM_BLOCKS_PER_LANE]) {
        for (int i = 1; (i < OBSERVABLE_BLOCKS_HUMAN_DRI)
                && (blockPos + i < NUM_BLOCKS_PER_LANE); i++) {
            if ((road[lane][blockPos+i] != NULL) &&
                (road[lane][blockPos+i]->getPreBlockPos() == blockPos+i) &&
                (road[lane][blockPos+i]->getS()-s
                    <= OBSERVABLE_DIST_HUMAN_DRI)) {
                return road[lane][blockPos+i];
            }
        }
        return NULL;
    }
    Car *backCar(Car *road[NUM_LANE][NUM_BLOCKS_PER_LANE]) {
        for (int i = 1; (i < OBSERVABLE_BLOCKS_HUMAN_DRI
                         ) && blockPos - i >= 0; i++) {
            if ((road[lane][blockPos-i] != NULL) &&
                (road[lane][blockPos-i]->getBlockPos() == blockPos+i) &&
                (s-road[lane][blockPos-i]->getS()
                    <= OBSERVABLE_DIST_HUMAN_DRI)) {
                return road[lane][blockPos-i];
            }
        }
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


    thisCar->setA(0);//TODO
    thisCar->updS();
    if ((int)(thisCar->getS()) >= ROAD_LENGTH) {
        delete thisCar;
        road[lane][blockPos] = NULL;
    }
    else {
//        std::cout << thisCar->getS() << "\n";
//        std::cout << ((int)(thisCar->getS())) / BLOCK_LENGTH << "\n";
        int newBlockPos = std::min((int)(thisCar->getS()) / BLOCK_LENGTH, NUM_BLOCKS_PER_LANE-1);
        road[lane][newBlockPos] = road[lane][blockPos];//assuming no lane change
//        if (thisCar->getS() > 1000.0) {
//            printf("%3f       %d\n", thisCar->getS(), newBlockPos);
//        }
        road[lane][blockPos] = NULL;


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

    thisCar->setPreA(thisCar->getA());
    thisCar->setPreV(thisCar->getV());
    thisCar->setPreS(thisCar->getS());
    thisCar->setA(a);
    thisCat->updS();

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
