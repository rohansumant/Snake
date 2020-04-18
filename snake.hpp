#ifndef _SNAKE_HPP_
#define _SNAKE_HPP_

#include "common.hpp"

class Snake {
  Dir_t dir;
  vector<pii> body;
  bool grow;
  // mutex for 'dir'.
  mutex mtx;
  
 public:
  Snake(int len) {
    dir = DOWN;
    grow = false;
    int r = ROW/2, c = COL/2;
    for(int i=0;i<len;i++) {
      body.push_back({r--,c});
    }
  }

  void UpdateDirection(Dir_t dir_) {
    dbg("Updating curr dir: %d to %d\n",dir,dir_);
    mtx.lock();
    if(abs(dx[dir]) == abs(dx[dir_]) && abs(dy[dir]) == abs(dy[dir_])) {
      // The updated direction is either the same or exactly opposite.
      // Either way, nothing to do here.
    } else {
      dir = dir_;
    }
    mtx.unlock();
  }

  vector<pii> Move() {
    if(grow) {
      pii tail = body.back();
      body.push_back(tail);
      grow = false;
    }
    int len = body.size();
    pii head = body[0];
    mtx.lock();
    pii new_head = {dx[dir]+head.first,dy[dir]+head.second};
    mtx.unlock();
    for(int i=len-1;i>=1;i--) {
      body[i] = body[i-1];
    }
    body[0] = new_head;
    return body;
  }

  Collision_t CheckCollision(const pii& food) {
    const pii &head = body[0];
    for(int i=1;i<body.size();i++) {
      if(head == body[i]) return SELF;
    }
    if(min(head.first,head.second) == 0 || head.first == ROW-1 || head.second == COL-1) return WALL;
    if(head == food) {
      grow = true;
      return FOOD;
    }
  }

  pii GenerateNewFoodPosition() {
    static bool first_call = true;
    if(first_call) {
      first_call = false;
      srand(time(NULL));
    }
    
    unordered_set<int> snake_pos_hash;
    int snake_len = body.size();

    auto hash = [](int x,int y) {
      return COL*x + y;
    };

    for(auto [x,y]: body) snake_pos_hash.insert(hash(x,y));
    
    float screen_area = (ROW-1)*(COL-1);
    if(snake_len/screen_area > 0.7) {
      vector<pii> available_positions;
      for(int i=1;i<ROW-1;i++) {
	for(int j=1;j<COL-1;j++) {
	  if(snake_pos_hash.find(hash(i,j)) == snake_pos_hash.end()) {
	    available_positions.push_back({i,j});
	  }
	}
      }
      int ix = rand() % available_positions.size();
      return available_positions[ix];
    } else {
      pii food;
      bool ok;
      do {
	int x = 1 + (rand() % (ROW-2));
	int y = 1 + (rand() % (COL-2));
	assert(x < ROW-1 && y < COL-1);
	food = {x,y};
	ok = snake_pos_hash.find(hash(x,y)) == snake_pos_hash.end();
      } while(!ok);
      return food;
    
    }
  }

};

#endif
