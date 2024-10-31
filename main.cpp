#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

struct Location {
    char x = 0;
    char y = 0;

    Location(char x_init, char y_init) {
        x = x_init;
        y = y_init;
    }

};

struct Stone : Location {  
    char side = '-';

    Stone(char side_init, char x_init, char y_init) : Location(x_init, y_init) {
        side = side_init;
    }

    void print() {
        if (side == 'X') {
            std::cout << "Black Stone:\tx: " << +x << "\ty: " << +y << "\n";
        }
        else {
            std::cout << "White Stone:\tx: " << +x << "\ty: " << +y << "\n";
        }
    }

    bool operator==(const Stone& other) const {
        if ((x == other.x) and (y == other.y) and (side == other.side)) {
            return true;
        }
        else 
        {
            return false;
        }
    }

};

struct StoneGroup {
    std::vector<Stone> stones;
    std::vector<Location> liberties;
    char side = '-';

    void print_group() const {

        std::cout << stones.size() << " stone(s) in the " << side << " group with " << liberties.size() << " liberties:\n";
        for (Stone stone : stones) {
            stone.print();
        }
    }

    StoneGroup merge_groups(const StoneGroup& s) const {
        StoneGroup novie(stones[0].side);
        for (Stone stone : s.stones) {
            novie.stones.push_back(stone);
        }
        for (Stone stone : stones) {
            novie.stones.push_back(stone);
        }
        return novie;
    }

    StoneGroup(char side_init) {
        side = side_init;
    }

    bool operator==(const StoneGroup& other) const {
        if (other.stones.size() != stones.size()) {
            return false;
        }
        for (int i = 0; i < stones.size(); i++) {
            if (not (other.stones[i] == stones[i])) {
                return false;
            }
        }
        if (other.side != side) {
            return false;
        }
        return true;
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
        if (x == 0) {
            pass_turn(); // we will use x = 0 for passing moves.
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
        std::vector<StoneGroup> groupsAdded;
        bool addedGroup = false;
        for (StoneGroup& group : stone_groups) {
            for (Stone stone : group.stones) {
                if ((stone.y == y + 1 and stone.x == x) or 
                    (stone.y == y - 1 and stone.x == x) or
                    (stone.x == x - 1 and stone.y == y) or
                    (stone.x == x + 1 and stone.y == y)) // check adjacent
                {
                    if (stone.side == side) {
                        if (not addedGroup) { // We don't want duplicates when merging groups.
                            Stone new_stone(side, x, y);
                            group.stones.push_back(new_stone);
                        }
                        groupsAdded.push_back(group);
                        addedGroup = true; // set flag
                    }
                }
            }
        }


        while (groupsAdded.size() > 1) {

            StoneGroup s1 = groupsAdded[groupsAdded.size()-1];
            StoneGroup s2 = groupsAdded[groupsAdded.size()-2];

            StoneGroup s3 = s1.merge_groups(s2);

           

            for (int i = 0; i < stone_groups.size(); i++) {
                if (stone_groups[i] == s1) {
                    stone_groups.erase(stone_groups.begin() + i);
                    break;
                }
            }
            for (int i = 0; i < stone_groups.size(); i++) {
                if (stone_groups[i] == s2) {
                    stone_groups.erase(stone_groups.begin() + i);
                    break;
                } 
            }

            groupsAdded.pop_back();
            groupsAdded.pop_back();

            groupsAdded.push_back(s3);
            stone_groups.push_back(s3);

        }

        for (StoneGroup& group : stone_groups) {
            update_liberties(group); // We need to do this outside incase of mergers.
        }

        // what? no stone groups match? make a new one.
        if (groupsAdded.size() == 0) {
            StoneGroup s(side);
            Stone stone(side, x, y);
            s.stones.push_back(stone);
            update_liberties(s);
            stone_groups.push_back(s);
        }
    }

    // Right now, this just clears the liberties of a group and then recreates them.
    // TODO: make this dynamic
    void update_liberties(StoneGroup& s) {
        if (s.liberties.size() > 0) {
            s.liberties.clear();
        }
        for (Stone stone : s.stones) {
            if (board[stone.y-1][stone.x-2] == '-') s.liberties.push_back(Location(stone.x-1,stone.y));
            if (board[stone.y-2][stone.x-1] == '-') s.liberties.push_back(Location(stone.x,stone.y-1));
            if (board[stone.y-1][stone.x] == '-') s.liberties.push_back(Location(stone.x+1,stone.y));
            if (board[stone.y][stone.x-1] == '-') s.liberties.push_back(Location(stone.x,stone.y+1));
        }

    }

    void pass_turn() {
        std::cout << "\tGroups:\n";
        for (StoneGroup group : stone_groups) {
            group.print_group();
        }
        print_board();

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
    board.make_move(8, 2);
    board.make_move(5, 5);
    board.make_move(8, 4);
    board.make_move(4, 6);
    board.make_move(8, 3);
    board.make_move(4, 7);
    board.make_move(5, 4);
    board.make_move(4,5);
    board.print_board();
    for (Stone move : board.move_stack) {
        move.print();
    }
}
