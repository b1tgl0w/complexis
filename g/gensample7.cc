#include <ctime>
#include <cstdlib>
#include <set>
#include <iostream>
#include <fstream>
using namespace std;
char *contents = new char[649600];
int main(int argc, char **argv) {
  srand(time(0));
  set<int> s;
  ifstream file("combos7");
  ofstream sample("sample7");
  long i = 0;
  while(i < 649600) {
    char c = file.get();
    contents[i] = c;
    i++;
  }
  contents[i] = 0;
  for(i = 0; i < 1000; ++i){
    int a = rand() % 11600;
    while (s.count(a) > 0)
      a = rand() % 11600;
    for (int j = (a * 8 * 7); j < ((a + 1) * 8 * 7); ++j)
      sample << contents[j];
  }
  file.close();
  sample.close();
}
