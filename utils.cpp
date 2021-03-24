#include <cstdio>
#include <cstdlib>

#include "subset.h"
#include "graph.h"

long long int pow(int a,int b){
    long long int ans=1;
    for (int i=0;i<b;i++)
        ans=ans*a;
    return ans;
}

bool isSideEdgeToSubset(const Edge &e,const Subset &s)
{
    return (s.has(e.startpoint)&&!s.has(e.endpoint))||(!s.has(e.startpoint)&&s.has(e.endpoint));
}


int abs(int a)
{
    if(a<0)
        return -a;
    return a;
}
