#include <bits/stdc++.h>
using namespace std;int main(){ostream&o=cout;string t,s;o<<"Szöveg: ";getline(cin,t);for(char c:t)if(isalpha(c))s+=tolower(c);o<<(s==string(s.rbegin(),s.rend())?"P":"Nem p")<<"alindróm\n";int n,j;o<<"\nHány szám? ";cin>>n;multiset<int>v;for(int i=1;i<=n;i++){o<<i<<". ";cin>>j;v.insert(j);}for(int i:v)o<<i<<" ";}
