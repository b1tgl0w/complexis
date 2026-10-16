#include <fstream>
#include <set>
#include <list>
using namespace std;
int ocur[9][9];
bool fv(int a){
  list<int> l;
  set<int> s;
  l.push_front(a);
  do {
    int m=l.front();
    l.pop_front();
    s.insert(m);
    int x=m/9;
    int y=m%9;
    if(x-1>0&&ocur[x-1][y]!=0&&s.count(m-9)==0)
      l.push_front(m-9);
    if(x+1<9&&ocur[x+1][y]!=0&&s.count(m+9)==0)
      l.push_front(m+9);
    if(y-1>0&&ocur[x][y-1]!=0&&s.count(m-1)==0)
      l.push_front(m-1);
    if(y+1<9&&ocur[x][y+1]!=0&&s.count(m+1)==0)
      l.push_front(m+1);
  } while(l.size()>0);

  return s.size()==9;
}
void fclr(int i, int j, int k, int l, int m, int n, int o, int oo, int ooo){
  ocur[i/9][i%9]=0;
  ocur[j/9][j%9]=0;
  ocur[k/9][k%9]=0;
  ocur[l/9][l%9]=0;
  ocur[m/9][m%9]=0;
  ocur[n/9][n%9]=0;
  ocur[o/9][o%9]=0;
  ocur[oo/9][oo%9]=0;
  ocur[ooo/9][ooo%9]=0;
}
int main(int argc, char**argv) {
  for(int i=0;i<9;++i){
    for(int j=0;j<9;++j){
      ocur[i][j]=0;
    }
  }
  ofstream of;
  of.open("combos9", fstream::out);
  for(int i=0;i<81;++i){
    for(int j=i+1;j<81;++j){
      for(int k=j+1;k<81;++k){
        for(int l=k+1;l<81;++l){
          for(int m=l+1;m<81;++m){
            for(int n=m+1;n<81;++n){
              for(int o=n+1;o<81;++o){
                for(int oo=o+1;oo<81;++oo){
                  for(int ooo=oo+1;ooo<81;++ooo){
                    ocur[i/9][i%9]=1;
                    ocur[j/9][j%9]=1;
                    ocur[k/9][k%9]=1;
                    ocur[l/9][l%9]=1;
                    ocur[m/9][m%9]=1;
                    ocur[n/9][n%9]=1;
                    ocur[o/9][o%9]=1;
                    ocur[oo/9][oo%9]=1;
                    ocur[ooo/9][ooo%9]=1;
                    if(fv(i)){
                      for(int p=0;p<81;++p){
                        of << ocur[p/9][p%9];
                        if(p%9==8)
                          of << endl;
                      }
                    }
                    fclr(i,j,k,l,m,n,o,oo,ooo);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  of.close();
}
