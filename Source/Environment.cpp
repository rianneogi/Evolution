#include "Environment.h"

Environment::Environment() : mGame(NULL), mPopulation(NULL), mPopulationSize(0)
{

}

Environment::Environment(AtariGame* game, Genotype* pop, int pop_size) : mGame(game), mPopulation(pop), mPopulationSize(pop_size)
{
	// mPopulation = new Genotype[mPopulationSize];
	int REG_SIZE = 8;
	int INPUT_SIZE = mGame->getStateSize();
	mExe.mRegisters = new int[REG_SIZE];
	mExe.mRegisterSize = REG_SIZE;
	mExe.mInputSize = INPUT_SIZE;
	mExe.mInputs = mGame->mALE->getRAM().array();
	mExe.reset();
}

Environment::~Environment()
{
	// delete mGame;
}

int Environment::getFitness(int indi)
{
	mGame->restart();
	// game.resetState();
	mExe.reset();
	// exe.mInputs = game.mALE->getRAM().array();
	// exe.mInputSize = game.mALE->getRAM().size();
	int totalReward = 0;
	// indi->print();
	while (!mGame->is_over())
	{
		totalReward += mGame->mALE->act(PLAYER_A_FIRE);
		mExe.resetStep();

		mExe.run_code(&mPopulation[indi], 0);

		ActionVect vect = mGame->mALE->getMinimalActionSet();
		int size = vect.size();
		totalReward += mGame->do_action(mExe.mRegisters[0] % size);
	}
	return totalReward;
}