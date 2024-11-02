#include "board.h"
#include <iostream>
#include <cstdlib>


Location::Location(char x_init, char y_init) {
    x = x_init;
    y = y_init;
}

void Location::print() {
    std::cout << "Location -- x: " << +x << " y: " << +y << std::endl;
}

bool Location::operator==(const Location& other) const {
    if (x == other.x and y == other.y) {
        return true;
    } else {
        return false;
    }
}


Stone::Stone(char side_init, char x_init, char y_init) : Location(x_init, y_init) {
    side = side_init;
}

void Stone::print() {
    if (side == 'X') {
        std::cout << "Black Stone:\tx: " << +x << "\ty: " << +y << "\n";
    }
    else {
        std::cout << "White Stone:\tx: " << +x << "\ty: " << +y << "\n";
    }
}

bool Stone::operator==(const Stone& other) const {
    if ((x == other.x) and (y == other.y) and (side == other.side)) {
        return true;
    }
    else 
    {
        return false;
    }
}

void StoneGroup::print_group() const {

    std::cout << stones.size() << " stone(s) in the " << side << " group with " << liberties.size() << " liberties:\n";
    for (Stone stone : stones) {
        stone.print();
    }
}

StoneGroup StoneGroup::merge_groups(const StoneGroup& s) const {
    StoneGroup novie(stones[0].side);
    for (Stone stone : s.stones) {
        novie.stones.push_back(stone);
    }
    for (Stone stone : stones) {
        novie.stones.push_back(stone);
    }
    return novie;
}

StoneGroup::StoneGroup(char side_init) {
    side = side_init;
}

bool StoneGroup::operator==(const StoneGroup& other) const {
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

void StoneGroup::remove_duplicates() {
    for (int i = 0; i < stones.size(); i++) {
        for (int j = 0; j < stones.size(); j++){
            if (i == j) {
                continue;
            }
            if (stones[i] == stones[j]) {
                stones[j].x = 0;
            }
        }
    }
    for (int i = 0; i < stones.size(); i++) {
        if (stones[i].x == 0) {
            stones.erase(stones.begin() + i);
            i--;
        }
    }
}

void StoneGroup::remove_duplicate_liberties()  {
    for (int j = 0; j < liberties.size(); j++) {
        for (int i = 0; i < liberties.size(); i++) {
            if (j == i) continue;
            if (liberties[j] == liberties[i]) {
                liberties[i].x = 0;
            }
        }
    }
    for (int i=0; i < liberties.size(); i++) {
        if (liberties[i].x == 0) {
            liberties.erase(liberties.begin() + i);
            i--;
        }
    }

}

void Board::print_board() {
    std::cout << "\t";
    for (int y = 9; y >= 1; y--) {
        for (int x = 1; x <= 9; x++) {
            std::cout << board[y][x] << " ";
        }
        std::cout << " " << y << "\n\t";
    }
    std::cout << "\n\t";
    
    for (char i = 1; i <= 9; i++) {
        std::cout << +i << " ";
    }
    std::cout << "\n\n\t ";
    if (side == 'X') {
        std::cout << "Black to move (X)\n\n";
    } else {
        std::cout << "White to move (O)\n\n";
    }
}

void Board::make_move(char x, char y) {
    if (x == 0) {
        pass_turn(); // we will use x = 0 for passing moves.
        return;
    }
    Location move(x, y);
    gen_legal_moves();
    bool is_legal_move = false;
    for (Location mv : legal_moves) {
        if (mv == move) {
            is_legal_move = true;
            break;
        }
    }
    if (not is_legal_move) {
        std::cout << "Illegal move: " << +x << " " << +y << std::endl;
    }
    else {
        place_stone(x, y);
    }
}

// will put legal moves into the legal_moves vector
void Board::gen_legal_moves() {
    legal_moves.clear();
    if (move_stack.size() > 1)
        if (move_stack[move_stack.size()-1] == move_stack[move_stack.size()-2]) {
            return;
        }
    legal_moves.push_back(Location(0,0));
    for (int y = 1; y <= 9; y++) {
        for (int x = 1; x <= 9; x++) {
            if (board[y][x] == '-') {
                Board z = clone();
                z.place_stone(x, y);
                
                if (z.board[y][x] != '-' and not (Location(x,y) == ko)) {
                    legal_moves.push_back(Location(x, y));
                }

            }
        }
    }
}

void Board::make_random_move() {
    gen_legal_moves();
    if (not legal_moves.size()) {
        return;
    }
    Location loc = legal_moves[std::rand() % legal_moves.size()];
    if (loc == Location(0,0)) {
        pass_turn();
        return;
    }
    place_stone(loc.x, loc.y);
}

Board::Board() {
    empty_board();
}

Board Board::clone() const {
    Board x;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            x.board[i][j] = board[i][j];
        }
    }
    x.side = side;
    x.stone_groups = std::vector(stone_groups);
    x.move_stack = std::vector(move_stack);

    return x;
}
void Board::empty_board() {
    for (int x = 0; x < 11; x++) {
        for (int y = 0; y < 11; y++) {
            board[y][x] = '-';
            if (x < 1 or x > 9 or y < 1 or y > 9) {
                board[y][x] = '.';
            }
        }
    }
}

