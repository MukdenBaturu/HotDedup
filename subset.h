#ifndef SUBSET_H
#define SUBSET_H

#include <iostream>
#include <cstdio>
#include <vector>



using namespace std;

class Subset{
private:
    int total;
    vector<int> mp;
    bool state;
public:
    Subset(int t);
    Subset(int t,vector<int>& m);
    Subset combine(const Subset &rhs) const;
    bool has(int component) const;
    bool add(int component);
    bool drop(int component);
    //for test
    void print() const;
    bool operator < (const Subset &rhs) const;
    void setState(const bool st);
    bool getState() const;
    bool isSame(const Subset &rhs) const;
};

#endif // SUBSET_H
