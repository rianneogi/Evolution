#include "TicTacToe.h"
// #include <string>

class AtariGame
{
public:
    ALEInterface* mALE;
    ALEState mBaseState;
    
    AtariGame(const std::string& rom, int seed, bool display);
    ~AtariGame();
    
    int make_move(int move);
    bool is_over();
    void restart();
    void resetState();
};

int run_atari(AtariGame& game, Execution& exe, const Genotype* indi);