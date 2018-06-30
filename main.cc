#include <iostream>
#include <dlfcn.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <list>
#include <set>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Bounds {
  int left;
  int right;
  int top;
  int bottom;
};

struct Coord {
  int row;
  int col;
};

Uint32 bgcolor = 0xffa7a7a7; //"hsl(50, 7%, 97%)";
int blockclocks [5] = { 6, 9, 3, 12, 12 };
const int blockwidth = 25;
Uint32 blue = 0xffecdfc6; //"hsl(200, 45%, 70%)";
bool blueInCorridor = false;
Coord boardCoord;
const int boardLength = 34;
const int board0Length = 19;
bool board[boardLength][board0Length]; //TODO
const int buttonWidth = 25;
const int clockDirectionsLength = 8;
const int clockDirections[clockDirectionsLength] = {
  12, 1, 3, 5, 6, 7, 9, 11
};
Uint32 colorme = 0x00000100;
int corridorClockBlueCw = -1;
int corridorClockBlueCcw = -1;
int corridorClockGreenCw = -1;
int corridorClockGreenCcw = -1;
int corridorClockOrangeCw = -1;
int corridorClockOrangeCcw = -1;
int corridorClockRedCw = -1;
int corridorClockRedCcw = -1;
int corridorClockYellowCw = -1;
int corridorClockYellowCcw = -1;
const int gravityTimeout = 750;
int gravityTimer = gravityTimeout;
Uint32 gray1 = 0xffc5c5c5; // "hsl(280, 0%, 70%)";
Uint32 gray2 = 0xffc6c6c5; // gray1 + 1 r.
Uint32 green = 0xffc6e6e0; // "hsl(80, 45%, 70%)";
bool greenInCorridor = false;
//let highlight: string = "hsl(250, 100%, 80%)";
Uint32 keystate = 0;
Uint32 keystateprev = 0;
int moveTimeout = 144;
int moveTimerRed = moveTimeout;
int moveTimerOrange = moveTimeout;
int moveTimerYellow = moveTimeout;
int moveTimerGreen = moveTimeout;
int moveTimerBlue = moveTimeout;
int moveTimerWhole = moveTimeout;
int moveTimerDown = moveTimeout;
int moveTimerLeft = moveTimeout;
int moveTimerRight = moveTimeout;
int noParam = 0;
int noParam2 = 0;
Uint32 orange = 0xffc6d9ec; //"hsl(30, 45%, 70%)";
bool orangeInCorridor = false;
int prevDirectionMain[5] = { -1, -1, -1, -1, -1 };
int prevDirectionSecondary[5] = { -1, -1, -1, -1, -1 };
Uint32 red = 0xffc6c6ec; //"hsl(0, 45%, 70%)";
bool redInCorridor = false;
SDL_Renderer *renderer;
Uint32 renderTimeout = 12;
int renderTimer = renderTimeout;
SDL_Window *screen;
int updateInterval = 12;
Coord wholecoord = { .row = 0, .col = 0 };
const int wholeLength = 13;
const int whole0Length = 13;
Uint32 whole[wholeLength][whole0Length] = {
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor},
  { bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor, bgcolor}
};
Uint32 violet1 = 0xffecc6e0; //"hsl(280, 45%, 70%)";
Uint32 violet2 = 0xffecc6e1;
Uint32 violet3 = 0xffedc6e0;
Uint32 violet4 = 0xffedc6e1;
Uint32 yellow = 0xffc6e6ec; //"hsl(50, 45%, 70%)";
bool yellowInCorridor = false;
Uint32 blockcolors[9] = { red, orange, yellow, green, blue, violet1, violet2, violet3, violet4 };

void applyWhole();
bool arrayHas(Uint32 ar[], int size, Uint32 element);
bool betterCorridorExit(bool cwTrueCcwFalse, int directionA, int directionB, Uint32 color);
Bounds bounds(Uint32 target[][whole0Length]);
void capitalizeRows();
void centerWhole(Uint32 target[][whole0Length]);
void clockMove(Uint32 color, bool cwTrueCcwFalse);
int clockwise(bool cw, int clockDirection);
void cloneWhole(Uint32 target[][whole0Length], Uint32 dest[][whole0Length]);
int colorIndex(Uint32 color);
void colorWhole();
void colorsInCorridor(Uint32 potentialWhole[][whole0Length], bool &r, bool &o, bool &y, bool &g, bool &b);
bool contiguous(const Coord &startingCoord, Uint32 potential[][whole0Length]);
void corridorClocks(Uint32 potentialWhole[][whole0Length]);
void corridorClocksColor(Uint32 potentialWhole[][whole0Length], int &a, int &b);
vector<Coord> getAdjOccupied(Coord coords);
Coord getCenterCoords(Uint32 target[][whole0Length]);
int getClockDirection(const Coord &center, const Coord &d);
Coord getCoords(Uint32 color);
Coord getCoordsPotentialWhole(Uint32 color, Uint32 potentialWhole[][whole0Length]);
Coord getPotentialCoords(const Coord & coords, int clockDirection);
bool inCorridor(const Coord &coords, Uint32 whole[][whole0Length]);
bool inCorridor(Uint32 color, Uint32 whole[][whole0Length]);
bool inMoreSimilarDirectionAsPrev(Uint32 clockDirection, Uint32 newCoordsDirection, int prevDirectionMain,
  int prevDirectionSecondary);
bool isOpen(const Coord & potentialcoords, Uint32 potentialWhole[][whole0Length], const Coord & wholecoord);
void keyboard();
void mirrorWholeHoriz(Uint32 target[][whole0Length]);
void newBoard();
void newWhole(Uint32 newWholeX[][whole0Length]);
int oppositeClock(int clockDirection);
void proc();
Coord randomColorMe(Uint32 potentialWhole[][whole0Length]);
int randomRange(int min, int max);
void renderBoard();
void renderUi();
void rotateWhole(bool cwTrueCcwFalse);
Coord shadow(Uint32 wholeCoords[][whole0Length], const Coord &wholecoord); 
void shiftWhole(Uint32 target[][whole0Length], int clockDirection, bool translate);
Uint32 update(Uint32 interval, void *param);

