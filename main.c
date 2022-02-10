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
#define _POSIX_C_SOURCE 199309L
#ifdef PLATFORM_PC
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> /* TODO remove */
#include <SDL2/SDL_image.h>
#endif /* PLATFORM_PC */
#ifdef PLATFORM_DC
typedef unsigned int Uint32; /*TODO For now*/
#include <arch/timer.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <unistd.h>
#ifdef PLATFORM_DC
#include <stdio.h>
#include <dc/maple.h>
#include <dc/maple/controller.h>
#include <dc/pvr.h>
#endif

typedef struct Bounds Bounds;
struct Bounds {
  int left;
  int right;
  int top;
  int bottom;
};

typedef struct Coord Coord;
struct Coord {
  int row;
  int col;
};

#define bgcolor 0xffdddddd
#ifdef PLATFORM_PC
const int blockwidth = 25;
#endif /* PLATFORM_PC */
#ifdef PLATFORM_DC
const int blockwidth = 13;
#endif /* PLATFORM_DC */
#ifdef PLATFORM_PC
#define blue 0xffffca67
SDL_Texture *blueTexture;
#endif
#ifdef PLATFORM_DC
Uint32 blue = 0xff579eff;
#endif
int blueInCorridor = 0;
Coord boardCoord = { 0, 0 };
#ifdef PLATFORM_PC
#define boardLength 34
#define board0Length 19
#endif /* PLATFORM_PC */
int board[boardLength][board0Length]; /*TODO*/
const int buttonWidth = 25;
#define clockDirectionsLength 8
const int clockDirections[clockDirectionsLength] = {
  12, 1, 3, 5, 6, 7, 9, 11
};
Uint32 colorme = 0x00000100;
Uint32 controllerstate = 0;
Uint32 controllerstateprev = 0;
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
const int gravityTimeout = 2750;
int gravityTimer = gravityTimeout;
Uint32 gray1 = 0xfffd66c0; /* This is violet, now, not gray, make sure not same value as other violets for game mechanics */
Uint32 gray2 = 0xfffd66c1; /* gray1 + 1 r. */
#ifdef PLATFORM_PC
#define green 0xff67ffb3
SDL_Texture *greenTexture;
#endif
#ifdef PLATFORM_DC
Uint32 green = 0xff76ff67;
#endif
int greenInCorridor = 0;
Uint32 keystate = 0;
Uint32 keystateprev = 0;
const int moveTimeout = 144;
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
int orangeComboButtonDown = 0;
#ifdef PLATFORM_PC
#define orange 0xff67bbff
SDL_Texture *orangeTexture;
#endif
#ifdef PLATFORM_DC
Uint32 orange = 0xffff9157;
#endif
int orangeInCorridor = 0;
#ifdef PLATFORM_PC
#define red 0xff6767ff 
SDL_Texture *redTexture;
#endif /* PLATFORM_PC */
#ifdef PLATFORM_DC
Uint32 red = 0xffff5757;
#endif /* PLATFORM_DC */
#ifdef PLATFORM_PC
SDL_Renderer *renderer;
#endif
#define renderTimeout 12 
int renderTimer = renderTimeout;
char *sampleAddr;
struct stat sampleSb;
int sampleFd;
off_t sampleLength = 49;
off_t sampleOffset;
off_t samplePaOffset;
#ifdef PLATFORM_PC
SDL_Window *screen;
#endif
Uint32 updateInterval = 12;
Coord wholecoord = { 0, 0 };
#define wholeLength 13
#define whole0Length 13
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
#ifdef PLATFORM_PC
#define violet1 0xfffd66c2
#define violet2 0xfffd66c3
#define violet3 0xffff66c4
#define violet4 0xffff66c5
#define violet5 0xffff66c6
#endif
#ifdef PLATFORM_DC
Uint32 violet1 = 0xff8457ff;
Uint32 violet2 = 0xff8457fe;
Uint32 violet3 = 0xff8457fd;
Uint32 violet4 = 0xff8457fc;
Uint32 violet5 = 0xff8457fb;
#endif
#ifdef PLATFORM_PC
#define yellow 0xff67fcff
SDL_Texture *yellowTexture;
#endif
#ifdef PLATFORM_DC
Uint32 yellow = 0xffffcb57;
#endif
int yellowInCorridor = 0;
Uint32 blockcolors[10] = { red, orange, yellow, green, blue, violet1, violet2, violet3, violet4, violet5 };

int absoluteValue(int a);
void applyWhole();
int arrayHas1(int a[], int size, int search);
int arrayHas2(Uint32 ar[], int size, Uint32 element);
void atExitQuit();
int betterCorridorExit(int cwTrueCcwFalse, int directionA, int directionB, Uint32 color);
void bounds(Uint32 target[][whole0Length], Bounds *ret);
#ifdef PLATFORM_DC
void boxColor(...);
#endif /* PLATFORM_DC */
void capitalizeRows();
void centerWhole(Uint32 target[][whole0Length], int translate);
void clockMove(Uint32 color, int cwTrueCcwFalse);
int clockwise(int cw, int clockDirection);
void cloneWhole(Uint32 target[][whole0Length], Uint32 dest[][whole0Length]);
int colorIndex(Uint32 color);
void colorWhole();
void colorsInCorridor(Uint32 potentialWhole[][whole0Length], const Coord *potentialWholeCoords,
  int *r, int *o, int *y, int *g, int *b);
int contiguous(const Coord *startingCoord, Uint32 potential[][whole0Length]);
void controller();
void corridorClocks1(Uint32 beforeWhole[][whole0Length], Uint32 afterWhole[][whole0Length],
  const Coord *beforeWholeCoords, const Coord *afterWholeCoords);
void corridorClocks2(Uint32 beforeWhole[][whole0Length], Uint32 afterWhole[][whole0Length],
  const Coord *beforeWholeCoords, const Coord *afterWholeCoords,
  Uint32 color, int clockDirection, int cwTrueCcwFalse);
void drawSquare(int x1, int y1, int x2, int y2, Uint32 color, int depth);
int getAdjOccupied(Coord coords, Coord adj[]);
void getCenterCoords(Uint32 target[][whole0Length], Coord *ret);
int getClockDirection(const Coord *center, const Coord *d);
void getCoords(Uint32 color, Coord *ret);
void getCoordsPotentialWhole(Uint32 color, Uint32 potentialWhole[][whole0Length], Coord *ret);
void getPotentialCoords(const Coord *coords, int clockDirection, Coord *ret);
int inCorridor1(const Coord *coords, Uint32 whole[][whole0Length], const Coord *wholeCoords);
int inCorridor2(Uint32 color, Uint32 whole[][whole0Length], const Coord *wholeCoords);
int isOpen(const Coord *potentialcoords, Uint32 potentialWhole[][whole0Length], const Coord *wholecoord);
void keyboard();
void memmapWhole();
void memmapCleanup();
void memmapWholeInit();
void mirrorWholeHoriz(Uint32 target[][whole0Length]);
int minimum(int a, int b, int c);
void newBoard();
void newWhole(Uint32 newWholeX[][whole0Length]);
int oppositeClock(int clockDirection);
void proc(Uint32 interval);
void randomColorMe(Uint32 potentialWhole[][whole0Length], Coord *ret);
int randomRange(int min, int max);
void renderBoard();
void renderUi();
void rotateWhole(int cwTrueCcwFalse);
void shadow(Uint32 wholeCoords[][whole0Length], const Coord *wholecoord, Coord *ret); 
void shiftWhole(Uint32 target[][whole0Length], int clockDirection, int translate);
Uint32 update(Uint32 interval, void *param);

int absoluteValue(int a) {
  if (a < 0)
    return -1 * a;
  return a;
}

void applyWhole() {
  int i = 0;
  int j = 0;
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
      if (whole[i][j] != bgcolor)
        board[wholecoord.row + i][wholecoord.col + j] = 1;
    }
  }
  wholecoord.row = 0;
  wholecoord.col = 0;
  newWhole(whole);
  colorWhole();
}

int arrayHas2(Uint32 ar[], int size, Uint32 element) {
  int i = 0;
  for (i = 0; i < size; ++i) {
    if(ar[i] == element)
      return 1;
  }
  return 0;
}