void Board::place_stone(char x, char y) {
    if (x > 9 or x < 1 or y > 9 or y < 1) {
        std::cout << "we shouldn't be here";
        return;
    }
    board[y][x] = side;
    move_stack.push_back(Stone(side, x, y));
    if (not add_stone_to_group(x, y)) {
        create_group(x, y);
    }
    for (int i = 0; i < stone_groups.size(); i++) {
        if (stone_groups[i].side != side) {
            if (update_liberties(stone_groups[i]) == 0) {
                capture_stone_group(stone_groups[i]);
                i--;
            }
        }
    }
    for (int i = 0; i < stone_groups.size(); i++) {
        if (stone_groups[i].side == side) { // We need this for suicide checking.
            if (update_liberties(stone_groups[i]) == 0) {
                capture_stone_group(stone_groups[i]);
                i--;
            }
        }
    }
    pass_turn();
}

std::vector<StoneGroup> Board::merge_loop(std::vector<StoneGroup> groupsAdded) {

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

        s3.remove_duplicates(); // don't know why they happen. But they do

        groupsAdded.push_back(s3);
        stone_groups.push_back(s3);

    }
    return groupsAdded;
}

// x, y are the position of the stone we're adding
// checks to see whether a nearby stone is in a group, then we add
// the stone into it if it exists. if not, make a new one.
bool Board::add_stone_to_group(char x, char y) {
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

    groupsAdded = merge_loop(groupsAdded);

    return groupsAdded.size() > 0;

}

void Board::create_group(char x, char y) {
    StoneGroup s(side);
    Stone stone(side, x, y);
    s.stones.push_back(stone);
    stone_groups.push_back(s);
}

// Right now, this just clears the liberties of a group and then recreates them.
// TODO: make this dynamic (far into the future)
int Board::update_liberties(StoneGroup& s) {
    s.liberties.clear();
    for (Stone stone : s.stones) { 
        //If we weren't planning on a dynamic system, this would be unnecessary, since
        //you could just check to see if the group has at least one liberty
        //and it'd alive at that point.
        if (board[stone.y][stone.x-1] == '-') s.liberties.push_back(Location(stone.x-1,stone.y));
        if (board[stone.y-1][stone.x] == '-') s.liberties.push_back(Location(stone.x,stone.y-1));
        if (board[stone.y][stone.x+1] == '-') s.liberties.push_back(Location(stone.x+1,stone.y));
        if (board[stone.y+1][stone.x] == '-') s.liberties.push_back(Location(stone.x,stone.y+1));
    }

    s.remove_duplicate_liberties();

    return s.liberties.size();


}

void Board::capture_stone_group(StoneGroup& s) {
    int mult = s.side == 'X' ? -1 : 1; // if white loses stones, +1. if black, -1. 
    for (Stone stone : s.stones) {
        board[stone.y][stone.x] = '-';
        capture_balance += mult;
    }
    for (int i = 0; i < stone_groups.size(); i++) {
        if (stone_groups[i] == s) {
            stone_groups.erase(stone_groups.begin() + i);
        }
    }
}

void Board::pass_turn() {
    /*
    std::cout << "\tGroups:\n";
    for (StoneGroup group : stone_groups) {
        group.print_group();
    }*/

    if (side == 'X') {
        side = 'O';
    } else {
        side = 'X';
    }

}