void applyWhole() {
  for(int i = 0; i < wholeLength; ++i) {
    for(int j = 0; j < whole0Length; ++j) {
      if (whole[i][j] != bgcolor)
        board[wholecoord.row + i][wholecoord.col + j] = true;
    }
  }
  wholecoord.row = 0;
  wholecoord.col = 0;
  newWhole(whole);
  colorWhole();
}

bool arrayHas(Uint32 ar[], int size, Uint32 element) {
  for(int i = 0; i < size; ++i) {
    if(ar[i] == element)
      return true;
  }
  return false;
}

bool betterCorridorExit(bool cwTrueCcwFalse, int directionA, int directionB, Uint32 color) {
  if (color == red) {
    if (cwTrueCcwFalse)
      return corridorClockRedCw == directionB;
    else
      return corridorClockRedCcw == directionB;
  }
  if (color == orange) {
    if (cwTrueCcwFalse)
      return corridorClockOrangeCw == directionB;
    else
      return corridorClockOrangeCcw == directionB;
  }
  if (color == yellow) {
    if (cwTrueCcwFalse)
      return corridorClockYellowCw == directionB;
    else
      return corridorClockYellowCcw == directionB;
  }
  if (color == green) {
    if (cwTrueCcwFalse)
      return corridorClockGreenCw == directionB;
    else
      return corridorClockGreenCcw == directionB;
  }
  if (color == blue) {
    if (cwTrueCcwFalse)
      return corridorClockBlueCw == directionB;
    else
      return corridorClockBlueCcw == directionB;
  }
  return false;
}

Bounds bounds(Uint32 target[][whole0Length]) {
  int minx = 999;
  int maxx = -1;
  int miny = 999;
  int maxy = -1;
  for(int i = 0; i < wholeLength; ++i) {
    for(int j = 0; j < whole0Length; ++j) {
      if(target[i][j] != bgcolor) {
        if(j < minx)
          minx = j;
        if(j > maxx)
          maxx = j;
        if(i < miny)
          miny = i;
        if(i > maxy)
          maxy = i;
      }
    }
  }
  
  return { .left = minx, .right = maxx, .top = miny, .bottom = maxy };
}

void capitalizeRows() {
  bool full = true;
  for (int i = boardLength - 1; i >= 0; --i) {
    full = true;
    for (int j = 0; j < board0Length; ++j) {
      if (board[i][j] != true)
        full = false;
    }
    if (full) {
      for (int k = i; k >= 1; --k) {
        for (int l = 0; l < board0Length; ++l) {
          board[k][l] = board[k - 1][l];
        }
      }
      for (int l = 0; l < board0Length; ++l) {
        board[0][l] = false;
      }
      capitalizeRows();
      break;
    }
  }
}

void centerWhole(Uint32 target[][whole0Length], bool translate) {
  Coord center = getCenterCoords(target);
  Bounds b = bounds(target);
  bool evenVert = false;
  bool evenHoriz = false;
  if ((b.bottom - b.top) % 2 == 1)
    evenVert = true;
  if ((b.right - b.left) % 2 == 1)
    evenHoriz = true;

  if (evenVert == false) {
    if (center.row < 6) {
      while(center.row < 6) {
        shiftWhole(target, 6, translate);
        center.row++;
      }
    }
    else if (center.row > 6) {
      while (center.row > 6) {
        shiftWhole(target, 12, translate);
        center.row--;
      }
    }
  }
  else {
    if (center.row < 6) { // TODO hardcoded
      while (center.row < 6) {
        shiftWhole(target, 6, translate);
        center.row++;
      }
    }
    else if (center.row + 1 > 6) {
      while (center.row + 1 > 6) {
        shiftWhole(target, 12, translate);
        center.row--;
      }
    }
  }
  if (evenHoriz == false) {
    if (center.col < 6) {
      while (center.col < 6) {
        shiftWhole(target, 3, translate);
        center.col++;
      }
    }
    else if (center.col > 6) {
      while (center.col > 6) {
        shiftWhole(target, 9, translate);
        center.col--;
      }
    }
  }
  else {
    if (center.col < 6) { // TODO hardcoded
      while (center.col < 6) {
        shiftWhole(target, 3, translate);
        center.col++;
      }
    }
    else if (center.col + 1 > 6) {
      while (center.col + 1 > 6) {
        shiftWhole(target, 9, translate);
        center.col--;
      }
    }
  }
}

