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
