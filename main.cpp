#include <iostream>
#include "subset.h"
#include "graph.h"
#include "HotDedupGraph.h"
#include <queue>
#include <set>

using namespace std;

int main()
{
//    Subset s(64);
//    while (true){
//        int t,n;
//        cin>>t>>n;
//        switch (t){
//        case 1:
//            if(!s.add(n))
//                cout<<"false";
//            break;
//        case 2:
//            if(!s.drop(n))
//                cout<<"false";
//            break;
//        case 3:
//            if(s.has(n))
//                cout<<"yes";
//            else
//                cout<<"no";
//            break;
//        }
//        s.print();
//    }
//    return 0;

    int mode=0;
    if (mode==1){
        int n,t;
        double p;
        cin>>n>>t;
        int k;
        cin>>k;
        vector<vector<int>> v(n,vector<int>(n,1<<30));
        for (int i=0;i<t;i++){int sp,ep,value;cin>>sp>>ep>>value;v[sp][ep]=value;}
        Graph g(v);
        g.print();
        g.kmst(0,1,3,k,1,0.5,0.1);
    }
    else{
        HotDedupGraph hg("testgraph.txt");
        map<int,vector<int>> mp;
        int bonus,totalV;
        Graph g=hg.getTransformedGraph(mp,bonus,totalV);
        int low=0;
        int high=2*totalV*2*hg.getFileNum();
        int B=6,curSize;
        set<int> ans;
        while(high-low>1){
            int curQ=(high+low)/2;
            Tree t=g.kmst(1,2,9,curQ,1,0.5,0.1);
//            {
//                queue<int> q;
//                q.push(t.root);
//                while (!q.empty()){
//                    int cur=q.front();
//                    q.pop();
//                    cout<<cur<<":";
//                    for (int i=0;i<t.children[cur].size();i++){
//                        if(t.children[cur][i]<10){
//                            q.push(t.children[cur][i]);
//                            cout<<t.children[cur][i]<<";";
//                        }
//                    }
//                    cout<<endl;
//                }
//            }
            ans.erase(ans.begin(),ans.end());
            int curSize=hg.evaluate(t,ans);

            cout<<"cur:"<<curSize<<" "<<curQ<<" "<<(abs(curSize-B)>1)<<endl;
            //system("pause");
            if(abs(curSize-B)<1)
                break;
            if(curSize>B)
                high=curQ;
            else
                low=curQ;
        }
//        while (high){
//            Tree t=g.kmst(1,2,9,high,20,0.5,0.1);
//            ans.erase(ans.begin(),ans.end());
//            int curSize=hg.evaluate(t,ans);
//            cout<<curSize<<" "<<high<<endl;
////            system("pause");;
//            if(curSize<=B)
//                break;
//            high--;
//        }
        cout<<"chose file"<<endl;
        for (auto n:ans){
            cout<<n<<" ";
        }
        return 0;
    }
}