void atExitQuit() {
  /* TODO other stuff */
  SDL_DestroyTexture(redTexture);
  SDL_DestroyTexture(orangeTexture);
  SDL_DestroyTexture(yellowTexture);
  SDL_DestroyTexture(greenTexture);
  SDL_DestroyTexture(blueTexture);
  IMG_Quit();
  SDL_Quit();
  close(sampleFd);
}

int betterCorridorExit(int cwTrueCcwFalse, int directionA, int directionB, Uint32 color) {
  int corridorDirection = -1;
  int i = 0;
  int j = 0;
  int corridorDirectionIndex = 999;
  int directionAIndex = 999;
  int directionBIndex = 999;
  int directionACloseness = 0;
  int directionBCloseness = 0;

  if (color == red) {
    if (cwTrueCcwFalse)
      corridorDirection = corridorClockRedCw;
    else
      corridorDirection = corridorClockRedCcw;
  }
  if (color == orange) {
    if (cwTrueCcwFalse)
      corridorDirection = corridorClockOrangeCw; 
    else
      corridorDirection = corridorClockOrangeCcw;
  }
  if (color == yellow) {
    if (cwTrueCcwFalse)
      corridorDirection = corridorClockYellowCw;
    else
      corridorDirection = corridorClockYellowCcw;
  }
  if (color == green) {
    if (cwTrueCcwFalse)
      corridorDirection = corridorClockGreenCw;
    else
      corridorDirection = corridorClockGreenCcw;
  }
  if (color == blue) {
    if (cwTrueCcwFalse)
      corridorDirection = corridorClockBlueCw;
    else
      corridorDirection = corridorClockBlueCcw;
  }
  for (i = 0; i < clockDirectionsLength; ++i) {
    if (clockDirections[i] == corridorDirection)
      corridorDirectionIndex = i; 
    if (clockDirections[i] == directionA)
      directionAIndex = i; 
    if (clockDirections[i] == directionB)
      directionBIndex = i; 
  }

  directionACloseness = minimum(
    absoluteValue(directionAIndex - corridorDirectionIndex),
    absoluteValue(directionAIndex - (corridorDirectionIndex + clockDirectionsLength)),
    absoluteValue((directionAIndex + clockDirectionsLength) - corridorDirectionIndex)
  );
 
  directionBCloseness = minimum(
    absoluteValue(directionBIndex - corridorDirectionIndex),
    absoluteValue(directionBIndex - (corridorDirectionIndex + clockDirectionsLength)),
    absoluteValue((directionBIndex + clockDirectionsLength) - corridorDirectionIndex)
  );

  return (directionACloseness < directionBCloseness) ? directionA : directionB;
}

void bounds(Uint32 target[][whole0Length], Bounds *ret) {
  int minx = 999;
  int maxx = -1;
  int miny = 999;
  int maxy = -1;
  int i = 0;
  int j = 0;
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
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
  
  ret->left = minx;
  ret->right = maxx;
  ret->top = miny;
  ret->bottom = maxy;
}

#ifdef PLATFORM_DC
void boxColor(...) {
}
#endif /* PLATFORM_DC */

void capitalizeRows() {
  int full = 1;
  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
  for (i = boardLength - 1; i >= 0; --i) {
    full = 1;
    for (j = 0; j < board0Length; ++j) {
      if (board[i][j] != 1)
        full = 0;
    }
    if (full) {
      for (k = i; k >= 1; --k) {
        for (l = 0; l < board0Length; ++l) {
          board[k][l] = board[k - 1][l];
        }
      }
      for (l = 0; l < board0Length; ++l) {
        board[0][l] = 0;
      }
      capitalizeRows();
      break;
    }
  }
}

void centerWhole(Uint32 target[][whole0Length], int translate) {
  Coord center = { 0, 0 };
  Bounds b = { 0, 0, 0, 0 };
  int evenVert = 0;
  int evenHoriz = 0;
  getCenterCoords(target, &center);
  bounds(target, &b);
  if ((b.bottom - b.top) % 2 == 1)
    evenVert = 1;
  if ((b.right - b.left) % 2 == 1)
    evenHoriz = 1;

  if (evenVert == 0) {
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
    if (center.row < 6) { /* TODO hardcoded */
      while (center.row < 6) {
        shiftWhole(target, 6, translate);
        center.row++;
      }
    }
    else if (center.row > 5) {
      while (center.row > 5) {
        shiftWhole(target, 12, translate);
        center.row--;
      }
    }
  }
  if (evenHoriz == 0) {
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
    if (center.col < 6) { /* TODO hardcoded */
      while (center.col < 6) {
        shiftWhole(target, 3, translate);
        center.col++;
      }
    }
    else if (center.col > 5) {
      while (center.col > 5) {
        shiftWhole(target, 9, translate);
        center.col--;
      }
    }
  }
}

