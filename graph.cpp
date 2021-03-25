#include "graph.h"
#include <cmath>
#include "utils.h"
#include <queue>
#include <set>
#include <cstdlib>

Subset getSubTreeComponents(Tree &t,int cur){
    if(t.childComponents.find(cur)!=t.childComponents.end())
        return t.childComponents.find(cur)->second;
    Subset s(t.maxnode);
    s.add(cur);
    for (int i=0;i<t.children[cur].size();i++){
        s=s.combine(getSubTreeComponents(t,t.children[cur][i]));
    }
    t.childComponents.insert(make_pair(cur,s));
    return s;
}


Graph::Graph(vector<vector<int>> & dist)
{
    nodenum=dist.size();
    this->dist=dist;
    edgenum=0;
    for (int i=0;i<nodenum;i++){
        for (int j=i+1;j<nodenum;j++){
            if(dist[i][j]!=1<<30){
                edgenum++;
                e.push_back(Edge(i,j));
            }
        }
    }
//    long long int top=pow(2,nodenum);
//    for (long long int i=0;i<top;i++){
//        vector<int> v;
//        int num=nodenum/32+1;
//        long long int n=i;
//        for (int j=0;j<num;j++){
//            v.push_back(n%(((long long int)1)<<32));
//            n=n>>32;
//        }
//        Subset s(nodenum,v);
//        y.insert(make_pair(s,0));
//    }
    for (int i=0;i<nodenum;i++){
        Subset s(nodenum);
        s.add(i);
        y.insert(make_pair(s,0));
        potential.insert(make_pair(s,0));
        nodeToSubsetMap.insert(make_pair(i,s));
    }

}

void Graph::setPotential(double p)
{
    y.erase(y.begin(),y.end());
    potential.erase(potential.begin(),potential.end());
    for (int i=0;i<nodenum;i++){
        Subset s(nodenum);
        s.add(i);
        y.insert(make_pair(s,0));
        potential.insert(make_pair(s,p));
    }
}

Tree Graph::getEdgesWithPotentialandRoot(double p,int root,double delta)
{
    setPotential(p);
    Subset s(nodenum);
    s.add(root);
    potential.erase(potential.find(s));
    s.setState(false);
    potential.insert(make_pair(s,p));
    vector<Subset> inactiveSets;

    map<Subset,vector<Edge>> forests;

    set<Edge> addedEdge;

    while (true){
        bool jumpout=true;
        for (map<Subset,double>::iterator iter=potential.begin();iter!=potential.end();iter++){
            if(iter->first.getState()){
                jumpout=false;
                break;
            }
        }
        if(jumpout)
            break;


        //更新y值
        vector<Subset> newSubsets;
        for (map<Subset,double>::iterator iter=potential.begin();iter!=potential.end();iter++){
            if(iter->first.getState()){
                double small=delta;
//                iter->first.print();
//                cout<<iter->second<<endl;
                if(iter->second<=delta){
                    small=iter->second;
                    inactiveSets.push_back(iter->first);
                    //iter->first.setState(false);
                }
                iter->second-=small;
                if(iter->second<delta/10){
                    iter->second=0;
                }
                y.find(iter->first)->second+=small;
                if(iter->second==0){
                    newSubsets.push_back(iter->first);
                }
            }
        }
        for (int i=0;i<newSubsets.size();i++){
//            cout<<"cancel"<<endl;
//            newSubsets[i].print();
//            cout<<endl;
            potential.erase(potential.find(newSubsets[i]));
            newSubsets[i].setState(false);
            potential.insert(make_pair(newSubsets[i],0));
        }

        //对每条边进行计算

        for (int i=0;i<e.size();i++){
            if(addedEdge.count(e[i])!=0)
                continue;
            double sum=0;
            for (map<Subset,double>::iterator iter=y.begin();iter!=y.end();iter++){
                if(isSideEdgeToSubset(e[i],iter->first)){
                    sum+=iter->second;
                }
            }
            if(sum>=dist[e[i].startpoint][e[i].endpoint]){
                //可以优化,做一个edge到subset的一个映射
//                cout<<"edge "<<i<<" startpoint:"<<e[i].startpoint<<" endpoint:"<<e[i].endpoint<<" value:"<<dist[e[i].startpoint][e[i].endpoint]<<" sum:"<<sum<<endl;
                vector<Subset> v;
                double newPotential=0;
                for (map<Subset,double>::iterator iter=potential.begin();iter!=potential.end();iter++){
                    if(iter->first.has(e[i].startpoint)||iter->first.has(e[i].endpoint)){
                        v.push_back(iter->first);
//                        cout<<"combining"<<endl;
//                        iter->first.print();
                        newPotential+=iter->second;
//                        cout<<"sum "<<newPotential<<endl;
                    }
                }
                //获取合并之后的子集
                if(v.size()==2){
//                    cout<<"merge"<<endl;
                    Subset newSubset=v[0].combine(v[1]);
                    if(newSubset.has(root)){
 //                       cout<<"root"<<endl;
//                        system("pause");
                        newSubset.setState(false);
                    }
                    //合并
                    //cout<<"here "<<v.size()<<endl;
                    potential.erase(potential.find(v[0]));
                    potential.erase(potential.find(v[1]));
                    potential.insert(make_pair(newSubset,newPotential));
                    y.insert(make_pair(newSubset,0));

                    //记录该子集里生成树的边
                    //cout<<"here"<<endl;
                    vector<Edge> edges;
                    for (int i=0;i<v.size();i++){
                        //cout<<"v["<<i<<"].printself"<<endl;
//                        v[i].print();
//                        cout<<endl;
                        if(forests.find(v[i])!=forests.end()){
                            //cout<<"get"<<endl;
                            edges.insert(edges.end(),forests.find(v[i])->second.begin(),forests.find(v[i])->second.end());
                            forests.erase(forests.find(v[i]));
                        }
                    }
                    edges.push_back(e[i]);
                    addedEdge.insert(e[i]);
//                    cout<<edges.size()<<endl;
                    forests.insert(make_pair(newSubset,edges));
                }
            }
        }
//        cout<<"inactivesets:"<<inactiveSets.size()<<endl;
    }

//    for (map<Subset,double>::iterator iter=potential.begin();iter!=potential.end();iter++){
//        iter->first.print();
//        cout<<endl;
//    }

    //根据集合生成这棵树
    Tree t(nodenum);
    for (map<Subset,vector<Edge>>::iterator iter=forests.begin();iter!=forests.end();iter++){
        if(iter->first.has(root)){
//            cout<<endl<<endl;
//            iter->first.print();
//            cout<<"edges;"<<iter->second.size()<<endl;
            generateTree(root,iter->second,t);
            getSubTreeComponents(t,root);
            //剪枝
            t.reduceSelf(inactiveSets,t.root);
            //整理
            Tree ans(nodenum);
            ans.nodenum=1;
            ans.root=t.root;
            for (int i=0;i<t.children.size();i++){
//                cout<<i<<":";
                for (int j=0;j<t.children[i].size();j++){
                    if(t.children[i][j]!=-1){
//                        cout<<t.children[i][j]<<" ";
                        ans.children[i].push_back(t.children[i][j]);
                        ans.nodenum++;
                    }
                }
//                cout<<endl;
            }
            return ans;
        }
    }

    t.root=root;
    t.nodenum=1;
    return t;

}