void clockMove(Uint32 color, bool cwTrueCcwFalse) {
  Coord curCoords = getCoords(color);
  Coord newCoords = { .row = curCoords.row, .col = curCoords.col };
  Uint32 newCoordsDirection = -1;

  corridorClocks(whole); // This stays up top... called once

  vector<Coord> adjCoords = getAdjOccupied(curCoords);

  if (adjCoords.size() < 4) {
    for (unsigned int i = 0; i < adjCoords.size(); ++i) {
      int clockDirection = getClockDirection(adjCoords[i], curCoords);
      if (clockDirection == -1)
        continue;
      int cw = clockwise(cwTrueCcwFalse, clockDirection);
      int cw2 = clockwise(cwTrueCcwFalse, cw);
      if (cw == -1 || cw2 == -1)
        continue;
      Coord step1 = getPotentialCoords(adjCoords[i], cw);
      if (step1.row == -1 || step1.col == -1)
        continue; // TODO is this right?
      Uint32 step1Whole[wholeLength][whole0Length];
      cloneWhole(whole, step1Whole); //TODO array copy
      step1Whole[curCoords.row][curCoords.col] = bgcolor;
      Coord potentialCoords = getPotentialCoords(adjCoords[i], cw2);
      if (potentialCoords.row == -1 || potentialCoords.col == -1)
        continue; // TODO is this right?
      Uint32 potentialWhole[wholeLength][whole0Length];
      cloneWhole(whole, potentialWhole); //TODO array copy
      potentialWhole[curCoords.row][curCoords.col] = bgcolor;
      potentialWhole[potentialCoords.row][potentialCoords.col] = color;
      // TODO step1Whole should be contiguous except for the piece in motion.
      if (isOpen(step1, whole, wholecoord) && isOpen(potentialCoords, whole, wholecoord) && contiguous(potentialCoords, potentialWhole) == true) {
        if(newCoords.row != curCoords.row && newCoords.col != curCoords.col) {
          if(inCorridor(curCoords, whole)) {
            if (betterCorridorExit(cwTrueCcwFalse, newCoordsDirection, clockDirection, color)) {
              newCoords = potentialCoords; // TODO copy ctor?
              newCoordsDirection = clockDirection;
            }
// Procedure:
// For each color:
// 1. If in corridor and no direction is set (such as a piece being in the corridor from initialization),
//    set its direction to 1. (For now, but see if there's something more logical later.)
// 2. Keep track of the direction you enter the corridor.
// 3. Exit the same direction you enter the corridor
// 4. If you reach a dead end (3 adjacent), then that's exiting the corridor.
// 4b.In this case, you'll re-enter the corridor in the next move in the opposite direction as when you exited.
// 5. Keep track of the button used to enter and exit the corridor, so that the player can reverse direction.
// 6. If moving a piece creates a corridor for another piece then reverse the direction of the piece.
// 7. If moving a piece destroys a corridor for another piece then reverse the direction of the piece.
          }
        }
        else {
          newCoords = potentialCoords; // TODO copy ctor?
          newCoordsDirection = clockDirection;
        }
      }
      else if (isOpen(potentialCoords, whole, wholecoord) == false || contiguous(potentialCoords, potentialWhole) == false) {
        int clockDirectionDiag = getClockDirection(potentialCoords, curCoords);
        if (clockDirectionDiag == -1)
          continue;
        int cwDiag = clockwise(cwTrueCcwFalse, clockDirectionDiag);
        if (cwDiag == -1)
          continue;
        Coord potentialCoordsDiag = getPotentialCoords(potentialCoords, cwDiag);
        if (potentialCoordsDiag.row == -1 || potentialCoordsDiag.col == -1)
          continue;
        Uint32 potentialWholeDiag[wholeLength][whole0Length];
        cloneWhole(whole, potentialWholeDiag); //TODO array copy
        potentialWholeDiag[curCoords.row][curCoords.col] = bgcolor;
        potentialWholeDiag[potentialCoordsDiag.row][potentialCoordsDiag.col] = color;
        if (isOpen(potentialCoordsDiag, whole, wholecoord) && contiguous(potentialCoordsDiag, potentialWholeDiag) == true) {
          if(newCoords.row != curCoords.row && newCoords.col != curCoords.col) {
            if(inCorridor(curCoords, whole)) {
              if (betterCorridorExit(cwTrueCcwFalse, newCoordsDirection, clockDirectionDiag, color)) {
                newCoords = potentialCoordsDiag; // TODO copy ctor?
                newCoordsDirection = clockDirectionDiag;
              }
            }
          }
          else {
            newCoords = potentialCoordsDiag; // TODO copy ctor?
            newCoordsDirection = clockDirectionDiag;
          }
        }
      }
    }
  }

  whole[curCoords.row][curCoords.col] = bgcolor;
  whole[newCoords.row][newCoords.col] = color;

  if (color == red) {
    if (!redInCorridor && inCorridor(red, whole)) {
      corridorClockRedCw = cwTrueCcwFalse ? oppositeClock(newCoordsDirection) : newCoordsDirection;
      corridorClockRedCcw = oppositeClock(corridorClockRedCw);
    }
  }
  if (color == orange) {
    if (!orangeInCorridor && inCorridor(orange, whole)) {
      corridorClockOrangeCw = cwTrueCcwFalse ? oppositeClock(newCoordsDirection) : newCoordsDirection;
      corridorClockOrangeCcw = oppositeClock(corridorClockOrangeCw);
    }
  }
  if (color == yellow) {
    if (!yellowInCorridor && inCorridor(yellow, whole)) {
      corridorClockYellowCw = cwTrueCcwFalse ? oppositeClock(newCoordsDirection) : newCoordsDirection;
      corridorClockYellowCcw = oppositeClock(corridorClockYellowCw);
    }
  }
  if (color == green) {
    if (!greenInCorridor && inCorridor(green, whole)) {
      corridorClockGreenCw = cwTrueCcwFalse ? oppositeClock(newCoordsDirection) : newCoordsDirection;
      corridorClockGreenCcw = oppositeClock(corridorClockGreenCw);
    }
  }
  if (color == blue) {
    if (!blueInCorridor && inCorridor(blue, whole)) {
      corridorClockBlueCw = cwTrueCcwFalse ? oppositeClock(newCoordsDirection) : newCoordsDirection;
      corridorClockBlueCcw = oppositeClock(corridorClockBlueCw);
    }
  }

// TODO jminer the following if statement might be old
  if (newCoordsDirection > -1) {
    if (prevDirectionMain[colorIndex(color)] > -1)
      prevDirectionSecondary[colorIndex(color)] = prevDirectionMain[colorIndex(color)];
    prevDirectionMain[colorIndex(color)] = newCoordsDirection;
  }
}

int clockwise(bool cw, int clockDirection) {
  for (int i = 0; i < clockDirectionsLength; ++i) {
    if (clockDirections[i] == clockDirection) {
      if (cw == true)
        return clockDirections[(i + 1) % clockDirectionsLength];
      else {
        if ((i - 1) == -1)
          return clockDirections[clockDirectionsLength - 1];
        else
          return clockDirections[i - 1];
      }
    }
  }
  
  return -1;
}

void cloneWhole(Uint32 target[][whole0Length], Uint32 dest[][whole0Length]) {
  for(int i = 0; i < wholeLength; ++i) {
    for(int j = 0; j < whole0Length; ++j) {
      dest[i][j] = target[i][j];
    }
  }
}

int colorIndex(Uint32 color) {
    if(color == red)
      return 0;
    if(color == orange)
      return 1;
    if(color == yellow)
      return 2;
    if(color == green)
      return 3;
    if(color == blue)
      return 4;
   return 5;
}