void clockMove(Uint32 color, int cwTrueCcwFalse) {
  Uint32 state = 0;
  Coord curCoords = { 0, 0 };
  Coord newCoords[clockDirectionsLength] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }  } ;
  int newCoordsDirection[clockDirectionsLength] = { 0, 0, 0, 0, 0, 0, 0, 0 } ;
  int newCoordsLastIndex = 0;
  int newCoordsAdded = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  Coord adjCoords[4] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
  Coord step1 = { 0, 0 };
  Coord potentialCoords = { 0, 0 };
  Coord potentialCoordsDiag = { 0, 0 };
  Uint32 potentialWhole[wholeLength][whole0Length];
  Uint32 potentialWholeDiag[wholeLength][whole0Length];
  Uint32 step1Whole[wholeLength][whole0Length];
  int numAdj = 0;
  int adjCountRemaining = numAdj;
  Coord newCoordsX = { 0, 0 };
  Uint32 beforeWhole[wholeLength][whole0Length];
  int finalClockDirection = 0;
  int clockDirection;
  int clockDirectionDiag;
  int invalidMove = 0;
  getCoords(color, &curCoords);

  for (i = 0; i < clockDirectionsLength; ++i) {
    newCoords[i].row = curCoords.row;
    newCoords[i].col = curCoords.col;
    newCoordsDirection[i] = -1;
  }

  numAdj = getAdjOccupied(curCoords, adjCoords);
  adjCountRemaining = numAdj;
  i = numAdj - adjCountRemaining;
  invalidMove = 0;

  while (1) {
    /* Transitions */
    if (state == 0) {
      state = 1;
    }
    else if (state == 1) {
      if (adjCountRemaining > 3)
        state = 2;
      else
        state = 3;
    }
    else if (state == 2) {
      break;
    }
    else if (state == 3) {
      state = 4;
    }
    else if (state == 4) {
      if (adjCountRemaining < 0)
        state = 5;
      else if (invalidMove)
        state = 3;
      else if (isOpen(&step1, whole, &wholecoord) 
        && isOpen(&potentialCoords, whole, &wholecoord)
        && contiguous(&potentialCoords, potentialWhole) == 1)
        state = 6;
      else if (isOpen(&potentialCoords, whole, &wholecoord) == 0 
        || contiguous(&potentialCoords, potentialWhole) == 0)
        state = 7;
      else
        state = 3;
    }
    else if (state == 5) {
      break;
    }
    else if (state == 6) {
      state = 3;
    }
    else if (state == 7) {
      state = 8;
    }
    else if (state == 8) {
      if (invalidMove)
        state = 3;
      else if(isOpen(&potentialCoordsDiag, whole, &wholecoord) && contiguous(&potentialCoordsDiag, potentialWholeDiag) == 1)
        state = 9;
      else
        state = 3;
    }
    else if (state == 9) {
      state = 3;
    }

    /* Actions */
    if (state == 0) {
    }
    else if (state == 1) {
    }
    else if (state == 2) {
    }
    else if (state == 3) {
      int cw;
      int cw2;
      if (newCoordsAdded == 1)
        newCoordsLastIndex++;
      newCoordsAdded = 0;
      invalidMove = 0;
      i = numAdj - adjCountRemaining;
      --adjCountRemaining;
      clockDirection = getClockDirection(&(adjCoords[i]), &curCoords);
      if (clockDirection == -1)
        invalidMove = 1;
      cw = clockwise(cwTrueCcwFalse, clockDirection);
      cw2 = clockwise(cwTrueCcwFalse, cw);
      if (cw == -1 || cw2 == -1)
        invalidMove = 1;
      getPotentialCoords(&(adjCoords[i]), cw, &step1);
      if (step1.row == -1 || step1.col == -1)
        invalidMove = 1;
      cloneWhole(whole, step1Whole); /*TODO array copy*/
      step1Whole[curCoords.row][curCoords.col] = bgcolor;
      getPotentialCoords(&(adjCoords[i]), cw2, &potentialCoords);
      if (potentialCoords.row == -1 || potentialCoords.col == -1)
        invalidMove = 1;
      cloneWhole(whole, potentialWhole); /*TODO array copy*/
      potentialWhole[curCoords.row][curCoords.col] = bgcolor;
      potentialWhole[potentialCoords.row][potentialCoords.col] = color;
    }
    else if (state == 4) {
    }
    else if (state == 5) {
    }
    else if (state == 6) {
      newCoords[newCoordsLastIndex].row = potentialCoords.row;
      newCoords[newCoordsLastIndex].col = potentialCoords.col;
      newCoordsDirection[newCoordsLastIndex] = clockDirection;
      newCoordsAdded = 1;
    }
    else if (state == 7) {
      int cwDiag;
      invalidMove = 0;
      clockDirectionDiag = getClockDirection(&potentialCoords, &curCoords);
      if (clockDirectionDiag == -1)
        invalidMove = 1;
      cwDiag = clockwise(cwTrueCcwFalse, clockDirectionDiag);
      if (cwDiag == -1)
        invalidMove = 1;
      getPotentialCoords(&potentialCoords, cwDiag, &potentialCoordsDiag);
      if (potentialCoordsDiag.row == -1 || potentialCoordsDiag.col == -1)
        invalidMove = 1;
      cloneWhole(whole, potentialWholeDiag); /*TODO array copy*/
      potentialWholeDiag[curCoords.row][curCoords.col] = bgcolor;
      potentialWholeDiag[potentialCoordsDiag.row][potentialCoordsDiag.col] = color;
    }
    else if (state == 8) {
    }
    else if (state == 9) {
      newCoords[newCoordsLastIndex].row = potentialCoordsDiag.row;
      newCoords[newCoordsLastIndex].col = potentialCoordsDiag.col;
      newCoordsDirection[newCoordsLastIndex] = clockDirectionDiag;
      newCoordsAdded = 1;
    }
  }

  newCoordsX.row = curCoords.row;
  newCoordsX.col = curCoords.col;
  /*int newCoordsDirectionX = -1; TODO What is this? */

  if (clockDirectionsLength > 0) {
    newCoordsX.row = newCoords[0].row;
    newCoordsX.col = newCoords[0].col;
  }

  for (j = 0; j < clockDirectionsLength; ++j) {
    for (k = j + 1; k < clockDirectionsLength; ++k) {
      int clock1 = 0;
      int clock2 = 0;
      int a = 0;
      if (newCoords[k].row == curCoords.row && newCoords[k].col == curCoords.col)
        continue;
      clock1 = getClockDirection(&curCoords, &newCoords[j]);
      clock2 = getClockDirection(&curCoords, &newCoords[k]);
      a = betterCorridorExit(cwTrueCcwFalse, clock1, clock2, color);
      if (a == clock1) {
        /*printf("(%d %d %d), ", a, clock1, clock2); TODO DEBUG */
        newCoordsX.row = newCoords[j].row;
        newCoordsX.col = newCoords[j].col;
      }
      else if (a == clock2) {
        /*printf("(%d %d %d), ", a, clock1, clock2); TODO DEBUG */
        newCoordsX.row = newCoords[k].row;
        newCoordsX.col = newCoords[k].col;
      }
    }
  }
  /*printf("\n\n\n"); TODO DEBUG */

  cloneWhole(whole, beforeWhole);
  if (curCoords.row > -1 && curCoords.col > -1)
    whole[curCoords.row][curCoords.col] = bgcolor;

  if (newCoordsX.row > -1 && newCoordsX.col > -1)
    whole[newCoordsX.row][newCoordsX.col] = color;

  finalClockDirection = getClockDirection(&curCoords, &newCoordsX);
  corridorClocks2(beforeWhole, whole, &wholecoord, &wholecoord, color, finalClockDirection, cwTrueCcwFalse);
}

int clockwise(int cw, int clockDirection) {
  int i = 0;
  for (i = 0; i < clockDirectionsLength; ++i) {
    if (clockDirections[i] == clockDirection) {
      if (cw == 1)
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
  int i = 0;
  int j = 0;
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
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
  int i = 0;
  int j = 0;
  int colorindex = 0;
  int numViolet = rand() % 3 + 2;
  /*int numViolet = 3;  TODO debug remove*/
  Coord r1 = { 0, 0 };
  Coord r2 = { 0, 0 };
  randomColorMe(whole, &r1);
  randomColorMe(whole, &r2);
  /*Coord r3 = randomColorMe(whole); TODO debug remove*/
  /*r1.row = 8; TODO debug remove*/
  /*r2.row = 9; TODO debug remove*/
  /*r2.row = 10; TODO debug remove*/
  whole[r1.row][r1.col] = violet1;
  whole[r2.row][r2.col] = violet2;
  if(numViolet > 2) {
    Coord r3 = { 0, 0 };
    randomColorMe(whole, &r3);
    whole[r3.row][r3.col] = violet3;
  }
  if(numViolet > 3) {
    Coord r4 = { 0, 0 };
    randomColorMe(whole, &r4);
    whole[r4.row][r4.col] = violet4;
  }
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
      if (whole[i][j] == colorme) {
        whole[i][j] = blockcolors[colorindex++];
      }
    }
  }
}

void colorsInCorridor(Uint32 potentialWhole[][whole0Length], const Coord *potentialWholeCoords,
  int *r, int *o, int *y, int *g, int *b) {
  Coord redCoords = { 0, 0 };
  Coord orangeCoords = { 0, 0 }; 
  Coord yellowCoords = { 0, 0 };
  Coord greenCoords = { 0, 0 };
  Coord blueCoords = { 0, 0 };
  getCoordsPotentialWhole(red, potentialWhole, &redCoords); 
  getCoordsPotentialWhole(orange, potentialWhole, &orangeCoords); 
  getCoordsPotentialWhole(yellow, potentialWhole, &yellowCoords);
  getCoordsPotentialWhole(green, potentialWhole, &greenCoords); 
  getCoordsPotentialWhole(blue, potentialWhole, &blueCoords);
 
  *r = inCorridor1(&redCoords, potentialWhole, potentialWholeCoords);
  *o = inCorridor1(&orangeCoords, potentialWhole, potentialWholeCoords);
  *y = inCorridor1(&yellowCoords, potentialWhole, potentialWholeCoords);
  *g = inCorridor1(&greenCoords, potentialWhole, potentialWholeCoords);
  *b = inCorridor1(&blueCoords, potentialWhole, potentialWholeCoords);
}

int arrayHas1(int a[], int size, int search) {
  int i = 0;
  for (i = 0; i < size; ++i)
    if(a[i] == search)
      return 1;
  return 0;
}

/* TODO this is specialized for an 7 piece whole */
/* TODO is this right? */
int contiguous(const Coord *startingCoord, Uint32 potential[][whole0Length]) {
  int lf = 0;
  int l[512];  
  int sf = 0;
  int s[512];
  l[lf++] = startingCoord->row * wholeLength + startingCoord->col;
  do {
    int m=l[--lf];
    int x;
    int y;
    if (arrayHas1(s, sf, m) == 0)
      s[sf++] = m;
    x=m/wholeLength; /* This is actually y */
    y=m%wholeLength; /* This is actually x */
    if(x>=1&&potential[x-1][y]!=bgcolor&&arrayHas1(s,sf,m-wholeLength)==0)
      l[lf++]=m-wholeLength;
    if(x<wholeLength-1&&potential[x+1][y]!=bgcolor&&arrayHas1(s,sf,m+wholeLength)==0)
      l[lf++] = m+wholeLength;
    if(y>=1&&potential[x][y-1]!=bgcolor&&arrayHas1(s,sf,m-1)==0)
      l[lf++]=m-1;
    if(y<wholeLength-1&&potential[x][y+1]!=bgcolor&&arrayHas1(s,sf,m+1)==0)
      l[lf++]=m+1;
  } while(lf>0);

  return sf==7; /* TODO hardcoded */
  /*return sf==8; TODO debug remove */
}

