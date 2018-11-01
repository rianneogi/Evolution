#include "AtariGame.h"

AtariGame::AtariGame(const std::string& rom, int seed, bool display)
{
    mALE = new ALEInterface(display);
	mALE->setInt("random_seed", seed);
	mALE->loadROM(rom);
    mBaseState = mALE->cloneSystemState();
}

AtariGame::~AtariGame()
{
    delete mALE;
}

int AtariGame::do_action(int action)
{
    ActionVect legal_actions = mALE->getMinimalActionSet();

    return mALE->act(legal_actions[action]);
    mFrames++;
}

bool AtariGame::is_over()
{
    return mALE->game_over();
}

byte_t* AtariGame::getState()
{
    return mALE->getRAM().array();
}

int AtariGame::getStateSize()
{
    return mALE->getRAM().size();
}

void AtariGame::restart()
{
    hardReset();
    // softReset();
}

void AtariGame::hardReset()
{
    mALE->restoreSystemState(mBaseState);
}

void AtariGame::softReset()
{
    mALE->reset_game();
}

int run_atari(AtariGame& game, Execution& exe, const Genotype* indi)
{
    game.restart();
    // game.resetState();
    exe.reset();
    // exe.mInputs = game.mALE->getRAM().array();
    // exe.mInputSize = game.mALE->getRAM().size();
    int totalReward = 0;
    // indi->print();
    while(!game.is_over())
    {
        totalReward += game.mALE->act(PLAYER_A_FIRE);
        // exe.reset();
        exe.resetStep();
        
        // auto screen = game.mALE->getScreen();
        // auto ram = game.mALE->getRAM();
        
        // exe.mInputs = game.mALE->getRAM().array();
        // exe.mInputSize = game.mALE->getRAM().size();

        // memcpy(exe.mRegisters, screen.getArray(), screen.arraySize()*sizeof(pixel_t));
        // memcpy(exe.mRegisters, ram.array(), ram.size()*sizeof(pixel_t));
        
        // for(int i = 0;i<screen.height();i++)
        // {
        //     for(int j = 0;j<screen.width();j++)
        //     {
        //         exe.mRegisters[screen.width()*i + j + 1] = game.mALE->getScreen().get(i, j);
        //     }
        // }
        exe.run_code(indi, 0);
        
        ActionVect vect = game.mALE->getMinimalActionSet();
        int size = vect.size();
        // printf("movemade %d\n", exe.mRegisters[0]%size);
        // if(exe.mRegisters[0]>=size) exe.mRegisters[0]=size-1;
        totalReward += game.do_action(exe.mRegisters[0]%size);
    }
    return totalReward;
}

Vector2I run_atari_supervised(AtariGame& game, Execution& exe, const Genotype* indi, const Genotype* supervisor)
{
    game.restart();
    exe.reset();
    int gameReward = 0;
    int supervisorReward = 0;
    while (!game.is_over())
    {
        gameReward += game.mALE->act(PLAYER_A_FIRE);
        exe.reset();
        exe.run_code(indi, 0);

        ActionVect vect = game.mALE->getMinimalActionSet();
        int size = vect.size();
        gameReward += game.do_action(exe.mRegisters[0] % size);

        exe.reset();
        exe.run_code(supervisor, 0);
        supervisorReward += exe.mRegisters[0];

    }
    return Vector2I(gameReward, supervisorReward);
}