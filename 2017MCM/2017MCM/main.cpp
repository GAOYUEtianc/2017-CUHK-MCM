//
//  main.cpp
//  2017MCM
//
//  Created by Shi Yue on 19/1/2017.
//  Copyright © 2017 Shi Yue. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
class Vertex;

class Vertex {
private:
    double mile;
    Vertex *next;
public:
    Vertex(double mile) {
        this.mile = mile;
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
        this.routeID = routeID;
        this.type = type;
        this.startMilepost = startMilepost;
        this.endMilepost = endMilepost;
        this.numLaneDec = numLaneDec;
        this.numLaneInc = numLaneInc;
    }
}