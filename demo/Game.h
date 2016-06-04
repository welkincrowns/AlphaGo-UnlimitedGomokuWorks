#ifndef GAME_H_
#define GAME_H_

#include "Chessboard.h"

class Game {
  public:
    Game() {}
    ~Game() {}
    void Run();

  private:
    Chessboard board_;
};

#endif // GAME_H_