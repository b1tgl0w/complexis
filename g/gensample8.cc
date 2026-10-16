#include <ctime>
#include <cstdlib>
#include <set>
#include <iostream>
#include <fstream>
using namespace std;
char *contents = new char[3997945];
int main(int argc, char **argv) {
  srand(time(0));
  set<int> s;
  ifstream file("combos8");
  ofstream sample("sample8");
  long i = 0;
  while(i < 3997944) {
    char c = file.get();
    contents[i] = c;
    i++;
  }
  contents[i] = 0;
  for(i = 0; i < 1000; ++i){
    int a = rand() % 55527;
    while (s.count(a) > 0)
      a = rand() % 55527;
    for (int j = (a * 9 * 8); j < ((a + 1) * 9 * 8); ++j)
      sample << contents[j];
  }
  file.close();
  sample.close();
}