void colorWhole() {
  int colorindex = 0;
  int numViolet = rand() % 3 + 2;
  Coord r1 = randomColorMe(whole);
  Coord r2 = randomColorMe(whole);
  whole[r1.row][r1.col] = violet1;
  whole[r2.row][r2.col] = violet2;
  if(numViolet > 2) {
    Coord r3 = randomColorMe(whole);
    whole[r3.row][r3.col] = violet3;
  }
  if(numViolet > 3) {
    Coord r4 = randomColorMe(whole);
    whole[r4.row][r4.col] = violet4;
  }
  for(int i = 0; i < wholeLength; ++i) {
    for(int j = 0; j < whole0Length; ++j) {
      if (whole[i][j] == colorme) {
        whole[i][j] = blockcolors[colorindex++];
      }
    }
  }
}

void colorsInCorridor(Uint32 potentialWhole[][whole0Length], bool &r, bool &o, bool &y, bool &g, bool &b) {
  Coord redCoords = getCoordsPotentialWhole(red, potentialWhole); 
  Coord orangeCoords = getCoordsPotentialWhole(orange, potentialWhole); 
  Coord yellowCoords = getCoordsPotentialWhole(yellow, potentialWhole);
  Coord greenCoords= getCoordsPotentialWhole(green, potentialWhole); 
  Coord blueCoords = getCoordsPotentialWhole(blue, potentialWhole);
 
  r = inCorridor(redCoords, potentialWhole);
  o = inCorridor(orangeCoords, potentialWhole);
  y = inCorridor(yellowCoords, potentialWhole);
  g = inCorridor(greenCoords, potentialWhole);
  b = inCorridor(blueCoords, potentialWhole);
}

// TODO this is specialized for an 7 piece whole
// TODO is this right?
bool contiguous(const Coord &startingCoord, Uint32 potential[][whole0Length]) {
  list<int> l;
  set<int> s;
  l.push_front(startingCoord.row * wholeLength + startingCoord.col);
  do {
    int m=l.front();
    l.pop_front();
    s.insert(m);
    int x=m/wholeLength; // This is actually y
    int y=m%wholeLength; // This is actually x
    if(x-1>=0&&potential[x-1][y]!=bgcolor&&s.count(m-wholeLength)==0)
      l.push_front(m-wholeLength);
    if(x+1<wholeLength&&potential[x+1][y]!=bgcolor&&s.count(m+wholeLength)==0)
      l.push_front(m+wholeLength);
    if(y-1>=0&&potential[x][y-1]!=bgcolor&&s.count(m-1)==0)
      l.push_front(m-1);
    if(y+1<wholeLength&&potential[x][y+1]!=bgcolor&&s.count(m+1)==0)
      l.push_front(m+1);
  } while(l.size()>0);

  return s.size()==7; // TODO hardcoded
}

void corridorClocks(Uint32 potentialWhole[][whole0Length]) {
  bool r, o, y, g, b;
  colorsInCorridor(potentialWhole, r, o, y, g, b);
  if ((redInCorridor && !r) || (!redInCorridor && r))
    corridorClocksColor(potentialWhole, corridorClockRedCw, corridorClockRedCcw);
  if ((orangeInCorridor && !o) || (!orangeInCorridor && o))
    corridorClocksColor(potentialWhole, corridorClockOrangeCw, corridorClockOrangeCcw);
  if ((yellowInCorridor && !y) || (!yellowInCorridor && y))
    corridorClocksColor(potentialWhole, corridorClockYellowCw, corridorClockYellowCcw);
  if ((greenInCorridor && !g) || (!greenInCorridor && g))
    corridorClocksColor(potentialWhole, corridorClockGreenCw, corridorClockGreenCcw);
  if ((blueInCorridor && !b) || (!blueInCorridor && b))
    corridorClocksColor(potentialWhole, corridorClockBlueCw, corridorClockBlueCcw);
  redInCorridor = r;
  orangeInCorridor = o;
  yellowInCorridor = y;
  greenInCorridor = g;
  blueInCorridor = b;
}

void corridorClocksColor(Uint32 potentialWhole[][whole0Length], int &a, int &b) {
  int tmp;
  if (a == -1) {
    a = 1;
    b = 7;
  }
  else {
    tmp = a;
    a = b;
    b = tmp;
  }
}

vector<Coord> getAdjOccupied(Coord coords) {
  vector<Coord> ret;
  
  if (isOpen( { .row = coords.row - 1, .col = coords.col }, whole, wholecoord) == false)
    ret.push_back( { .row = coords.row - 1, .col = coords.col} );
  if (isOpen( { .row = coords.row + 1, .col = coords.col }, whole, wholecoord) == false)
    ret.push_back( { .row = coords.row + 1, .col = coords.col} );
  if (isOpen( { .row = coords.row, .col = coords.col - 1}, whole, wholecoord) == false)
    ret.push_back( { .row = coords.row, .col = coords.col - 1} );
  if (isOpen( { .row = coords.row, .col = coords.col + 1}, whole, wholecoord) == false)
    ret.push_back( { .row = coords.row, .col = coords.col + 1} );
  return ret;
}

Coord getCenterCoords(Uint32 target[][whole0Length]) {
  Bounds b = bounds(target);
  return { .row = (int) ((b.bottom - b.top) / 2 + b.top), .col = (int) ((b.right - b.left) / 2 + b.left) };
}

int getClockDirection(const Coord &center, const Coord &d) {
  // Assumes center and d are adj.
  if (d.row == center.row - 1) {
    if (d.col == center.col - 1)
      return 11;
    if (d.col == center.col)
      return 12;
    if (d.col == center.col + 1)
      return 1;
  }
  if (d.row == center.row) {
    if (d.col == center.col - 1)
      return 9;
    if (d.col == center.col)
      return 0;
    if (d.col == center.col + 1)
      return 3;
  }
  if (d.row == center.row + 1) {
    if (d.col == center.col - 1)
      return 7;
    if (d.col == center.col)
      return 6;
    if (d.col == center.col + 1)
      return 5;
  }

  return -1;
}

Coord getCoords(Uint32 color) {
  return getCoordsPotentialWhole(color, whole);
}

Coord getCoordsPotentialWhole(Uint32 color, Uint32 potentialWhole[][whole0Length]) {
  for(int i = 0; i < wholeLength; ++i) {
    for(int j = 0; j < whole0Length; ++j) {
      if(potentialWhole[i][j] == color)
        return { .row = i, .col = j };
    }
  }

  //TODO jminer
  return { .row = -1, .col = -1 };
}

