#include <iostream>
#include <ostream>
#include "scoreboard.h"
//#include "board.h" // redefinition error so let's not


class TestBoard : public Board {

public:
    char test_board[11][11];
    void update_test_board_to_stonegroups() {
        //empty board
        for (int x = 0; x < 11; x++) {
            for (int y = 0; y < 11; y++) {
                test_board[y][x] = '-';
                if (x < 1 or x > 9 or y < 1 or y > 9) {
                    test_board[y][x] = '.';
                }
            }
        }
        // add 
        for (StoneGroup group : stone_groups) {
            for (Stone stone : group.stones) {
                if (test_board[stone.y][stone.x] != '-') { std::cout <<"x"; }
                test_board[stone.y][stone.x] = stone.side;
            }
        }
        std::cout<<"\n";
    }

    void print_test_board_and_regular_board() {

        std::cout << "\t";
        for (int y = 9; y >= 1; y--) {
            for (int x = 1; x <= 9; x++) {
                std::cout << board[y][x] << " ";
            }
            std::cout << " " << y << "\t\t\t";
            for (int x = 1; x <= 9; x++) {
                std::cout << test_board[y][x] << " ";
            }
            std::cout << " " << y << "\n\t";
        }
        std::cout << "\n\t";
        for (char i = 1; i <= 9; i++) {
            std::cout << +i << " ";
        }
        std::cout << "\n\n\t\t\t\t ";
        if (side == 'X') {
            std::cout << "Black to move (X)\n\n";
        } else {
            std::cout << "White to move (O)\n\n";
        }
    }
};

int main() {
    Board board;
    srand(5);
    for (int i = 0; i < 80; i++) {
        board.make_random_move();
    }
    /*for (int i = 0; i < 50; i++) {
        board.print_board();
        //std::cout << board.capture_balance;
        char x;
        char y;
        std::cout << "Enter x: ";
        std::cin >> x;
        std::cout << "Enter y: \n";
        std::cin >> y;
        if (x == 48) {
            std::cout << "Groups:\n";
            for (StoneGroup group : board.stone_groups) {
                group.print_group();
                for (Location liberty : group.liberties) {
                    liberty.print();
                }
            }
        }
        x = x - 48;
        y = y - 48;
        board.make_move(x, y);
    }*/
    board.print_board();

    ScoreBoard sb(board);

    bool result = sb.score_game();

    sb.board.print_board();

    if (result) {
        std::cout << "Black wins!\n";
    } else {
        std::cout << "White wins!\n";
    }

    std::cout << sb.board.capture_balance << "points \n";

}
