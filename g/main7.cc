#include <fstream>
#include <set>
#include <list>
using namespace std;
int ocur[7][7];
bool fv(int a){
  list<int> l;
  set<int> s;
  l.push_front(a);
  do {
    int m=l.front();
    l.pop_front();
    s.insert(m);
    int x=m/7;
    int y=m%7;
    if(x-1>0&&ocur[x-1][y]!=0&&s.count(m-7)==0)
      l.push_front(m-7);
    if(x+1<7&&ocur[x+1][y]!=0&&s.count(m+7)==0)
      l.push_front(m+7);
    if(y-1>0&&ocur[x][y-1]!=0&&s.count(m-1)==0)
      l.push_front(m-1);
    if(y+1<7&&ocur[x][y+1]!=0&&s.count(m+1)==0)
      l.push_front(m+1);
  } while(l.size()>0);

  return s.size()==7;
}
void fclr(int i, int j, int k, int l, int m, int n, int o){
  ocur[i/7][i%7]=0;
  ocur[j/7][j%7]=0;
  ocur[k/7][k%7]=0;
  ocur[l/7][l%7]=0;
  ocur[m/7][m%7]=0;
  ocur[n/7][n%7]=0;
  ocur[o/7][o%7]=0;
}
int main(int argc, char**argv) {
  for(int i=0;i<7;++i){
    for(int j=0;j<7;++j){
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
              for(int o=n+1;o<49;++o){
                ocur[i/7][i%7]=1;
                ocur[j/7][j%7]=1;
                ocur[k/7][k%7]=1;
                ocur[l/7][l%7]=1;
                ocur[m/7][m%7]=1;
                ocur[n/7][n%7]=1;
                ocur[o/7][o%7]=1;
                if(fv(i)){
                  for(int p=0;p<49;++p){
                    of << ocur[p/7][p%7];
                    if(p%7==6)
                      of << endl;
                  }
                }
                fclr(i,j,k,l,m,n,o);
              }
            }
          }
        }
      }
    }
  }
  of.close();
}