Coord getPotentialCoords(const Coord & coords, int clockDirection) {
  int y = coords.row;
  int x = coords.col;

  if (clockDirection == 12)
    y -= 1;
  if (clockDirection == 1) {
    y -= 1;
    x += 1;
  }
  if (clockDirection == 3)
    x += 1;
  if (clockDirection == 5) {
    y += 1;
    x += 1;
  }
  if (clockDirection == 6)
    y += 1;
  if (clockDirection == 7) {
    y += 1;
    x -= 1;
  }
  if (clockDirection == 9)
    x -= 1;
  if (clockDirection == 11) {
    y -= 1;
    x -= 1;
  }

  if (y < 0 || y >= wholeLength)
    y = -1;

  if (x < 0 || x >= whole0Length)
    x = -1;

  return { .row = y, .col = x };
}

bool inCorridor(Uint32 color, Uint32 whole[][whole0Length]) {
  return inCorridor(getCoords(color), whole);
}

bool inCorridor(const Coord &coords, Uint32 whole[][whole0Length]) {
  Coord coordsA = { .row = coords.row, .col = coords.col - 1 };
  Coord coordsB = { .row = coords.row, .col = coords.col + 1 };
  Coord coordsC = { .row = coords.row - 1, .col = coords.col };
  Coord coordsD = { .row = coords.row + 1, .col = coords.col, };
  if(isOpen(coordsA, whole, wholecoord) && isOpen(coordsB, whole, wholecoord)
    && !isOpen(coordsC, whole, wholecoord) && !isOpen(coordsD, whole, wholecoord))
    return true;
  if(isOpen(coordsC, whole, wholecoord) && isOpen(coordsD, whole, wholecoord)
    && !isOpen(coordsA, whole, wholecoord) && !isOpen(coordsB, whole, wholecoord))
    return true;
// TODO does isOpen check all of adjacent blocks, concreted blocks, and boundaries?
  return false;
}

bool inMoreSimilarDirectionAsPrev(Uint32 defendingChampionDirection, Uint32 contenderDirection, int prevDirectionMain,
// TODO jminer get rid of this function
  int prevDirectionSecondary) {
  if (prevDirectionMain < 0)
    return true;
  if (contenderDirection == prevDirectionMain 
    || contenderDirection == clockwise(true, prevDirectionMain)
    || contenderDirection == clockwise(false, prevDirectionMain))
    return true;
  if (defendingChampionDirection == prevDirectionMain 
    || defendingChampionDirection == clockwise(true, prevDirectionMain)
    || defendingChampionDirection == clockwise(false, prevDirectionMain))
    return false;
  if (prevDirectionSecondary < 0)
    return true;
  if (contenderDirection == prevDirectionSecondary
    || contenderDirection == clockwise(true, prevDirectionSecondary)
    || contenderDirection == clockwise(false, prevDirectionSecondary))
    return true;
  return false;
}

bool isOpen(const Coord & piececoords, Uint32 potentialWhole[][whole0Length], const Coord & wholecoord) {
  if (piececoords.col < 0 || piececoords.row < 0)
    return false;
  if (piececoords.col + wholecoord.col < 0)
    return false;
  if (piececoords.row + wholecoord.row < 0)
    return false;
  if (piececoords.col + wholecoord.col >= board0Length)
    return false;
  if (piececoords.row + wholecoord.row >= boardLength)
    return false;

  if (board[piececoords.row + wholecoord.row][piececoords.col + wholecoord.col])
    return false;

  return potentialWhole[piececoords.row][piececoords.col] == bgcolor;
}

void keyboard() {
  SDL_PumpEvents();
  const Uint8 *state =
    SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_ESCAPE])
    exit(0);
  if(state[SDL_SCANCODE_1])
    keystate |= 0x1;
  if(state[SDL_SCANCODE_Q])
    keystate |= 0x2;
  if((!state[SDL_SCANCODE_1] && !state[SDL_SCANCODE_Q])
    || (state[SDL_SCANCODE_1] && state[SDL_SCANCODE_Q])) {
    keystate &= ~0x1;
    keystate &= ~0x2;
  }
  if(state[SDL_SCANCODE_2])
    keystate |= 0x4;
  if(state[SDL_SCANCODE_W])
    keystate |= 0x8;
  if((!state[SDL_SCANCODE_2] && !state[SDL_SCANCODE_W])
    || (state[SDL_SCANCODE_2] && state[SDL_SCANCODE_W])) {
    keystate &= ~0x4;
    keystate &= ~0x8;
  }
  if(state[SDL_SCANCODE_3])
    keystate |= 0x10;
  if(state[SDL_SCANCODE_E])
    keystate |= 0x20;
  if((!state[SDL_SCANCODE_3] && !state[SDL_SCANCODE_E])
    || (state[SDL_SCANCODE_3] && state[SDL_SCANCODE_E])) {
    keystate &= ~0x10;
    keystate &= ~0x20;
  }
  if(state[SDL_SCANCODE_4])
    keystate |= 0x40;
  if(state[SDL_SCANCODE_R])
    keystate |= 0x80;
  if((!state[SDL_SCANCODE_4] && !state[SDL_SCANCODE_R])
    || (state[SDL_SCANCODE_4] && state[SDL_SCANCODE_R])) {
    keystate &= ~0x40;
    keystate &= ~0x80;
  }
  if(state[SDL_SCANCODE_5])
    keystate |= 0x100;
  if(state[SDL_SCANCODE_T])
    keystate |= 0x200;
  if((!state[SDL_SCANCODE_5] && !state[SDL_SCANCODE_T])
    || (state[SDL_SCANCODE_5] && state[SDL_SCANCODE_T])) {
    keystate &= ~0x100;
    keystate &= ~0x200;
  }
  if(state[SDL_SCANCODE_6])
    keystate |= 0x400;
  if(state[SDL_SCANCODE_Y])
    keystate |= 0x800;
  if((!state[SDL_SCANCODE_6] && !state[SDL_SCANCODE_Y])
    || (state[SDL_SCANCODE_6] && state[SDL_SCANCODE_Y])) {
    keystate &= ~0x400;
    keystate &= ~0x800;
  }
  if(state[SDL_SCANCODE_6])
    keystate |= 0x1000;
  if(state[SDL_SCANCODE_Y])
    keystate |= 0x2000;
  if((!state[SDL_SCANCODE_6] && !state[SDL_SCANCODE_Y])
    || (state[SDL_SCANCODE_6] && state[SDL_SCANCODE_Y])) {
    keystate &= ~0x1000;
    keystate &= ~0x2000;
  }
  if(state[SDL_SCANCODE_UP])
    keystate |= 0x4000;
  else
    keystate &= ~0x4000;
  if(state[SDL_SCANCODE_DOWN])
    keystate |= 0x8000;
  else
    keystate &= ~0x8000;
  if(state[SDL_SCANCODE_LEFT])
    keystate |= 0x10000;
  else
    keystate &= ~0x10000;
  if(state[SDL_SCANCODE_RIGHT])
    keystate |= 0x20000;
  else
    keystate &= ~0x20000;
  if(state[SDL_SCANCODE_SPACE])
    keystate |= 0x40000;
  else
    keystate &= ~0x40000;
}