Tree Graph::kmst(int startPt,int deltaPt,int topPt,int k,double startp,double deltap,double deltam)
{
    Tree ans;
    bool hasans=false;
    Tree old;
    for (int i=startPt;i<=topPt;i=i+deltaPt){
        double potential=deltap;

        bool hasold=false;
        while (true){
//            cout<<"currentroot:"<<i<<" "<<"current potential"<<potential<<endl;
            Tree t=getEdgesWithPotentialandRoot(potential,i,deltam);
//            cout<<"nodenum:"<<t.nodenum<<endl;
//            system("pause");
            if(t.nodenum>k){
                //合并old
                Tree cur(nodenum);
                if(!hasold){
                    vector<Edge> edges;
                    vector<int> distance (nodenum,1<<30);
                    vector<int> edge (nodenum,-1);
                    for (int j=0;j<nodenum;j++){
                        int val=min(distance[j],min(dist[i][j],dist[j][i]));
                        if(val<distance[j]){
                            for (int k=0;k<e.size();k++){
                                if(e[k].isSame(i,j)){
                                    edge[j]=k;
                                    break;
                                }
                            }
                            distance[j]=val;
                        }
                    }
                    int cnt=1;
                    vector<int> curNodes;
                    curNodes.push_back(i);
                    while (cnt!=k){
                        int small=1<<30;
                        int pos;
                        for (int j=0;j<nodenum;j++){
                            if(small>distance[j]){
                                small=distance[j];
                                pos=j;
                            }
                        }
                        edges.push_back(e[edge[pos]]);
                        curNodes.push_back(pos);
                        for (int j=0;j<nodenum;j++){
                            int val=min(distance[j],min(dist[pos][j],dist[pos][i]));
                            if(val<distance[j]){
                                for (int k=0;k<e.size();k++){
                                    if(e[k].isSame(pos,j)){
                                        edge[j]=k;
                                        break;
                                    }
                                }
                                distance[j]=val;
                            }
                        }
                        for (int j=0;j<curNodes.size();j++)
                            distance[curNodes[j]]=1<<30;
                        cnt++;
                    }
//                    cout<<"ans tree new:"<<edges.size()<<endl;
//                    for (auto e:edges){
//                        cout<<e.startpoint<<" "<<e.endpoint<<endl;
//                    }
//                    system("pause");
                    generateTree(i,edges,cur);
                }
                else{
                    vector<Edge> edges=old.getEdges();
//                    cout<<"edges.size:"<<edges.size()<<endl;
                    vector<int> distance (nodenum,1<<30);
                    vector<int> edge (nodenum,-1);
                    vector<int> curNodes=old.getAllNodes();
//                    cout<<"curNodes.size:"<<curNodes.size()<<endl;
//                    system("pause");

                    for (int j=0;j<nodenum;j++){
                        int small=1<<30;
                        int node=-1;
                        for (int k=0;k<curNodes.size();k++){
                            if(small>min(dist[curNodes[k]][j],dist[j][curNodes[k]])){
                                small=min(dist[curNodes[k]][j],dist[j][curNodes[k]]);
                                node=curNodes[k];
                            }
                        }
                        if(node!=-1){
//                            cout<<"here"<<endl;
                            distance[j]=small;
                            for(int k=0;k<e.size();k++){
                                if(e[k].isSame(j,node)){
//                                    cout<<"edge num:"<<k<<" "<<j<<" "<<node<<endl;
                                    edge[j]=k;
                                    break;
                                }
                            }
                        }
                    }
                    for (int i=0;i<curNodes.size();i++)
                        distance[curNodes[i]]=1<<30;




                    int cnt=curNodes.size();

                    while (cnt!=k){
                        int small=1<<30;
                        int pos;
                        for (int j=0;j<nodenum;j++){
                            if(small>distance[j]){
                                small=distance[j];
                                pos=j;
                            }
                        }
//                        cout<<"push edge:"<<small<<" "<<pos<<" "<<edge[pos]<<endl;
                        edges.push_back(e[edge[pos]]);
                        curNodes.push_back(pos);
                        for (int j=0;j<nodenum;j++){
                            int val=min(distance[j],min(dist[pos][j],dist[j][pos]));
                            if(val<distance[j]){
                                for (int k=0;k<e.size();k++){
                                    if(e[k].isSame(pos,j)){
                                        edge[j]=k;
                                        break;
                                    }
                                }
                                distance[j]=val;
                            }
                        }
                        for (int j=0;j<curNodes.size();j++)
                            distance[curNodes[j]]=1<<30;
                        cnt++;
                    }
//                    cout<<"ans tree old:"<<edges.size()<<endl;
//                    for (auto e:edges){
//                        cout<<e.startpoint<<" "<<e.endpoint<<endl;
//                    }
//                    system("pause");
                    generateTree(i,edges,cur);
                }

//                cout<<"cur-root:"<<cur.root<<endl;
//                for (int i=0;i<cur.children.size();i++){
//                    cout<<i<<":";
//                    for (int j=0;j<cur.children[i].size();j++){
//                        cout<<cur.children[i][j]<<" ";
//                    }
//                    cout<<endl;
//                }
//                system("pause");

                if(!hasans){
                    ans=cur;
                    hasans=true;
                }
                else{
//                    cout<<"values:"<<cur.value(dist)<<" "<<ans.value(dist)<<endl;
                    if(cur.value(dist)<ans.value(dist))
                        ans=cur;
//                    cout<<ans.nodenum<<" "<<k<<endl;
//                    system("pause");
                }
                break;

            }
            old=t;
            potential+=deltap;
            hasold=true;
        }
    }
    return ans;
}

