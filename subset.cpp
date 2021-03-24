#include "subset.h"

Subset::Subset(int t)
{
    total=t;
    int num=t/32+(t%32?1:0);
    state=true;
    for (int i=0;i<num;i++){
        mp.push_back(0);
    }
}

Subset::Subset(int t,vector<int> &m)
{
    total=t;
    mp=m;
}

bool Subset::has(int component) const
{
    if (component>=total)
        return false;
    return mp[component/32]&(1<<(component%32));
}

bool Subset::add(int component)
{
    if(component>=total)
        return false;
    mp[component/32]=mp[component/32]|(1<<(component%32));
    return true;
}

bool Subset::drop(int component)
{
    if(component>=total)
        return false;
    mp[component/32]=mp[component/32]&(~(1<<(component%32)));
    return true;
}

void Subset::print() const
{
    cout<<"total components:"<<total<<endl;
    int cnt=0;
    for (int i=0;i<mp.size();i++){
        for (int j=0;j<32;j++){
            if(mp[i]&(1<<j))
                cout<<cnt<<" ";
            cnt++;
        }
    }
    cout<<endl;
}

bool Subset:: operator < (const Subset &rhs) const
{
    if(total!=rhs.total)
        return total<rhs.total;
    return mp<rhs.mp;
}

Subset Subset::combine(const Subset &rhs) const {
    if(rhs.total!=total)
        return Subset(0);
    Subset ans(total);
    for (int i=0;i<mp.size();i++){
        ans.mp[i]=mp[i]|rhs.mp[i];
    }
    return ans;
}

void Subset::setState(const bool st){
    state=st;
}

bool Subset::getState() const
{
    return state;
}

bool Subset::isSame(const Subset &rhs) const
{
    if(mp.size()!=rhs.mp.size())
        return false;
    if(total!=rhs.total)
        return false;
    for (int i=0;i<mp.size();i++)
        if(mp[i]!=rhs.mp[i])
            return false;
    return true;
}
