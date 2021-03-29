#include "HotDedupGraph.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

HotDedupGraph::HotDedupGraph(const char *filename)
{
    ifstream input(filename);
    input>>fileNum;
    for (int i=0;i<fileNum;i++)
    {
        int t;
        input>>t;
        value.push_back(t);
    }
    arrivalRate=vector<int>(2*fileNum,0);
    for (int i=0;i<fileNum;i++){
        int t;
        input>>t;
        arrivalRate[2*i]=t;
    }
    int n;
    input>>n;
    edges=vector<vector<int>>(2*fileNum,vector<int>(2*fileNum,0));
    for (int i=0;i<n;i++){
        int s,e,t;
        input>>s>>e>>t;
        edges[2*s][2*e]=-t;
    }
}

/*
*返回值应该是论文里的|V|，意义不明
*/
int HotDedupGraph::modify()
{
    for (int i=0;i<fileNum;i++){
        edges[2*i][2*i+1]=value[i];
    }
    for (int i=0;i<fileNum;i++){
        arrivalRate[2*i+1]=2*arrivalRate[2*i]*2*fileNum+1;
        arrivalRate[2*i]=1;
    }
    return 2*fileNum;
}

Graph HotDedupGraph::getTransformedGraph(map<int,vector<int>>&m,int &bonus,int &totalV)
{
    int v=modify();
    totalV=v;
    int small=0;
    for (int i=0;i<edges.size();i++){for (int j=0;j<edges[i].size();j++){small=min(small,edges[i][j]);}}
    bonus=-small+1;
    int cnt=fileNum*2;

    for (int i=0;i<fileNum;i++){
        vector<int> v;
        for (int j=0;j<arrivalRate[2*i+1]-1;j++)
            v.push_back(cnt++);
        m[i]=v;
    }

    vector<vector<int>> dist(cnt,vector<int>(cnt,1<<30));
    int ecnt=0;
    for (int i=0;i<edges.size();i++){
        for (int j=0;j<edges[i].size();j++){
            if(edges[i][j]!=0){
                cout<<i<<" "<<j<<" "<<edges[i][j]+bonus<<endl;
                dist[i][j]=edges[i][j]+bonus;
                ecnt++;
            }
        }
    }
    for (int i=0;i<fileNum;i++){
        vector<int> v=m[i];
        for (auto n:v){
            dist[2*i+1][n]=bonus;
            ecnt++;
        }
    }
    cout<<ecnt<<endl;
    Graph ans(dist);
    return ans;
}

int HotDedupGraph::evaluate(const Tree &t,set<int> &fileset)
{
    queue<int> q;
    q.push(t.root);
    int ans=0;
    while (!q.empty()){
        int cur=q.front();
        q.pop();
        for (auto c:t.children[cur]){
            if(c<2*fileNum){
                bool flag=true;
                if(cur!=c+1&&c%2==0){
                    flag=false;
                    for (int i=0;i<t.children[c].size();i++){
                        if(t.children[c][i]==c+1&&t.children[c+1].size()>=arrivalRate[c+1]/2){
                            flag=true;
                            break;
                        }
                    }
                }
                if(flag){
                    int add=edges[c][cur]==0?edges[cur][c]:edges[c][cur];
//                    cout<<cur<<" "<<c<<" "<<add<<endl;
                    ans+=add;
                    q.push(c);
                    if(fileset.count(c/2)==0)
                        fileset.insert(c/2);
                }

            }
        }
    }
    return ans;
}