void mirrorWholeHoriz(Uint32 target[][whole0Length]) {
  for (int i = 0; i < wholeLength; ++i) {
    for (int j = 0; j < whole0Length / 2; ++j) {
      Uint32 tmp = target[i][j];
      target[i][j] = target[i][whole0Length - 1 - j];
      target[i][whole0Length - 1 - j] = tmp;
    }
  }
}

void newBoard() {
  for (int j = 0; j < board0Length; ++j) {
    int r = rand() % 7;
    int i = 0;
    for (i = 0; i < boardLength; ++i) {
      if (i >= boardLength - r)
        board[i][j] = true;
      else
        board[i][j] = false;
    }
  }
}

void newWhole(Uint32 newWholeX[][whole0Length]) {
  int r = randomRange(0, 999);
  string s = sample7.substr(r * 49, 49); //TODO hardcoded
  for (int i = 0; i < wholeLength; ++i) {
    for (int j = 0; j < whole0Length; ++j) {
      newWholeX[i][j] = bgcolor;
    }
  }
  for (int i = 0; i < 7; ++i) { //TODO hardcoded
    for(int j = 0; j < 7; ++j) {
      if (s.at(i * 7 + j) == '1')
        newWholeX[i][j] = colorme;
    }
  }
  for (int i = 0; i < 5; ++i) {
    prevDirectionMain[i] = -1;
    prevDirectionSecondary[i] = -1;
  }

  centerWhole(newWholeX, true);
  corridorClockRedCw = -1;
  corridorClockOrangeCw = -1;
  corridorClockYellowCw = -1;
  corridorClockGreenCw = -1;
  corridorClockBlueCw = -1;
  corridorClockRedCcw = -1;
  corridorClockOrangeCcw = -1;
  corridorClockYellowCcw = -1;
  corridorClockGreenCcw = -1;
  corridorClockBlueCcw = -1;
  colorsInCorridor(newWholeX, redInCorridor, orangeInCorridor, yellowInCorridor, greenInCorridor, blueInCorridor);
}

int oppositeClock(int clockDirection) {
  if (clockDirection == 12)
    return 6;
  if (clockDirection == 1)
    return 7;
  if (clockDirection == 2)
    return 8;
  if (clockDirection == 3)
    return 9;
  if (clockDirection == 4)
    return 10;
  if (clockDirection == 5)
    return 11;
  if (clockDirection == 6)
    return 12;
  if (clockDirection == 7)
    return 1;
  if (clockDirection == 8)
    return 2;
  if (clockDirection == 9)
    return 3;
  if (clockDirection == 10)
    return 4;
  return 5;
}

