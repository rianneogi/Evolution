#include "TicTacToe.h"
// #include <string>

class AtariGame : public Game
{
public:
    ALEInterface* mALE;
    ALEState mBaseState;
    int mFrames;

    AtariGame();
    AtariGame(const std::string &rom, int seed, bool display);
    ~AtariGame();

    void init(const std::string &rom, int seed, bool display);
    int do_action(int action);
    bool is_over();
    byte_t* getState();
    int getStateSize();
    void restart();
    void softReset();
    void hardReset();
};

int run_atari(AtariGame& game, Execution& exe, const Genotype* indi);