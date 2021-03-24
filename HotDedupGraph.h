#ifndef HOTDEDUPGRAPH_H
#define HOTDEDUPGRAPH_H

#include <vector>
#include <iostream>
#include "graph.h"

using namespace std;

class HotDedupGraph{
private:
    int fileNum;
    vector<vector<int>> edges;
    vector<int> value;
    vector<int> arrivalRate;
    int modify();
public:
    HotDedupGraph(const char *filename);
    //需要获得
    Graph getTransformedGraph(map<int,vector<int>>&m,int &bonus,int &totalV);
    int evaluate(const Tree &t);
    int getFileNum(){return fileNum;};
};

#endif // HOTDEDUPGRAPH_H
