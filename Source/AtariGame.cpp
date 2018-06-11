#include "AtariGame.h"

AtariGame::AtariGame(const std::string& rom, int seed, bool display)
{
    mALE = new ALEInterface(display);
	mALE->setInt("random_seed", seed);
	mALE->loadROM(rom);
}

AtariGame::~AtariGame()
{
    delete mALE;
}

int AtariGame::make_move(int move)
{
    ActionVect legal_actions = mALE->getMinimalActionSet();
    
    return mALE->act(legal_actions[move]);
}

bool AtariGame::is_over()
{
    return mALE->game_over();
}

void AtariGame::restart()
{
    mALE->reset_game();
}

int run_atari(AtariGame& game, Execution& exe, const Genotype* indi)
{
    game.restart();
    exe.reset();
    // exe.mInputs = game.mALE->getRAM().array();
    // exe.mInputSize = game.mALE->getRAM().size();
    int totalReward = 0;
    // indi->print();
    while(!game.is_over())
    {
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
        totalReward += game.make_move(exe.mRegisters[0]%size);
    }
    return totalReward;
}