void proc(Uint32 interval) {
  centerWhole(whole, true);
  if((keystate & 0x1) == 0x1 && (((keystateprev & 0x1) != 0x1) || moveTimerRed < 0)) {
    moveTimerRed = moveTimeout;
    clockMove(red, true);
  }
  if((keystate & 0x2) == 0x2 && (((keystateprev & 0x2) != 0x2) || moveTimerRed < 0)) {
    moveTimerRed = moveTimeout;
    clockMove(red, false);
  }
  if((keystate & 0x4) == 0x4 && (((keystateprev & 0x4) != 0x4) || moveTimerOrange < 0)) {
    moveTimerOrange = moveTimeout;
    clockMove(orange, true);
  }
  if((keystate & 0x8) == 0x8 && (((keystateprev & 0x8) != 0x8) || moveTimerOrange < 0)) {
    moveTimerOrange = moveTimeout;
    clockMove(orange, false);
  }
  if((keystate & 0x10) == 0x10 && (((keystateprev & 0x10) != 0x10) || moveTimerYellow < 0)) {
    moveTimerYellow = moveTimeout;
    clockMove(yellow, true);
  }
  if((keystate & 0x20) == 0x20 && (((keystateprev & 0x20) != 0x20) || moveTimerYellow < 0)) {
    moveTimerYellow = moveTimeout;
    clockMove(yellow, false);
  }
  if((keystate & 0x40) == 0x40 && (((keystateprev & 0x40) != 0x40) || moveTimerGreen < 0)) {
    moveTimerGreen = moveTimeout;
    clockMove(green, true);
  }
  if((keystate & 0x80) == 0x80 && (((keystateprev & 0x80) != 0x80) || moveTimerGreen < 0)) {
    moveTimerGreen = moveTimeout;
    clockMove(green, false);
  }
  if((keystate & 0x100) == 0x100 && (((keystateprev & 0x100) != 0x100) || moveTimerBlue < 0)) {
    moveTimerBlue = moveTimeout;
    clockMove(blue, true);
  }
  if((keystate & 0x200) == 0x200 && (((keystateprev & 0x200) != 0x200) || moveTimerBlue < 0)) {
    moveTimerBlue = moveTimeout;
    clockMove(blue, false);
  }
  if((keystate & 0x400) == 0x400 && (((keystateprev & 0x400) != 0x400) || moveTimerWhole < 0)) {
    moveTimerWhole = moveTimeout;
    rotateWhole(true);
  }
  if((keystate & 0x800) == 0x800 && (((keystateprev & 0x800) != 0x800) || moveTimerWhole < 0)) {
    moveTimerWhole = moveTimeout;
    rotateWhole(false);
  }

  if((keystate & 0x20000) == 0x20000 && (((keystateprev & 0x20000) != 0x20000) || moveTimerRight < 0)) {
    moveTimerRight = moveTimeout;
    ++wholecoord.col;
    for (int i = 0; i < wholeLength; ++i) {
      for (int j = 0; j < whole0Length; ++j) {
        if (whole[i][j] != bgcolor) {
          Uint32 color = whole[i][j];
          whole[i][j] = bgcolor;
          if(isOpen({ .row = i, .col = j}, whole, wholecoord) == false)
            --wholecoord.col;
          whole[i][j] = color;
        }
      }
    }
  }
  if((keystate & 0x10000) == 0x10000 && (((keystateprev & 0x10000) != 0x10000) || moveTimerLeft < 0)) {
    moveTimerLeft = moveTimeout;
    --wholecoord.col;
    for (int i = 0; i < wholeLength; ++i) {
      for (int j = 0; j < whole0Length; ++j) {
        if (whole[i][j] != bgcolor) {
          Uint32 color = whole[i][j];
          whole[i][j] = bgcolor;
          if(isOpen({ .row = i, .col = j}, whole, wholecoord) == false)
            ++wholecoord.col;
          whole[i][j] = color;
        }
      }
    }
  }
  bool apply = false;
  if((keystate & 0x4000) == 0x4000 && (((keystateprev & 0x4000) != 0x4000))) {
    gravityTimer = gravityTimeout;
    bool hit = false;
    while (hit == false) {
      ++wholecoord.row;
      for (int i = 0; i < wholeLength; ++i) {
        for (int j = 0; j < whole0Length; ++j) {
          if (whole[i][j] != bgcolor) {
            Uint32 color = whole[i][j];
            whole[i][j] = bgcolor;
            if(isOpen({ .row = i, .col = j}, whole, wholecoord) == false) {
              --wholecoord.row;
              hit = true;
              apply = true;
            }
            whole[i][j] = color;
          }
        }
      }
    }
  }
  else if((keystate & 0x8000) == 0x8000 && (((keystateprev & 0x8000) != 0x8000) || moveTimerDown < 0)) {
    moveTimerDown = moveTimeout;
    gravityTimer = gravityTimeout;
    ++wholecoord.row;
    for (int i = 0; i < wholeLength; ++i) {
      for (int j = 0; j < whole0Length; ++j) {
        if (whole[i][j] != bgcolor) {
          Uint32 color = whole[i][j];
          whole[i][j] = bgcolor;
          if(isOpen({ .row = i, .col = j}, whole, wholecoord) == false) {
            --wholecoord.row;
            apply = true;
          }
          whole[i][j] = color;
        }
      }
    }
  }
  else {
    gravityTimer -= interval;
    if (gravityTimer < 0) {
      gravityTimer += gravityTimeout;
      ++wholecoord.row;
      for (int i = 0; i < wholeLength; ++i) {
        for (int j = 0; j < whole0Length; ++j) {
          if (whole[i][j] != bgcolor) {
            Uint32 color = whole[i][j];
            whole[i][j] = bgcolor;
            if(isOpen({ .row = i, .col = j}, whole, wholecoord) == false) {
              --wholecoord.row;
              apply = true;
            }
            whole[i][j] = color;
          }
        }
      }
    }
  }
  if (apply == true)
    applyWhole();
  else if((keystate & 0x40000) == 0x40000 && (((keystateprev & 0x40000) == 0x40000))) {
    capitalizeRows();
  }
}

Coord randomColorMe(Uint32 potentialWhole[][whole0Length]) {
  vector<Coord> colorMes;
  for (int i = 0; i < wholeLength; ++i) {
    for (int j = 0; j < whole0Length; ++j) {
      if (potentialWhole[i][j] == colorme) {
        colorMes.push_back( { .row = i, .col = j } );
      }
    }
  }
  return colorMes[randomRange(0, colorMes.size() - 1)];
}

int randomRange(int min, int max) {
  return rand() % (max - min) + min + 1;
}

void renderBoard() {
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderClear(renderer);
  boxColor(renderer, boardCoord.col, boardCoord.row, boardCoord.col + board0Length * blockwidth, boardCoord.row + boardLength * blockwidth, bgcolor);
  for(int i = 0; i < boardLength; ++i) {
    for(int j = 0; j < board0Length; ++j) {
      //ctx.fillStyle = board[i][j];
      //ctx.fillRect(j * blockwidth, i * blockwidth, blockwidth - 1, blockwidth - 1);
    }
  }
  Coord shadowcoord = shadow(whole, wholecoord);
  for(int i = 0; i < wholeLength; ++i) {
    for(int j = 0; j < whole0Length; ++j) {
      if(arrayHas(blockcolors, 9, whole[i][j])){ //TODO hardcoded
        int boxX = boardCoord.col + (wholecoord.col + j) * blockwidth;
        int boxY = boardCoord.row + (wholecoord.row + i) * blockwidth;
        boxColor(renderer, boxX + 2, boxY + 2, boxX + blockwidth - 1, boxY + blockwidth - 1, whole[i][j]);
        int shadowX = boardCoord.col + (shadowcoord.col + j) * blockwidth;
        int shadowY = boardCoord.row + (shadowcoord.row + i) * blockwidth;
        Uint32 shadowcolor = (whole[i][j] & 0x00ffffff) | 0xa0000000; //TODO endian bug
        boxColor(renderer, shadowX + 2, shadowY + 2, shadowX + blockwidth - 1, shadowY + blockwidth - 1, shadowcolor);
      }
    }
  }
  bool full = true;
  for(int i = 0; i < boardLength; ++i) {
    full = true;
    for (int j = 0; j < board0Length; ++j) {
      if (board[i][j] == false)
        full = false;
    }
    if (full == true) {
      for(int j = 0; j < board0Length; ++j) {
        if(board[i][j]) {
          int boxX = boardCoord.col + j * blockwidth;
          int boxY = boardCoord.row + i * blockwidth;
          boxColor(renderer, boxX, boxY, boxX + blockwidth - 1, boxY + blockwidth - 1, blockcolors[j % 5]); //TODO hardcoded
        }
      }
    }
    else {
      for(int j = 0; j < board0Length; ++j) {
        if(board[i][j]) {
          int boxX = boardCoord.col + j * blockwidth;
          int boxY = boardCoord.row + i * blockwidth;
          boxColor(renderer, boxX + 2, boxY + 2, boxX + blockwidth - 1, boxY + blockwidth - 1, gray1);
        }
      }
    }
  }
}