void Graph::generateTree(int root,const vector<Edge> &edges,Tree &t)
{
    int nm=1;
    t.root=root;
    queue<int> q;
    q.push(root);
    vector<bool> vis(edges.size(),false);
    while (!q.empty()){
        int fnt=q.front();
        q.pop();
        for (int i=0;i<vis.size();i++){
            if(!vis[i]){
                if(edges[i].startpoint==fnt){
                    vis[i]=true;
                    t.children[fnt].push_back(edges[i].endpoint);
                    q.push(edges[i].endpoint);
                    nm++;
                }
                else if(edges[i].endpoint==fnt){
                    vis[i]=true;
                    nm++;
                    t.children[fnt].push_back(edges[i].startpoint);
                    q.push(edges[i].startpoint);
                }
            }
        }
    }
    t.nodenum=nm;
}


void Graph::print()
{
    cout<<"nodenum:"<<nodenum<<endl;
    cout<<"edgenum:"<<edgenum<<endl;
    for(auto &sglE:e){
        cout<<sglE.startpoint<<" "<<sglE.endpoint<<endl;
    }
    for (map<Subset,double>::iterator iter=y.begin();iter!=y.end();iter++){
        iter->first.print();
        cout<<iter->second<<endl;
    }
    for (map<Subset,double>::iterator iter=potential.begin();iter!=potential.end();iter++){
        iter->first.print();
        cout<<iter->second<<endl;
    }
}
