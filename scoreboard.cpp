#include "scoreboard.h"
#include <iostream>
#include <vector>

/*void ScoreBoard::flood_fill(char x, char y, Fill *f) {
    if (board.board[y][x] != '-' and // if it IS - or ? or f.color we want to NOT return.
        board.board[y][x] != '?' and 
        board.board[y][x] != f->color*//*) 
    {
        if (board.board[y][x] != f->side and // if the blocking square in question is NOT the same as f.side
            board.board[y][x] != '.') // if the square in question IS on the board
        {
            if (f->side == '?') { //  NOTE: logic errors beware
                f->side = board.board[y][x]; // we HAVEN'T set the side yet, so set it to 'X' or 'O'
            } else {
                f->side = '-'; // there are TWO colors touching this territory. this SHOULD be neutral territory.
            }
        }
        return;
    }
    board.board[y][x] = f->color;
    
    flood_fill(x, y-1, f);
    flood_fill(x, y+1, f);
    flood_fill(x-1, y, f);
    flood_fill(x+1, y, f);
    return;
}*/

void ScoreBoard::flood_fill(char x, char y, Fill *f, char pattern) {
    std::vector<Location> Q;
    Q.push_back(Location(x,y));
    while (Q.size()) {
        Location n = Q[0];
        Q.erase(Q.begin());
        if (board.board[n.y][n.x] == pattern) {
            board.board[n.y][n.x] = f->color;
            Q.push_back(Location(n.x,n.y-1));
            Q.push_back(Location(n.x,n.y+1));
            Q.push_back(Location(n.x+1,n.y));
            Q.push_back(Location(n.x-1,n.y));
        } else {
            if (not (board.board[n.y][n.x] == '.')) {
                if (f->side == 'X') {
                    if (board.board[n.y][n.x] == 'O') {
                        f->side = '-';
                    }
                } else
                if (f->side == 'O') {
                    if (board.board[n.y][n.x] == 'X') {
                        f->side = '-';
                    }
                } else
                if (f->side == '?') {
                    f->side = board.board[n.y][n.x];
                }
            }
            
        }
    }
    return;
}

void ScoreBoard::fill_territories() {
    for (int x = 1; x <=9; x++) {
        for (int y = 1; y <= 9; y++) {
            if (board.board[y][x] == '-') {
                Fill f;
                flood_fill(x, y, &f, '-'); 
                if (f.side == 'X') {
                    f.color = 'b';
                    flood_fill(x, y, &f, '?');
                } else if (f.side == 'O') {
                    f.color = 'w';
                    flood_fill(x, y, &f, '?');
                }
            } 
        }
    }
}

bool ScoreBoard::score_game() {
    fill_territories();

    for (int x = 1; x <=9; x++) {
        for (int y = 1; y <= 9; y++) {
            if (board.board[y][x] == 'b') {
                board.capture_balance += 1;
            } else if (board.board[y][x] == 'w') {
                board.capture_balance -= 1;
            }
        }
    }

    if (board.capture_balance > 0) {
        return true;
    } else {
        return false; // if it's zero, we say white wins cause komi is supposed to be 6.5 not 6
    }

}


ScoreBoard::ScoreBoard(Board b) {
    board = b.clone();
}

