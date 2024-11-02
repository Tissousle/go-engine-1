#include "board.h"

struct Fill {
    char color = '?';
    char side = '?';
};

class ScoreBoard {
public:
    Board board;

    void flood_fill(char x, char y, Fill *f, char pattern);

    void fill_territories();

    // returns true if black won, false if white
    bool score_game();

    ScoreBoard(Board b);
};
