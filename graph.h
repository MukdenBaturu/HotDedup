#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <queue>
#include "subset.h"

using namespace std;

typedef struct edge{
    int startpoint;
    int endpoint;
    edge(int s,int e){
        startpoint=s;
        endpoint=e;
    };
    bool isSame(int i,int j){
        return (startpoint==i&&endpoint==j)||(startpoint==j&&endpoint==i);
    }
    bool operator < (const edge &rhs) const{
        if(startpoint!=rhs.startpoint)
            return startpoint<rhs.startpoint;
        return endpoint<rhs.endpoint;
    }
}Edge;

typedef struct tree{
private:
    void cutSubTree(int r){
        for (int i=0;i<children[r].size();i++){
            cutSubTree(children[r][i]);
        }
        vector<int> v;
        children[r]=v;
    }
public:

    int maxnode;
    int nodenum;
    int root;
    vector<vector<int>> children;
    //以到顶点集的映射
    map<int,Subset> childComponents;
    tree(){};
    tree(int n){
        maxnode=n;
        for (int i=0;i<n;i++){
            vector<int> v;
            children.push_back(v);
        }
    }
    void reduceSelf(const vector<Subset> & inactive,int cur)
    {
        for (int i=0;i<children[cur].size();i++){
            Subset s=childComponents.find(children[cur][i])->second;
            bool cut=false;
            for (int j=0;j<inactive.size();j++){
                if(s.isSame(inactive[j])){
                    children[cur][i]=-1;
                    cutSubTree(i);
                    cut=true;
                    break;
                }
            }
            if(!cut)
                reduceSelf(inactive,children[cur][i]);
        }
        nodenum=0;
        queue<int> q;
        q.push(root);
        while (!q.empty()){
            int cur=q.front();
            q.pop();
            nodenum++;
            for (int i=0;i<children[cur].size();i++)
                q.push(children[cur][i]);
        }
    };
    int value(const vector<vector<int>> &dist)
    {
        int ans=0;
        for (int i=0;i<children.size();i++){
            for (int j=0;j<children[i].size();j++)
                if(children[i][j]!=-1){
                    ans+=min(dist[i][children[i][j]],dist[children[i][j]][i]);
                }
        }
        return ans;
    };
    vector<int> getAllNodes()
    {
        vector<int> ans;
        queue<int> q;
        q.push(root);
        while (!q.empty()){
            int cur=q.front();
            q.pop();
            ans.push_back(cur);
            for (int i=0;i<children[cur].size();i++)
                q.push(children[cur][i]);
        }
        return ans;
    }
    vector<Edge> getEdges(){
        queue<int> q;
        q.push(root);
        vector<Edge> ans;
        while (!q.empty()){
            int cur=q.front();
            q.pop();
            for (int i=0;i<children[cur].size();i++){
                ans.push_back(edge(cur,children[cur][i]));
                q.push(children[cur][i]);
            }
        }
        return ans;
    }
}Tree;

class Graph{
private:
    int nodenum;
    int edgenum;
    vector<Edge> e;
    //vector<int> p;
    map<Subset,double> y;
    map<Subset,double> potential;
    map<int,Subset> nodeToSubsetMap;
    vector<vector<int>> dist;
    void generateTree(int root,const vector<Edge> &edges,Tree &t);
public:
    void setPotential(double p);
    Graph(vector<vector<int>> & dist);
    Tree getEdgesWithPotentialandRoot(double p,int root,double delta);
    // only for test
    void print();
    Tree kmst(int startPt,int deltaPt,int topPt,int k,double startp,double deltap,double deltam);
};

#endif // GRAPH_H