void renderUi() {
}

void rotateWhole(bool cwTrueCcwFalse) {
  Uint32  potentialWhole[wholeLength][whole0Length];
  Uint32 emptyWhole[wholeLength][whole0Length];
  
  for (int i = 0; i < wholeLength; ++i) {
    for (int j = 0; j < whole0Length; ++j) {
      potentialWhole[i][j] = bgcolor;
      emptyWhole[i][j] = bgcolor;
    }
  }

  for (int i = 0; i < wholeLength; ++i) {
    for (int j = 0; j < whole0Length; ++j) {
      if (cwTrueCcwFalse == true)
        potentialWhole[i][j] = whole[j][i];
      else {
        potentialWhole[wholeLength - 1 - i][j] = whole[j][i];
      }
    }
  }

  if (cwTrueCcwFalse == true)
    mirrorWholeHoriz(potentialWhole);

  bool open = true;

  for (int i = 0; i < wholeLength; ++i) {
    for (int j = 0; j < whole0Length; ++j) {
      if (potentialWhole[i][j] != bgcolor
        && isOpen( { .row = i, .col = j }, emptyWhole, wholecoord) == false)
        open = false;
    }
  }
  
  if (open == true) {
    for (int i = 0; i < wholeLength; ++i) {
      for (int j = 0; j < whole0Length; ++j) {
        whole[i][j] = potentialWhole[i][j];
      }
    }

    centerWhole(whole, true);
  }
}

Coord shadow(Uint32 whole[][whole0Length], const Coord &coords) {
  Uint32 clone[wholeLength][whole0Length];
  cloneWhole(whole, clone);
  Coord ret = { .row = coords.row, .col = coords.col };
  bool loop = true;
  while(loop == true) {
    ret.row++;
    for (int i = 0; i < wholeLength; ++i) {
      for (int j = 0; j < whole0Length; ++j) {
        if (clone[i][j] != bgcolor && (board[i + ret.row][j + ret.col] == true || (i + ret.row) >= boardLength)) {
          ret.row--;
          loop = false;
        }
      }
    }
  } 
  return ret;
}

void shiftWhole(Uint32 target[][whole0Length], int clockDirection, bool translate) {
  if (clockDirection == 12) {
    for (int i = 0; i < wholeLength; ++i) {
      for (int j = 0; j < whole0Length; ++j) {
        if (i + 1 < wholeLength) {
          target[i][j] = target[i+1][j];
        }
        else {
          target[i][j] = bgcolor;
        }
      }
    }
    if (translate)
      wholecoord.row++;
  }
  if (clockDirection == 6) {
    for (int i = wholeLength- 1; i >= 0; --i) {
      for (int j = 0; j < whole0Length; ++j) {
        if (i - 1 >= 0) {
          target[i][j] = target[i-1][j];
        }
        else {
          target[i][j] = bgcolor;
        }
      }
    }
    if (translate)
      wholecoord.row--;
  }
  if (clockDirection == 3) {
    for (int i = 0; i < wholeLength; ++i) {
      for (int j = whole0Length - 1; j >= 0; --j) {
        if (j - 1 >= 0) {
          target[i][j] = target[i][j - 1];
        }
        else {
          target[i][j] = bgcolor;
        }
      }
    }
    if (translate)
      wholecoord.col--;
  }
  if (clockDirection == 9) {
    for (int i = 0; i < wholeLength; ++i) {
      for (int j = 0; j < whole0Length; ++j) {
        if (j + 1 < whole0Length) {
          target[i][j] = target[i][j + 1];
        }
        else {
          target[i][j] = bgcolor;
        }
      }
    }
    if (translate)
      wholecoord.col++;
  }
}
Uint32 update(Uint32 interval, void *param) {
  moveTimerRed -= interval;
  moveTimerOrange -= interval;
  moveTimerYellow -= interval;
  moveTimerGreen -= interval;
  moveTimerBlue -= interval;
  moveTimerDown -= interval;
  moveTimerLeft -= interval;
  moveTimerRight -= interval;
  proc(interval);
  renderTimer -= interval;
  if (renderTimer < 0) {
    renderTimer = renderTimeout;
    renderBoard();
    renderUi();
    SDL_RenderPresent(renderer);
  }
  param = param; // To suppress warning;
  keystateprev = keystate;
  return interval;
}

int main(int argc, char **argv) {
// TODO wiki.libsdl.org create window "you must set the nshighresolutioncapable info.plist property to YES" on mac.
  argc = argc; // To suppress warning
  argv = argv; // To suppress warning
  srand(time_t(time(0)));
  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  screen = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
  // TODO if (screen == null)
  renderer = SDL_CreateRenderer(screen, -1, 0);
  // TODO if (renderer == null)
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(renderer, 1824, 984);
  boardCoord.col = 1824 / 2 - board0Length * blockwidth / 2;
  boardCoord.row = 984 / 2 - boardLength * blockwidth / 2;
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
    bgcolor = SDL_Swap32(bgcolor);
    red = SDL_Swap32(red);
    orange = SDL_Swap32(orange);
    yellow = SDL_Swap32(yellow);
    green = SDL_Swap32(green);
    blue = SDL_Swap32(blue);
    violet1 = SDL_Swap32(violet1);
    violet2 = SDL_Swap32(violet2);
    violet3 = SDL_Swap32(violet3);
    violet4 = SDL_Swap32(violet4);
    // TODO ALL OTHER COLORS
  }
  else if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
  }
  else
    cout << "Could not determine endianness" << endl;

// TODO destroy window release renderer.
// Here's orig main.
// TODO Assert whole length and width.
  newBoard();
  newWhole(whole);
  colorWhole();
  SDL_AddTimer(updateInterval, update, &noParam);
  while(true)
    keyboard();
}