void controller() {
#ifdef PLATFORM_DC
  maple_device_t *cont;
  cont_state_t *state;
  cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

  if (cont) {
    state = (cont_state_t *)maple_dev_status(cont);

    if (!state)
      return;

    if (state->buttons & CONT_A && state->buttons & CONT_X) {
      controllerstate |= 0x4;
      controllerstate &= ~(Uint32)(0x1);
      controllerstate &= ~(Uint32)(0x10);
      controllerstate &= ~(Uint32)(0x40);
      controllerstate &= ~(Uint32)(0x100);
      orangeComboButtonDown = 1;
    }
    else if (!(state->buttons & CONT_A) && !(state->buttons & CONT_X)) {
      orangeComboButtonDown = 0;
    }
    if (state->buttons & CONT_A && orangeComboButtonDown == 0) {
      controllerstate |= 0x1;
      controllerstate &= ~(Uint32)(0x04);
      controllerstate &= ~(Uint32)(0x10);
      controllerstate &= ~(Uint32)(0x40);
      controllerstate &= ~(Uint32)(0x100);
    }
    if (state->buttons & CONT_X && orangeComboButtonDown == 0) {
      controllerstate |= 0x10;
      controllerstate &= ~(Uint32)(0x1);
      controllerstate &= ~(Uint32)(0x4);
      controllerstate &= ~(Uint32)(0x40);
      controllerstate &= ~(Uint32)(0x100);
    }
    if (state->buttons & CONT_Y) {
      controllerstate |= 0x40;
      controllerstate &= ~(Uint32)(0x1);
      controllerstate &= ~(Uint32)(0x4);
      controllerstate &= ~(Uint32)(0x10);
      controllerstate &= ~(Uint32)(0x100);
    }
    if (state->buttons & CONT_B) {
      controllerstate |= 0x100;
      controllerstate &= ~(Uint32)(0x1);
      controllerstate &= ~(Uint32)(0x4);
      controllerstate &= ~(Uint32)(0x10);
      controllerstate &= ~(Uint32)(0x40);
    }
    if (state->rtrig > 0x7f && state->ltrig <= 0x7f) {
       /*TODO extra wait here not necessary*/
       if((controllerstate & 0x1) == 0x1)
         keystate |= 0x1;
       if((controllerstate & 0x4) == 0x4) 
         keystate |= 0x4;
       if((controllerstate & 0x10) == 0x10) 
         keystate |= 0x10;
       if((controllerstate & 0x40) == 0x40)
         keystate |= 0x40;
       if((controllerstate & 0x100) == 0x100)
         keystate |= 0x100;
    }
    else {
      keystate &= ~(Uint32)(0x1);
      keystate &= ~(Uint32)(0x4);
      keystate &= ~(Uint32)(0x10);
      keystate &= ~(Uint32)(0x40);
      keystate &= ~(Uint32)(0x100);
    }
    if (state->ltrig > 0x7f && state->rtrig <= 0x7f) {
       /*TODO extra wait here not necessary*/
       if((controllerstate & 0x1) == 0x1)
         keystate |= 0x2;
       if((controllerstate & 0x4) == 0x4)
         keystate |= 0x8;
       if((controllerstate & 0x10) == 0x10)
         keystate |= 0x20;
       if((controllerstate & 0x40) == 0x40)
         keystate |= 0x80;
       if((controllerstate & 0x100) == 0x100)
         keystate |= 0x200;
    }
    else {
      keystate &= ~(Uint32)(0x2);
      keystate &= ~(Uint32)(0x8);
      keystate &= ~(Uint32)(0x20);
      keystate &= ~(Uint32)(0x80);
      keystate &= ~(Uint32)(0x200);
    }
    if(state->joyx < -0x7f) {
      keystate |= 0x800;
    }
    else
      keystate &= ~(Uint32)(0x800);
    if(state->joyx >= 0x7f) {
      keystate |= 0x400;
    }
    else
      keystate &= ~(Uint32)(0x400);
    if (state->buttons & CONT_DPAD_UP)
      keystate |= 0x4000;
    else
      keystate &= ~(Uint32)(0x4000);
    if(state->buttons & CONT_DPAD_DOWN)
      keystate |= 0x8000;
    else
      keystate &= ~(Uint32)(0x8000);
    if(state->buttons & CONT_DPAD_LEFT)
      keystate |= 0x10000;
    else
      keystate &= ~(Uint32)(0x10000);
    if(state->buttons & CONT_DPAD_RIGHT)
      keystate |= 0x20000;
    else
      keystate &= ~(Uint32)(0x20000);
    if(state->ltrig > 0x7f && state->rtrig > 0x7f)
      keystate |= 0x40000;
    else
      keystate &= ~(Uint32)(0x40000);
  }
#endif /* PLATFORM_DC */
}

void corridorClocks1(Uint32 beforeWhole[][whole0Length], Uint32 afterWhole[][whole0Length],
  const Coord *beforeWholeCoords, const Coord *afterWholeCoords) {
  int r, o, y, g, b;
  int rx, ox, yx, gx, bx;
  colorsInCorridor(beforeWhole, beforeWholeCoords, &r, &o, &y, &g, &b);
  colorsInCorridor(afterWhole, afterWholeCoords, &rx, &ox, &yx, &gx, &bx);
  if (rx == 0 && r != 0) {
    corridorClockRedCw = oppositeClock(corridorClockRedCw);
    corridorClockRedCcw = oppositeClock(corridorClockRedCcw);
  }
  if (ox == 0 && o != 0) {
    corridorClockOrangeCw = oppositeClock(corridorClockOrangeCw);
    corridorClockOrangeCcw = oppositeClock(corridorClockOrangeCcw);
  }
  if (yx == 0 && y != 0) {
    corridorClockYellowCw = oppositeClock(corridorClockYellowCw);
    corridorClockYellowCcw = oppositeClock(corridorClockYellowCcw);
  }
  if (gx == 0 && g != 0) {
    corridorClockGreenCw = oppositeClock(corridorClockGreenCw);
    corridorClockGreenCcw = oppositeClock(corridorClockGreenCcw);
  }
  if (bx == 0 && b != 0) {
    corridorClockBlueCw = oppositeClock(corridorClockBlueCw);
    corridorClockBlueCcw = oppositeClock(corridorClockBlueCcw);
  }
}

