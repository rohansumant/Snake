#include <unistd.h>
#include <cstdlib>
#include <thread>
#include "snake.hpp"
#include "common.hpp"

char screen[ROW][COL];

void draw() {
  clear();
  static int iter = 1;  
  printw("Iteration: %d\n",iter++);
  for(int i=0;i<ROW;i++) {
    for(int j=0;j<COL;j++) {
      printw("%c",screen[i][j]);
    }
    printw("\n");
  }
  refresh();
}

void init_screen() {
  for(int i=0;i<COL;i++) screen[0][i] = screen[ROW-1][i] = '-';
  for(int i=0;i<ROW;i++) screen[i][0] = screen[i][COL-1] = '|';
  for(int i=1;i<ROW-1;i++) {
    for(int j=1;j<COL-1;j++) {
      screen[i][j] = ' ';
    }
  }
  screen[0][0] = screen[0][COL-1] = '+';
  screen[ROW-1][0] = screen[ROW-1][COL-1] = '+';
}

Snake snake(5);
pii food;

void user_input() {
  char ch;
  while(true) {
    dbg("Waiting for user input\n");
    ch = getch();
    dbg("Got: %c\n",ch);
    if(ch == 'w') snake.UpdateDirection(UP);
    else if (ch == 'a') snake.UpdateDirection(LEFT);
    else if (ch == 'd') snake.UpdateDirection(RIGHT);
    else if (ch == 's') snake.UpdateDirection(DOWN);
  }
}

void game_over() {
  exit(0);
}

int main() {
  initscr();
  init_screen();
  
  thread t1(user_input);
#define ONESEC 1000000LL
  float delay = 0.1 * ONESEC;
  food = snake.GenerateNewFoodPosition();
  
  while(true) {  
    vector<pii> curr = snake.Move();
    Collision_t collision = snake.CheckCollision(food);
    if(collision != NONE) {
      if(collision == SELF || collision == WALL) game_over();
      else {
	// Collision is with food! <3
	food = snake.GenerateNewFoodPosition();
	delay -= 0.01 * ONESEC;
      }
    }
    
    screen[food.first][food.second] = 'X';
    for(auto [x,y]: curr) {
      screen[x][y] = 'O';
    }
    
    draw();
    usleep(delay);
    
    for(auto [x,y]: curr) {
      screen[x][y] = ' '; 
    }
  }

  endwin();
  return 0;
}
  
