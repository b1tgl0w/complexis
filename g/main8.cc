#include <fstream>
#include <set>
#include <list>
using namespace std;
int ocur[8][8];
bool fv(int a){
  list<int> l;
  set<int> s;
  l.push_front(a);
  do {
    int m=l.front();
    l.pop_front();
    s.insert(m);
    int x=m/8;
    int y=m%8;
    if(x-1>0&&ocur[x-1][y]!=0&&s.count(m-8)==0)
      l.push_front(m-8);
    if(x+1<8&&ocur[x+1][y]!=0&&s.count(m+8)==0)
      l.push_front(m+8);
    if(y-1>0&&ocur[x][y-1]!=0&&s.count(m-1)==0)
      l.push_front(m-1);
    if(y+1<8&&ocur[x][y+1]!=0&&s.count(m+1)==0)
      l.push_front(m+1);
  } while(l.size()>0);

  return s.size()==8;
}
void fclr(int i, int j, int k, int l, int m, int n, int o, int oo){
  ocur[i/8][i%8]=0;
  ocur[j/8][j%8]=0;
  ocur[k/8][k%8]=0;
  ocur[l/8][l%8]=0;
  ocur[m/8][m%8]=0;
  ocur[n/8][n%8]=0;
  ocur[o/8][o%8]=0;
  ocur[oo/8][oo%8]=0;
}
int main(int argc, char**argv) {
  for(int i=0;i<8;++i){
    for(int j=0;j<8;++j){
      ocur[i][j]=0;
    }
  }
  ofstream of;
  of.open("combos8", fstream::out);
  for(int i=0;i<64;++i){
    for(int j=i+1;j<64;++j){
      for(int k=j+1;k<64;++k){
        for(int l=k+1;l<64;++l){
          for(int m=l+1;m<64;++m){
            for(int n=m+1;n<64;++n){
              for(int o=n+1;o<64;++o){
                for(int oo=o+1;oo<64;++oo){
                  ocur[i/8][i%8]=1;
                  ocur[j/8][j%8]=1;
                  ocur[k/8][k%8]=1;
                  ocur[l/8][l%8]=1;
                  ocur[m/8][m%8]=1;
                  ocur[n/8][n%8]=1;
                  ocur[o/8][o%8]=1;
                  ocur[oo/8][oo%8]=1;
                  if(fv(i)){
                    for(int p=0;p<64;++p){
                      of << ocur[p/8][p%8];
                      if(p%8==7)
                        of << endl;
                    }
                  }
                  fclr(i,j,k,l,m,n,o,oo);
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
