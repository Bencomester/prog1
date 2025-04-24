#include <bits/stdc++.h>
using namespace std;int main(){string t,s;cout<<"text: ";getline(cin,t);for(char c:t)if(isalpha(c))s+=tolower(c);cout<<(s==string(s.rbegin(),s.rend())?"":"nem ")<<"palindróm\n";int n,j;cout<<"\nhány? ";multiset<int>v;for(cin>>n;n--;){cin>>j;v.insert(j);}for(int i:v)cout<<i<<" ";}
