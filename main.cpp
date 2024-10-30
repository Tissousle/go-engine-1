#include <iostream>
#include <vector>

struct Location {
    char x = 0;
    char y = 0;
};

struct Stone : Location {  
    char side = '-';

    Stone(char sidep, char xp, char yp) {
        side = sidep;
        x = xp;
        y = yp;
    }

    void print() {
        if (side == 'X') {
            std::cout << "Black Stone:\tx: " << +x << "\ty: " << +y << "\n";
        }
        else {
            std::cout << "White Stone:\tx: " << +x << "\ty: " << +y << "\n";
        }
    }
};

struct StoneGroup {
    std::vector<Stone> stones;
    std::vector<Location> liberties;

    void print_group() {
        std::cout << stones.size() << " stone(s) in the group\n";
        for (Stone stone : stones) {
            stone.print();
        }
    }

};

class Board {

public:
    char board[9][9];
    char side = 'X';
    std::vector<StoneGroup> stone_groups;
    std::vector<Stone> move_stack; // TODO: test this

    void print_board() {
        for (int y = 8; y >= 0; y--) {
            for (int x = 0; x < 9; x++) {
                std::cout << board[y][x] << " ";
            }
            std::cout << y + 1 << "\n";
        }
        for (char i = 65; i < 65 + 9; i++) {
            std::cout << i << " ";
        }
        std::cout << "\n\n";
        if (side == 'X') {
            std::cout << "Black to move (X)\n\n";
        } else {
            std::cout << "White to move (O)\n\n";
        }
    }

    void make_move(char x, char y) {
        if (x > 9) {
            pass_turn(); // we will use x = 10 for passing moves.
            return;
        }
        place_stone(x, y);
    }


    Board() {
        empty_board();
    }
private:

    void empty_board() {
        for (int x = 0; x < 9; x++) {
            for (int y = 0; y < 9; y++) {
                board[y][x] = '-';
            }
        }
    }

    void place_stone(char x, char y) {
        if (x > 9 or x < 1 or y > 9 or y < 1) {
            std::cout << "we shouldn't be here";
            return;
        }
        board[y-1][x-1] = side;
        move_stack.push_back(Stone(side, x, y));
        add_stone_to_group(x, y);
        pass_turn();
    }

    // x, y are the position of the stone we're adding
    // checks to see whether a nearby stone is in a group, then we add
    // the stone into it if it exists. if not, make a new one.
    void add_stone_to_group(char x, char y) {
        for (StoneGroup group : stone_groups) {
            for (Stone stone : group.stones) {
                if (stone.y == y + 1 or 
                    stone.y == y - 1 or
                    stone.x == x - 1 or
                    stone.x == x + 1) 
                {
                    if (stone.side == side) {
                        Stone new_stone(side, x, y);
                        group.stones.push_back(new_stone);
                        group.print_group();
                        return;
                    }
                }
            }
        }
        // what? no stone groups match? make a new one.
        StoneGroup s;
        Stone stone(side, x, y);
        s.stones.push_back(stone);
        stone_groups.push_back(s);
    }

    

    void pass_turn() {
        if (side == 'X') {
            side = 'O';
        } else {
            side = 'X';
        }

    }

};

int main() {
    Board board;
    board.make_move(3, 5);
    board.make_move(9, 1);
    board.make_move(4, 5);
    board.print_board();
    for (StoneGroup group : board.stone_groups) {
        group.print_group();
    }
}