void corridorClocks2(Uint32 beforeWhole[][whole0Length], Uint32 afterWhole[][whole0Length],
  const Coord *beforeWholeCoords, const Coord *afterWholeCoords,
  Uint32 color, int clockDirection, int cwTrueCcwFalse) {
  /* Cooridor Procedure NOT BEING USED CURRENTLY and PROBABLY WRONG:
  * This needs work. The issue is that entering a corridor from a dead end the direction is straight on
  * And entering a corridor from 1 adj will have a diagonal direction
  * For each color:
  * 1. If in corridor and no direction is set (such as a piece being in the corridor from initialization),
  *    set its direction to 1. (For now, but see if there's something more logical later.)
  * 2. Keep track of the direction you enter the corridor.
  * 3. Exit the same direction you enter the corridor
  * 4a.If you reach a dead end (3 adjacent), then that's exiting the corridor.
  * 4b.Note that exiting into a dead end is not exiting diagonally
  * 4c.In this case, you'll re-enter the corridor in the next move in the opposite direction stored prior
  *    to exiting.
  * 5. Keep track of the button used to enter and exit the corridor, so that the player can reverse direction.
  * 6. If moving a piece creates a corridor for another piece then reverse the direction of the piece.
  * 7. If moving a piece destroys a corridor for another piece then reverse the direction of the piece.
  */

  int r, o, y, g, b;
  int rx, ox, yx, gx, bx;
  colorsInCorridor(beforeWhole, beforeWholeCoords, &r, &o, &y, &g, &b);
  colorsInCorridor(afterWhole, afterWholeCoords, &rx, &ox, &yx, &gx, &bx);
  if (color == red && ((r == 0 && rx != 0) || (r != 0 && r != rx))) {
    if (cwTrueCcwFalse) {
      corridorClockRedCw = clockDirection;
      corridorClockRedCcw = oppositeClock(clockDirection);
    }
    else {
      corridorClockRedCw = oppositeClock(clockDirection);
      corridorClockRedCcw = clockDirection;
    }
  }
  if (color == orange && ((o == 0 && ox != 0) || (o != 0 && o != ox))) {
    if (cwTrueCcwFalse) {
      corridorClockOrangeCw = clockDirection;
      corridorClockOrangeCcw = oppositeClock(clockDirection);
    }
    else {
      corridorClockOrangeCw = oppositeClock(clockDirection);
      corridorClockOrangeCcw = clockDirection;
    }
  }
  if (color == yellow && ((y == 0 && yx != 0) || (y != 0 && y != yx))) {
    if (cwTrueCcwFalse) {
      corridorClockYellowCw = clockDirection;
      corridorClockYellowCcw = oppositeClock(clockDirection);
    }
    else {
      corridorClockYellowCw = oppositeClock(clockDirection);
      corridorClockYellowCcw = clockDirection;
    }
  }
  if (color == green && ((g == 0 && gx != 0) || (g != 0 && g != gx))) {
    if (cwTrueCcwFalse) {
      corridorClockGreenCw = clockDirection;
      corridorClockGreenCcw = oppositeClock(clockDirection);
    }
    else {
      corridorClockGreenCw = oppositeClock(clockDirection);
      corridorClockGreenCcw = clockDirection;
    }
  }
  if (color == blue && ((b == 0 && bx != 0) || (b != 0 && b != bx))) {
    if (cwTrueCcwFalse) {
      corridorClockBlueCw = clockDirection;
      corridorClockBlueCcw = oppositeClock(clockDirection);
    }
    else {
      corridorClockBlueCw = oppositeClock(clockDirection);
      corridorClockBlueCcw = clockDirection;
    }
  }
  corridorClocks1(beforeWhole, afterWhole, beforeWholeCoords, afterWholeCoords);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void drawSquare(int x1, int y1, int x2, int y2, Uint32 color, int depth) {
  color = color + 0;
  depth = depth + 0;
#ifdef PLATFORM_DC
  pvr_vertex_t vert;
  vert.flags = PVR_CMD_VERTEX;
  vert.x = x1;
  vert.y = y2;
  vert.z = depth;
  vert.u = vert.v = 0.0f;
  vert.argb = color;
  vert.oargb = 0;
  pvr_prim(&vert, sizeof(vert));

  vert.y = y1;
  pvr_prim(&vert, sizeof(vert));

  vert.x = x2;
  vert.y = y2;
  pvr_prim(&vert, sizeof(vert));

  vert.flags = PVR_CMD_VERTEX_EOL;
  vert.y = y1;
  pvr_prim(&vert, sizeof(vert));
#endif /* PLATFORM_DC */
}
#pragma GCC diagnostic pop

int getAdjOccupied(Coord coords, Coord adj[]) {
  int idx=0;
  Coord c1 = { 0, 0 };
  Coord c2 = { 0, 0 };
  Coord c3 = { 0, 0 };
  Coord c4 = { 0, 0 };
  c1.row = coords.row - 1;
  c1.col = coords.col;
  c2.row = coords.row + 1;
  c2.col = coords.col;
  c3.row = coords.row;
  c3.col = coords.col - 1;
  c4.row = coords.row;
  c4.col = coords.col + 1;
  if (isOpen(&c1, whole, &wholecoord) == 0) {
    adj[idx].row = c1.row;
    adj[idx++].col = c1.col;
  }
  if (isOpen(&c2, whole, &wholecoord) == 0) {
    adj[idx].row = c2.row;
    adj[idx++].col = c2.col;
  }
  if (isOpen(&c3, whole, &wholecoord) == 0) {
    adj[idx].row = c3.row;
    adj[idx++].col = c3.col;
  }
  if (isOpen(&c4, whole, &wholecoord) == 0) {
    adj[idx].row = c4.row;
    adj[idx++].col = c4.col;
  }
  return idx;
}

void getCenterCoords(Uint32 target[][whole0Length], Coord *ret) {
  Bounds b = { 0, 0, 0, 0 };
  bounds(target, &b);
  ret->row = (int)(((b.bottom - b.top) / 2 + b.top));
  ret->col = (int)(((b.right - b.left) / 2 + b.left));
}

int getClockDirection(const Coord *center, const Coord *d) {
  /* Assumes center and d are adj. */
  if (d->row == center->row - 1) {
    if (d->col == center->col - 1)
      return 11;
    if (d->col == center->col)
      return 12;
    if (d->col == center->col + 1)
      return 1;
  }
  if (d->row == center->row) {
    if (d->col == center->col - 1)
      return 9;
    if (d->col == center->col)
      return 0;
    if (d->col == center->col + 1)
      return 3;
  }
  if (d->row == center->row + 1) {
    if (d->col == center->col - 1)
      return 7;
    if (d->col == center->col)
      return 6;
    if (d->col == center->col + 1)
      return 5;
  }

  return -1;
}

void getCoords(Uint32 color, Coord *ret) {
  getCoordsPotentialWhole(color, whole, ret);
}

void getCoordsPotentialWhole(Uint32 color, Uint32 potentialWhole[][whole0Length], Coord *ret) {
  int i = 0;
  int j = 0;
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
      if(potentialWhole[i][j] == color){ 
        ret->row = i;
        ret->col = j;
        return;
      }
    }
  }

  /*TODO jminer*/
  ret->row = -1;
  ret->col = -1;
}

void getPotentialCoords(const Coord *coords, int clockDirection, Coord *ret) {
  int y = coords->row;
  int x = coords->col;

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

  ret->row = y;
  ret->col = x;
}

int inCorridor1(const Coord *coords, Uint32 wholeEx[][whole0Length], const Coord *wholeExCoords) {
  Coord coordsA = { 0, 0 };
  Coord coordsB = { 0, 0 };
  Coord coordsC = { 0, 0 };
  Coord coordsD = { 0, 0 };
  coordsA.row = coords->row;
  coordsA.col = coords->col - 1;
  coordsB.row = coords->row;
  coordsB.col = coords->col + 1;
  coordsC.row = coords->row - 1;
  coordsC.col = coords->col;
  coordsD.row = coords->row + 1;
  coordsD.col = coords->col;
  if(isOpen(&coordsA, wholeEx, wholeExCoords) && isOpen(&coordsB, wholeEx, wholeExCoords)
    && !isOpen(&coordsC, wholeEx, wholeExCoords) && !isOpen(&coordsD, wholeEx, wholeExCoords))
    return 1;
  if(isOpen(&coordsC, wholeEx, wholeExCoords) && isOpen(&coordsD, wholeEx, wholeExCoords)
    && !isOpen(&coordsA, wholeEx, wholeExCoords) && !isOpen(&coordsB, wholeEx, wholeExCoords))
    return 2;
  return 0;
}

int inCorridor2(Uint32 color, Uint32 wholeEx[][whole0Length], const Coord *wholeExCoords) {
  Coord c = { 0, 0 };
  getCoords(color, &c);
  return inCorridor1(&c, wholeEx, wholeExCoords);
}


int isOpen(const Coord *piececoords, Uint32 potentialWhole[][whole0Length], const Coord *wholeCoords) {
  if (piececoords->col < 0 || piececoords->row < 0)
    return 0;
  if (piececoords->col + wholeCoords->col < 0)
    return 0;
  if (piececoords->row + wholeCoords->row < 0)
    return 0;
  if (piececoords->col + wholeCoords->col >= board0Length)
    return 0;
  if (piececoords->row + wholeCoords->row >= boardLength)
    return 0;

  if (board[piececoords->row + wholeCoords->row][piececoords->col + wholeCoords->col])
    return 0;

  return potentialWhole[piececoords->row][piececoords->col] == bgcolor;
}

