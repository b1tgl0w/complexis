/*
  Copyright (c) 2019-2021 John Miner

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include <fstream>
#include <set>
#include <list>
using namespace std;
int ocur[6][6];
bool fv(int a){
  list<int> l;
  set<int> s;
  l.push_front(a);
  do {
    int m=l.front();
    l.pop_front();
    s.insert(m);
    int x=m/6;
    int y=m%6;
    if(x-1>0&&ocur[x-1][y]!=0&&s.count(m-6)==0)
      l.push_front(m-6);
    if(x+1<6&&ocur[x+1][y]!=0&&s.count(m+6)==0)
      l.push_front(m+6);
    if(y-1>0&&ocur[x][y-1]!=0&&s.count(m-1)==0)
      l.push_front(m-1);
    if(y+1<6&&ocur[x][y+1]!=0&&s.count(m+1)==0)
      l.push_front(m+1);
  } while(l.size()>0);

  return s.size()==6;
}
void fclr(int i, int j, int k, int l, int m, int n){
  ocur[i/6][i%6]=0;
  ocur[j/6][j%6]=0;
  ocur[k/6][k%6]=0;
  ocur[l/6][l%6]=0;
  ocur[m/6][m%6]=0;
  ocur[n/6][n%6]=0;
}
int main(int argc, char**argv) {
  for(int i=0;i<6;++i){
    for(int j=0;j<6;++j){
      ocur[i][j]=0;
    }
  }
  ofstream of;
  of.open("combos", fstream::out);
  for(int i=0;i<49;++i){
    for(int j=i+1;j<49;++j){
      for(int k=j+1;k<49;++k){
        for(int l=k+1;l<49;++l){
          for(int m=l+1;m<49;++m){
            for(int n=m+1;n<49;++n){
              ocur[i/6][i%6]=1;
              ocur[j/6][j%6]=1;
              ocur[k/6][k%6]=1;
              ocur[l/6][l%6]=1;
              ocur[m/6][m%6]=1;
              ocur[n/6][n%6]=1;
              if(fv(i)){
                for(int o=0;o<36;++o){
                  of << ocur[o/6][o%6];
                  if(o%6==5)
                    of << endl;
                }
              }
              fclr(i,j,k,l,m,n);
            }
          }
        }
      }
    }
  }
  of.close();
}
