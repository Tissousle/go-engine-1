#include <vector>
struct Location {
    char x = 0;
    char y = 0;

    Location(char x_init, char y_init);

    void print();

    bool operator==(const Location& other) const;

};

struct Stone : Location {  
    char side = '-';

    Stone(char side_init, char x_init, char y_init);

    void print();

    bool operator==(const Stone& other) const;

};

struct StoneGroup {
    std::vector<Stone> stones;
    std::vector<Location> liberties;
    char side = '-';

    void print_group() const;

    StoneGroup merge_groups(const StoneGroup& s) const;

    StoneGroup(char side_init);

    bool operator==(const StoneGroup& other) const;

    void remove_duplicates();

    void remove_duplicate_liberties();

};

class Board {

public:
    char board[11][11]; // The board is actually a 9x9. This is for liberty checking
    char side = 'X';
    std::vector<StoneGroup> stone_groups;
    std::vector<Stone> move_stack; 
    std::vector<Location> legal_moves;

    void print_board();

    void make_move(char x, char y);

    // puts legal moves into the legal_moves vector
    void gen_legal_moves();

    Board clone() const;

    Board();
private:
    void empty_board(); 

    void place_stone(char x, char y);

    std::vector<StoneGroup> merge_loop(std::vector<StoneGroup> groupsAdded);

    bool add_stone_to_group(char x, char y);

    void create_group(char x, char y);

    // Right now, this just clears the liberties of a group and then recreates them.
    // TODO: make this dynamic (far into the future)
    int update_liberties(StoneGroup& s);

    void capture_stone_group(StoneGroup& s);

    void pass_turn();

};