void keyboard() {
#ifdef PLATFORM_PC
  const Uint8 *state;
  SDL_PumpEvents();
  state =
    SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_ESCAPE])
    exit(0);
  if(state[SDL_SCANCODE_1])
    keystate |= 0x1;
  if(state[SDL_SCANCODE_Q])
    keystate |= 0x2;
  if((!state[SDL_SCANCODE_1] && !state[SDL_SCANCODE_Q])
    || (state[SDL_SCANCODE_1] && state[SDL_SCANCODE_Q])) {
    keystate &= ~(Uint32)(0x1);
    keystate &= ~(Uint32)(0x2);
  }
  if(state[SDL_SCANCODE_2])
    keystate |= 0x4;
  if(state[SDL_SCANCODE_W])
    keystate |= 0x8;
  if((!state[SDL_SCANCODE_2] && !state[SDL_SCANCODE_W])
    || (state[SDL_SCANCODE_2] && state[SDL_SCANCODE_W])) {
    keystate &= ~(Uint32)(0x4);
    keystate &= ~(Uint32)(0x8);
  }
  if(state[SDL_SCANCODE_3])
    keystate |= 0x10;
  if(state[SDL_SCANCODE_E])
    keystate |= 0x20;
  if((!state[SDL_SCANCODE_3] && !state[SDL_SCANCODE_E])
    || (state[SDL_SCANCODE_3] && state[SDL_SCANCODE_E])) {
    keystate &= ~(Uint32)(0x10);
    keystate &= ~(Uint32)(0x20);
  }
  if(state[SDL_SCANCODE_4])
    keystate |= 0x40;
  if(state[SDL_SCANCODE_R])
    keystate |= 0x80;
  if((!state[SDL_SCANCODE_4] && !state[SDL_SCANCODE_R])
    || (state[SDL_SCANCODE_4] && state[SDL_SCANCODE_R])) {
    keystate &= ~(Uint32)(0x40);
    keystate &= ~(Uint32)(0x80);
  }
  if(state[SDL_SCANCODE_5])
    keystate |= 0x100;
  if(state[SDL_SCANCODE_T])
    keystate |= 0x200;
  if((!state[SDL_SCANCODE_5] && !state[SDL_SCANCODE_T])
    || (state[SDL_SCANCODE_5] && state[SDL_SCANCODE_T])) {
    keystate &= ~(Uint32)(0x100);
    keystate &= ~(Uint32)(0x200);
  }
  if(state[SDL_SCANCODE_6])
    keystate |= 0x400;
  if(state[SDL_SCANCODE_Y])
    keystate |= 0x800;
  if((!state[SDL_SCANCODE_6] && !state[SDL_SCANCODE_Y])
    || (state[SDL_SCANCODE_6] && state[SDL_SCANCODE_Y])) {
    keystate &= ~(Uint32)(0x400);
    keystate &= ~(Uint32)(0x800);
  }
  if(state[SDL_SCANCODE_6])
    keystate |= 0x1000;
  if(state[SDL_SCANCODE_Y])
    keystate |= 0x2000;
  if((!state[SDL_SCANCODE_6] && !state[SDL_SCANCODE_Y])
    || (state[SDL_SCANCODE_6] && state[SDL_SCANCODE_Y])) {
    keystate &= ~(Uint32)(0x1000);
    keystate &= ~(Uint32)(0x2000);
  }
  if(state[SDL_SCANCODE_UP])
    keystate |= 0x4000;
  else
    keystate &= ~(Uint32)(0x4000);
  if(state[SDL_SCANCODE_DOWN])
    keystate |= 0x8000;
  else
    keystate &= ~(Uint32)(0x8000);
  if(state[SDL_SCANCODE_LEFT])
    keystate |= 0x10000;
  else
    keystate &= ~(Uint32)(0x10000);
  if(state[SDL_SCANCODE_RIGHT])
    keystate |= 0x20000;
  else
    keystate &= ~(Uint32)(0x20000);
  if(state[SDL_SCANCODE_SPACE])
    keystate |= 0x40000;
  else
    keystate &= ~(Uint32)(0x40000);
#endif
}

void memmapWholeInit() {
  sampleFd = open("sample7.bin", O_RDONLY);
  if (sampleFd == -1) {
    char *errmsg = strerror(errno);
    exit(1);
  }
  if (fstat(sampleFd, &sampleSb) == -1) {
    char *errmsg = strerror(errno);
    exit(1);
  }
}

void memmapWhole() {
  sampleOffset = randomRange(0, 999) * 49; 
  samplePaOffset = sampleOffset & ~(sysconf(_SC_PAGE_SIZE) - 1);

  if (sampleOffset >= sampleSb.st_size) {
    exit(1);
  }

  sampleLength = 49;

  if (sampleOffset + sampleLength > sampleSb.st_size)
    sampleLength = sampleSb.st_size - sampleOffset;

  sampleAddr = (char *) mmap(NULL, sampleLength + sampleOffset - samplePaOffset, PROT_READ, MAP_PRIVATE, sampleFd, samplePaOffset);
  if (sampleAddr == MAP_FAILED) {
    char *errmsg = strerror(errno);
    exit(1);
  }
}

void memmapCleanup() {
  munmap(sampleAddr, sampleLength + sampleOffset - samplePaOffset);
  close(sampleFd);
}

void mirrorWholeHoriz(Uint32 target[][whole0Length]) {
  int i = 0;
  int j = 0;
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length / 2; ++j) {
      Uint32 tmp = target[i][j];
      target[i][j] = target[i][whole0Length - 1 - j];
      target[i][whole0Length - 1 - j] = tmp;
    }
  }
}

int minimum(int a, int b, int c) {
  if (a < b && a < c)
    return a;
  if (b < a && b < c)
    return b;
  return c;
}

void newBoard() {
  int j = 0;
  for (j = 0; j < board0Length; ++j) {
    int r = rand() % 7;
    int i = 0;
    for (i = 0; i < boardLength; ++i) {
      if (i >= boardLength - r)
        board[i][j] = 1;
      else
        board[i][j] = 0;
    }
  }
}

void newWhole(Uint32 newWholeX[][whole0Length]) {
  int i = 0;
  int j = 0;
  memmapWhole();
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
      newWholeX[i][j] = bgcolor;
    }
  }
  for (i = 0; i < 7; ++i) { /*TODO hardcoded*/
    for (j = 0; j < 7; ++j) {
      if (sampleAddr[sampleOffset - samplePaOffset + i * 7 + j] == 1)
        newWholeX[i][j] = colorme;
    }
  }

