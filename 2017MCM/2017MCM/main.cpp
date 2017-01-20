//
//  main.cpp
//  2017MCM
//
//  Created by Shi Yue on 19/1/2017.
//  Copyright © 2017 Shi Yue. All rights reserved.
//
#include <iostream>
#include <fstream>

const int MAX_EDGES_PER_VERTEX = 4

class Vertex {
private:
    double mile;
    Edge *edges[MAX_EDGES_PER_VERTEX];
    //edges is an array, containing "Edge *" pointers.
public:
    Vertex(double mile) {
        this->mile = mile;
        for (int i = 0; i < MAX_EDGES_PER_VERTEX; i++) {
            edges[i] = NULL;
        }
    }
    double getMile() {
        return this->mile;
    }
    void setEdge(Edge *e) {
        for (int i = 0; i < MAX_EDGES_PER_VERTEX; i++) {
            if (this->edges[i] == NULL;) {
                this->edges[i] = e;
            }
        }
    }
    Edge *getEdge(int index) {
        return this->edges[i];
    }
};

class Edge {
private:
    int routeID;
    char type;
    Vertex *startMilepost;
    Vertex *endMilepost;
    int numLaneDec;
    int numLaneInc;
public:
    Edge(int routeID, char type, Vertex *startMilepost, Vertex *endMilepost, int numLaneDec, int numLaneInc) {
        this->routeID = routeID;
        this->type = type;                 //type of road, possible "IS" or "SR"
        this->startMilepost = startMilepost;
        this->endMilepost = endMilepost;
        this->numLaneDec = numLaneDec;
        this->numLaneInc = numLaneInc;
    }
    int getRouteID() {
        return this->routeID;
    }
    char getType() {
        return this->type;
    }
    Vertex *getStartMilepost() {
        return this->startMilepost;
    }
    Vertex *getEndMilepost() {
        return this->endMilepost;
    }
    int getNumLaneDec() {
        return this->numLaneDec;
    }
    int getNumLaneInc() {
        return this->numLaneInc;
    }
};


int main( ) {


    return 0;
}
