#include <unistd.h>
#include <cstdlib>
#include <thread>
#include "snake.hpp"
#include "common.hpp"

char screen[ROW][COL];

void draw() {
  clear();
  static int iter = 1;
  printw("\nSNAKE!! (W)^ (A)< (S)v (D)> (Q)uit\n\n");
  dbg("Iteration: %d\n",iter++);
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

void prompt_screen() {
  int mr = ROW/2, mc = COL/2;
  char *ptr = &screen[mr][mc-7];
  const char *msg = "Press (Y) when ready.";
  int len = strlen(msg);
  strcpy(ptr,msg);
  ptr[len] = ' ';
  draw();
  char ch = '.';
  do {
    ch = getch();
  } while(ch != 'y');
  memset(ptr,' ',len+1); // replace the last \0 too
}

Snake snake(5);
pii food;
int points = 0;

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
    else if (ch == 'q') {
      endwin();
      exit(0);
    }
  }
}

void game_over() {
  char buff[200] = {0};
  sprintf(buff,"Game over! You scored %d points %s",points,(points ? ":)" : ":("));
  printw(buff);
  getch();
  endwin();
  exit(0);
}

int main() {
  initscr();
  noecho();

  init_screen();
  prompt_screen();
  
  thread t1(user_input);
  float delay = 0.1 * ONESEC;
  food = snake.GenerateNewFoodPosition();
  
  while(true) {  
    vector<pii> curr = snake.Move();
    Collision_t collision = snake.CheckCollision(food);
    if(collision != NONE) {
      if(collision == SELF || collision == WALL) game_over();
      else {
	// Collision is with food <3, speed up!
	food = snake.GenerateNewFoodPosition();
	delay -= 0.001 * ONESEC;
	points++;
      }
    }
    
    screen[food.first][food.second] = 'X';

    char snake_heads[] = {'v','^','>','<'};
    for(int i=0;i<curr.size();i++) {
      auto [x,y] = curr[i];
      if(!i) {
	screen[x][y] = snake_heads[snake.GetDirection()];
      } else {
	screen[x][y] = 'O';
      }
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
  
