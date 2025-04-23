#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
using namespace std;
bool p(string t) {
	string s;
    for(char c:t)if(isalpha(c))s+=tolower(c);
    for(int l=0,r=s.size()-1;l<r;l++,r--)if(s[l]!=s[r])return 0;
    return 1;
}
int main() {
    string s;
    cout<<"Szót a palindróm ellenőrzéshez: ";
    getline(cin,s);
    cout<<"A megadott szöveg "<<(p(s)?"palindróm.\n":"nem palindróm.\n");
    int n;
    cout<<"\nHány számot szeretnél rendezni? ";
    cin>>n;
    cout<<"Add meg a számokat:\n";
    vector<int> v(n);
    for(int i=0;i<n;i++){cout<<i+1<<". ";cin>>v[i];}
    sort(v.begin(),v.end());
    cout<<"Rendezett tömb: ";
    for(int i:v)cout<<i<<" ";
    cout<<"\n";
}
