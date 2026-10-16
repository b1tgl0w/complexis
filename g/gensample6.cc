#include <ctime>
#include <cstdlib>
#include <set>
#include <iostream>
#include <fstream>
using namespace std;
char *contents = new char[100380];
int main(int argc, char **argv) {
  srand(time(0));
  set<int> s;
  ifstream file("combos6");
  ofstream sample("sample6");
  long i = 0;
  while(i < 100380) {
    char c = file.get();
    contents[i] = c;
    i++;
  }
  contents[i] = 0;
  for(i = 0; i < 1000; ++i){
    int a = rand() % 2390;
    while (s.count(a) > 0)
      a = rand() % 2390;
    for (int j = (a * 7 * 6); j < ((a + 1) * 7 * 6); ++j)
      sample << contents[j];
  }
  file.close();
  sample.close();
}