/*
  newWholeX[0][0] = colorme;
  newWholeX[1][0] = colorme;
  newWholeX[2][0] = colorme;
  newWholeX[3][0] = colorme;
  newWholeX[4][0] = colorme;
  newWholeX[5][0] = colorme;
  newWholeX[6][0] = colorme;
*/

  centerWhole(newWholeX, 1);
  corridorClockRedCw = 1;
  corridorClockOrangeCw = 1;
  corridorClockYellowCw = 1;
  corridorClockGreenCw = 1;
  corridorClockBlueCw = 1;
  corridorClockRedCcw = 7;
  corridorClockOrangeCcw = 7;
  corridorClockYellowCcw = 7;
  corridorClockGreenCcw = 7;
  corridorClockBlueCcw = 7;
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
  int i = 0;
  int j = 0;
  int apply = 0;
  centerWhole(whole, 1);
  if((keystate & 0x1) == 0x1 && (((keystateprev & 0x1) != 0x1) || moveTimerRed < 0)) {
    moveTimerRed = moveTimeout;
    clockMove(red, 1);
  }
  if((keystate & 0x2) == 0x2 && (((keystateprev & 0x2) != 0x2) || moveTimerRed < 0)) {
    moveTimerRed = moveTimeout;
    clockMove(red, 0);
  }
  if((keystate & 0x4) == 0x4 && (((keystateprev & 0x4) != 0x4) || moveTimerOrange < 0)) {
    moveTimerOrange = moveTimeout;
    clockMove(orange, 1);
  }
  if((keystate & 0x8) == 0x8 && (((keystateprev & 0x8) != 0x8) || moveTimerOrange < 0)) {
    moveTimerOrange = moveTimeout;
    clockMove(orange, 0);
  }
  if((keystate & 0x10) == 0x10 && (((keystateprev & 0x10) != 0x10) || moveTimerYellow < 0)) {
    moveTimerYellow = moveTimeout;
    clockMove(yellow, 1);
  }
  if((keystate & 0x20) == 0x20 && (((keystateprev & 0x20) != 0x20) || moveTimerYellow < 0)) {
    moveTimerYellow = moveTimeout;
    clockMove(yellow, 0);
  }
  if((keystate & 0x40) == 0x40 && (((keystateprev & 0x40) != 0x40) || moveTimerGreen < 0)) {
    moveTimerGreen = moveTimeout;
    clockMove(green, 1);
  }
  if((keystate & 0x80) == 0x80 && (((keystateprev & 0x80) != 0x80) || moveTimerGreen < 0)) {
    moveTimerGreen = moveTimeout;
    clockMove(green, 0);
  }
  if((keystate & 0x100) == 0x100 && (((keystateprev & 0x100) != 0x100) || moveTimerBlue < 0)) {
    moveTimerBlue = moveTimeout;
    clockMove(blue, 1);
  }
  if((keystate & 0x200) == 0x200 && (((keystateprev & 0x200) != 0x200) || moveTimerBlue < 0)) {
    moveTimerBlue = moveTimeout;
    clockMove(blue, 0);
  }
  if((keystate & 0x400) == 0x400 && (((keystateprev & 0x400) != 0x400) || moveTimerWhole < 0)) {
    moveTimerWhole = moveTimeout;
    rotateWhole(1);
  }
  if((keystate & 0x800) == 0x800 && (((keystateprev & 0x800) != 0x800) || moveTimerWhole < 0)) {
    moveTimerWhole = moveTimeout;
    rotateWhole(0);
    /* TODO how does this affect corridor clocks? */
  }

  if((keystate & 0x20000) == 0x20000 && (((keystateprev & 0x20000) != 0x20000) || moveTimerRight < 0)) {
    moveTimerRight = moveTimeout;
    ++wholecoord.col;
    for (i = 0; i < wholeLength; ++i) {
      for (j = 0; j < whole0Length; ++j) {
        if (whole[i][j] != bgcolor) {
          Coord c1 = { 0, 0 };
          Uint32 color = whole[i][j];
          whole[i][j] = bgcolor;
          c1.row = i;
          c1.col = j;
          if(isOpen(&c1, whole, &wholecoord) == 0)
            --wholecoord.col;
          whole[i][j] = color;
        }
      }
    }
  }
  if((keystate & 0x10000) == 0x10000 && (((keystateprev & 0x10000) != 0x10000) || moveTimerLeft < 0)) {
    moveTimerLeft = moveTimeout;
    --wholecoord.col;
    for (i = 0; i < wholeLength; ++i) {
      for (j = 0; j < whole0Length; ++j) {
        if (whole[i][j] != bgcolor) {
          Coord c2 = { 0, 0 };
          Uint32 color = whole[i][j];
          whole[i][j] = bgcolor;
          c2.row = i;
          c2.col = j;
          if(isOpen(&c2, whole, &wholecoord) == 0)
            ++wholecoord.col;
          whole[i][j] = color;
        }
      }
    }
  }
  apply = 0;
  if((keystate & 0x4000) == 0x4000 && (((keystateprev & 0x4000) != 0x4000))) {
    int hit = 0;
    gravityTimer = gravityTimeout;
    while (hit == 0) {
      ++wholecoord.row;
      for (i = 0; i < wholeLength; ++i) {
        for (j = 0; j < whole0Length; ++j) {
          if (whole[i][j] != bgcolor) {
            Uint32 color = whole[i][j];
            Coord c3 = { 0, 0 };
            c3.row = i;
            c3.col = j;
            whole[i][j] = bgcolor;
            if(isOpen(&c3, whole, &wholecoord) == 0) {
              --wholecoord.row;
              hit = 1;
              apply = 1;
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
    for (i = 0; i < wholeLength; ++i) {
      for (j = 0; j < whole0Length; ++j) {
        if (whole[i][j] != bgcolor) {
          Coord c4 = { 0, 0 };
          Uint32 color = whole[i][j];
          whole[i][j] = bgcolor;
          c4.row = i;
          c4.col = j;
          if(isOpen(&c4, whole, &wholecoord) == 0) {
            --wholecoord.row;
            apply = 1;
          }
          whole[i][j] = color;
        }
      }
    }
  }
  else {
    gravityTimer -= (int)(interval & 0x7fffffff);
#ifndef DEBUG_MODE
    if (gravityTimer < 0) {
      gravityTimer += gravityTimeout;
      ++wholecoord.row; /* TODO jminer COMMENT */
      for (i = 0; i < wholeLength; ++i) {
        for (j = 0; j < whole0Length; ++j) {
          if (whole[i][j] != bgcolor) {
            Coord c5 = { 0, 0 };
            Uint32 color = whole[i][j];
            whole[i][j] = bgcolor;
            c5.row = i;
            c5.col = j;
            if(isOpen(&c5, whole, &wholecoord) == 0) {
              --wholecoord.row;
              apply = 1;
            }
            whole[i][j] = color;
          }
        }
      }
    }
#endif /* DEBUG_MODE */
  }
  if (apply == 1)
    applyWhole();
  else if((keystate & 0x40000) == 0x40000 && (((keystateprev & 0x40000) == 0x40000))) {
    capitalizeRows();
  }
}

void randomColorMe(Uint32 potentialWhole[][whole0Length], Coord *ret) {
  int size=0;
  int i = 0;
  int j = 0;
  int r;
  Coord colorMes[169]; /* wholeLength * whole0Length 13*13;*/
  for (i = 0; i < 169; ++i) {
    colorMes[i].row = 0;
    colorMes[i].col = 0;
  }
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
      if (potentialWhole[i][j] == colorme) {
        colorMes[size].row = i;
        colorMes[size++].col = j;
      }
    }
  }
  r  = randomRange(0, size - 1);
  ret->row = colorMes[r].row;
  ret->col = colorMes[r].col;
}

int randomRange(int min, int max) {
  return rand() % (max - min) + min + 1;
}

void renderBoard() {
 int i = 0;
 int j = 0;
 SDL_Texture *texture;
 SDL_Rect dstRect;
 Coord shadowcoord = { 0, 0 };
 int boxX;
 int boxY;
 int full = 1;
#ifdef PLATFORM_PC
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderClear(renderer);
#endif /* PLATFORM_PC */
#ifdef PLATFORM_DC
  void *renderer = NULL; /* Not used*/
  pvr_poly_cxt_t cxt;
  pvr_poly_hdr_t poly;

  pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
  pvr_poly_compile(&poly, &cxt);

  pvr_wait_ready();

  pvr_scene_begin();
  pvr_list_begin(PVR_LIST_OP_POLY);
  pvr_prim(&poly, sizeof(poly));
#endif /* PLATFORM_DC */
  boxColor(renderer, boardCoord.col, boardCoord.row, boardCoord.col + board0Length * blockwidth, boardCoord.row + boardLength * blockwidth, bgcolor);
  drawSquare(boardCoord.col, boardCoord.row, boardCoord.col + board0Length * blockwidth, boardCoord.row + boardLength * blockwidth, bgcolor, 1);
  for (i = 0; i < boardLength; ++i) {
    for (j = 0; j < board0Length; ++j) {
      /*ctx.fillStyle = board[i][j];*/
      /*ctx.fillRect(j * blockwidth, i * blockwidth, blockwidth - 1, blockwidth - 1);*/
    }
  }
  shadow(whole, &wholecoord, &shadowcoord);
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
      if(arrayHas2(blockcolors, 10, whole[i][j])){ /*TODO hardcoded*/
        int shadowX;
        int shadowY;
        Uint32 shadowcolor;
        texture = NULL;
        boxX = boardCoord.col + (wholecoord.col + j) * blockwidth;
        boxY = boardCoord.row + (wholecoord.row + i) * blockwidth;
        dstRect.x = boxX;
        dstRect.y = boxY;
        dstRect.w = blockwidth;
        dstRect.h = blockwidth;
        if (whole[i][j] == red)
          texture = redTexture;
        else if (whole[i][j] == orange)
          texture = orangeTexture;
        else if (whole[i][j] == yellow)
          texture = yellowTexture;
        else if (whole[i][j] == green)
          texture = greenTexture;
        else if (whole[i][j] == blue)
          texture = blueTexture;
        if (texture == NULL)
          boxColor(renderer, boxX + 2, boxY + 2, boxX + blockwidth - 1, boxY + blockwidth - 1, whole[i][j]);
        else
          SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        drawSquare(boxX + 1, boxY + 1, boxX + blockwidth - 1, boxY + blockwidth - 1, whole[i][j], 2);
        shadowX = boardCoord.col + (shadowcoord.col + j) * blockwidth;
        shadowY = boardCoord.row + (shadowcoord.row + i) * blockwidth;
        shadowcolor = (whole[i][j] & 0x00ffffff) | 0x40000000; /*TODO endian bug*/
        boxColor(renderer, shadowX + 2, shadowY + 2, shadowX + blockwidth - 1, shadowY + blockwidth - 1, shadowcolor); /*TODO reimplement shadow*/
        /* Don't draw shadow on dreamcast because we don't use translucency*/
      }
    }
  }
  full = 1;
  for (i = 0; i < boardLength; ++i) {
    full = 1;
    for (j = 0; j < board0Length; ++j) {
      if (board[i][j] == 0)
        full = 0;
    }
    if (full == 1) {
      for (j = 0; j < board0Length; ++j) {
        if(board[i][j]) {
          boxX = boardCoord.col + j * blockwidth;
          boxY = boardCoord.row + i * blockwidth;
          boxColor(renderer, boxX, boxY, boxX + blockwidth - 1, boxY + blockwidth - 1, blockcolors[j % 5]); /*TODO hardcoded*/
          drawSquare(boxX, boxY, boxX + blockwidth - 1, boxY + blockwidth - 1, blockcolors[j % 5], 2); /*TODO hardcoded*/
        }
      }
    }
    else {
      for (j = 0; j < board0Length; ++j) {
        if(board[i][j]) {
          boxX = boardCoord.col + j * blockwidth;
          boxY = boardCoord.row + i * blockwidth;
          boxColor(renderer, boxX + 2, boxY + 2, boxX + blockwidth - 1, boxY + blockwidth - 1, gray1);
          drawSquare(boxX + 1, boxY + 1, boxX + blockwidth - 1, boxY + blockwidth - 1, gray1, 2);
        }
      }
    }
  }

#ifdef PLATFORM_DC
  pvr_list_finish();
  pvr_scene_finish();
#endif /* PLATFORM_DC */
}

