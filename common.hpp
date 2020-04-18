#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <bits/stdc++.h>
#include <curses.h>
using namespace std;

// COL can't be < 20, otherwise we need to refactor prompt_screen()
const int ROW = 20, COL = 40;
const int dx[] = {1,-1,0,0}, dy[] = {0,0,1,-1};

typedef pair<int,int> pii;

enum Dir_t {
  DOWN = 0,
  UP,
  RIGHT,
  LEFT
};

enum Collision_t {
  NONE = 0,
  SELF,
  WALL,
  FOOD
};

#define ONESEC 1000000LL

#ifdef DEBUG
#define dbg(...) printw(__VA_ARGS__);
#else
#define dbg(...) // Debug mode disabled
#endif

#endif