void renderUi() {
}

void rotateWhole(int cwTrueCcwFalse) {
  int i = 0;
  int j = 0;
  Uint32  potentialWhole[wholeLength][whole0Length];
  Uint32 emptyWhole[wholeLength][whole0Length];
  int open = 1;
  
  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
      potentialWhole[i][j] = bgcolor;
      emptyWhole[i][j] = bgcolor;
    }
  }

  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
      if (cwTrueCcwFalse == 1)
        potentialWhole[i][j] = whole[j][i];
      else {
        potentialWhole[wholeLength - 1 - i][j] = whole[j][i];
      }
    }
  }

  if (cwTrueCcwFalse == 1)
    mirrorWholeHoriz(potentialWhole);

  open = 1;

  for (i = 0; i < wholeLength; ++i) {
    for (j = 0; j < whole0Length; ++j) {
      Coord c1 = { 0, 0 };
      c1.row = i;
      c1.col = j;
      if (potentialWhole[i][j] != bgcolor
        && isOpen(&c1, emptyWhole, &wholecoord) == 0)
        open = 0;
    }
  }
  
  if (open == 1) {
    for (i = 0; i < wholeLength; ++i) {
      for (j = 0; j < whole0Length; ++j) {
        whole[i][j] = potentialWhole[i][j];
      }
    }

    centerWhole(whole, 1);
  }
}

void shadow(Uint32 wholeCoords[][whole0Length], const Coord *coords, Coord *ret) {
  int i = 0;
  int j = 0;
  int loop = 1;
  Uint32 clone[wholeLength][whole0Length];
  cloneWhole(wholeCoords, clone);
  ret->row = coords->row;
  ret->col = coords->col;
  loop = 1;
  while(loop == 1) {
    ret->row++;
    for (i = 0; i < wholeLength; ++i) {
      for (j = 0; j < whole0Length; ++j) {
        if (clone[i][j] != bgcolor && ((i + ret->row) >= boardLength || board[i + ret->row][j + ret->col] == 1)) {
          ret->row--;
          loop = 0;
        }
      }
    }
  } 
}

void shiftWhole(Uint32 target[][whole0Length], int clockDirection, int translate) {
  int i = 0;
  int j = 0;
  if (clockDirection == 12) {
    for (i = 0; i < wholeLength; ++i) {
      for (j = 0; j < whole0Length; ++j) {
        if (i < wholeLength - 1) {
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
    for (i = wholeLength- 1; i >= 0; --i) {
      for (j = 0; j < whole0Length; ++j) {
        if (i >= 1) {
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
    for (i = 0; i < wholeLength; ++i) {
      for (j = whole0Length - 1; j >= 0; --j) {
        if (j >= 1) {
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
    for (i = 0; i < wholeLength; ++i) {
      for (j = 0; j < whole0Length; ++j) {
        if (j < whole0Length - 1) {
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
Uint32 update(Uint32 interval, void *param) {
  moveTimerRed -= (int)(interval & 0x7fffffff);
  moveTimerOrange -= (int)(interval & 0x7fffffff);
  moveTimerYellow -= (int)(interval & 0x7fffffff);
  moveTimerGreen -= (int)(interval & 0x7fffffff);
  moveTimerBlue -= (int)(interval & 0x7fffffff);
  moveTimerDown -= (int)(interval & 0x7fffffff);
  moveTimerLeft -= (int)(interval & 0x7fffffff);
  moveTimerRight -=(int)(interval & 0x7fffffff);
  proc(interval);
  renderTimer -= (int)(interval & 0x7fffffff);
  if (renderTimer < 0) {
    renderTimer = (int)(renderTimeout & 0x7fffffff);
    renderBoard();
    renderUi();
#ifdef PLATFORM_PC
    SDL_RenderPresent(renderer);
#endif
  }
  keystateprev = keystate;
  controllerstateprev = controllerstate;
  return interval;
}
#pragma GCC diagnostic pop

#ifdef PLATFORM_DC
pvr_init_params_t pvr_params = {
  { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0,
    PVR_BINSIZE_0, PVR_BINSIZE_0 },
  512 * 1024
};
#endif /* PLATFORM_DC */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char **argv) {
/* TODO wiki.libsdl.org create window "you must set the nshighresolutioncapable info.plist property to YES" on mac. */
  srand((time_t)time(0));
#ifdef PLATFORM_PC
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);
  atexit(atExitQuit);
  screen = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
  /* TODO if (screen == null) */
  renderer = SDL_CreateRenderer(screen, -1, 0);
  /* TODO if (renderer == null) */
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(renderer, 1824, 984);
  boardCoord.col = 1824 / 2 - board0Length * blockwidth / 2;
  boardCoord.row = 984 / 2 - boardLength * (blockwidth / 2);
  redTexture = IMG_LoadTexture(renderer, "./graphics/Q.png");
  orangeTexture = IMG_LoadTexture(renderer, "./graphics/R.png");
  yellowTexture = IMG_LoadTexture(renderer, "./graphics/B.png");
  greenTexture = IMG_LoadTexture(renderer, "./graphics/Kt.png");
  blueTexture = IMG_LoadTexture(renderer, "./graphics/P.png");
/**
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
    TODO ALL OTHER COLORS
  }
  else if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
  }
  else
    printf("Could not determine endianness\n");
****/
#endif

#ifdef PLATFORM_DC
  vid_init(DM_640x480, PM_RGB565);
  pvr_stats_t stats;
  pvr_init(&pvr_params);
  pvr_set_bg_color(.78f, .78f, .78f);
  /*start = timer_ms_gettime64();*/
  boardCoord.col = 640 / 2 - board0Length * blockwidth / 2;
  boardCoord.row = 480 / 2 - boardLength * blockwidth / 2;
#endif /* PLATFORM_DC */

/* TODO destroy window release renderer. */
/* Here's orig main.*/
/* TODO Assert whole length and width. */
  memmapWholeInit(); /*TODO error handling*/
  newBoard();
  newWhole(whole);
  colorWhole();
#ifdef PLATFORM_PC
  SDL_AddTimer(updateInterval, update, &noParam);
  while(1)
    keyboard();
#endif
#ifdef PLATFORM_DC
    int unusedi = 0;
    uint32 nowSeconds = 0;
    uint32 nowMillis = 0;
    uint32 nowTime = 0;
    uint32 lastTime = 0;
    uint32 elapsedTime = 0; /* TODO 64bit timer important */
    while(1) {
      timer_ms_gettime(&nowSeconds, &nowMillis);
      nowTime = nowSeconds * 1000 + nowMillis;
      elapsedTime = nowTime - lastTime;
      lastTime = nowTime;
      controller();
      update(elapsedTime, &unusedi);
      renderBoard();
    }
    pvr_get_stats(&stats);
    printf("From pvr_get_stats:\n\tVBlank Count: %lu\n\tFrame Count: %lu\n",
           stats.vbl_count, stats.frame_count);
    pvr_shutdown();
#endif
  return 0;
}
#pragma GCC diagnostic pop
#pragma GCC diagnostic ignored "-Wunused-parameter"
